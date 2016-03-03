#include  "mymalloc.h"
//#include  <unistd.h>
#include  <stdio.h>
#include  <string.h>


int main(int argc, char*argv[]){

        //    1. free pointer never allocated
        int i;
        int *ip;
        printf("\tTA: Freeing pointer %p\n",&i);
        free(&i);
        printf("Done\n");


        return 0;
      //    2. free pointer in middle of allocated memory block
 

}