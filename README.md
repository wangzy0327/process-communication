# 进程通信

### 一：进程通信概述

1. 什么是进程间通信？

   什么是线程间通信？
   进程间通信：在用户空间实现进程通信是不可能的([示例](00_fork_process/process.c))，通过Linux内核通信
   线程间通信：可以在用户空间就能实现，可以通过全局变量通信。[示例](01_pthread_communicate/process.c)
   ![进程间通信](imgs/process-communicate.png "进程间通信")
2. 有哪几种通信方式？

   管道通信：无名管道、有名管道（文件系统中有名）
   信号通信：信号（通知）通信包括：信号的发送、信号的接收和信号的处理。
   IPC(Inter-Process Communication) 通信：共享内存、消息队列和信号灯（信号量）.
   以上是单机模式下的进程通信（只有一个Linux内核）
   Socket通信：存在于一个网络中两个进程之间的通信（两个Linux内核）
3. 进程通信的学习思路：每一种通信方式都是基于文件IO的思想。

   open：功能：创建或打开进程通信对象。函数形式不一样，有的是有多个函数完成。
   write：功能：向进程通信对象中写入内容。函数形式可能不一样。
   read：功能：从进程通信对象中读取内容。函数形式可能不一样。
   close：功能：关闭或删除进程通信对象。形式可能不一样。

### 二：无名管道

通信原理：

![无名管道通信方式](imgs/anonymous-pipeline.png "无名管道通信方式")

管道文件是一个特殊的文件，是由队列来实现的。

在文件IO中创建一个普通文件或打开一个普通文件是由**open函数**来实现的，它**不能**创建管道文件。只能用pipe函数来创建管道。

函数形式：int pipe(int fd[2])

功能：创建管道，为系统调用；unistd.h

参数：就是得到的文件描述符。可见有两个文件描述符：fd[0]和fd[1]，管道有一个读端fd[0]用来读和一个写端fd[1]用来写，这个规定不能变。

返回值：成功是0，出错是-1；

[示例](02_anonymous_pipeline_communicate/pipe/process.c)

注意：

* 管道是创建在内存中的，进程结束，空间释放，管道就不存在了；
* 管道中的东西，读完了就删除了：队列；
* 如果管道中没有东西可读，则会阻塞；[读阻塞示例](02_anonymous_pipeline_communicate/read_block/process.c)  [写阻塞示例](02_anonymous_pipeline_communicate/write_block/process.c)

无名管道的缺点：不能实现不是父子进程（亲缘关系）之间的通信。[示例](02_anonymous_pipeline_communicate/parent_child/process.c)

### 三：有名管道
