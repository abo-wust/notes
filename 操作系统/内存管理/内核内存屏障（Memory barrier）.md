
https://zhuanlan.zhihu.com/p/522576430

前言：要了解如何使用memory barrier，最好的方法是明白它为什么存在。CPU硬件设计为了提高指令的执行速度，增设了两个缓冲区（store buffer, invalidate queue）。这个两个缓冲区可以避免CPU在某些情况下进行不必要的等待，从而提高速度，但是这两个缓冲区的存在也同时带来了新的问题。


### 1. cache一致性问题

- Cache 一致性问题出现的原因是在一个多处理器系统中，每个处理器核心都有独占的Cache 系统（比如一级 Cache 和二级 Cache），而导致一个内存块在系统中同时可能有多个备份，从而引起访问时的不一致性问题。

- Cache 一致性问题的根源是因为存在多个处理器独占的 Cache，而不是多个处理器。它的限制条件比较多：多核，独占 Cache，Cache 写策略。当其中任一个条件不满足时便不存在cache一致性问题。


### 2. cache一致性协议：MESI

MESI 协议是 Cache line 四种状态的首字母的缩写，分别是修改（Modified）态、独占（Exclusive）态、共享（Shared）态和失效（Invalid）态。 Cache 中缓存的每个 Cache Line 都必须是这四种状态中的一种。

修改态（Modified），如果该 Cache Line 在多个 Cache 中都有备份，那么只有一个备份能处于这种状态，并且“dirty”标志位被置上。拥有修改态 Cache Line 的 Cache 需要在某个合适的时候把该 Cache Line 写回到内存中。但是在写回之前，任何处理器对该 Cache Line在内存中相对应的内存块都不能进行读操作。 Cache Line 被写回到内存中之后，其状态就由修改态变为共享态。

独占态（Exclusive），和修改状态一样，如果该 Cache Line 在多个 Cache 中都有备份，那么只有一个备份能处于这种状态，但是“dirty”标志位没有置上，因为它是和主内存内容保持一致的一份拷贝。如果产生一个读请求，它就可以在任何时候变成共享态。相应地，如果产生了一个写请求，它就可以在任何时候变成修改态。

共享态（Shared），意味着该 Cache Line 可能在多个 Cache 中都有备份，并且是相同的状态，它是和内存内容保持一致的一份拷贝，而且可以在任何时候都变成其他三种状态。

失效态（Invalid），该 Cache Line 要么已经不在 Cache 中，要么它的内容已经过时。一旦某个Cache Line 被标记为失效，那它就被当作从来没被加载到 Cache 中

![](images/Pasted%20image%2020230213090235.png)

MESI使用消息传递的方式在上述几种状态之间切换，具体转换过程参见[1]。常见的消息类型：

read: 包含要读取的CACHE-LINE的物理地址
read response: 包含READ请求的数据，要么由内存满足要么由cache满足
invalidate: 包含要invalidate的cache-line的物理地址，所有其他cache必须移除相应的数据项
invalidate ack: 回复消息
read invalidate: 包含要读取的cache-line的物理地址，同时使其他cache移除该数据。需要read response和invalidate ack消息
writeback：包含要写回的数据和地址，该状态将处于modified状态的lines写回内存，为其他数据腾出空间


### 3. Store buffer的引入

虽然该MESI协议可以保证数据的一致性，但是在某种情况下并不高效。举例来说，如果CPU0要更新一个处于CPU1-cache中的数据，那么它必须等待 cache-line从CPU1-cache传递到CPU0-cache，然后再执行写操作。cache之间的传递需要花费大量的时间，比执行一个简单的 操作寄存器的指令高出几个数量级。而事实上，花费这个时间根本毫无意义，因为不论从CPU1-cache传递过来的数据是什么，CPU0都会覆盖它。为了 解决这个问题，硬件设计者引入了store buffer，该缓冲区位于CPU和cache之间，当进行写操作时，CPU直接将数据写入store buffer，而不再等待另一个CPU的消息。但是这个设计会导致一个很明显的错误情况。

![](images/Pasted%20image%2020230213090218.png)


#### 3.1 引入store buffer后出现的问题1

试考虑如下代码：
```
a = 1;
b = a + 1;
assert(b == 2);
```

假设初始时a和b的值都是0，a处于CPU1-cache中，b处于CPU0-cache中。

如果按照下面流程执行这段代码：

- CPU 0 starts executing the a = 1
- CPU 0 looks “a” up in the cache, and finds that it is missing
- CPU 0 therefore sends a “read invalidate” message in order to get exclusive ownership of the cache line containing “a”.
- CPU 0 records the store to “a” in its store buffer.
- CPU 1 receives the “read invalidate” message, and responds by transmitting the cache line and removing that cacheline from its cache.
- CPU 0 starts executing the b = a + 1.
- CPU 0 receives the cache line from CPU 1, which still has a value of zero for “a”.
- CPU 0 loads “a” from its cache, finding the value zero.
- CPU 0 applies the entry from its store buffer to the newly arrived cache line, setting the value of “a” in its cache to one.
- CPU 0 adds one to the value zero loaded for “a” above, and stores it into the cache line containing “b” (which we will assume is already owned by CPU 0).
- CPU 0 executes assert(b == 2), which fails.

出现问题的原因是我们有两份”a”的拷贝，一份在cache-line中，一份在store buffer中。硬件设计师的解决办法是“store forwarding”，当执行load操作时，会同时从cache和store buffer里读取。也就是说，当进行一次load操作，如果store-buffer里有该数据，则CPU会从store-buffer里直接取出数 据，而不经过cache。因为“store forwarding”是硬件实现，我们并不需要太关心。


#### 3.2 引入store buffer后出现的问题2

请看下面的代码：

```
void foo(void)
{
    a = 1;
    b = 1;
}
 
void bar(void)
{
    while (b == 0) continue;
    assert(a == 1);
}
```

假设变量a在CPU1-cache中，b在CPU0-cache中。CPU0执行foo()，CPU1执行bar()。

![](images/Pasted%20image%2020230213090156.png)

程序执行的顺序如下：

- 1.CPU 0 执行a = 1。缓存行不在CPU0的缓存中，因此CPU0将“a”的新值放到存储缓冲区，并发送一个“读使无效”消息
- 2.CPU 1 执行while (b == 0) continue，但是包含“b”的缓存行不在缓存中，它发送一个“读”消息。
- 3.CPU 0 执行b = 1，它已经在缓存行中有“b”的值了(换句话说，缓存行已经处于“modified”或者“exclusive”状态)，因此它存储新的“b”值在它的缓存行中。
- 4.CPU 0 接收到“读”消息，并且发送缓存行中的最新的“b”的值到CPU1，同时将缓存行设置为“shared”状态
- 5.CPU 1 接收到包含“b”值的缓存行，并将其值写到它的缓存行中
- 6.CPU 1 现在结束执行while (b == 0) continue，因为它发现“b”的值是1，它开始处理下一条语句。
- 7.CPU 1 执行assert(a == 1)，并且，由于CPU 1 工作在旧的“a”的值，因此验证失败。
- 8.CPU 1 接收到“读使无效”消息, 并且发送包含“a”的缓存行到CPU0，同时使它的缓存行变成无效。但是已经太迟了。
- 9.CPU 0 接收到包含“a”的缓存行，将且将存储缓冲区的数据保存到缓存行中，这使得CPU1验证失败。

就是说，可能出现这类情况，b已经赋值了，但是a还没有，所以出现了b = 1, a = 0的情况。对于这类问题，硬件设计者也爱莫能助，因为CPU无法知道变量之间的关联关系。所以硬件设计者提供了memory barrier指令，让软件来告诉CPU这类关系。

解决办法是：使用硬件设计者提供的“内存屏障”来修改代码：

```
void foo(void)
{
    a = 1;
    smp_mb();
    b = 1;
}
```

smp_mb()指令可以迫使CPU在进行后续store操作前刷新store-buffer。以上面的程序为例，增加memory barrier之后，就可以保证在执行b=1的时候CPU0-store-buffer中的a已经刷新到cache中了，此时CPU1-cache中的a 必然已经标记为invalid。对于CPU1中执行的代码，则可以保证当`b==0`为假时，a已经不在CPU1-cache中，从而必须从CPU0- cache传递，得到新值“1”


### 4. Invalidation Queue的引入

store buffer一般很小，所以CPU执行几个store操作就会填满, 这时候CPU必须等待invalidation ACK消息（得到invalidation ACK消息后会将storebuffer中的数据存储到cache中，然后将其从store buffer中移除），来释放store buffer缓冲区空间。

“Invalidation ACK”消息需要如此长的时间，其原因之一是它们必须确保相应的缓存行实际变成无效了。如果缓存比较忙的话，这个使无效操作可能被延迟。例如，如果CPU密集的装载或者存储数据，并且这些数据都在缓存中。另外，如果在一个较短的时间内，大量的“使无效”消息到达，一个特定的CPU会忙于处理它们。这会使得其他CPU陷于停顿。但是，在发送应答前，CPU 不必真正的使无效缓存行。它可以将使无效消息排队。并且它明白，在发送更多的关于该缓存行的消息前，需要处理这个消息。

一个带Invalidation Queue的CPU可以迅速应答一个Invalidation Ack消息，而不必等待相应的行真正变成无效状态。于是乎出现了下面的组织架构：

![](images/Pasted%20image%2020230213090115.png)

但是，此种方法也存在如下问题：
```
void foo(void)
{
    a = 1;
    smp_mb();
    b = 1;
}
 
void bar(void)
{
    while (b == 0) continue;
    assert(a == 1);
}
```

- 1.CPU0执行a=1。因为cache-line是shared状态，所以新值放到store-buffer里，并传递invalidate消息来通知CPU1
- 2.CPU1执行` while(b==0) continue;`但是b不再CPU1-cache中，所以发送read消息
- 3.CPU1接受到CPU0的invalidate消息，将其排队，然后返回ACK消息
- 4.CPU0接收到来自CPU1的ACK消息，然后执行smp_mb()，将a从store-buffer移到cache-line中。（内存屏蔽在此处生效了）
- 5.CPU0执行b=1;因为已经包含了该cache-line，所以将b的新值写入cache-line
- 6.CPU0接收到了read消息，于是传递包含b新值的cache-line给CPU1，并标记为shared状态
- 7.CPU1接收到包含b的cache-line
- 8.CPU1继续执行`while(b==0) continue;`因为为假所以进行下一个语句
- 9.CPU1执行`assert(a==1)`，因为a的旧值依然在CPU1-cache中，断言失败
- 10.尽管断言失败了，但是CPU1还是处理了队列中的invalidate消息，并真的invalidate了包含a的cache-line，但是为时已晚

出现问题的原因是，当CPU排队某个invalidate消息后，并做错了应答Invalidate Ack, 但是在它还没有处理这个消息之前，就再次读取了位于cache中的数据，该数据此时本应该已经失效，但由于未处理invalidate消息导致使用错误。

解决方法是在bar()中也增加一个memory barrier：
```
void bar(void)
{
    while (b == 0) continue;
    smp_mb();
    assert(a == 1);
}
```

此处smp_mb()的作用是处理“Invalidate Queues”中的消息，于是在执行`assert(a==1)`时，CPU1中的包含a的cache-line已经无效了，新的值要重新从CPU0-cache中读取

从这两个例子可以看出：

smp_mb();既可以用来处理storebuffer中的数据，也可以用来处理Invalidation Queue中的Invalid消息。实际上，memory barrier确实可以细分为“write memory barrier(wmb)”和“read memory barrier(rmb)”。rmb只处理Invalidate Queues，wmb只处理store buffer。
```
void foo(void)
{
    a = 1;
    smp_wmb();/*CPU1要使用该值，因此需要及时更新处理store buffer中的数据*/
    b = 1;
}
 
void bar(void)
{
    while (b == 0) continue;
    smp_rmb();/*由于CPU0修改了a值，使用此值时及时处理Invalidation Queue中的消息*/
    assert(a == 1);
}
```


参考文献：

【1】Why Memory Barriers ？
【2】深入理解并行编程V2.0
【3】Perf:Is Parallel Programming Hard, And, If So, What Can You Do About It?