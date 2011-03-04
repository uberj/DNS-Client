#include "constructor.h"

/*
 *  I'm sure string.h has a function for this. Take a sub string and copy it into another.
 *  idx1 is inclusive
 *  idx2 is exclusive *cough* java *cough*
 *  precondition: idx1 < idx2
 */
void _sub_string(int idx1, int idx2, char *string , unsigned char *dest_str){
    while(idx1 < idx2){
        dest_str[idx1] = string[idx1];
        idx1++;
    }
    return;
}
/*
 *  Sure this function is laying around in some lib. oh well!
 *  copy size number of byets from block into dest.
 */
void _byte_copy(unsigned char *block,unsigned char *dest, int size){
    int i = 0;
    while(i<size){
        dest[i]=block[i];
        i++;
    }
    return;
}
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
int build_header(struct DNS_REQUEST* data){
    // Byte offset we are at in the packet
    int offset = 0;
    // char is 1 byte, 8 bits
    // Bits 0-15
/* Line 0 */
    _byte_copy(data->id,data->query+offset,2);    
    offset = offset + 2;

/* Line 1 */
    // First byte
    _byte_copy(&data->flags1,data->query+offset,2);    
    offset = offset + 1;
    // Second byte
    _byte_copy(&data->flags2,data->query+offset,2);    
    offset = offset + 1;

/* Line 2 */
    _byte_copy(data->qdcount,data->query+offset,2);    
    offset = offset + 2;

/* Line 3 - 5 */
    _byte_copy(data->cruft,data->query+offset,6);    
    offset = offset + 6;

    data->query[offset] = '\0'; // So we can print stuff

    return 0;

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
int build_query(struct DNS_REQUEST* data, char *query_addr){
    // char '0' is 48
    unsigned char count;
    int size = strlen(query_addr);
    unsigned char temp[size+4];

    // this is going to have to be debugged.
    int i = 0;
    int p = 0;
    count = '\0';
    while(1){
        if(query_addr[i] == '.' || query_addr[i] == '\0'){
            temp[p]= count;
            _byte_copy((unsigned char *)&query_addr[p],&temp[p+1],count);
            count = '\0';
            p=i+1;
        }else{
            count += '\1';
        }
        i++;
        // This is kind of weird, but necissary. We need to execute the copy
        // statment one extra time to get the last .xxx string
        if(query_addr[i-1] == '\0') break;
    }
    temp[i] = 'A'-'A'; // Null at the end
    if(i != size+1){
        printf("Sanity check failed in build_query\n");
    }
    
    // Copy in the qtype on qclass bytes
    _byte_copy(data->qtype,&temp[size+2],2);
    _byte_copy(data->qclass,&temp[size+4],2);

    // Now copy the entire temp var into the actual message
    _byte_copy(temp,data->query+DNS_HEADER_SIZE,size+6);
    // Need this for sending
    data->size=DNS_HEADER_SIZE+size+6;

    return 0;
}
