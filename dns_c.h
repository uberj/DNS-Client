#define SIZE 200
#define NAME_SERVER "ns1.oregonstate.edu"
#define LISTEN_PORT 7001
#define DNS_PORT 53

struct DNS_REQUEST{
    unsigned char query[SIZE];
    int size;
}
