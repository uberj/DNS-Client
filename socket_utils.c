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

int send_request(struct DNS_REQUEST* query, int sockfd);
int set_up_socket(struct addrinfo *name_server);
void *get_in_addr(struct sockaddr *sa);

void *get_in_addr(struct sockaddr *sa){
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
/*
 *  Set up a UDP connection (not really connected) so we can send()
 *  and recv(). 
 *  param: struct addrinfo name_server. info about where to connect to.
 *  return: a file descriptor that can be sent and received from.
 */

int set_up_socket(struct addrinfo *name_server){
    int warn;
    int sockfd;
    struct addrinfo hints, *p;
    memset(&hints,0,sizeof hints);
    hints.ai_family = AF_UNSPEC; //IPv4
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; // Use local IP

    if((warn = getaddrinfo(NULL, DNS_PORT, &hints, &name_server)) != 0){
        printf("failed getting name server addr_info\n");
    }
    for( p = name_server; p != NULL; p = p->ai_next ){
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
            perror("socket create failed\n");
            continue;
        }
        // Set sending port
        if (bind(sockfd, p->ai_addr,p->ai_addrlen) == -1){
            perror("bind failed\n");
            continue;
        }

        break;
    }

    if( p == NULL ){
        printf("We got issues\n");
        return -1;
    }
    freeaddrinfo(name_server);
    return sockfd;
}

/*
 *  Send data in the DNS_REQUEST struct to the file descriptor sockfd.
 */
int send_request(struct DNS_REQUEST* data, int sockfd){
    int s;
    s = send(sockfd,data->query,data->size,0);
    if(s != data->size){
        printf("Not all data was sent.\n");
        return -1;
    }
    return 0;
}
