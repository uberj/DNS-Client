#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define STDIN 0
#define MAX_MSGSIZE 500

void load_handle(char *array,char* handle);
void print_handle(char *array);
void get_handle(char *array);
void get_msg(char *msg, char *handle);
void get_port(char *PORT, char *arg);
int get_socket(char *PORT);
int wait_and_listen(int listener);
