#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>  // open
#include <sys/stat.h>  // S_IRUSR  S_IWOTH
#include <signal.h>  //kill alarm signal
//SystemV IPC
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>   // memcpy


int main(int argc,char* argv[]){
    int shmid;
    //ipcrm -m id
    if(argc < 3){
        printf("please input param , input param error\n");
        return -1;
    }
    if(strcmp(argv[1],"-m") == 0){
        printf("delete share memory\n");
    }else{
        printf("illegal argument delete share memory\n");
        return -2;
    }
    shmid = atoi(argv[2]);
    system("ipcs -m");
    printf("shmid = %d\n",shmid);
    //extern int shmctl (int __shmid, int __cmd, struct shmid_ds *__buf) __THROW;
    shmctl(shmid,IPC_RMID,NULL);
    system("ipcs -m");
    /**
     * @brief result
     * 
     * 首先通过share-mem.c 创建一个共享内存，然后采用当前进程进行删除
     * 
        ./cmake-build/05_ipc_share_mem_communicate/my-ipcrm/my-ipcrm -m 65536
        delete share memory

        ------ Shared Memory Segments --------
        key        shmid      owner      perms      bytes      nattch     status      
        0x61060465 65536      wzy        777        128        0                       

        shmid = 65536

        ------ Shared Memory Segments --------
        key        shmid      owner      perms      bytes      nattch     status 
     */
    return 0;
}
