#include  "mymalloc.h"
#include  <stdio.h>
#include  <string.h>



#define BLOCK 100

int main(int argc, char*argv[]){

    int casenum;


    if(argc != 2){
        printf("ENTER VALID ARGS!!\n");
    }
    casenum = atoi(argv[1]);


    printf("Case num: %d\n",casenum);

    if(casenum == 1){
        int  * ptr;
        free(ptr);
        printf("Done\n");

    }else if(casenum == 2){
        char * c = malloc(BLOCK);
        free(c-(BLOCK/2));
        printf("Done\n");

    }else if(casenum == 3){
        char * c = malloc(BLOCK);
        free(c);
        free(c);
        printf("Done\n");

    }else if(casenum == 4){
        char * c = malloc(BLOCK);
        free(c+12);
        printf("Done\n");
    }else if(casenum == 5){
        int *ip1;
        printf("\tTA: Mallocing pointer\n");
        ip1 = malloc(10*sizeof(int));
        printf("\tTA: Freeing pointer %p\n",ip1);
        free(ip1);
        printf("\tTA: Freeing pointer %p\n",ip1);
        free(ip1);
        printf("Done\n");


    } else if(casenum ==6){
        char * c = malloc(BLOCK);
        free(c-2);
        printf("Done\n");
    }else if (casenum ==7){
        char * ptr0 = malloc(BLOCK);
        char * ptr1 = malloc(BLOCK);
        char * ptr2 = malloc(BLOCK);
        free(ptr0);
        free(ptr1);
        free(ptr2);
        free(ptr2);
    
    }else if(casenum == 8){

        int *a1;
        int *a2;
        a1 = malloc(sizeof(int));
        a2 = malloc(sizeof(int));
        *a1 = 123;
        *a2 = 512;
        int temp;
        temp = *a1;
        *a1 = *a2;
        *a2 = temp;
        printf("a1 = %d, a2 = %d\n",*a1,*a2);
        free(a1);
        free(a2);
        printf("Done\n");
    }else if(casenum == 9){
       char ** m;
       int count = 0;
       m = malloc(10*sizeof(char*));
       count ++;
       int i = 0;
       for(i = 0; i < 10; i++){
        m[i]=malloc(sizeof(char) *500);
        if(m[i]!=0){
            count++;
        }
       }
       i = 0;
       for(i = 0; i < 10; i ++){
        free(m[i]);

       }
       free(m);
       int count1 = 0;
       m = malloc(10*sizeof(char*));
       count1 ++;
       i = 0;
       for(i = 0; i < 10; i++){
        m[i]=malloc(sizeof(char) *500);
        if(m[i]!=0){
            count1++;
        }
       }
       i = 0;
       for(i = 0; i < 10; i ++){
        free(m[i]);
       }
       free(m);
       if(count1 == count){
        printf("PASSED\n");
       }else{
        printf("FAILED\n" );
       }

    }else if(casenum == 10){
        int *x;
        int *y;
        printf("Mallocing pointer\n");
        x = malloc(sizeof(int));
        printf("Mallocing pointer\n");
        y = malloc(4*sizeof(int));
        free(y);
        free(x); 
    }
    else if(casenum == 11){
        int *x;
        int *y;
        printf("Mallocing pointer\n");
        x = malloc(sizeof(int));
        printf("Mallocing pointer\n");
        y = malloc(4*sizeof(int));
        free(x);
        free(y); 
        printf("DONE");
    } else if(casenum == 12) {
        //check if data from head - mementry can be free
        //shouldn't break
        int *x = malloc(sizeof(int));
        free((MemEntry*)x-1);
        printf("attempting to free\n");
        free(x);
        printf("Done\n");
    } else if (casenum==13) {
        int *x = malloc(sizeof(int) *14);
        int *y = malloc(sizeof(int));
        int *j = malloc(sizeof(int) *2);
        int *i = malloc(sizeof(int) *4);
        int *z = malloc(sizeof(int) *3);
        free(j);
        free(z);
        free(x);
        free(y);
        free(x);
        free(z);
        free(i);



    }
    return 0;
}