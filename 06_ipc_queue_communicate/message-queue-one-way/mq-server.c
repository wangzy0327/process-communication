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
    struct msgbuf sendbuf;
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
    sendbuf.type = 100;
    //write message queue

    while(1){
        memset(sendbuf.msgdata,0,128); //clear
        printf("please input message:\n");
        fgets(sendbuf.msgdata,sizeof(sendbuf.msgdata),stdin);
        //start write message to message queue
        //如果写入end 则退出并删除 message queue
        // char contents[128];
        // memset(contents,0,sizeof(contents));
        // strcpy(contents,sendbuf.msgdata);
        // contents[strlen(contents)-1] = 0;
        // int flag = strcmp(contents,"end");
        // printf("flag = %d\n",flag);
        //将末尾\n 设置为 \0
        int sndret;
        sndret = msgsnd(msgid,(void *)&sendbuf,strlen(sendbuf.msgdata),0);
        if(sndret < 0){
            printf("send message queue buffer failure!\n");
        }
        printf("message type : %ld ; send message queue buffer success!\n",sendbuf.type);
        sendbuf.msgdata[strlen(sendbuf.msgdata) - 1] = 0;
        if(strcmp(sendbuf.msgdata,"end") == 0)
            break;
    }
    //wait client read 
    // usleep(500);
    // msgctl(msgid,IPC_RMID,NULL);
    //交给client 去 删除mq
    printf("delete message queue msgid = %d\n",msgid);
    system("ipcs -q");

    /**
     * @brief result
     * 
        create key success!
        create message queue success msgid = 1048576

        ------ Message Queues --------
        key        msqid      owner      perms      used-bytes   messages    
        0x61060609 1048576    wzy        777        0            0           

        please input message:
        hello IPC message queue communicate
        message type : 100 ; send message queue buffer success!
        please input message:
        IPC message queue again!
        message type : 100 ; send message queue buffer success!
        please input message:
     * 
     */
    return 0;
}