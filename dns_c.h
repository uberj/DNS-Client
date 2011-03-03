#define SIZE 512
#define NAME_SERVER "ns1.oregonstate.edu"
#define LISTEN_PORT 7001
#define DNS_PORT "53"
#define DNS_HEADER_SIZE 12

struct DNS_REQUEST{
    unsigned char query[SIZE];
    int size;
    /************************************************
     *              THE HEADER                      *
     ***********************************************/
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
  
 *
 */

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
    unsigned char flags2;  // 0      : RA, recursion available
                          // 2-3    : set to zero
                          // 4-7    : Rcode, set 0's for client
/* Line 2 */
    unsigned char qdcount[2]; // how many queries
/* Line 3 - 5 */
    unsigned char cruft[6]; // Stuff for the server to fill out.

    /************************************************
     *              THE MESSAGE                     *
     ***********************************************/
 
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
    unsigned char qname[SIZE];
    unsigned char qtype[2];
    unsigned char qclass[2];

};
