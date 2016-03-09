'#include <stdio.h>
#include <stdlib.h>

int main()
{
    int num1,num2,nume1, nume2,mmdc;

    printf("Numero 1: \n");
    scanf("%d", &nume1);
    printf("Numero 2: \n");
    scanf("%d", &nume2);
mmdc=mdc(nume1,nume2);

    printf("o MDC de %d e %d e %d \n",nume1,nume2,mmdc);
}

int mdc(num1,num2){
    int i,hcf;
for(i=1;i<num1 || i<num2;++i){
        if(num1%i==0 && num2%i==0)
            hcf=i;
    }
    return hcf;
    }
