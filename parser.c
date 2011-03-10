#include "parser.h"

/*
 *  Print a pointer. If it finds a nother pointer while it is printing it will call it's self recursivly on that
 *  new pointer.
 *  array - 0th index of array
 *  i - int pointint to the offset
 */
int _print_pointer( unsigned char* array, int i){
    unsigned char tempanswer[512]; // Need this for resolving pointers
    int offset = (int)array[i+1];

    while( array[offset] != 0x00 ){
        // It's a pointer print it, then move up i past it
        if(array[offset] == 0xC0){
           memcpy(&tempanswer,array,SIZE_OF_RESP);
           _print_pointer(array,offset);
           array[offset] = '\0'; //We don't want to resolve this pointer agian. It could cause an infanite loop.
           offset+=2;
           break;
        }
        if(array[offset]>0x21){
            printf("%c",array[offset]);
        }else{
            printf(".");
        }
        offset++;
    }
    return 1;
}
/*
 *  I'm sure this function is laying around in some lib. oh well!
 *  copy size number of byets from block into dest.
 *  *cough* memcopy() *cough*
 */
void _byte_copy2(unsigned char *block,unsigned char *dest, int size){
    int i = 0;
    while(i<size){
        dest[i]=block[i];
        i++;
    }
    return;
}
/*
 *  Print out RDATA. array is a pointer to the response. Size if RDLENTH, index is where the data should start.
 */
int _data_print( unsigned char* array, unsigned char size, int index, int ip){
    int i;
    unsigned char tempanswer[512]; // Need this for resolving pointers
    for(i=0;i<size;i++){
        if(array[index+i] == 0xC0){
           memcpy(&tempanswer,array,SIZE_OF_RESP);
           _print_pointer(array,index+i);
           array[index] = '\0'; //We don't want to resolve this pointer agian. It could cause an infanite loop.

           continue;
        }
        if(i+1<size){
            if(ip){
                printf("%i.",array[index+i]);
            }else{
                if(array[index+i]>0x21){
                    if(array[index+i]>0x21){
                        printf("%c",array[index+i]);
                    }else{
                        printf(".");
                    }
                }
            }
        }else{
            if(ip){
            printf("%i",array[index+i]);
            }
        }
    }
    return 1;
}
/*
 *  Used for debugging. Prints out a string of unsigned char's length "size"
 */
int _hex_print2( unsigned char* array, unsigned char size){
    int i;
    for(i=0;i<size;i++){
        printf("%02hhX ",array[i]);
    }
    return 1;
}
/*
 *  Given a dns response in answer, parse all the needed ip's out of it.
 */
int parse_answer(unsigned char *answer, struct DNS_REQUEST *data){
    unsigned char acount;
    unsigned char tempanswer[512]; // Need this for resolving pointers
    int i,index,len;
    int type;
    _byte_copy2(&answer[7],&acount,1);
    printf("there were %i answers\n",acount);
    index = data->size; //Let's where the header ends
    for(i=0;i<acount;i++){
        /* NAME */
        printf("NAME: ");
        memcpy(&tempanswer,answer,SIZE_OF_RESP);
        _print_pointer(tempanswer,index);
        printf("\n");
        index +=2;
        /* TYPE */
        printf("TYPE: ");
        type = answer[index+1];
        _hex_print2(&answer[index],2);
        printf("\n");
        index+=2;
        /* CLASS */
        printf("CLASS: ");
        _hex_print2(&answer[index],2);
        printf("\n");
        index+=2;
        /* TTL */
        printf("TTL (32 bit): ");
        _hex_print2(&answer[index],4);
        printf("\n");
        index+=4;
        /* RDLENTH */
        printf("RDLENGTH: ");
        _hex_print2(&answer[index],2);
        printf("\n");
        len = (int)answer[index+1];
        index+=2;
        printf("DATA: ");
        memcpy(&tempanswer,answer,SIZE_OF_RESP);
        if(type==5){
            _data_print(tempanswer,len,index,0);
        }else{
            _data_print(tempanswer,len,index,1);
        }
        index = index+len;
        printf("\n\n");
    }
    return 1;
}
