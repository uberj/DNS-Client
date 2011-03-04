#include "socket_utils.h"

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

int set_up_socket(struct addrinfo *server){
    int sockfd;
    struct addrinfo hints, *p;
    int x;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if ((x = getaddrinfo(NAME_SERVER, DNS_PORT, &hints, &server)) != 0) {
        perror("getaddrinfo:\n");
        return 1;
    }

    // loop through all the results and make a socket
    for(p = server; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
            perror("talker: socket");
            continue;
        }
        server = p;
        break;
    }

    if (p == NULL) {
        printf("bind failed\n");
        return 1;
    }

    

    return sockfd;
}

/*
 *  Send data in the DNS_REQUEST struct to the file descriptor sockfd.
 */
int send_request(struct DNS_REQUEST* data, unsigned char *answer){
    struct addrinfo server;
    int s;
    int sockfd;
    struct sockaddr_storage temp;
    socklen_t addr_len;
    sockfd = set_up_socket( &server );
    s = sendto(sockfd, data->query, data->size, 0,server.ai_addr, server.ai_addrlen);
    if (s == -1) {
        perror("talker: sendto");
        exit(1);
    } if(s != data->size){
        printf("Not all data was sent.\n");
        return -1;
    }
    addr_len = sizeof temp;
    s = recvfrom(sockfd,answer,SIZE_OF_RESP,0,(struct sockaddr *)&temp,&addr_len);

    return 0;
}
