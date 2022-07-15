#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>  // open
#include <sys/stat.h>  // S_IRUSR  S_IWOTH
#include <signal.h>  //kill alarm

int main(int argc,char* argv[]){
    int i;
    i = 0;
    printf("alarm before\n");
    alarm(9); //9 秒后 会发送 默认终止进程信号
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
     * alarm after
        process things, i = 1
        process things, i = 2
        process things, i = 3
        process things, i = 4
        process things, i = 5
        process things, i = 6
        process things, i = 7
        process things, i = 8
       Alarm clock
     * 
     */
    return 0;
}
