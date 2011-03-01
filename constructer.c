#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include "dns_c.h"

void _sub_string(int idx1, int idx2, char *string , char *dest_str);
int build_query(char *query_addr);

/*
 * 
     0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                      ID                       |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |QR|   Opcode  |AA|TC|RD|RA|   Z    |   RCODE   |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    QDCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    ANCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    NSCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    ARCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
  
 */
/*
 * Building a query any way I can.
 * Pass in an address that should be looked up and a
 * DNS_REQUEST struct.
 */
int build_header(struct DNS_REQUEST data){
    // char is 1 byte, 8 bits
    // Bits 0-15
/* Line 0 */
    unsigned char id[2]; // Bits 0-15 are the query identifier

/* Line 1 */
    // First byte
    unsigned char flags1; // 0      : Bit one Query or response
                          // 1 - 4  : OPcode, set 0 for standard
                          // 5      : AA, set to 1 for response 0 for query
                          // 6      : TC, truncated
                          // 7      : RD, Recursion desired 
    // Second byte
    unsigned char flag2;  // 0      : RA, recursion available
                          // 2-3    : set to zero
                          // 4-7    : Rcode, set 0's for client
/* Line 2 */
    unsigned char QDcount[2]; // how many queries
/* Line 3 - 5 */
    unsigned char cruft[6]; // Stuff for the server to fill out.


}

/*
      0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                                               |
    /                     QNAME                     /
    /                                               /
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                     QTYPE                     |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                     QCLASS                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
*/
/*
 *  Tricky funtion. Take a string with '.' in it. Start at the first char and count in x until
 *  you hit the first '.'. Write X in hex to another string, then all the characters
 *  you read over in the other string. Continue until you reach null term.
 *
 *  ex: www.google.com
 *      3www6google3com
 */
int build_query(struct DNS_REQUEST data, char *query_addr){
    // char '0' is 48
    unsigned char count = '0';
    unsigned char temp[strlen(query_addr)];
    int p = 0;
    count = count-48;
    // this is going to have to be debugged.
    for(int i=0; query_addr[i] != '\0';i++){
        if(query_addr[i] == '.'){
            temp[p]= count;
            _sub_string(p,i,query_addr,temp[p+1]);
            count = '0';
            count = count - 48;
            p=i;
        }else{
            count = count + 1;
        }
    }
    strcp(data->query+DNS_HEADER_SIZE,query_addr);
    // Should there be a null terminator in the query?
}
/*
 *  I'm sure string.h has a function for this. Take a sub string and copy it into another.
 *  idx1 is inclusive
 *  idx2 is exclusive *cough* java *cough*
 *  precondition: idx1 < idx2
 */
void _sub_string(int idx1, int idx2, char *string , char *dest_str){
    while(idx1 < idx2){
        dest_str[idx1] = string[idx2];
        idx1++;
    }
    return;
}
