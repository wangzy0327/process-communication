#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>  // open
#include <sys/stat.h>  // S_IRUSR  S_IWOTH

int main(int argc,char* argv[]){
    int fd;
    //打开有名管道文件 去读
    fd = open("./myfifo",O_RDONLY);
    if(fd < 0){
        printf("open myfifo filename pipe failure\n");
        return -1;
    }
    printf("second process open myfifo filename pipe success\n");
    char process_inter = 0;
    read(fd,&process_inter,1);
    //如果process_inter 为 0 阻塞
    while(process_inter == 0);
    for(int i = 0;i < 5;i++){
        printf("this is second process i = %d\n",i);
        usleep(100);
    }
    /**
     * @brief result
     * 
     * second process open myfifo filename pipe success
        this is second process i = 0
        this is second process i = 1
        this is second process i = 2
        this is second process i = 3
        this is second process i = 4
     * 
     */
    return 0;
}