#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include "socket_utils.h"
#include "constructor.h"
#include "parser.h"
#include "dns_c.h"


int _hex_print( unsigned char* array, int size);

int main( int argc, char **argv ){
    struct DNS_REQUEST question;
    unsigned char answer[512];
    /*
     *  Ahead of all else, make the head.
     */
    //TODO write some freakin macro's
    question.id[0] = 'A';           //1010
    question.id[1] = 'B';           //1010
    question.flags1 = '\1';      //0001
    question.flags2 = '\0';      //0000
    question.qdcount[0]= '\0';   //0000
    question.qdcount[1]='\1';    //0001
    // query get's filled in at run time
    question.qtype[0] = '\0';    //0000
    question.qtype[1] = '\1';    //0001
    question.qclass[0] = '\0';   //0000
    question.qclass[1] = '\1';   //0001
//    int qsize = ( sizeof(unsigned char) * strlen(argv[1]) ) + 5;
    //question.query = (unsigned char *) malloc( qsize );
    unsigned char query[19];
    question.query = &query;
    // Fill in that cruft
    int i;
    for(i=0;i<6;i++){
        question.cruft[i]='\0';          //0000
    }

    build_header(&question);
    printf("HEADER:\n");
    _hex_print(question.query,12);

    /*
     * Get the query in the query.
     */

    // start the query where we left off

    // We want the len of the RR to be size of the request url, 1 null byte, and 4
    // bytes for qtype and class
    build_query(&question,argv[1]);
    // Print all the bytes in the DNS_HEADER, the request and the 4 bytes 
    // of qtype qclass data.
    printf("MESSAGE:\n");
    _hex_print(question.query,DNS_HEADER_SIZE+strlen(argv[1])+6);

    /*
     *  Sending request
     */
    send_request(&question,&answer);
    printf("RESPONSE:\n");
    _hex_print(&answer,512);
    parse_answer(&answer,&question);
    return 0;

}

int _hex_print( unsigned char* array, int size){
    int i;
    printf("hex string: \n");
    for(i=0;i<size;i++){
        printf("%02hhX ",array[i]);
        if((i+1)%16==0)
            printf("\n");
    }
    printf("\n");
    return 1;
}
