#include <stdio.h>
#include <stdio.h>
/*This is a simple file writing program to play with the mechanics*/
int main(){
	  FILE * file = fopen("output.txt","w");
	  char * start = "{\"list\" : [";
 	  fwrite(start, 1 , sizeof(start) , file);
 	  char * start2 = "IM BACK IN BLACK\n : [";
 	   	  fwrite(start2, 1 , sizeof(start2) , file);

 	  fclose(file);
 	  return 0;
}