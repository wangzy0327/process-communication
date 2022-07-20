#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>  // open
#include <sys/stat.h>  // S_IRUSR  S_IWOTH
#include <signal.h>  //kill alarm signal
//SystemV IPC
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>   // memcpy
#include <sys/msg.h>  // msg_get
#include <sys/sem.h>  // sem_get

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


//server process 后运行 但是先打印for循环
int main(int argc,char* argv[]){
    
    int semid;
    /* Get semaphore.  */
    // extern int semget (key_t __key, int __nsems, int __semflg) __THROW;
    // key值，信号量数量，权限标志
    key_t key;
    key = ftok("sem.key",'a');
    semid = semget(key,3,IPC_CREAT | 0777);
    if(semid < 0){
        printf("create semaphore set failure!\n");
        return -1;
    }
    printf("create semaphore set success! msgid = %d\n",semid);
    system("ipcs -s");


    for(int i = 0;i < 10;i++){
        usleep(100);
        printf("this is server process i = %d\n",i);
    }
    
    //init sem
    mysemun.val = 0;
    semctl(semid,0,SETVAL,mysemun);

    mysembuf.sem_num = 0;
    //信号等待标志 0 阻塞一直等
    mysembuf.sem_flg = 0;

    //V 操作 释放
    //sem_op 0：等待，直到信号灯的值变为0  1：释放资源，V操作  -1：分配资源，P操作
    mysembuf.sem_op = 1;
    semop(semid,&mysembuf,1);

    //P 操作
    // mysembuf.sem_op = -1;
    // semop(semid,&mysembuf,1);

    // for(int i = 0;i < 10;i++){
    //     usleep(100);
    //     printf("this is client process i = %d\n",i);
    // }

    /**
     * result
     * 
        create semaphore set success! msgid = 294912

        ------ Semaphore Arrays --------
        key        semid      owner      perms      nsems     
        0x6106060b 294912     wzy        777        3         

        this is server process i = 0
        this is server process i = 1
        this is server process i = 2
        this is server process i = 3
        this is server process i = 4
        this is server process i = 5
        this is server process i = 6
        this is server process i = 7
        this is server process i = 8
        this is server process i = 9
     * 
     **/

    return 0;
}