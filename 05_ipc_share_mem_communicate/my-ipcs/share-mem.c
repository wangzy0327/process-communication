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
#include <time.h>

// 打印 ipc_perm
void printPerm(struct ipc_perm *perm) {
  printf("euid of owner = %d\n", perm->uid);
  printf("egid of owner = %d\n", perm->gid);
  printf("euid of creator = %d\n", perm->cuid);
  printf("egid of creator = %d\n", perm->cgid);
  printf("mode = 0%o\n", perm->mode);
}

// 打印 ipc 内核对象信息
void printShmid(struct shmid_ds *shmid) {
  printPerm(&shmid->shm_perm); 
  printf("segment size = %ld\n", shmid->shm_segsz);
  printf("last attach time = %s", ctime(&shmid->shm_atime));
  printf("last detach time = %s", ctime(&shmid->shm_dtime));
  printf("last change time = %s", ctime(&shmid->shm_ctime));
  printf("pid of creator = %d\n", shmid->shm_cpid);
  printf("pid of last shmat/shmdt = %d\n", shmid->shm_lpid);
  printf("No. of current attaches = %ld\n", shmid->shm_nattch);
}

int main(int argc,char* argv[]){
    int shmid;
    //ipcs -m 
    if(argc < 2){
        printf("please input param , input param error\n");
        return -1;
    }
    if(strcmp(argv[1],"-m") == 0){
        printf("show share memory\n");
    }else{
        printf("illegal argument delete share memory\n");
        return -2;
    }

//     struct shmid_ds
//   {
//     struct ipc_perm shm_perm;		/* operation permission struct */
//     size_t shm_segsz;			/* size of segment in bytes */
//     __time_t shm_atime;			/* time of last shmat() */
// #ifndef __x86_64__
//     unsigned long int __glibc_reserved1;
// #endif
//     __time_t shm_dtime;			/* time of last shmdt() */
// #ifndef __x86_64__
//     unsigned long int __glibc_reserved2;
// #endif
//     __time_t shm_ctime;			/* time of last change by shmctl() */
// #ifndef __x86_64__
//     unsigned long int __glibc_reserved3;
// #endif
//     __pid_t shm_cpid;			/* pid of creator */
//     __pid_t shm_lpid;			/* pid of last shmop */
//     shmatt_t shm_nattch;		/* number of current attaches */
//     __syscall_ulong_t __glibc_reserved4;
//     __syscall_ulong_t __glibc_reserved5;
//   };
    struct shmid_ds show_shm;
    //extern int shmctl (int __shmid, int __cmd, struct shmid_ds *__buf) __THROW;
    shmctl(shmid,IPC_STAT,&show_shm);
    printShmid(&show_shm);


    /**
     * @brief result
     * 
     * ipcs -m
     * ------ Shared Memory Segments --------
key        shmid      owner      perms      bytes      nattch     status      
0x61060465 98304      wzy        777        128        0 
     * 
     * ./cmake-build/05_ipc_share_mem_communicate/my-ipcs/my-ipcs -m
     * 
        show share memory
        euid of owner = 0
        egid of owner = 0
        euid of creator = 0
        egid of creator = 0
        mode = 00
        segment size = 140735960814280
        last attach time = Thu Jul  2 14:00:31 1970
        last detach time = Thu Jan  1 00:00:01 1970
        last change time = Thu Jun 10 14:13:01 2985824
        pid of creator = 187108160
        pid of last shmat/shmdt = 32747
        No. of current attaches = 0
     */
    return 0;
}
