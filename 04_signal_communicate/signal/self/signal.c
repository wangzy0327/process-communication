#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>  // open
#include <sys/stat.h>  // S_IRUSR  S_IWOTH
#include <signal.h>  //kill alarm signal

void myfun(int signum){
    int i = 0;
    while(i < 10){
        printf("process signal signum=%d\n",signum);
        sleep(1);
        i++;
    }
    return ;
}

int main(int argc,char* argv[]){
    int i;
    i = 0;
        //__sighandler_t signal (int __sig, __sighandler_t __handler)
    //typedef void (*__sighandler_t) (int);
    //14) SIGALRM
    //处理 14号信号 SIGALRM  处理函数为myfun
    //当前进程在接收到14号内核信号后 执行 myfun函数 然后继续往下执行
    signal(SIGALRM,myfun); 
    printf("alarm before\n");
    //内核在指定时间内发送SIGALRM信号给当前进程
    alarm(9);
    //exit() 会在进程退出前 将内容打印到控制台上
    printf("alarm after\n");
    while(i < 20){
        i++;
        sleep(1);
        printf("process things, i = %d\n",i);
    }
    /**
     * @brief res
     * 
     * alarm before
        alarm after
        process things, i = 1
        process things, i = 2
        process things, i = 3
        process things, i = 4
        process things, i = 5
        process things, i = 6
        process things, i = 7
        process things, i = 8
        process signal signum=14
        process signal signum=14
        process signal signum=14
        process signal signum=14
        process signal signum=14
        process signal signum=14
        process signal signum=14
        process signal signum=14
        process signal signum=14
        process signal signum=14
        process things, i = 9
        process things, i = 10
        process things, i = 11
        process things, i = 12
        process things, i = 13
        process things, i = 14
        process things, i = 15
        process things, i = 16
        process things, i = 17
        process things, i = 18
        process things, i = 19
        process things, i = 20
     * 
     */
    return 0;
}
