#include  "mymalloc.h"
#include  <stdio.h>
#include  <string.h>


int main(int argc, char*argv[]){

        int i;
        printf("\tFreeing pointer %p\n",&i);
        free(&i);
        printf("Done\n");


        return 0;
 

}