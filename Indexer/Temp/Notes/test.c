#include <stdio.h>
typedef struct datastuff
{
	int val0;
	float val1;
	long val2;
};
union otherdata
{
	int val0;
	float val;
	long val2;
};
int main() {
	union otherdata data;
	data.val = 3.125134;
	data.val0 =105;
	float fl = 3.1491234;
	int c = fl;
	int a =2;
	int b = 2;
	printf("%d\n",data.val0);
	printf("%f\n",data.val);
	printf("%ld\n",data.val2);

	//char * cs = &'5';
	//int * p = 5;
	//float * p1 = &a;
	int * p2 = &b;
	int myarray[5];
	myarray[1]=5;
	int * mypointer = myarray;
	mypointer++;
	printf("%d\n",*p2);
	return 0;
//	printf("\n%c, %d",cs,p);
}