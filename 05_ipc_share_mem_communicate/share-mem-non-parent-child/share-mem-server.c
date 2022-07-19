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

void myfun(int signum){
    printf("server process receive signal number : %d\n",signum);
    return ;
}

struct shmbuf{
    pid_t pid;
    char data[128];
};


/**
 * @brief 共享内存 非父子进程 通信 server 写共享内存
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc,char* argv[]){
    int shmid;
    key_t key = ftok("./share-mem.key",'n');
    if(key < 0){
        printf("create key failure!\n");
    }
    printf("create key success!\n");
    shmid = shmget(key,128,IPC_CREAT | 0777);
    if(shmid < 0){
        printf("create share memory failure!\n");
        return -1;
    }
    printf("create share memory success shmid = %d!\n",shmid);
    system("ipcs -m");

    //这里server和client需要先写入pid号，知道对方的pid号才能进行通信
    //这里假设 
    //1.server端先写入pid，然后client读
    //2.client端写入pid，然后server读
    //接下来server端往共享内存中写数据，client端读数据

    struct shmbuf * p;
    //0可读写
    p = (struct shmbuf *)shmat(shmid,NULL,0);
    if(p == NULL){
        printf("server process shmat function failure\n");
        return -2;
    }
    //get client pid
    pid_t client_pid;
    p->pid = getpid();  //write server pid to share memory
    signal(SIGUSR2,myfun);
    pause(); //wait client read server pid
    client_pid = p->pid;
    //发送信号告诉client端自己已经读完pid
    kill(client_pid,SIGUSR1);
    

    while(1){
        //设置服务端 处理函数
        signal(SIGUSR2,myfun);
        printf("server process start write share memory:\n");
        fgets(p->data,sizeof(p->data),stdin);
        kill(client_pid,SIGUSR1);
        //写阻塞 等待接收client信号 kill(pid,SIGUSR2)
        pause();
    }
    
    /**
     * @brief result
     * 
        create key success!
        create share memory success shmid = 294912!

        ------ Shared Memory Segments --------
        key        shmid      owner      perms      bytes      nattch     status      
        0x6e060465 294912     wzy        777        128        1                       

        server process receive signal number : 12
        server process start write share memory:
        hello client
        server process receive signal number : 12
        server process start write share memory:
        hello non parent-client share-memory communicate 
        server process receive signal number : 12
        server process start write share memory:
     * 
     */
        
    return 0;
}
