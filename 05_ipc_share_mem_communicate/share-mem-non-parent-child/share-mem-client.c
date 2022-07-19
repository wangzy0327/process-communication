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
    printf("client process receive signal number : %d\n",signum);
    return ;
}

struct shmbuf{
    pid_t pid;
    char data[128];
};


/**
 * @brief 共享内存 非父子进程 通信 client 读共享内存
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
    //get server pid
    pid_t server_pid;
    server_pid = p->pid;
    p->pid = getpid();  //write client pid to share memory
    //发送信号让server端去读取自己的pid
    kill(server_pid,SIGUSR2);
    signal(SIGUSR1,myfun);
    pause();
    
    while(1){
        //设置客户端信号处理函数
        signal(SIGUSR1,myfun);
        //读阻塞，等待服务端信号 kill(pid,SIGUSR1)
        pause();
        printf("client process read share memory data : %s\n",p->data);
        //给服务端发送信号
        kill(server_pid,SIGUSR2);
    }

    /**
     * @brief result
     * 
        create key success!
        create share memory success shmid = 294912!

        ------ Shared Memory Segments --------
        key        shmid      owner      perms      bytes      nattch     status      
        0x6e060465 294912     wzy        777        128        1                       

        client process receive signal number : 10
        client process receive signal number : 10
        client process read share memory data : hello client

        client process receive signal number : 10
        client process read share memory data : hello non parent-client share-memory communicate 
     * 
     */

    return 0;
}
