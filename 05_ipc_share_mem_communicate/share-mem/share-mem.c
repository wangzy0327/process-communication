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



int main(int argc,char* argv[]){
    int key;
    ///* Generates key for System V style IPC.  */
    //extern key_t ftok (const char *__pathname, int __proj_id) __THROW;
    //不同的 id值生成不同的key值
    key = ftok("./share-mem.key",'a');
    if(key < 0){
        printf("create key failure\n");
        return -2;
    }
    //打印16进制的key值
    printf("create key success key = %X\n",key);

    // create key success key = 61060465
    // create share memory success shmid = 32768

    // ------ Shared Memory Segments --------
    // key        shmid      owner      perms      bytes      nattch     status      
    // 0x61060465 32768      wzy        777        128        0 

    /* Get shared memory segment.  */
    //extern int shmget (key_t __key, size_t __size, int __shmflg) __THROW;
    // key,字节数,权限 IPC_PRIVATE 默认生成的key为 0x00000000
    // int shmid = shmget(IPC_PRIVATE,128,0777);

    int shmid = shmget(key,128,IPC_CREAT | 0777);
    if(shmid < 0){
        printf("create share memory failure!\n");
        return -1;
    }
    printf("create share memory success shmid = %d\n",shmid);
    
    // 查看IPC对象 ipcs -m (共享内存) | ipcs -q (消息队列) | ipcs -s (信号量)
    //删除IPC对象 ipcrm -m id
    //调用系统命令 
    system("ipcs -m");

    char* p;
    p = (char *)shmat(shmid,NULL,0);
    if(p == NULL){
        printf("shmat function failure \n");
        return -2;
    }
    //共享内存的读写 
    //write share memory
    //写 fgets() scanf() 通过stdin 键盘读入 写到共享内存中
    fgets(p,128,stdin);
    //start read share memory
    printf("share memory data : %s\n",p);
    printf("second read share memory data : %s\n",p);

    // create key success key = 61060465
    // create share memory success shmid = 32768

    // ------ Shared Memory Segments --------
    // key        shmid      owner      perms      bytes      nattch     status      
    // 0x61060465 32768      wzy        777        128        0                       

    // hello linux
    // share memory data : hello linux
    
    // second read share memory data : hello linux


    /**
     * @brief res
     * 
    create share memory success shmid = 0

    ------ Shared Memory Segments --------
    key        shmid      owner      perms      bytes      nattch     status      
    0x00000000 0          wzy        777        128        0                     
     * 
     */
    
    return 0;
}
