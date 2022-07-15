#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>  // open
#include <sys/stat.h>  // S_IRUSR  S_IWOTH

/**
 * @brief first 先运行，second后运行
 * 
 * 在first执行后会阻塞，知道建立读端，才会写
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc,char* argv[]){
    int fd;
    //打开有名管道文件 去写  只建立写端
    fd = open("./myfifo",O_WRONLY);
    //读端不存在执行管道写操作 也会等待
    if(fd < 0){
        printf("open myfifo filename pipe failure\n");
        return -1;
    }
    printf("first process open myfifo filename pipe success\n");
    for(int i = 0;i < 5;i++){
        printf("this is first process i = %d\n",i);
        usleep(100);
    }
    char process_inter = 0;
    //设置传递通信内容为1
    process_inter = 1;
    //睡5秒后 通知 子进程执行
    sleep(5);
    write(fd,&process_inter,1);

    /**
     * @brief result
     * 
     * first process open myfifo filename pipe success
        this is first process i = 0
        this is first process i = 1
        this is first process i = 2
        this is first process i = 3
        this is first process i = 4
     * 
     * 
     */
    return 0;
}