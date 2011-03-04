#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "dns_c.h"

int send_request(struct DNS_REQUEST* data, unsigned char *answer);
int set_up_socket(struct addrinfo *server);
void *get_in_addr(struct sockaddr *sa);
