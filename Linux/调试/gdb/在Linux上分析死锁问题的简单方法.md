
http://blog.chinaunix.net/uid-30343738-id-5757210.html

死锁 (deallocks)： 是指两个或两个以上的进程（线程）在执行过程中，因争夺资源而造成的一种互相等待的现象，若无外力作用，它们都将无法推进下去。此时称系统处于死锁状态或系统产生了死锁，这些永远在互相等待的进程（线程）称为死锁进程（线程）。 由于资源占用是互斥的，当某个进程提出申请资源后，使得有关进程（线程）在无外力协助下，永远分配不到必需的资源而无法继续运行，这就产生了一种特殊现象死锁。
一种交叉持锁死锁的情形，此时执行程序中两个或多个线程发生永久堵塞（等待），每个线程都在等待被其它线程占用并堵塞了的资源。例如，如果线程 1 锁住了记录 A 并等待记录 B，而线程 2 锁住了记录 B 并等待记录 A，这样两个线程就发生了死锁现象。在计算机系统中 , 如果系统的资源分配策略不当，更常见的可能是程序员写的程序有错误等，则会导致进程因竞争资源不当而产生死锁的现象。


产生死锁的四个必要条件

（1） 互斥条件：一个资源每次只能被一个进程（线程）使用。
（2） 请求与保持条件：一个进程（线程）因请求资源而阻塞时，对已获得的资源保持不放。
（3） 不剥夺条件 : 此进程（线程）已获得的资源，在末使用完之前，不能强行剥夺。
（4） 循环等待条件 : 多个进程（线程）之间形成一种头尾相接的循环等待资源关系。

图 1. 交叉持锁的死锁示意图：



注释：在执行 func2 和 func4 之后，子线程 1 获得了锁 A，正试图获得锁 B，但是子线程 2 此时获得了锁 B，正试图获得锁 A，所以子线程 1 和子线程 2 将没有办法得到锁 A 和锁 B，因为它们各自被对方占有，永远不会释放，所以发生了死锁的现象。

使用 pstack 和 gdb 工具对死锁程序进行分析
pstack 在  平台上的简单介绍

pstack 是 Linux（比如 Red Hat Linux 系统、Ubuntu Linux 系统等）下一个很有用的工具，它的功能是打印输出此进程的堆栈信息。可以输出所有线程的调用关系栈。

gdb 在 Linux 平台上的简单介绍

GDB 是 GNU 开源组织发布的一个强大的 UNIX 下的程序调试工具。Linux 系统中包含了 GNU 调试程序 gdb，它是一个用来调试 C 和 C++ 程序的调试器。可以使程序开发者在程序运行时观察程序的内部结构和内存的使用情况 .

gdb 所提供的一些主要功能如下所示：

1 运行程序，设置能影响程序运行的参数和环境 ;

2 控制程序在指定的条件下停止运行；

3 当程序停止时，可以检查程序的状态；

4 当程序 crash 时，可以检查 core 文件；

5 可以修改程序的错误，并重新运行程序；

6 可以动态监视程序中变量的值；

7 可以单步执行代码，观察程序的运行状态。


gdb 程序调试的对象是可执行文件或者进程，而不是程序的源代码文件。然而，并不是所有的可执行文件都可以用 gdb 调试。如果要让产生的可执行文件可以用来调试，需在执行 g++（gcc）指令编译程序时，加上 -g 参数，指定程序在编译时包含调试信息。调试信息包含程序里的每个变量的类型和在可执行文件里的地址映射以及源代码的行号。gdb 利用这些信息使源代码和机器码相关联。gdb 的基本命令较多，不做详细介绍，大家如果需要进一步了解，请参见 gdb 手册。

清单 1. 程序

```
#include  
 #include  
 #include  

 pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER; 
 pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER; 
 pthread_mutex_t mutex3 = PTHREAD_MUTEX_INITIALIZER; 
 pthread_mutex_t mutex4 = PTHREAD_MUTEX_INITIALIZER; 

 static int sequence1 = 0; 
 static int sequence2 = 0; 

 int func1() 
 { 
    pthread_mutex_lock(&mutex1); 
    ++sequence1; 
    sleep(1); 
    pthread_mutex_lock(&mutex2); 
    ++sequence2; 
    pthread_mutex_unlock(&mutex2); 
    pthread_mutex_unlock(&mutex1); 

    return sequence1; 
 } 

 int func2() 
 { 
    pthread_mutex_lock(&mutex2); 
    ++sequence2; 
    sleep(1); 
    pthread_mutex_lock(&mutex1); 
    ++sequence1; 
    pthread_mutex_unlock(&mutex1); 
    pthread_mutex_unlock(&mutex2); 

    return sequence2; 
 } 

 void* thread1(void* arg) 
 { 
    while (1) 
    { 
        int iRetValue = func1(); 

        if (iRetValue == 100000) 
        { 
            pthread_exit(NULL); 
        } 
    } 
 } 

 void* thread2(void* arg) 
 { 
    while (1) 
    { 
        int iRetValue = func2(); 

        if (iRetValue == 100000) 
        { 
            pthread_exit(NULL); 
        } 
    } 
 } 

 void* thread3(void* arg) 
 { 
    while (1) 
    { 
        sleep(1); 
        char szBuf[128]; 
        memset(szBuf, 0, sizeof(szBuf)); 
        strcpy(szBuf, "thread3"); 
    } 
 } 

 void* thread4(void* arg) 
 { 
    while (1) 
    { 
        sleep(1); 
        char szBuf[128]; 
        memset(szBuf, 0, sizeof(szBuf)); 
        strcpy(szBuf, "thread3"); 
    } 
 } 

 int main() 
 { 
    pthread_t tid[4]; 
    if (pthread_create(&tid[0], NULL, &thread1, NULL) != 0) 
    { 
        _exit(1); 
    } 
    if (pthread_create(&tid[1], NULL, &thread2, NULL) != 0) 
    { 
        _exit(1); 
    } 
    if (pthread_create(&tid[2], NULL, &thread3, NULL) != 0) 
    { 
        _exit(1); 
    } 
    if (pthread_create(&tid[3], NULL, &thread4, NULL) != 0) 
    { 
        _exit(1); 
    } 

    sleep(5); 
    //pthread_cancel(tid[0]); 

    pthread_join(tid[0], NULL); 
    pthread_join(tid[1], NULL); 
    pthread_join(tid[2], NULL); 
    pthread_join(tid[3], NULL); 

    pthread_mutex_destroy(&mutex1); 
    pthread_mutex_destroy(&mutex2); 
    pthread_mutex_destroy(&mutex3); 
    pthread_mutex_destroy(&mutex4); 

    return 0; 
 } 
```

清单 2. 编译测试程序

```
 [dyu@xilinuxbldsrv purify]$ g++ -g lock.cpp -o lock -lpthread 
```

清单 3. 查找测试程序的进程号
```
 [dyu@xilinuxbldsrv purify]$ ps -ef|grep lock 
 dyu       6721  5751  0 15:21 pts/3    00:00:00 ./lock 
```

清单 4. 对死锁进程第一次执行 pstack（pstack –进程号）的输出结果
```
[dyu@xilinuxbldsrv purify]$ pstack 6721 
 Thread 5 (Thread 0x41e37940 (LWP 6722)): 
 #0  0x0000003d1a80d4c4 in __lll_lock_wait () from /lib64/libpthread.so.0 
 #1  0x0000003d1a808e1a in _L_lock_1034 () from /lib64/libpthread.so.0 
 #2  0x0000003d1a808cdc in pthread_mutex_lock () from /lib64/libpthread.so.0 
 #3  0x0000000000400a9b in func1() () 
 #4  0x0000000000400ad7 in thread1(void*) () 
 #5  0x0000003d1a80673d in start_thread () from /lib64/libpthread.so.0 
 #6  0x0000003d19cd40cd in clone () from /lib64/libc.so.6 
 Thread 4 (Thread 0x42838940 (LWP 6723)): 
 #0  0x0000003d1a80d4c4 in __lll_lock_wait () from /lib64/libpthread.so.0 
 #1  0x0000003d1a808e1a in _L_lock_1034 () from /lib64/libpthread.so.0 
 #2  0x0000003d1a808cdc in pthread_mutex_lock () from /lib64/libpthread.so.0 
 #3  0x0000000000400a17 in func2() () 
 #4  0x0000000000400a53 in thread2(void*) () 
 #5  0x0000003d1a80673d in start_thread () from /lib64/libpthread.so.0 
 #6  0x0000003d19cd40cd in clone () from /lib64/libc.so.6 
 Thread 3 (Thread 0x43239940 (LWP 6724)): 
 #0  0x0000003d19c9a541 in nanosleep () from /lib64/libc.so.6 
 #1  0x0000003d19c9a364 in sleep () from /lib64/libc.so.6 
 #2  0x00000000004009bc in thread3(void*) () 
 #3  0x0000003d1a80673d in start_thread () from /lib64/libpthread.so.0 
 #4  0x0000003d19cd40cd in clone () from /lib64/libc.so.6 
 Thread 2 (Thread 0x43c3a940 (LWP 6725)): 
 #0  0x0000003d19c9a541 in nanosleep () from /lib64/libc.so.6 
 #1  0x0000003d19c9a364 in sleep () from /lib64/libc.so.6 
 #2  0x0000000000400976 in thread4(void*) () 
 #3  0x0000003d1a80673d in start_thread () from /lib64/libpthread.so.0 
 #4  0x0000003d19cd40cd in clone () from /lib64/libc.so.6 
 Thread 1 (Thread 0x2b984ecabd90 (LWP 6721)): 
 #0  0x0000003d1a807b35 in pthread_join () from /lib64/libpthread.so.0 
 #1  0x0000000000400900 in main ()  
```

清单 5. 对死锁进程第二次执行 pstack（pstack –进程号）的输出结果
```
[dyu@xilinuxbldsrv purify]$ pstack 6721 
 Thread 5 (Thread 0x40bd6940 (LWP 6722)): 
 #0  0x0000003d1a80d4c4 in __lll_lock_wait () from /lib64/libpthread.so.0 
 #1  0x0000003d1a808e1a in _L_lock_1034 () from /lib64/libpthread.so.0 
 #2  0x0000003d1a808cdc in pthread_mutex_lock () from /lib64/libpthread.so.0 
 #3  0x0000000000400a87 in func1() () 
 #4  0x0000000000400ac3 in thread1(void*) () 
 #5  0x0000003d1a80673d in start_thread () from /lib64/libpthread.so.0 
 #6  0x0000003d19cd40cd in clone () from /lib64/libc.so.6 
 Thread 4 (Thread 0x415d7940 (LWP 6723)): 
 #0  0x0000003d1a80d4c4 in __lll_lock_wait () from /lib64/libpthread.so.0 
 #1  0x0000003d1a808e1a in _L_lock_1034 () from /lib64/libpthread.so.0 
 #2  0x0000003d1a808cdc in pthread_mutex_lock () from /lib64/libpthread.so.0 
 #3  0x0000000000400a03 in func2() () 
 #4  0x0000000000400a3f in thread2(void*) () 
 #5  0x0000003d1a80673d in start_thread () from /lib64/libpthread.so.0 
 #6  0x0000003d19cd40cd in clone () from /lib64/libc.so.6 
 Thread 3 (Thread 0x41fd8940 (LWP 6724)): 
 #0  0x0000003d19c7aec2 in memset () from /lib64/libc.so.6 
 #1  0x00000000004009be in thread3(void*) () 
 #2  0x0000003d1a80673d in start_thread () from /lib64/libpthread.so.0 
 #3  0x0000003d19cd40cd in clone () from /lib64/libc.so.6 
 Thread 2 (Thread 0x429d9940 (LWP 6725)): 
 #0  0x0000003d19c7ae0d in memset () from /lib64/libc.so.6 
 #1  0x0000000000400982 in thread4(void*) () 
 #2  0x0000003d1a80673d in start_thread () from /lib64/libpthread.so.0 
 #3  0x0000003d19cd40cd in clone () from /lib64/libc.so.6 
 Thread 1 (Thread 0x2af906fd9d90 (LWP 6721)): 
 #0  0x0000003d1a807b35 in pthread_join () from /lib64/libpthread.so.0 
 #1  0x0000000000400900 in main () 
```

连续多次查看这个进程的函数调用关系堆栈进行分析：当进程吊死时，多次使用 pstack 查看进程的函数调用堆栈，死锁线程将一直处于等锁的状态，对比多次的函数调用堆栈输出结果，确定哪两个线程（或者几个线程）一直没有变化且一直处于等锁的状态（可能存在两个线程 一直没有变化）。

输出分析：

根据上面的输出对比可以发现，线程 1 和线程 2 由第一次 pstack 输出的处在 sleep 函数变化为第二次 pstack 输出的处在 memset 函数。但是线程 4 和线程 5 一直处在等锁状态（pthread_mutex_lock），在连续两次的 pstack 信息输出中没有变化，所以我们可以推测线程 4 和线程 5 发生了死锁。

Gdb into thread输出：

清单 6. 然后通过 gdb attach 到死锁进程

```
 (gdb) info thread 
  5 Thread 0x41e37940 (LWP 6722)  0x0000003d1a80d4c4 in __lll_lock_wait () 
  from /lib64/libpthread.so.0 
  4 Thread 0x42838940 (LWP 6723)  0x0000003d1a80d4c4 in __lll_lock_wait () 
  from /lib64/libpthread.so.0 
  3 Thread 0x43239940 (LWP 6724)  0x0000003d19c9a541 in nanosleep () 
 from /lib64/libc.so.6 
  2 Thread 0x43c3a940 (LWP 6725)  0x0000003d19c9a541 in nanosleep () 
 from /lib64/libc.so.6 
 * 1 Thread 0x2b984ecabd90 (LWP 6721)  0x0000003d1a807b35 in pthread_join () 
 from /lib64/libpthread.so.0 
```

清单 7. 切换到线程 5 的输出

```
(gdb) thread 5 
 [Switching to thread 5 (Thread 0x41e37940 (LWP 6722))]#0  0x0000003d1a80d4c4 in 
 __lll_lock_wait () from /lib64/libpthread.so.0 
 (gdb) where 
 #0  0x0000003d1a80d4c4 in __lll_lock_wait () from /lib64/libpthread.so.0 
 #1  0x0000003d1a808e1a in _L_lock_1034 () from /lib64/libpthread.so.0 
 #2  0x0000003d1a808cdc in pthread_mutex_lock () from /lib64/libpthread.so.0 
 #3  0x0000000000400a9b in func1 () at lock.cpp:18 
 #4  0x0000000000400ad7 in thread1 (arg=0x0) at lock.cpp:43 
 #5  0x0000003d1a80673d in start_thread () from /lib64/libpthread.so.0 
 #6  0x0000003d19cd40cd in clone () from /lib64/libc.so.6 
```

清单 8. 线程 4 和线程 5 的输出

```
(gdb) f 3 
 #3  0x0000000000400a9b in func1 () at lock.cpp:18 
 18          pthread_mutex_lock(&mutex2); 
 (gdb) thread 4 
 [Switching to thread 4 (Thread 0x42838940 (LWP 6723))]#0  0x0000003d1a80d4c4 in 
 __lll_lock_wait () from /lib64/libpthread.so.0 
 (gdb) f 3 
 #3  0x0000000000400a17 in func2 () at lock.cpp:31 
 31          pthread_mutex_lock(&mutex1); 
 (gdb) p mutex1 
 $1 = {__data = {__lock = 2, __count = 0, __owner = 6722, __nusers = 1, __kind = 0, 
 __spins = 0, __list = {__prev = 0x0, __next = 0x0}}, 
  __size = "\002\000\000\000\000\000\000\000B\032\000\000\001", '\000'
 , __align = 2} 
 (gdb) p mutex3 
 $2 = {__data = {__lock = 0, __count = 0, __owner = 0, __nusers = 0, 
 __kind = 0, __spins = 0, __list = {__prev = 0x0, __next = 0x0}}, 
 __size = '\000' , __align = 0} 
 (gdb) p mutex2 
 $3 = {__data = {__lock = 2, __count = 0, __owner = 6723, __nusers = 1, 
 __kind = 0, __spins = 0, __list = {__prev = 0x0, __next = 0x0}}, 
  __size = "\002\000\000\000\000\000\000\000C\032\000\000\001", '\000'
 , __align = 2} 
 (gdb) 
```

从上面可以发现，线程 4 正试图获得锁 mutex1，但是锁 mutex1 已经被 LWP 为 6722 的线程得到（__owner = 6722），线程 5 正试图获得锁 mutex2，但是锁 mutex2 已经被 LWP 为 6723 的 得到（__owner = 6723），从 pstack 的输出可以发现，LWP 6722 与线程 5 是对应的，LWP 6723 与线程 4 是对应的。所以我们可以得出， 线程 4 和线程 5 发生了交叉持锁的死锁现象。查看线程的源代码发现，线程 4 和线程 5 同时使用 mutex1 和 mutex2，且申请顺序不合理。

总结

本文简单介绍了一种在 Linux 平台下分析死锁问题的方法，对一些死锁问题的分析有一定作用。希望对大家有帮助。理解了死锁的原因，尤其是产生死锁的四个必要条件，就可以最大可能地避免、预防和解除死锁。所以，在系统设计、进程调度等方面注意如何不让这四个必要条件成立，如何确定资源的合理分配，避免进程永久占据系统资源。此外，也要防止进程在处于等待状态的情况下占用资源 , 在系统运行过程中，对进程发出的每一个系统能够满足的资源申请进行动态检查，并根据检查结果决定是否分配资源，若分配后系统可能发生死锁，则不予分配，否则予以分配。因此，对资源的分配要给予合理的规划，使用有序资源分配法和银行家算法等是避免死锁的有效方法。



