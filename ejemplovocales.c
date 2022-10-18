#include <stdio.h>
#include <ctype.h>
#include <unistd.h>

int main(){
    char a;
    char mayu;
    int voc=0;
    int n=0;
    do{
        n= read(STDIN_FILENO,&a,1); /*Variable dentro de librería*/
        mayu=toupper(a);
        if (mayu=='A' || mayu=='E' || mayu=='I' || mayu=='O' || mayu=='U'){
            voc ++;
        }
    }while(n!=0);
    printf("El total de vocales es %d\n", voc);
    return 0;
}