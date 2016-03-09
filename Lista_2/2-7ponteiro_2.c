#include <stdio.h>
#include <stdlib.h>

int main(void){
	int x = 10;
	int y = 20;
	swap(&x,&y);
	printf("x=%d , y=%d",x,y);

}

void swap(int *p1, int *p2){
	int ctrl;
	ctrl=*p1;
	*p1=*p2;
	*p2=ctrl;
}
