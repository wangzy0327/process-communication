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

void myfun1(int signum){
    printf("parent process receive signal number : %d\n",signum);
}

void myfun2(int signum){
    printf("child process receive signal number : %d\n",signum);
}

/**
 * @brief 共享内存 父子进程 通信（单向）
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc,char* argv[]){
    int shmid;
    shmid = shmget(IPC_PRIVATE,128,IPC_CREAT | 0777);
    if(shmid < 0){
        printf("create share memory failure!\n");
        return -1;
    }
    printf("create share memory success shmid = %d!\n",shmid);
    system("ipcs -m");
    pid_t pid;
    pid = fork();
    char* p;
    if(pid > 0){  //parent process
        //0可读写
        p = (char *)shmat(shmid,NULL,0);
        if(p == NULL){
            printf("parent process shmat function failure\n");
            return -2;
        }
        while(1){
            signal(SIGUSR2,myfun1);
            //write share memory
            printf("parent process write share memory buffer:\n");
            fgets(p,128,stdin);
            //表示用户定义的信号 默认SIGUSR1信号是终止进程 这里需要自定义信号处理函数
            kill(pid,SIGUSR1);  //child process read data
            pause(); // wait child process read
        }
    }else{  //child process
        //pid == 0
        //映射读写
        p = (char *)shmat(shmid,NULL,0);
        if(p == NULL){
            printf("child process shmat function failure\n");
            return -2;
        }
        while(1){
            signal(SIGUSR1,myfun2);
            //读阻塞，等待父进程信号
            pause(); // wait parent process write
            printf("child process read share memory data : %s\n",p);
            //发送信号给父进程，让其写，
            kill(getppid(),SIGUSR2);
        }
    }

    /**
     * @brief res
     * 
        create share memory success shmid = 229376!

        ------ Shared Memory Segments --------
        key        shmid      owner      perms      bytes      nattch     status      
        0x00000000 229376     wzy        777        128        0                       

        parent process write share memory buffer:
        hello linux
        child process receive signal number : 10
        child process read share memory data : hello linux

        parent process receive signal number : 12
        parent process write share memory buffer:
        hello share memory parent-child commnuicate
        child process receive signal number : 10
        child process read share memory data : hello share memory parent-child commnuicate

        parent process receive signal number : 12
        parent process write share memory buffer:
     * 
     */
    return 0;
}
