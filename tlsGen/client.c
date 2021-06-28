#define _GNU_SOURCE
#include "client.h"
int PROTOCOL = 0;
#define HIGH_PORT 443
#define LOW_PORT 444

volatile int begin_make_epollout = 0;

char html_high[] = 
"POST /index.html HTTP/1.1\r\n"
"Host: 192.168.100.120:443\r\n"
"\r\n"
"sfwopfejwpfwkfpwkwpkpwkpkwpokfkpwkkwkfwkfpfkffwopfqkopwfpkfkokfqokwkfokfpwkfkfokofkokrkiwopkifropkokkfwlofkowkfopwkffkpffkpfffkkpewfkfpkfkfkkksjeiowjfwoiwofjwofjjsljflsjeoeijeowjfjiowfjofjfjfjwfjwfjwfjwfowfjflfjlfjljwewfoewijwow";

char html_low[] = 
"POST /index.html HTTP/1.1\r\n"
"Host: 192.168.100.120:443\r\n"
"\r\n";



static int getPktTypeNum(const int upper_bound)
{
	pktTypeCounter += 1;
	if (pktTypeCounter <= upper_bound) {
		return REQUEST_PKT;
	} else if (pktTypeCounter >= upper_bound && pktTypeCounter < RATE_ACCURACY) {
		return HEARTBEAT_PKT;
	} else {
		pktTypeCounter = 0;
		return HEARTBEAT_PKT;
	}
}


static int getPktType(const int upper_bound) 
{
	return getPktTypeNum(upper_bound);
}


static int addConnections(const int conns) 
{
	int conns_per_port, port, restore_left;
	int ports = conf->ports_count;
	conns_per_port = conns / ports;

	for (port=0; port<ports; port++) {
		restore_left = restoreConnections(params->efds[port], conns_per_port, conf->epoch, conf->interval);
#ifdef DEBUG
		printf("add %d connections, left %d connections\n", conns_per_port, restore_left);
#endif
		createEvents(restore_left, params->server[port], params->efds[port], params->ctxs[port]);
		params->connections[port] += conns_per_port;
	}
	return 0;
}


static int deleteConnections(const int conns)
{
	int conns_per_port, port;
	int ports = conf->ports_count;
	conns_per_port = conns / ports;

	for (port=0; port<ports; port++) {
		params->close_conns[port] += conns_per_port;
	}
	return 0;
}


static int updateConnections(const int prior_conns, const int conns)
{
	int diff;
	if (prior_conns < conns) {
		diff = conns - prior_conns;
		return addConnections(diff);
	} else if (prior_conns > conns) {
		diff = prior_conns - conns;
		return deleteConnections(diff);
	} else {
		return 0;
	}
}


void *update_cfg(void *context)
{
    int flag = 0;
    int prior_conns;
	while (1) {
		sleep(5);
        prior_conns = conf->connections;
		flag = updateEpoch(conf);
		if (conf->kill == 1) {
#ifdef DEBUG
			printf("kill the program. close all the connections...\n");
#endif
			closeAllConnections();	
#ifdef DEBUG
			printf("closed successfully. exit!\n");
#endif
			exit(1);
		}
        if (flag == INCRE || flag == DECRE) {
            updateConnections(prior_conns, conf->connections);  
        }
	}
}


static void getAliasIPs()
{
	assert(params->client_ips == 0);

	struct ifaddrs *ifaddr, *ifa;
	int family, s;
	char host[NI_MAXHOST];
	
	if (getifaddrs(&ifaddr) == -1) {
		perror("getifaddr");
		exit(EXIT_FAILURE);
	}
	for (ifa=ifaddr; ifa!=NULL; ifa=ifa->ifa_next) {
		if (ifa->ifa_addr == NULL) {
			continue;
		}
		family = ifa->ifa_addr->sa_family;
		if (family != AF_INET || strstr(ifa->ifa_name, conf->dev) == NULL) {
			continue;
		}

		s = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in), 
						host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
		if (s != 0) {
			perror("getnameinfo");
			exit(EXIT_FAILURE);
		}
		strcpy(params->aliasIPs[params->client_ips], host);
#ifdef DEBUG
		printf("ip_index: %d, ip: %s\n", params->client_ips, params->aliasIPs[params->client_ips]);
#endif
		if (params->client_ips >= MAXIPS) {
			break;
		}
		params->client_ips += 1;
	}
	freeifaddrs(ifaddr);
}

static int tls_priority(int fd)
{
    static int counter = 0;
    static int sum = 0;

    sum ++;
    if (counter*1.0/sum < 0.05) {
        counter++;
        packets_time[fd].priority = 1;
		return HIGH_PORT;
    } else {
        packets_time[fd].priority = 0;
    }

	return LOW_PORT;
}


static void createClients()
{
	int ips = params->client_ips;
	assert(ips < MAXIPS);
	int ip;
	for (ip=0; ip<ips; ip++) {
		bzero (&(params->clients[ip]), sizeof(struct sockaddr_in));
		params->clients[ip].sin_family = AF_INET;
		params->clients[ip].sin_port = htons(0);
		params->clients[ip].sin_addr.s_addr = inet_addr(params->aliasIPs[ip]);
	}
}


static void createServer()
{
	char dest_ip[NI_MAXHOST];
	int n, count;
	strcpy(dest_ip, conf->dest_ip);
	count = conf->ports_count;

    if ((gethostbyname(dest_ip)) == NULL) {
		perror ("gethostbyname error. \n");
		exit (1);
	}
	for (n=0; n<count; n++) {
		bzero (&(params->server[n]), sizeof(struct sockaddr_in));
		params->server[n].sin_family = AF_INET;
		params->server[n].sin_port = htons(conf->ports[n]);
		params->server[n].sin_addr.s_addr = inet_addr(dest_ip);
	}
}


static void pauseSignal(int *counter, const int thres)
{
	*counter += 1;
	if (*counter >= thres) {
		sleep(PAUSETIME);
		*counter = 0;
	}
}


static void initTLS()
{
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
}

static int ssl_connect(int fd, SSL_CTX *ctx)
{
    int err;
	packets_time[fd].ssl = SSL_new(ctx);
    SSL *ssl = packets_time[fd].ssl;
//    CHK_NULL(ssl);
    SSL_set_fd(ssl, fd);
    SSL_set_connect_state(ssl);

    for (;;) {
        int success = SSL_connect(ssl);

        if (success < 0) {
            err = SSL_get_error(ssl, success);
            if (err == SSL_ERROR_WANT_READ ||
                err == SSL_ERROR_WANT_WRITE) {
                continue;    
            } else {
				printf("free ssl in ssl connect!\n");
                free(packets_time[fd].ssl);
                return -1;
            }          
        } else {
            break;
        }
    }

    return 0;
}



static void createEvents(const int connections, struct sockaddr_in server, const int efd, SSL_CTX *ctx)
{
	int sockfd;
	struct epoll_event event;
	int average, remainder;
	int ip, ips, connection;
	int error;
	int total = connections;
	int connections_counter = 0;
	struct sockaddr_in *client;
	struct timeval time, start, end, now;
	int dest_port = 443;

	ips = params->client_ips;

		average = total / ips;
		if (average > MAX_CONN_PER_PORT) {
			average = MAX_CONN_PER_PORT;
			total = ips * average;
#ifdef DEBUG
			printf("Run out of ports, reduce the number of connections to %d.\n", total);
#endif
		}
		remainder = total % ips;

		//gettimeofday(&time, NULL);
		gettimeofday(&start, NULL);
		for (ip=0; ip<ips; ip++) {
			client = &(params->clients[ip]);
			for (connection=0; connection<average; connection++) {
				if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
					perror ("socket error. \n");
					exit(EXIT_FAILURE);
				}
				if ((bind(sockfd, (struct sockaddr*)client, sizeof(struct sockaddr_in))) < 0) {
					perror ("bind error. \n");
					continue;
				}
				make_socket_nonblocking(sockfd);
#ifdef DEBUG
				if (connection % DEBUG_GROUP == 0 ) {
					printf("fd: %d, connection: %d, ip: %d, total: %d, avg: %d\n", sockfd, connection, ip, total, average);
				}
#endif

				/* high 443, low 444 */
				//dest_port = tls_priority(sockfd);
				//server.sin_port = dest_port;
				if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0) {
					if (errno != EINPROGRESS) {
						close(sockfd);
						perror ("connect error.\n");
						continue;
					}
				}
				if (packets_time[sockfd].ssl == NULL) {
					ssl_connect(sockfd, ctx);
					dest_port = tls_priority(sockfd);
				}

				setEventTimeEfd(efd, sockfd);
				pauseSignal(&connections_counter, PAUSETHRES);
				// set virtual event send time and recv time
				//gettimeofday(&time, NULL);
				setVirtualEventTime(sockfd, total, &time, conf->epoch, conf->interval, conf->unit_delay);
			}
		}
#if 1
		gettimeofday(&now, NULL);
		for (ip = 0; ip < total; ip++) {
			if (packets_time[ip].efd == 0)
				continue;
			setVirtualEventTime(ip, total, &now, conf->epoch, conf->interval, conf->unit_delay);
		}
#endif
	gettimeofday(&end, NULL);
//#ifdef DEBUG
	fprintf(stderr, "time: %d\n", getTimeDiff(&start, &end));
//#endif
}

/* Added by wanwenkai*/
static int create_http_requests(char *buffer, int len, int priority)
{
    memset(buffer, 0, len);

    if (priority || PROTOCOL) {
        strcat(buffer, "POST /index.html HTTP/1.1\r\n");
        strcat(buffer, "Host: 192.168.100.120:443\r\n");
        strcat(buffer, "\r\n");
        strcat(buffer, "sfwopfejwpfwkfpwkwpkpwkpkwpokfkpwkkwkfwkfpfkffwopfqkopwfpkfkokfqokwkfokfpwkfkfokofkokrkiwopkifropkokkfwlofkowkfopwkffkpffkpfffkkpewfkfpkfkfkkksjeiowjfwoiwofjwofjjsljflsjeoeijeowjfjiowfjofjfjfjwfjwfjwfjwfowfjflfjlfjljwewfoewijwow");
    } else {
//        buffer = "sjfwofjwojfwofjwofjwojfwojfwojfwojfowjfowjojoojofjfo2o";
        strcat(buffer, "GET /index.html HTTP/1.1\r\n");
        strcat(buffer, "Host: 192.168.100.120:443\r\n");
        strcat(buffer, "\r\n");
    }

    return strlen(buffer);
}
/* end */


static int __ssl_write(SSL *ssl, char *payload_buf, int len)
{
    len = SSL_write(ssl, payload_buf, len);
    if (len < 0) {
        perror("ssl write error!\n");
    }
    
    return len;
}


static int ssl_read(SSL *ssl, char *buf)
{
    SSL_read(ssl, buf, BUFF_SIZE);
}

static void close_connect(const int efd, const int port)
{
	int close_conns;

	close_conns = params->close_conns[port];
	if (close_conns > 0) {
		closeConnections(efd, close_conns);	
		params->connections[port] -= close_conns;
		params->close_conns[port] -= close_conns;
	}
}


static void ssl_write_error(const int efd, int fd, int count, const int port)
{
	if (count == -1) {
		if (errno != EINTR && errno != EWOULDBLOCK && errno != EAGAIN) {
			perror ("write error.");
			deleteEpollEvent(efd, fd);
			params->connections[port] -= 1;
		}
	} else if (count == 0) {
		// the connection has been closed
		printf ("the connection has been closed\n");
		deleteEpollEvent(efd, fd);
		params->connections[port] -= 1;
	}
}


static int ssl_write(int fd)
{
	int count;

	if (packets_time[fd].priority)
		count = __ssl_write(packets_time[fd].ssl, html_high, sizeof(html_high)-1);
	else 
		count = __ssl_write(packets_time[fd].ssl, html_low, sizeof(html_low)-1);
	return count;
}


static void events_deal_epollout(int fd, uint32_t events,const int efd, const int port, int proportion)
{
	int count;

	if (!(events & EPOLLOUT))
		return;

	if (controlSendingRate(fd, proportion) == STALL) {
		setEventSendTime(fd, NULL, conf->interval);
		return;
	}

	count = ssl_write(fd);
	if (count > 0) {
		increSendPkts();
		modifyEpollEvent(efd, fd, EPOLLIN);
		setEventSendTime(fd, NULL, conf->interval);
	} else {
		ssl_write_error(efd, fd, count, port);
	} 
}


static void events_deal_err_epollin(int fd, uint32_t events, const int efd, const int port)
{
	int count;
	char buf[BUFF_SIZE];

	if (!(events & EPOLLIN))
		return;

	count = ssl_read(packets_time[fd].ssl, buf);
	if (count == -1) {
		if (errno != EAGAIN && errno != EWOULDBLOCK && errno != EINTR) {
			//perror("read error");
			deleteEpollEvent(efd, fd);
			params->connections[port] -= 1;
		}
		return;
	} else if (count == 0) {
		// the connection has been closed
		perror("the connection has been closed by server.\n");
		deleteEpollEvent(efd, fd);
		params->connections[port] -= 1;
		return;
	}
	setEventRecvTime(fd, NULL);
	increReceivePkts(getRTT(fd));
}

static void events_deal(struct epoll_event* events, int proportion, const int port, const int efd)
{
	int i, fd, nfds;

	nfds = epoll_wait(efd, events, MAXCONNECTIONS, -1);
	for (i = 0; i < nfds; i++) {
		fd = events[i].data.fd;
		if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP)) {
			deleteEpollEvent(efd, fd);
			params->connections[port] -= 1;
			continue;
		} 
		events_deal_epollout(fd, events[i].events, efd, port, proportion);
		events_deal_err_epollin(fd, events[i].events, efd, port);
	}
}

static void epollLoop(const int efd, const int port, SSL_CTX *ctx)
{
	int proportion;
	struct epoll_event * events;
	int random_current = 0;
	int random_last = 0;

	events = calloc(MAXCONNECTIONS, sizeof(struct epoll_event));
	while (1) {
		close_connect(efd, port);
		random_current = getRandomCounter();
		if (random_current != random_last) {
			proportion = myRandom(random_current);
			printf("current: %d, previous: %d, proportion: %d\n", random_current, random_last, proportion);
			random_last = random_current;
		}
		events_deal(events, proportion, port, efd);
	}
	free(events);
}

static SSL_CTX *createCtx()
{
	return SSL_CTX_new(SSLv23_client_method());
}


void *epoll_client(void *context)
{	
	struct epoll_thread *param = (struct epoll_thread*)context;
	int efd, port;
	SSL_CTX *ctx;

	port = param->port;
	ctx = createCtx();	
	efd = createEpoll();
	params->efds[port] = efd;
	params->ctxs[port] = ctx;
	params->connections[port] = param->total;
	createEvents(param->total, params->server[port], efd, ctx);
	begin_make_epollout = 1;
	epollLoop(efd, port, ctx);

	return 0;
}

static void initGlobal()
{
	int i;
	params = malloc(sizeof(struct global_params));
	params->client_ips = 0;
	for (i=0; i<MAXPORTS; i++) {
		params->efds[i] = 0;
		params->connections[i] = 0;
		params->close_conns[i] = 0;
		params->failed_conns[i] = 0;
	}
}


static void initParams()
{
    conf = malloc(sizeof(struct epoll_configs));
	initConfigs(conf);
	initGlobal();
}


static void init()
{
	initParams();
	getConfigs(conf);
	getAliasIPs();
	initEventTime();
	initIDMap(conf->SID_start, conf->SID_end);
	initKeyTable();
	initTLS();
	createClients();
	createServer();
}


void *updateEvents(void *context)
{
	int fd, mask;
	struct timeval now;

	while (1) {
		/* added by wanwenkai */
		if (begin_make_epollout == 0) {
			usleep(10);
			continue;
		}
		/* end */
		for (fd = 0; fd<MAX_EVENTS; fd++) {
			if (packets_time[fd].efd == 0) {
				continue;
			}
			if (canSendPkt(fd, conf->epoch) == TRUE) {
				modifyEpollEvent(packets_time[fd].efd, fd, EPOLLOUT);
			}
		}
	}
}

static int mcc_parse_argc(int argc, char **argv)
{
    int opt, ret, timer_secs;
    char *string = "fp:o:t";

    while ((opt = getopt(argc, argv, string)) != -1) {
        switch (opt) {
            case 'f':
                PROTOCOL = 1;
                break;
            case 'p':       //protocol
                break;
            case 'o':
                break;
            case 't':
                break;
            default:
                return 0;
        }
    }

    return 0;
}


int main(int argc, char *argv[])
{
	int rc, n, total, ports;
	struct epoll_thread param[THREADNUM];
	cpu_set_t cpus;

	pthread_t client_thread[THREADNUM], throughput_sec_thread, throughput_min_thread, update_thread, admin_thread;
	pthread_t update_events_thread;
	pthread_attr_t attr;
	pthread_attr_init(&attr);

	init();
	ports = conf->ports_count;
	total = conf->connections;
#ifdef DEBUG
	printf ("the total connections is %d\n", total);
#endif
	if (total == 0) {
		printf("the number of connnections is zero!\n");
		return 0;
	}

	for (n=0; n<ports; n++) {
		param[n].total = total/ports;
		param[n].port = n;

		CPU_ZERO(&cpus);
		CPU_SET(n+1 , &cpus);
		pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpus);
		rc = pthread_create(&client_thread[n], &attr, epoll_client, &param[n]);
		if (rc) {
			printf("ERROR.\n");
			exit(-1);
		}
	}
#ifdef TPUT
	rc = pthread_create(&throughput_sec_thread, &attr, throughput_sec, NULL);
    if (rc) {
		printf("ERROR.\n");
		exit(-1);
	}
	rc = pthread_create(&throughput_min_thread, &attr, throughput_min, NULL);
    if (rc) {
		printf("ERROR.\n");
		exit(-1);
	}
#endif
	CPU_ZERO(&cpus);
	CPU_SET(ports+1 , &cpus);
	pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpus);
	rc = pthread_create(&update_thread, &attr, update_cfg, 0);
    if (rc) {
		printf("ERROR.\n");
		exit(-1);
	}
	CPU_ZERO(&cpus);
	CPU_SET(ports+2 , &cpus);
	pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpus);
	rc = pthread_create(&update_events_thread, &attr, updateEvents, 0);
    if (rc) {
		printf("ERROR.\n");
		exit(-1);
	}
	pthread_exit(NULL);
}
