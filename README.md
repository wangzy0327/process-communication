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

函数形式：int open(const char *__path, int __oflag,mode_t mode)

功能：创建普通文件，为系统调用；

参数：文件名，文件访问模式，文件访问权限

返回值：文件描述符

示例：int ret = open("file",O_CREAT,S_IRUSR | S_IWGRP);

说明：创建一个名为 file 的文件，该文件对文件属主可读，对文件所在组可写

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

前置知识：linux文件类型

所谓的有名，即文件系统中存在这样一个的文件节点，每一个文件节点都有一个inode号而且这是一个特殊的文件类型：p管道类型。

1. 创建这个文件节点，不可以通过open函数，open函数只能创建普通文件，不能创建特殊文件
2. 管道文件只有inode号，不占磁盘空间，和套接字，字符设备文件，块设备文件一样。普通文件和符号链接文件及目录文件不仅有inode号，还占磁盘空间

| 文件类型标识 | 文件类型          | 创建方式 | 性质                              |
| ------------ | ----------------- | -------- | --------------------------------- |
| -            | 普通文件          | open     | 不仅有inode，还占磁盘空间         |
| d            | 目录文件          | mkdir    | 不仅有inode，还占磁盘空间         |
| l            | 符号链接文件      | ln -s    | 不仅有inode，还占磁盘空间         |
| s(伪文件)    | 套接字文件（s）   | socket   | 只有文件节点inode，不占磁盘IO空间 |
| b(伪文件)    | 块设备文件（b）   | mknod b  | 只有文件节点inode，不占磁盘IO空间 |
| c(伪文件)    | 字符设备文件（c） | mknod c  | 只有文件节点inode，不占磁盘IO空间 |
| p(伪文件)    | 管道文件（p）     | mkfifo   | 只有文件节点inode，不占磁盘IO空间 |

mkfifo

mkfifo：用来创建管道文件的节点，**没有在内核中创建管道**。

只有通过**open函数**打开这个文件时**才**会在**内核**空间**创建管道**。

函数形式：int mkfifo(const char* filename,mode_t mode);

功能：创建管道文件

参数：管道文件文件名，权限，创建的文件权限仍然和umask有关系。

返回值：创建成功返回0，创建失败返回-1。

作用：mkfifo [示例](03_filename_pipeline_communicate/filename_pipe/process.c)

通过有名管道可以实现无亲缘关系进程间通信。[示例first](03_filename_pipeline_communicate/filename_parent_child/first/first.c) [示例second](03_filename_pipeline_communicate/filename_parent_child/second/second.c)

### 四：信号通信

### 五：共享内存

### 六：消息队列

### 七：信号灯（信号量）
