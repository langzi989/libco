### 背景

组内最近打算引入微信开源的分布式消息队列phxqueue(https://github.com/tencent/phxqueue)，此库引用了libco协程库，在研究的过程中顺便对libco的源代码阅读了一下，并做相关注解，以便后续维护系统的时候查找问题查阅。libco库比较小巧精练，但是阅读的时候还是触及到一些知识盲区，相关学习的一些知识都记录在了博客中(https://langzi989.github.io/all-categories/)，有需要可以进行查阅。

### 简介
libco是微信后台大规模使用的c/c++协程库，2013年至今稳定运行在微信后台的数万台机器上。  

libco通过仅有的几个函数接口 co_create/co_resume/co_yield 再配合 co_poll，可以支持同步或者异步的写法，如线程库一样轻松。同时库里面提供了socket族函数的hook，使得后台逻辑服务几乎不用修改逻辑代码就可以完成异步化改造。

作者: sunnyxu(sunnyxu@tencent.com), leiffyli(leiffyli@tencent.com), dengoswei@gmail.com(dengoswei@tencent.com), sarlmolchen(sarlmolchen@tencent.com)

PS: **近期将开源PaxosStore，敬请期待。**

### libco的特性
- 无需侵入业务逻辑，把多进程、多线程服务改造成协程服务，并发能力得到百倍提升;
- 支持CGI框架，轻松构建web服务(New);
- 支持gethostbyname、mysqlclient、ssl等常用第三库(New);
- 可选的共享栈模式，单机轻松接入千万连接(New);
- 完善简洁的协程编程接口
 * 类pthread接口设计，通过co_create、co_resume等简单清晰接口即可完成协程的创建与恢复；
 * __thread的协程私有变量、协程间通信的协程信号量co_signal (New);
 * 语言级别的lambda实现，结合协程原地编写并执行后台异步任务 (New);
 * 基于epoll/kqueue实现的小而轻的网络框架，基于时间轮盘实现的高性能定时器;

### Build

```bash
$ cd /path/to/libco
$ make
```

or use cmake

```bash
$ cd /path/to/libco
$ mkdir build
$ cd build
$ cmake ..
$ make
```
