#include <stdio.h>
#include <signal.h>
#include <unistd.h>
int con;

void holaMundo(int signalNumber){
    /*registrando holaMundo*/
    if (signalNumber == 10){
        printf("Es la senal 10\n");
        con=2;
    }else{
        printf("Es otra senal\n");
    }
}

void nomematas(int sig){
    printf("jajajaja, no me matas\n");
}



int main(){
    signal(12,holaMundo);
    signal(10,holaMundo);
    signal(2,nomematas);
    con=1;
    while(con == 1){
        printf("Estoy trabajando\n");
        sleep(1);
    }
    printf("Nunca llega\n");
    return 0;
}