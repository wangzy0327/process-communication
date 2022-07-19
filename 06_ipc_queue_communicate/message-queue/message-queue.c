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
    struct msgbuf sendbuf;
    //结构体要赋值 否则出现段错误
    // struct msgbuf *sendbuf = (struct msgbuf*)malloc(sizeof(struct msgbuf));
    // 默认只是定义了一个指针 struct msgbuf* 的指针，这个指针可能是NULL（并没有在内存中给其分配任何空间所以对它成员的赋值会出现段错误）
    //也可能是指向任意地址的指针，
    //init sendbuf
    sendbuf.type = 100;
    printf("send message queue type : %ld\n",sendbuf.type);
    msgid = msgget(IPC_PRIVATE,0777);
    if(msgid < 0){
        printf("create message queue failure!\n");
        return -1;
    }
    printf("create message queue success! msgid = %d\n",msgid);
    system("ipcs -q");

    /**
     * @brief result
     * 
        create message queue success! msgid = 32769

        ------ Message Queues --------
        key        msqid      owner      perms      used-bytes   messages    
        0x00000000 0          wzy        777        0            0  
        0x00000000 32769      wzy        777        0            0          
     * 
     */


    printf("send buffer msgdata size : %ld\n",sizeof(sendbuf.msgdata));
    printf("please input message : \n");
    fgets(sendbuf.msgdata,sizeof(sendbuf.msgdata),stdin);
    //start write message to message queue

    /* Send message to message queue.

   This function is a cancellation point and therefore not marked with
   __THROW.  */
    // extern int msgsnd (int __msqid, const void *__msgp, size_t __msgsz,
	// 	   int __msgflg);
    int sndret;
    sndret = msgsnd(msgid,(void *)&sendbuf,strlen(sendbuf.msgdata),0);
    if(sndret < 0){
        printf("send message queue buffer failure!\n");
    }
    printf("send message queue buffer success!\n");

    // while(1);

    // create message queue success! msgid = 196610

    // ------ Message Queues --------
    // key        msqid      owner      perms      used-bytes   messages             
    // 0x00000000 196610     wzy        777        0            0           

    // please input message : 
    // hello linux

    // ipcs -q
    // ------ Message Queues --------
    // key        msqid      owner      perms      used-bytes   messages               
    // 0x00000000 196610     wzy        777        12           1 
 
    //start read

    struct msgbuf recvbuf ;
    recvbuf.type = 100;
    printf("receive buffer msgdata size : %ld\n",sizeof(recvbuf.msgdata));
    memset(recvbuf.msgdata,0,sizeof(recvbuf.msgdata));
//  This function is a cancellation point and therefore not marked with
//    __THROW.  */
// extern ssize_t msgrcv (int __msqid, void *__msgp, size_t __msgsz,
// 		       long int __msgtyp, int __msgflg);
    printf("receive message queue type : %ld\n",recvbuf.type);
    int readret = msgrcv(msgid,(void *)&recvbuf,sizeof(recvbuf.msgdata),recvbuf.type,0);
    printf("receive message queue buffer contents : %s\n",recvbuf.msgdata);
    printf("readret = %d\n",readret);

    /**
     * @brief result
     * 
        send message queue type : 100
        create message queue success! msgid = 983040

        ------ Message Queues --------
        key        msqid      owner      perms      used-bytes   messages    
        0x00000000 983040     wzy        777        0            0           

        send buffer msgdata size : 128
        please input message : 
        hello linux
        send message queue buffer success!
        receive buffer msgdata size : 128
        receive message queue type : 100
        receive message queue buffer contents : hello linux

        readret = 12
        delete message queue  msgid = 983040

        ------ Message Queues --------
        key        msqid      owner      perms      used-bytes   messages   
     * 
     */

    //delete message queue
    printf("delete message queue  msgid = %d\n",msgid);
    msgctl(msgid,IPC_RMID,NULL);
    system("ipcs -q");

    
    return 0;
}
