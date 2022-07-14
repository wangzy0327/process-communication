#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>  // open
#include <sys/stat.h>  // S_IRUSR  S_IWOTH

int main(){
    //文件描述符
    int fd[2];
    int ret;
    char writebuf[] = "hello linux";
    char readbuf[128] = {0};
    //open(const char *__path, int __oflag,mode_t mode);
    //文件名，文件访问模式，文件访问权限  返回文件描述符
    //S_IXUSR	文件属主可执行	S_IWOTH	文件所在组可执行	S_IXOTH	其他用户可执行
    //ep:创建一个名为 file 的文件，该文件对文件属主可读，对文件所在组可写
    // int ret = open("file",O_CREAT,S_IRUSR | S_IWGRP);
    
    //pipe()创建管道，传入文件描述符 一个入队（写），一个出队（读）
    /* Create a one-way communication channel (pipe).
   If successful, two file descriptors are stored in PIPEDES;
   bytes written on PIPEDES[1] can be read from PIPEDES[0].
   Returns 0 if successful, -1 if not.  */
    ret = pipe(fd);
    if(ret < 0){
        printf("create pipe failure\n");
        return -1;
    }
    //因为进程启动时，打开了标准输入（0）、标准输出（1）和标准出错处理（2）
    //文件描述符默认从3开始
    printf("create pipe success fd[0]=%d,f[1]=%d\n",fd[0],fd[1]);
    /**
     * @brief res
     * create pipe success fd[0]=3,f[1]=4
     * 
     */
    // (int*) malloc(sizeof(int)*10)
    write(fd[1],writebuf,sizeof(writebuf));
    //start read from pipe

    //读多少个
    read(fd[0],readbuf,sizeof(readbuf));
    // read(fd[0],readbuf,sizeof(writebuf));
    printf("readbuf=%s\n",readbuf);
    /**
     * @brief read buffer
     * 
     * readbuf=hello linux
     * 
     */

    //关闭管道
    close(fd[0]);
    close(fd[1]);

    return 0;
}