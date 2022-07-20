#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <pthread.h>
// #include <semaphore.h>
#include <sys/ipc.h>
#include <sys/sem.h>

union semun{
    int val;  //SETVAL : 设置信号灯的值
    struct semid_ds *buf;
              //IPC_STAT (获取对象属性)
              //IPC_SET（设置对象属性）
    unsigned short * array; //
    struct seminfo *__buf;  //Buffer for IPC
};

union semun mysemun;

int semid;

struct sembuf mysembuf;

void *fun(void * var){  //child thread second execute
    // pv操作中的p操作 wait
    // 阻塞
    // sem_wait(&sem);
    //sem_op 的取值
    //0：等待，直到信号灯的值变为0 
    //1：释放资源，V操作
    //-1：分配资源，P操作
    //P 操作设置为-1
    mysembuf.sem_op = -1;
    /* Operate on semaphore.  */
    // extern int semop (int __semid, struct sembuf *__sops, size_t __nsops) __THROW;
    //semid, sembuf, 1 操作信号灯个数
    semop(semid,&mysembuf,1);
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
    semid = semget(IPC_PRIVATE,3,0777);
    if(semid < 0){
        printf("create semaphrore set failure!\n");
    }
    printf("create semaphrore set success! semid = %d\n",semid);
    system("ipcs -s");

    mysemun.val = 0;
    /* Semaphore control operation.  */
    // extern int semctl (int __semid, int __semnum, int __cmd, ...) __THROW;
    semctl(semid,0,SETVAL,mysemun);
    
    // sem_init(&sem,0,0);
    mysembuf.sem_num = 0;
    //信号标志 0 阻塞
    mysembuf.sem_flg = 0;

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
        //sem_op 的取值
    //0：等待，直到信号灯的值变为0  1：释放资源，V操作  -1：分配资源，P操作
    mysembuf.sem_op = 1;
    semop(semid,&mysembuf,1);

    // sem_post(&sem);

    //销毁信号量
    // sem_destroy(&sem);

    //注意没有while(1) 主线程执行完成后就退出了
    while(1);

    /**
     * @brief result
     * 
        create semaphrore set success! semid = 163840
        ------ Semaphore Arrays --------
        key        semid      owner      perms      nsems     
        0x00000000 163840     wzy        777        3         

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