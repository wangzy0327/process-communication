#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
/**
 * @brief 父进程先运行，子进程后运行，通过fork()之前创建的匿名管道进行通信
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc,char* argv[]){
    pid_t pid;
    //并不能控制父子进程执行顺序
    int process_inter = 0;

    //注意这里需要在fork()之前创建管道，否则父进程跟子进程分别创建自己的管道将无法通信
    //fork后创建管道 则他们的文件描述符不共享
    int fd[2];
    int ret = pipe(fd);
    if(ret < 0){
        printf("create pipe failure\n");
        return -1;
    }
    printf("create pipe success\n");
    //fork 调用一次，执行两次，子进程执行fork返回0，父进程执行返回子进程pid
    pid = fork();
    //child process code  
    if(pid == 0){
        int i = 0;
        read(fd[0],&process_inter,1);  // if pipe empty sleep
        // read(fd[0],&process_inter,sizeof(process_inter));  // if pipe empty sleep
        while(process_inter == 0); //process_inter = 1 exit

        //255 先读取低位再读高位
        printf("process_inter value : %d\n",process_inter);
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
        //从低位开始一个字节一个字节写 只读取一个字节读到的是255
        // process_inter = 0x0fff;
        process_inter = 1;
        printf("process_inter byte size : %ld\n",sizeof(process_inter));
        //从低位开始一个字节一个字节写
        write(fd[1],&process_inter,sizeof(process_inter));
        // write(fd[1],&process_inter,1);
    }
    // while(1);

    /**
     * @brief result
     * 
     * 父进程先运行，子进程后运行
     * 
        create pipe success
        this is parent process i=0
        this is parent process i=1
        this is parent process i=2
        this is parent process i=3
        this is parent process i=4
        process_inter byte size : 4
        this is child process i=0
        this is child process i=1
        this is child process i=2
        this is child process i=3
        this is child process i=4
     * 
     */
    return 0;
}