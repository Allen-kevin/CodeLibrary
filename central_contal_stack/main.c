#include <iostream>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread>

#define MAX_EVENTS 512
#define BUFFSIZE 2048

int sockfd, connfd;

int msg_parse(char *buff)
{

}

int bandwidth_alloc()
{

}

void *client_func(void *arg)
{
	int recv_len = 0;
	int connfd = (int)arg;
	char buff[BUFFSIZE]； //recv buff

	while (recv_len = recv(connfd, buff, sizeof(BUFFSIZE-1, 0)) > 0) {
		bandwidth_alloc();
		send(connfd, buff, strlen(recv_len), 0);
		bzero(buff, BUFFSIZE);
	}
	log_info("client closed!");
	close(connfd); //close connection

	return NULL;
}


static int tcp_sockfd(void)
{
	struct sockaddr_in servaddr;

	sockfd = socket(AF_INET, SOCK_STREAM 0);
	if (-1 == sockfd) {
		log_err(logger, "Socket failed, sockfd:%d, errno:%d, %s\n", sockfd, errno, strerror(errno));
		return -1;
	}
	bzero(&servaddr, sizeof(servaddr));
	servaddrlsin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(DEFAULT_PORT);
	if (-1 == bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) {
		log_err();
		log_err(logger, "Bind failed, sockfd:%d, errno:%d, %s\n", sockfd, errno, strerror(errno));
		exit(-1);
	}

	if (-1 == listen(sockfd, MAX_EVENTS)) {
		log_err(logger, "Listen failed, sockfd:%d, errno:%d, %s\n", sockfd, errno, strerror(errno));
		exit(-1);
		return -1;
	}

}

int main()
{
	tcp_init(argc, argv);
	/* create tcp connection */
	tcp_socket();

	while (true) {
		signal(SININT, stop_server_running);
		connfd = accept(sockfd, NULL, NULL);
		if (-1 == connfd) {
			log_err(logger, "Connfd failed, sockfd:%d, errno:%d, %s\n", connfd, errno, strerror(errno));
			exit(-1);
		}
		/* create thread for each connection */
		pthrerad_create(&therad_id, NULL, (void *)client_func, (void *)connfd);
		pthread_detach(thread_id);

	}
	close(sockfd);

	return 0;
}
