#include "c_utils.h"

/*
 *  function: takes a socket and listens for connections. Uses accept to return a new
 *      file descriptor.
 *  param: file descriptor to listen to.
 *  return: live fd to pass data into and receive from.
 */
int wait_and_listen(int listener){
    int newfd;
    struct sockaddr_storage remoteaddr;
    socklen_t addrlen;
    /*
     * Wait for new connections.
     */
    if (listen(listener, 10) == -1) {
        perror("listen");
        exit(3);
    }
    /*
     *  A client has connected to our server.
     */
    addrlen = sizeof remoteaddr;
	newfd = accept(listener,(struct sockaddr *)&remoteaddr,&addrlen);
	if (newfd == -1) {
        perror("accept");
    }
    return newfd;
}
/*
 *  function: Used to open a tcp connected and create a file descriptor to write into that connection.
 *  param:  PORT - what to bind the socket to.
 *          remote_host - what remote (chat server in out case) to connect to.
 */
int client_get_socket(char *PORT, char *remote_host){
	int rv;
    int sockfd; // Socket we will return.
	struct addrinfo hints, *servinfo, *p;
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	rv = getaddrinfo(remote_host, PORT, &hints, &servinfo);
    if(rv != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}
	// loop through all the results and connect to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("client: socket");
			continue;
		}
		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("client: connect");
			continue;
		}
		break;
	}
	if (p == NULL) {
		fprintf(stderr, "client: failed to connect\n");
		return 2;
	}
	freeaddrinfo(servinfo); // all done with this structure
    return sockfd;
}
/*
 *  function: Create a SOCK_STREAM socket bound to PORT.
 *  param: Port to bind to.
 *  return: a valid "connected" file descriptor.
 */
int get_socket(char *PORT){
    int rv;
    int yes=1;
    int listener;
    struct addrinfo hints, *ai, *p;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	if ((rv = getaddrinfo(NULL,PORT, &hints, &ai)) != 0) {
		fprintf(stderr, "selectserver: %s\n", gai_strerror(rv));
		exit(1);
	}
	for(p = ai; p != NULL; p = p->ai_next) {
    	listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (listener < 0) { 
			continue;
		}
		// lose the pesky "address already in use" error message
		setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
		if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) {
			close(listener);
			continue;
		}
		break;
	}
	// if we got here, it means we didn't get bound
	if (p == NULL) {
		fprintf(stderr, "selectserver: failed to bind\n");
		exit(2);
	}
	freeaddrinfo(ai); // all done with this
    return listener;
}
/* 
 * NOT USED
 */
void load_handle(char *array,char* handle){
    int i;
    for (i=0; i<sizeof handle;i++){
        array[i]=handle[i];
    }   
}
/* 
 * NOT USED
 */
void print_handle(char *array){
    int i;
    for (i=0; array[i] != '\0';i++){
        printf("%c",array[i]);
    }
}
/* 
 * Used by client to choose a users handle.
 */
void get_handle(char *array){
    char handle[11];
    int x;
    printf("Enter a handle less at most 10 chars long: ");
    fgets(handle,10,stdin);
    printf("You chose %s",handle);
    x = strlen(handle);
    handle[x-1]='\0'; // We don't want the new line.
    strcpy(array,handle);
}

/*  
 *  function: Get's the message a user wants to send to their peer and prepends their
 *      handle to that message.
 *  param:  msg - message they want to send
 *          handle - the peer's handle.
 *  return: args passed by reference.
 */
void get_msg(char *msg, char *handle){
    int i,m;
    char text[500];
    memset(text,0,500);

    fgets(text,sizeof text,stdin);
    m=strlen(handle);
    strcpy(msg,handle);
    msg[m++]='?';
    i=0;
    while(text[i] != '\n'){
        msg[m]=text[i];
        i++;
        m++;
    }
    msg[m]='\0';
}
/* 
 * Utility function to do a strcpy()  :\
 */
void get_port(char *PORT, char *arg){
    int i;
    for (i=0; i<strlen(arg);i++){
        PORT[i]=arg[i];
    }
    PORT[i]='\0';
}
