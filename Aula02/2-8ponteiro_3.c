#include <stdio.h>
#include <stdlib.h>

int main(void){
	int *v;
	int i;
	v= malloc (100 * sizeof (int));
	for (i=0;i<100;i++){
		v[i]=i+1;
        printf("%d\n",*v+i);
	}
}
