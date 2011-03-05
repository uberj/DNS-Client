#include "socket_utils.h"
#include "c_utils.c"

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
    int nb;
    int sockfd = client_get_socket(DNS_PORT,NAME_SERVER);
    send(sockfd,data->query,data->size,0);
    nb = recv(sockfd,answer,512,0);

    return 0;
}
