#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#if def
#include "dns_c.h"
#endif
#include "constructor.c"

int main( int argc, char **argv ){
    struct DNS_REQUEST question;
    /*
     *  how to get b00000001 ? Take A (0x41) and B (0x42) and subtract!!1
     */
    question.id[0] = 'A'; // arbitrary id
    question.id[1] = 'A';
    question.flags1 = 'B'-'A'; //
    question.flags2 = 'D';
    question.qdcount[0]='E';
    question.qdcount[1]='F';
    question.cruft[0]='G';

    build_header(&question);
    printf("hex input: %s\n",question.query);
//    printf("hex input: %02hhX",question.query);
    return 0;

}
