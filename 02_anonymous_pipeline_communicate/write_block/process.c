#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>  // open
#include <sys/stat.h>  // S_IRUSR  S_IWOTH
#include <string.h>  // memset

int main(){
    //文件描述符
    int fd[2];
    int ret;
    char writebuf[] = "a";
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
    
    //可通过命令 ulimit -a 看到pipe_size指的是一次原子写入大小
    // 管道大小为64 KB.PIPE_BUF是4KB的原因是PIPE_BUF是保证原子写入的最大大小
    long pipe_size = fpathconf(fd[1],_PC_PIPE_BUF);
    printf("pipe buffer size %ld\n",pipe_size);
        // core file size          (blocks, -c) 0
        // data seg size           (kbytes, -d) unlimited
        // scheduling priority             (-e) 0
        // file size               (blocks, -f) unlimited
        // pending signals                 (-i) 14743
        // max locked memory       (kbytes, -l) 64
        // max memory size         (kbytes, -m) unlimited
        // open files                      (-n) 1024
        // pipe size            (512 bytes, -p) 8
        // POSIX message queues     (bytes, -q) 819200
        // real-time priority              (-r) 0
        // stack size              (kbytes, -s) 10240
        // cpu time               (seconds, -t) unlimited
        // max user processes              (-u) 1024
        // virtual memory          (kbytes, -v) unlimited
        // file locks                      (-x) unlimited

    int i = 0;
    printf("write pipe start \n");
    //循环写
    while( write(fd[1],writebuf,sizeof(writebuf)) != -1){
        printf("current buffer size %ld\n",i*sizeof(writebuf));
        i ++;
    }
    /**
     * @brief result
     * 
     *  pipe buffer size 4096
     * current buffer size 65520
        current buffer size 65522
        current buffer size 65524
        current buffer size 65526
        current buffer size 65528
        current buffer size 65530
        current buffer size 65532
        current buffer size 65534
     */
    //下面没打印说明写阻塞住了....
    printf("write pipeline buffer size %ld\n",i*sizeof(writebuf));
    printf("write pipe end \n");

    //关闭管道
    close(fd[0]);
    close(fd[1]);

    return 0;
}