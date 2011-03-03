#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#if def
#include "dns_c.h"
#endif
#include "constructor.c"

int _hex_print( unsigned char* array, int size){
    int i;
    printf("hex string: ");
    for(i=0;i<size;i++){
        printf("%02hhX ",array[i]);
    }
    printf("\n");
    return 1;
}

int main( int argc, char **argv ){
    struct DNS_REQUEST question;
    /*
     *  Ahead of all else, make the head.
     */
    /*
     *  how to get b00000001 ? Take A (0x41) and B (0x42) and subtract!!1
     */
    //TODO write some freakin macro's
    question.id[0] = 'A';           //1010
    question.id[1] = 'A';           //1010
    question.flags1 = 'B'-'A';      //0001
    question.flags2 = 'B'-'B';      //0000
    question.qdcount[0]= 'B'-'B';   //0000
    question.qdcount[1]='B'-'A';    //0001
    // query get's filled in at run time
    question.qtype[0] = 'A'-'A';    //0000
    question.qtype[1] = 'B'-'A';    //0001
    question.qclass[0] = 'A'-'A';   //0000
    question.qclass[1] = 'B'-'A';   //0001
    // Fill in that cruft
    int i;
    for(i=0;i<6;i++){
        question.cruft[i]='B'-'B';          //0000
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
    question.RR = (unsigned char*) malloc(sizeof(unsigned char)*strlen(argv[1])+5);
    build_query(&question,argv[1]);
    // Print all the bytes in the DNS_HEADER, the request and the 4 bytes 
    // of qtype qclass data.
    printf("MESSAGE:\n");
    _hex_print(question.query,DNS_HEADER_SIZE+strlen(argv[1])+6);

    
    free(question.RR);
    return 0;

}
