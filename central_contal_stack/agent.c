#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <errno.h>
#include <time.h>
#include "logger.h"

#define TIME_THRESHOLD 1000000    //1ms
#define SEC_TO_NSEC 1000000000

int sockfd;
static enum LOG_LEVEL logger=LOG_LEVEL_INFO;
static volatile int keep_running = 1;

void tcp_socket(char **argv)
{
	struct sockaddr_in client_addr;

	bzero(&client_addr, sizeof(struct sockaddr_in));
	client_addr.sin_family = AF_INET;
	client_addr.sin_port = htons(atoi(argv[1]));

	if (inet_aton(argv[2], (struct in_addr *)&client_addr.sin_addr.s_addr) == 0) {
		log_info(logger, "ip addr invalid!");
		exit(-1);
	}

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		log_err(logger, "Sockfd failed, sockfd:%d, errno:%d, %s\n", sockfd, errno, strerror(errno));
		exit(-1);
	}

	if (connect(sockfd, (struct sockaddr *)&client_addr, sizeof(client_addr)) == -1) {
		log_err(logger, "Connfd failed, sockfd:%d, errno:%d, %s\n", sockfd, errno, strerror(errno));
		exit(-1);
	}
}

static void stop_server_running(int sig)
{
	keep_running = 0;
}

int main(int argc, char **argv)
{
    int flags, recv_len;
    uint32_t elapsed;
    struct timespec time_old = {0, 0};

	/* create tcp connection */
	tcp_socket(argv);
	log_info(logger, "socket init .........");
    clock_gettime(CLOCK_REALTIME, &time_old);
#if 1 
    if ((flags = fcntl(sockfd, F_GETFL, 0)) < 0) {
        perror("Failed to get flags..\n");
        return 0;
    }

    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
#endif
	while (keep_running) {
	    char buff[100] = "Hello world";
		signal(SIGINT, stop_server_running);
        struct timespec time_cur;
        clock_gettime(CLOCK_REALTIME, &time_cur);
        elapsed = (time_cur.tv_sec - time_old.tv_sec)*SEC_TO_NSEC + time_cur.tv_nsec - time_old.tv_nsec;
        //printf("elapsed = %d\n", elapsed);
#if 1
        if (elapsed >= TIME_THRESHOLD) {
            time_old = time_cur;
		    send(sockfd, buff, strlen(buff)+1, 0);
        }
#endif
	//	send(sockfd, buff, strlen(buff)+1, 0);
		recv_len = recv(sockfd, buff, 1024, MSG_DONTWAIT);
        if (recv_len <= 0) {
//            log_info(logger, "recv failed...\n");
            continue;
        }

		log_info(logger, "%s\n", buff);
	}
    sleep(5);
	close(sockfd);

	return 0;
}
