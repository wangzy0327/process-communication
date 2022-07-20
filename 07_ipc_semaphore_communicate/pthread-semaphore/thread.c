#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>

//定义信号量
sem_t sem;

void *fun(void * var){  //child thread second execute
    // pv操作中的p操作 wait
    // 阻塞
    sem_wait(&sem);
    for(int j = 0;j < 10;j++){ //second 
        usleep(100);
        printf("this is fun j = %d\n",j);
    }
}

int main(int argc,char* argv[]){   //main thread first execute
    int i;
    char str[] = "hello linux\n";
    pthread_t tid;
    int ret;
    /* Initialize semaphore object SEM to VALUE.  If PSHARED then share it
   with other processes.  */
    // extern int sem_init (sem_t *__sem, int __pshared, unsigned int __value) __THROW;
    // sem:表示要初始化的信号量 ;pshared 该信号量是否可以与其他进程共享，为0表示不共享，值为1表示共享
    // __value : 设置信号量的初始值
    sem_init(&sem,0,0);
    ret = pthread_create(&tid,NULL,fun,(void *)str);
    if(ret < 0){
        printf("create thread failure\n");
        return -1;
    }
    for(int i = 0;i < 10;i++){  //first
        usleep(100);
        printf("this is main fun i = %d\n",i);
    }
    //pv操作中的v操作
    sem_post(&sem);

    //销毁信号量
    sem_destroy(&sem);

    //注意没有while(1) 主线程执行完成后就退出了
    while(1);
    /**
     * @brief result
     * 
        this is main fun i = 0
        this is main fun i = 1
        this is main fun i = 2
        this is main fun i = 3
        this is main fun i = 4
        this is main fun i = 5
        this is main fun i = 6
        this is main fun i = 7
        this is main fun i = 8
        this is main fun i = 9
        this is fun j = 0
        this is fun j = 1
        this is fun j = 2
        this is fun j = 3
        this is fun j = 4
        this is fun j = 5
        this is fun j = 6
        this is fun j = 7
        this is fun j = 8
        this is fun j = 9
     * 
     */
    return 0;
}