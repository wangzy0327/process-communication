#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>  // open
#include <sys/stat.h>  // S_IRUSR  S_IWOTH
#include <signal.h>  //kill raise

int main(int argc,char* argv[]){
    //这里注意如果before前的打印没加换行符
    //会将字符串输出到行缓存中 （printf为库函数）不会打印到控制台进程就终止了（看不到打印内容）
    //只有加了回车换行符，才会将打印内容交给内核去打印
    printf("raise before");
    printf("raise before\n");
    raise(9); // 相当于 _exit() 进程退出前 不会打印内容到控制台
    //exit() 会在进程退出前 将内容打印到控制台上
    printf("raise after\n");
    /**
     * @brief res
     * 
     * raise before
        Killed
     * 
     */
    return 0;
}
