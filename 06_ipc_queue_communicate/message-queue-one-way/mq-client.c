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

struct msgbuf{
    long type;  //消息类型
    char msgdata[128];   //消息内容
};

int main(int argc,char* argv[]){
    int msgid;
    int readret;
    int key;
    struct msgbuf recvbuf;
    //提前执行 touch mq.key
    key = ftok("mq.key",'a');
    if(key < 0){
        printf("create key failure!\n");
        return -1;
    }
    printf("create key success!\n");
    msgid = msgget(key,IPC_CREAT|0777);
    if(msgid < 0){
        printf("create message queue failure\n");
        return -1;
    }
    printf("create message queue success msgid = %d\n",msgid);
    system("ipcs -q");

    //set message queue type
    recvbuf.type = 100;

    //read message queue

    while(1){
        memset(recvbuf.msgdata,0,128); //clear
        int readret = msgrcv(msgid,(void *)&recvbuf,sizeof(recvbuf.msgdata),recvbuf.type,0);
        printf("message type : %ld ; receive message queue buffer contents : %s\n",recvbuf.type,recvbuf.msgdata);
        printf("readret = %d\n",readret);
        recvbuf.msgdata[strlen(recvbuf.msgdata)-1] = 0;
        if(strcmp(recvbuf.msgdata,"end") == 0)
            break;
    }

    msgctl(msgid,IPC_RMID,NULL);
    system("ipcs -q");

    /**
     * @brief result
     * 
        create key success!
        create message queue success msgid = 1048576

        ------ Message Queues --------
        key        msqid      owner      perms      used-bytes   messages    
        0x61060609 1048576    wzy        777        0            0           

        message type : 100 ; receive message queue buffer contents : hello IPC message queue communicate

        readret = 36
        message type : 100 ; receive message queue buffer contents : IPC message queue again!

        readret = 25
     * 
     */
    return 0;
}