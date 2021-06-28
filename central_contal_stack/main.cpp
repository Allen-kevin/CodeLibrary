#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <errno.h>
#include <signal.h>
#include <deque>
#include <algorithm>
#include <iterator>

#define MAX_EVENTS 512
#define MAX_MESSAGE_LEN 512
#define TIME_THRESHOLD 1000000000
#define SEC_TO_NSEC 1000000000
#define DEFAULT_PORT 4321
#define TOTAL_BD 1024*1024*1024
#define MAX_NUM 10

using namespace std;

int sockfd, connfd;
int end_node;
uint32_t total_avail_bd;

//static enum LOG_LEVEL logger=LOG_LEVEL_INFO;
static volatile int keep_running = 1;

struct time_event {
	uint32_t event_alloc_bd;
	uint32_t accumulative_alloc_bd; 
	struct timespec time_event;
};


struct index_deque {
	int fd;
	int index;
	deque<struct time_event> deq;
} node_deque[MAX_NUM];



/* string to int */
static int msg_parse(char *str)
{
	return atoi(str);
}

static int update_total_bandwidth()
{
	return TOTAL_BD/end_node;
}

static int update_require_alloc(uint32_t require_len, uint32_t cur_consum_bd)
{
	/* end_total_bd represents the avg bandwidth allocated to each node. */
	uint32_t end_total_bd = update_total_bandwidth();

	if (require_len + cur_consum_bd <= end_total_bd)
		return end_total_bd;
	
	return min(require_len, total_avail_bd);
}


static uint32_t add_event_queue(uint32_t require_len, 
		uint32_t cur_consum_bd, struct timespec &time_now, 
		deque<struct time_event> &time_queue)
{
	struct time_event cur_event;

	require_len = update_require_alloc(require_len, cur_consum_bd);
	cur_event.event_alloc_bd = require_len;
	cur_event.accumulative_alloc_bd = cur_consum_bd + require_len;
	cur_event.time_event = time_now;
	time_queue.push_back(cur_event);

	return require_len;

}

static void pop_timeout_event(struct timespec &time_now, 
		uint32_t &cur_consum_bd, deque<struct time_event> &time_queue)
{
	uint32_t elapsed;
	/* Access recent element. */
	struct time_event recent_event = time_queue.back();
	/* Access first element. */
	struct time_event oldest_event = time_queue.front();
	struct timespec time_oldest = oldest_event.time_event;

	cur_consum_bd = recent_event.accumulative_alloc_bd;
	elapsed = (time_now.tv_sec - time_oldest.tv_sec)*SEC_TO_NSEC + time_now.tv_nsec - time_oldest.tv_nsec;
	while (elapsed >= TIME_THRESHOLD && time_queue.size()>2) {
		cur_consum_bd -= oldest_event.event_alloc_bd;
		total_avail_bd += oldest_event.event_alloc_bd;
		time_queue.pop_front();
		oldest_event = time_queue.front();
		time_oldest = oldest_event.time_event;
		elapsed = (time_now.tv_sec - time_oldest.tv_sec)*SEC_TO_NSEC + time_now.tv_nsec - time_oldest.tv_nsec;
	}
}

static uint32_t update_event_queue(uint32_t require_len, 
		deque<struct time_event> &time_queue)
{
	struct timespec time_now = {0, 0};
	uint32_t cur_consum_bd = 0;
	clock_gettime(CLOCK_REALTIME, &time_now);
	if (time_queue.size() < 2) {
		require_len = add_event_queue(require_len, 0, time_now, time_queue);
		total_avail_bd -= require_len;
		
		return require_len;
	}

	pop_timeout_event(time_now, cur_consum_bd, time_queue);
	add_event_queue(require_len, cur_consum_bd, time_now, time_queue);
	total_avail_bd -= require_len;

	return require_len;
}



static void add_index_deque(int fd)
{
	node_deque[end_node].fd = fd;
	node_deque[end_node].index = end_node;
	end_node++;
}


static int get_index_deque(int fd)
{
	int i = 0;

	for (i = 0; i < MAX_NUM; i++) {
		if (node_deque[i].fd == fd) {
			return node_deque[i].index;
		}
	}

	return -1;
}


static int bandwidth_alloc(uint32_t require_len, int fd)
{
	int index = get_index_deque(fd);

	if (index < 0 || require_len <= 0) {
		cout <<"deque index = " << index <<"require len = "<<require_len<<endl;
		exit(-1);
	}

	return update_event_queue(require_len, node_deque[index].deq);
}

static int tcp_socket(char **argv)
{
	struct sockaddr_in servaddr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == sockfd) {
		perror("Socket failed.");
		exit(-1);
	}
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(argv[1]);
	servaddr.sin_port = htons(DEFAULT_PORT);
	if (-1 == bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) {
		perror("Bind failed.");
		exit(-1);
	}

	if (-1 == listen(sockfd, MAX_EVENTS)) {
		perror("Listen failed.");
		exit(-1);
	}
	
	return 0;
}


static void stop_server_running(int sig)
{
	keep_running = 0;
}

static int epoll_init_handler(struct epoll_event *ev)
{
	int epollfd;

	epollfd = epoll_create(MAX_EVENTS);
	if (epollfd < 0) {
		perror("Error creating epoll..\n");
	}
	ev->events = EPOLLIN;
	ev->data.fd = sockfd;
	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, ev) == -1) {
		perror("Error adding new listeding socket to epoll..\n");
	}
	
	return epollfd;
}


static void epoll_accept(int epollfd, struct epoll_event *ev)
{
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);

	connfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_len);
	if (-1 == connfd) {
		perror("Error accepting new connection..\n");
		exit(-1);
	}

	cout << "Success establish connection.."<<std::endl;
	ev->events = EPOLLIN|EPOLLET;
	ev->data.fd = connfd;
	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, connfd, ev) == -1) {
		perror("Error adding new event to epoll..\n");
	}

}

static void recv_send(int epollfd, struct epoll_event *events)
{
	int recv_len = 0;
	char buff[MAX_MESSAGE_LEN];

	memset(buff, 0, sizeof(buff));
	recv_len = recv(events->data.fd, buff, MAX_MESSAGE_LEN, 0);
	if (recv_len <= 0) {
		epoll_ctl(epollfd, EPOLL_CTL_DEL, events->data.fd, NULL);
		shutdown(events->data.fd, SHUT_RDWR);
		return;
	}
    recv_len = bandwidth_alloc(msg_parse(buff), events->data.fd);
	if (recv_len <= 0) {
		cout <<"The msg should consist of a string of numbers. recv len = " <<recv_len <<endl;
	}

#if 1 //for latency testing
	struct timespec time_now = {0, 0};
	clock_gettime(CLOCK_REALTIME, &time_now);
	string str = std::to_string(time_now.tv_nsec);
#endif
	//string str = std::to_string(recv_len);
	send(events->data.fd, str.c_str(), str.size()+1, 0);
	//cout <<str <<endl;

}

static int pkt_process(int epollfd, struct epoll_event *ev)
{
	int new_events;
	struct epoll_event events[MAX_EVENTS];

	new_events = epoll_wait(epollfd, events, MAX_EVENTS, -1);
	if (new_events == -1) 
		perror("Error in epoll_wait..\n");
	
	for (int i = 0; i < new_events; i++) {
		if (events[i].data.fd == sockfd) {
			epoll_accept(epollfd, ev);
			add_index_deque(ev->data.fd);
		}
		else {
			recv_send(epollfd, &events[i]);
		}
	}

	return 0;
}

static void init()
{
	end_node = 0;
	total_avail_bd = TOTAL_BD;
}

int main(int argc, char **argv)
{

	int epollfd;
	struct epoll_event ev;
	
	init();
	/* create tcp connection */
	tcp_socket(argv);
	/* create epoll handler */
	epollfd = epoll_init_handler(&ev);

	while (keep_running) {
		signal(SIGINT, stop_server_running);
		pkt_process(epollfd, &ev);
	}

	cout<<"Server close sockfd.."<<endl;
	close(sockfd);

	return 0;
}
