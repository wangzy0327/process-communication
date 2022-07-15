#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>  // open
#include <sys/stat.h>  // S_IRUSR  S_IWOTH
#include <signal.h>  //kill alarm pause

int main(int argc,char* argv[]){
    int i;
    i = 0;
    printf("pause before\n");
    pause(); // 暂停进程
    //exit() 会在进程退出前 将内容打印到控制台上
    printf("pause after\n");
    /**
     * @brief res
     * 
     * pause before
     * 
     */
    return 0;
}
