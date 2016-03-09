#include <stdio.h>
#include <stdlib.h>

int main(void){
	
}

void printarray(int *v){
	for (i=0;i<sizeof(v);i++){
        printf("%d\n",*v+i);
}