#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>  // open
#include <sys/stat.h>  // S_IRUSR  S_IWOTH

int main(int argc,char* argv[]){
    int ret;
    ret = mkfifo("./myfifo",0777);
    if(ret < 0){
        printf("create filename pipe failure!\n");
        return -1;
    }
    printf("create filename pipe myfifo success \n");
    /**
     * @brief result
     * 
     * create filename pipe myfifo success 
     * 
     * 不占磁盘IO空间
     * 
     * prwxrwxr-x  1 wzy wzy    0 Jul 15 02:51 myfifo|
     * 
     */
    return 0;
}