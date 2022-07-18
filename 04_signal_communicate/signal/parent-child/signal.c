#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>  // open
#include <sys/stat.h>  // S_IRUSR  S_IWOTH
#include <signal.h>  //kill raise  signal
#include <sys/wait.h>  //waitpid

void myfun(int signum){
    int i = 0;
    while(i < 5){
        printf("receive signum = %d , i = %d\n",signum,i);
        sleep(1);
        i++;
    }
    return ;
}

void myfun1(int signum){
    //处理子进程结束信号
    printf("receive signum = %d \n",signum);
    // wait 等待 子进程结束，不会出现僵尸进程
    // waitpid(pid,NULL,WNOHANG)
    wait(NULL);
    return ;
}

int main(int argc,char* argv[]){
    pid_t pid;
    pid = fork();
    if(pid > 0){
        int i = 0;
        //对子进程发来的 10) SIGUSR1 信号进行处理
        signal(SIGUSR1,myfun); 
        //如果不加下面 signal(SIGCHLD,myfun1); 进程状态如下会有僵尸进程
        /**
         * 
         * ps -axj | head -1;ps -axj | grep signal-parent
 PPID   PID  PGID   SID TTY      TPGID STAT   UID   TIME COMMAND
 9835 15724 15724  9835 pts/5    15724 S+    1000   0:00 ./cmake-build/04_signal_communicate/signal/parent-child/signal-parent-child
15724 15725 15724  9835 pts/5    15724 Z+    1000   0:00 [signal-parent-c] <defunct>
         * 
         */
        //对子进程结束发送的 17) SIGCHLD 信号进行处理
        signal(SIGCHLD,myfun1);
        /**
        * ps -axj | head -1;ps -axj | grep signal-parent
         PPID   PID  PGID   SID TTY      TPGID STAT   UID   TIME COMMAND
 9835 18514 18514  9835 pts/5    18514 S+    1000   0:00 ./cmake-build/04_signal_communicate/signal/parent-child/signal-parent-child
        */

        while(1){
            printf("parent process things,i = %d\n",i);
            sleep(1);
            i++;
        }
    }
    if(pid == 0){
       sleep(10);
       //给父进程发信号  10) SIGUSR1
       kill(getppid(),SIGUSR1);
       sleep(10);
       //17) SIGCHLD
       exit(0);  // kill(getppid(),SIGCHLD) 如果父进程不进行处理 则子进程成为僵尸进程
    }
    /**
     * @brief res
     * 
        parent process things,i = 0
        parent process things,i = 1
        parent process things,i = 2
        parent process things,i = 3
        parent process things,i = 4
        parent process things,i = 5
        parent process things,i = 6
        parent process things,i = 7
        parent process things,i = 8
        parent process things,i = 9
        receive signum = 10 , i = 0
        receive signum = 10 , i = 1
        receive signum = 10 , i = 2
        receive signum = 10 , i = 3
        receive signum = 10 , i = 4
        parent process things,i = 10
        parent process things,i = 11
        parent process things,i = 12
        parent process things,i = 13
        parent process things,i = 14
        receive signum = 17 
        parent process things,i = 15
        parent process things,i = 16
        parent process things,i = 17
        parent process things,i = 18
        parent process things,i = 19
        parent process things,i = 20
     * 
     */
    return 0;
}
