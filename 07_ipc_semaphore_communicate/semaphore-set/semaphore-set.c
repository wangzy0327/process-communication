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
#include <sys/msg.h>  // msg_get
#include <sys/sem.h>  // sem_get

int main(int argc,char* argv[]){
    
    int semid;
    /* Get semaphore.  */
    // extern int semget (key_t __key, int __nsems, int __semflg) __THROW;
    // key值，信号量数量，权限标志
    semid = semget(IPC_PRIVATE,3,0777);
    if(semid < 0){
        printf("create semaphore set failure!\n");
        return -1;
    }
    printf("create semaphore set success! msgid = %d\n",semid);
    system("ipcs -s");

    // create semaphore set success! msgid = 0

    // ------ Semaphore Arrays --------
    // key        semid      owner      perms      nsems     
    // 0x00000000 0          wzy        777        3   

    //delete semaphore
    /* Semaphore control operation.  */
    // extern int semctl (int __semid, int __semnum, int __cmd, ...) __THROW;
    //删除的时候 第三个参数指定 IPC_RMID 第四个参数可以不要，第二个参数可以随意指定
    semctl(semid,0,IPC_RMID);
    system("ipcs -s");
    return 0;
}