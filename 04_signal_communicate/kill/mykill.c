#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>  // open
#include <sys/stat.h>  // S_IRUSR  S_IWOTH
#include <signal.h>  //kill

int main(int argc,char* argv[]){
    int sig;
    int pid;
    if(argc < 3){
        printf("please input param\n");
        return -1;
    }
    sig = atoi(argv[1]);
    pid = atoi(argv[2]);
    printf("sig = %d, pid = %d\n",sig,pid);
    
    //这里需要提前运行loop 死循环程序
    kill(pid,sig);

    //ps -axj | head -1;ps -axj | grep loop
    // PPID   PID  PGID   SID TTY      TPGID STAT   UID   TIME COMMAND
    // 18098 22828 22828 18098 pts/1    22828 R+    1000   2:39 ./cmake-build/04_signal_commnicate/infinite-loop/loop
    // 18853 23321 23320 18853 pts/9    23320 S+    1000   0:00 grep --color=auto loop

    // ./mykill 9 22828

    /**
     * @brief result
     * 
     * sig = 9, pid = 22828 
     * 
     *  loop program:
     *   Killed
     * 
     */

    return 0;
}
