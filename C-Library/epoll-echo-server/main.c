/* server */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <fcntl.h>          //for nonblocking
#include <sys/epoll.h>      //for epoll

#define MAXEPOLL 10000
#define MAXLINE 1024


static int set_non_blocking(int fd)
{
	if (fcntl(fd, F_SETFL, fcntl(fd, F_GETFD, 0) | O_NONBLOCK) == -1) {
		printf("set blocking error: %d\n", errno);
		return -1;
	}

	return 0;
}


int tcp_socket(char **argv)
{
    int sockfd;
    struct sockaddr_in my_addr;
    unsigned int myport, lisnum;

	if (argv[1])
		myport = atoi(argv[1]);
	else
		myport = 7838;

    printf("begin bind!\n");
	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	} else {
		printf("socket created\n");
	}
	
	/* set non blocking */
	if (set_non_blocking(sockfd) == -1) {
		printf("set non blocking error: %d ...\n", errno);
		exit(EXIT_FAILURE);
	}

	bzero(&my_addr, sizeof(my_addr));
	my_addr.sin_family = PF_INET;
	my_addr.sin_port = htons(myport);
	if (argv[2])
		my_addr.sin_addr.s_addr = inet_addr(argv[2]);
	else
		my_addr.sin_addr.s_addr = INADDR_ANY;
	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr))
		== -1) {
		perror("bind");		
		exit(1);
	} else
		printf("binded\n");

	if (listen(sockfd, lisnum) == -1) {
		perror("listen");
		exit(1);
	} else
		printf("begin listen\n");
    
    return sockfd;
}


int main(int argc, char **argv)
{
	int sockfd;
	int cur_fds = 1;
	int nread;
	int wait_fds;
	int i = 0;
	int connfd;
	int ep_fd;
	struct sockaddr_in cliaddr;
	struct epoll_event ev;
	struct epoll_event evs[MAXEPOLL];

	char buf[MAXLINE];

	socklen_t len = sizeof(struct sockaddr_in);

	sockfd = tcp_socket(argv);
	
	ep_fd = epoll_create(MAXEPOLL);
	ev.events = EPOLLIN | EPOLLET;
	ev.data.fd = sockfd;

	if (epoll_ctl(ep_fd, EPOLL_CTL_ADD, sockfd, &ev) < 0) {
		printf("epoll_ctl error : %d\n", errno);	
		exit(EXIT_FAILURE);
	}

	while (1) {
		if ((wait_fds = epoll_wait(ep_fd, evs, cur_fds, -1)) == -1) {
			printf("epoll_wait error:%d\n", errno);
			exit(EXIT_FAILURE);
		}

		for (i = 0; i < wait_fds; i++) {
			if (evs[i].data.fd == sockfd &&cur_fds < MAXEPOLL) {
				if (connfd == accept(sockfd, (struct sockaddr *)&cliaddr, &len)) {
					printf("accept error: %d\n", errno);
					exit(EXIT_FAILURE);
				}

				ev.events = EPOLLIN | EPOLLET;
				ev.data.fd = connfd;
				if (epoll_ctl(ep_fd, EPOLL_CTL_ADD, connfd, &ev) < 0) {
					printf("epoll_ctl error : %d\n", errno);	
					exit(EXIT_FAILURE);
				}

				++cur_fds;
				continue;
			}
			nread = read(evs[i].data.fd, buf, sizeof(buf));
			if (nread < 0) {
				close(evs[i].data.fd);
				epoll_ctl(ep_fd, EPOLL_CTL_DEL, evs[i].data.fd, &ev);
				--cur_fds;
				continue;
			}
			printf("buf: %s\n", buf);
			write(evs[i].data.fd, buf, nread);
		}
	}

	close(sockfd);

	return 0;
}
