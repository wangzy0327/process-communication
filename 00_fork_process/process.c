#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc,char* argv[]){
    pid_t pid;
    //并不能控制父子进程执行顺序
    int process_inter = 0;
    //fork 调用一次，执行两次，子进程执行fork返回0，父进程执行返回子进程pid
    pid = fork();
    //child process code  
    if(pid == 0){
        int i = 0;
        while(process_inter == 0); //process_inter = 1 exit
        for(int i = 0;i < 5;i++){
            printf("this is child process i=%d\n",i);
            //Sleep USECONDS microseconds
            usleep(100);
        }
    }
    //parent process code
    if(pid > 0){
        int i = 0;
            for(int i = 0;i < 5;i++){
            printf("this is parent process i=%d\n",i);
            //Sleep USECONDS microseconds
            usleep(100);
        }
        process_inter = 1;
    }
    while(1);
    return 0;
}