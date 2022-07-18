#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>  // open
#include <sys/stat.h>  // S_IRUSR  S_IWOTH
#include <signal.h>  //kill raise
#include <sys/wait.h>  //waitpid

int main(int argc,char* argv[]){
    
    pid_t pid;
    pid = fork();
    if(pid > 0){
        //最开始进程状态为 S(TASK_INTERRUPTIBLE) 可中断的睡眠状态
        sleep(10);
        //睡眠过后 会进入 R(TASK_RUNNING) 可执行状态
        //等待子进程 
        //参数 进程pid，不接受等待进程返回值，阻塞方式 WNOHANG设置采用非阻塞
        if(waitpid(pid,NULL,WNOHANG) == 0){
            //沙雕子进程，子进程进入僵尸状态 Z
            kill(pid,9);
        }
        //wait负责回收进程，不会出现僵尸进程
        //ps -axj | head -1;ps -axj | grep raise-parent
        /**
         * @brief res
         * 
 PPID   PID  PGID   SID TTY      TPGID STAT   UID   TIME COMMAND
 6121  7977  7977  6121 pts/5     7977 S+    1000   0:00 ./cmake-build/04_signal_communicate/raise-parent-child/raise-parent-child
 7977  7978  7977  6121 pts/5     7977 Z+    1000   0:00 [raise-parent-ch] <defunct>
 6685  8000  7999  6685 pts/6     7999 S+    1000   0:00 grep --color=auto raise-parent

  PPID   PID  PGID   SID TTY      TPGID STAT   UID   TIME COMMAND
 6121  7977  7977  6121 pts/5     7977 R+    1000   0:02 ./cmake-build/04_signal_communicate/raise-parent-child/raise-parent-child
         * 
         */
        wait(NULL);
        while(1);
    }
    if(pid == 0){
        printf("raise function before\n");
        //暂停交互进程 相当于 Ctrl-Z
        //进程状态进入 T (TASK_STOPPED)
        // raise(SIGTSTP);
        printf("raise function after\n");
        //结束当前进程，返回到父进程 相当于 kill(ppid(),SIGUSER1)
        exit(0);  // kill(ppid(),SIGUSR1) 10)SIGUSR1
        //进入僵尸进程
    }
    /**
     * @brief res
     * 
     *  PPID   PID  PGID   SID TTY      TPGID STAT   UID   TIME COMMAND
18098 30467 30467 18098 pts/1    30467 S+    1000   0:00 ./cmake-build/04_signal_commnicate/raise-parent-child/raise-parent-child
30467 30468 30467 18098 pts/1    30467 T+    1000   0:00 ./cmake-build/04_signal_commnicate/raise-parent-child/raise-parent-child

 PPID   PID  PGID   SID TTY      TPGID STAT   UID   TIME COMMAND
18098 30467 30467 18098 pts/1    30467 R+    1000   0:00 ./cmake-build/04_signal_commnicate/raise-parent-child/raise-parent-child
     * 
     */
    return 0;
}
