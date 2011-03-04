#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include "dns_c.h"

int build_query(struct DNS_REQUEST* data, char *query_addr);
int build_header(struct DNS_REQUEST* data);
