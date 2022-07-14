#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <pthread.h>

//全局变量
int thread_inter = 0;

void *fun(void * var){  //child thread second execute
    int j;
    while(thread_inter == 0);
    for(int j = 0;j < 10;j++){
        usleep(100);
        printf("this is fun j=%d\n",j);
    }
}

int main(int argc,char* argv[]){   //main thread first execute
    int i;
    char str[] = "hello linux\n";
    pthread_t tid;
    int ret;
    ret = pthread_create(&tid,NULL,fun,(void *)str);
    if(ret < 0){
        printf("create thread failure\n");
        return -1;
    }
    for(int i = 0;i < 10;i++){
        usleep(100);
        printf("this is main fun i=%d\n",i);
    }
    thread_inter = 1;
    while(1);
    return 0;
}