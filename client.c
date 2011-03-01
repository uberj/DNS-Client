#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include "dns_c.h"

int main( int argc, char **argv ){
    printf("hex input: %02hhX",argv[1][0]);
    return 0;
}
