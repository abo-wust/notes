OpenVPN是一个很难评价的东西，我可以说出超过10个它的好处，并发出一些感慨。然而针对每一个优点，它却又有些美中不足，然而这又有什么大不了的呢，够用就可以了吧！不过，如果你遇到了一些场合，它真的就不够用了怎么办？答案只有一个，改掉它！
OpenVPN架设方便，配置简单，安全策略丰富，这些都很好，然而却不适合大型的网到网的拓扑，那种应用大家都用高端的专用设备了比如Cisco或者华为的VPN箱子。对于一个技术偏执狂来讲，在linux或者BSD上能完成这样的小盒子也算是对自己的一种鼓舞。本文介绍一个自己对tun网卡的小修改，使得多个OpenVPN进程可以负载均衡，分担大流量。讨论基于Linux，内核版本2.6.8

0.问题出在哪

OpenVPN的性能问题出在哪呢？前面的《OpenVPN性能》系列文章深入的探讨了这个问题，本文着重分析这一篇(《[OpenVPN性能\-OpenVPN的第一个瓶颈在tun驱动](http://blog.csdn.net/dog250/article/details/6300347)》)谈到的tun驱动。
Linux下的tun驱动很简单，多个用户态进程无法共享一个虚拟网卡，猛抬头，这是很不合理的，你见过物理网卡是进程独占的么？肯定没有！然而低头沉思后，发现这个对比不合理，物理网卡在进程的下端，而tun虚拟网卡则在用户进程上端，对于虚拟网卡而言，用户态进程就是一根线缆，姑且叫做进程线缆吧。

1.TUN网卡怎么啦

要理解一个技术以及评价它，不要认为只了解它的原理并且会使用就够了，更重要的要从历史的角度去考虑，这样你就会明白设计者当初为何这么搞，其实最终很多技术纯粹是瞎搞。举个例子，科班出身的，没有谁不知道Intel处理器的分段，背得滚瓜烂熟的大有人在，可是这个技术在纯技术方面就是一种瞎搞，只是为了产品的兼容！
那么TUN网卡怎么啦？通过阅读Linux内核的文档Documentation/networking/tuntap.txt，我们看到：
Universal TUN/TAP device driver.
Copyright (C) 1999-2000 Maxim Krasnyansky
Linux, Solaris drivers
Copyright (C) 1999-2000 Maxim Krasnyansky
FreeBSD TAP driver
Copyright (c) 1999-2000 Maksim Yevmenkin
通用的tun驱动设计和其Linux平台以及Solaris平台的设计是同一人，当初在这些开放平台上设计tun驱动的初衷是将数据包导入导出到用户态，这个理念一定要彻底理解才行，不要将tun/tap网卡和那些其它的虚拟网卡相混淆，这些其它的虚拟网卡包括FreeSWAN使用的隧道口，以及Cisco的VTI，注意，TUN仅仅为了将数据包导入导出到用户态。
TUN驱动完全模拟一个点对点的或者以太网的网卡，一个实例仅仅模拟一个这样的网卡，这是很合理的，因为对于点对点网卡而言，比如HDLC适配器，和对端仅仅通过一根线缆连接，如果一个实例可以模拟任意多个点对点网卡，就需要对端也模拟同样多个，而我们不希望有这种额外的耦合；对于以太网卡的模拟，虽然一个实例一个，仍然可以通过配置将多个实例的tap网卡bonding在一起，bonding驱动在linux中实现的很好，专门针对以太网，而且配置简单，除了这个，我们还可以通过配置将多个tap网卡bridge在一起，bridge驱动在linux中实现的很好，而且配置简单。
因此，目前为止，TUN驱动没有错。

2.OpenVPN又怎么啦

从第0小节可知，此时OpenVPN只是一根线缆，而不是传统的写入物理网卡数据的进程，它单进程，大循环，结构超原始。一个网卡口只能插一根线啊，那么也就是只能有一个OpenVPN进程，如果这样，难道没有办法了么？在写前面那篇文章（[http://blog.csdn.net/dog250/article/details/6300347](http://blog.csdn.net/dog250/article/details/6300347)）时，我很失望，于是我改了tun驱动，增加了吞吐量。现在想想，突然又有了新的想法，虽然我们不能在一个虚拟网卡上插两根进程线缆，但是我们可以加粗它啊，不怪路太少，只怪线太细。
于是，新的办法就来了，多个OpenVPN共享一个虚拟网卡。可是问题又来了，这需要修改tun驱动，因为原生的tun是独占性的，不过这个倒不难。于是问题就仅仅成为将OpenVPN改成多进程的了，为何不改成多线程的呢，因为怕遇到全局变量和并发问题...改成多进程的方式很简单，bash脚本即可做到，启动多个OpenVPN进程，使用同一个dev参数即可。
因此，目前，OpenVPN本身的架构出了点问题。

3.它们结合呢

本小节暂且不谈共享虚拟网卡的问题，仅仅说一下单个的OpenVPN和单路的虚拟网卡结合会有什么性能瓶颈。
这个性能瓶颈体现在几个排队上：首先，进入OpenVPN的socket数据会排在一个单一的队列上上，由于OpenVPN单进程大循环，造成即使你有X个cpu，Intel82576千兆卡，在大数据量下也会出现只有一个cpu忙碌，网卡利用率超低的现象；其次，由于tun驱动只从单一的OpenVPN进程收取和发送封装后的IP数据报或者以太帧，进入tun/tap网卡的流量限于OpenVPN的流进tun字符设备的速率，十分有限，对于从tun/tap发出的IP数据报或者以太帧，由于单个的OpenVPN的socket发送能力有限，和tun/tap网卡forward对接的是一个高性能千兆卡，也毫无用武之地！如下图：

![](image_1.ef0f845a.png)

单一的CPU超级忙碌，其它CPU无法分担，超棒的千兆卡闲在那里...这简直是愤怒爆发的千兆！其实Intel千兆卡的收发芯片处理能力要比CPU高的，因为每块卡要应对所有的CPU的并发读写，有时甚至需要自己去内存收揽数据...如此的高僧，闲着可惜！唯一快累死的就是那唯一不幸的cpu，OpenVPN此时在它之上运行！

4.改掉TUN驱动

我们看看改了TUN驱动会发生什么。改是肯定能改的，问题是怎么改，前天受老大又一番教诲：“时刻写出可重用的代码”，我决定不再往代码中随意加入printk("dsdsffgerg####################:------------%s\\n",ptr);之类的啦，我决定一开始就将它做成可配置的独占策略，也就是说完全兼容现有的独占策略，具体实现看第7小节吧。现在还不是实现的时候，现在只是说一下解决了哪些性能问题。我先给出图示，然后再分析：

![](image_2.2ef9232d.png)

多个OpenVPN共享一个虚拟网卡之后，多个OpenVPN可以运行在多个CPU上，并且都可以从单一的虚拟网卡收发数据，虚拟网卡字符设备的排队延迟现象减轻了，吞吐量更高，由于出现了多个OpenVPN进程，充分利用的多个CPU可以向Intel82576千兆卡同时发送大量的数据，网卡的利用率增加了，相信Intel的以太网芯片吧，它同时还会减轻单个OpenVPN进程的socket排队现象。
此时的整台机器运行的比较和谐，没有闲着的，既然都很忙，计算机体系结构的可用性以及能量守恒定律会保证数据包的转发率以及吞吐量的增加。以上的结论是实际测试的结果，采用这种方式后，查看服务器端的日志，会发现负载分担给了多个OpenVPN进程，这正是想要的结果。如果能配合CPU绑定，配置中断绑定，那么多CPU的服务器就能作为多台机器使用了，实验得真知！
对于一个需要forward到虚拟网卡的数据包，最终到达了tun的字符设备，在linux驱动中tun\_net\_xmit函数中(每一个网卡都会有一个hard\_xmit回调函数)注意以下这一句：
```
1. if (tun->flags & TUN_FASYNC)
2. kill_fasync(&tun->fasync, SIGIO, POLL_IN);
3. wake_up_interruptible(&tun->read_wait);
4. return 0;
```

如果我们使用了OpenVPN共享一个虚拟网卡，那么在tun->read\_wait上睡眠的就可能不止一个进程，可能是很多进程，对于性能比较好的CPU和网卡而言，会出现有多少CPU唤醒多少OpenVPN进程的壮观现象。

5.OpenVPN和TUN的多对多结合

在列出真的实现之前，先考虑另一种替代方案，这种方案不需要修改任何东西，那就是运行多个OpenVPN实例，然后将它们的tap网卡bonding在一起，注意这种方案使用了bonding驱动，而bonding驱动只适用于以太网场合，这就意味着你不能使用tun网卡了，而tun网卡不需要以太网封装，没有arp等链路层地址解析，效率更高些，这就产生了一个矛盾，然而也能带来一些性能提升，具体操作可参见《[OpenVPN性能\-当tap遇到bonding](http://blog.csdn.net/dog250/article/details/6398331)》。
最终，这种方案的应用场合有限，并且bonding驱动本身就能带来一些开销。我倒是觉得，如果需要bridge VPN，将bonding和bridge结合是一种很好的方式。

6.OpenVPN和TUN的多对一结合

这个就不多说了，就是本文最终推荐的方式，你需要做的仅仅是修改下tun驱动。但是如果你使用Windows客户端且仅有一个客户端的话，修改Window的TAP-win32也是必要的，否则只能带来OpenVPN服务器的最大连接数的增加。然则本人的DDK环境坏掉了，也不想面对频繁的蓝屏，因此也就不给出windows的驱动代码了，说实话，这个代码我还没有完成！

7.第6小节的实现

实现是评估后的最后的步骤，其实也是相对简单的步骤，对于linux平台的tun驱动的修改，再也没有比这个更简单的了，本文基于kernel 2.6.8，因为这个版本简单，可以最快速度做实验，对于高版本修改思想类似。首先修改linux/if\_tun.h头文件：
```
1. struct tun_struct {
2. struct list_head list;
3. ...
4. int users; //增加引用计数
5. #define IFF_MULTI 0x08 //增加一个flags，指示是否独占方式，1为共享，2为独占
6. };
```

然后修改driver/net/tun.c吧
第一步：修改ioctl函数tun\_chr\_ioctl。增加一个命令，设置或者清除IFF\_MULTI位。
第二步：修改tun\_set\_iff函数：
```
1. tun = tun_get_by_name(ifr->ifr_name);
2. if (tun) {==
3. if (tun->attached)
4. return -EBUSY;
5. /* Check permissions */
6. if (tun->owner != -1 &&
7. current->euid != tun->owner && !capable(CAP_NET_ADMIN))
8. return -EPERM;
9. }
```

将之修改为：

```
1. tun = tun_get_by_name(ifr->ifr_name);
2. if (tun) {
3. if (tun->flags & IFF\_MULTI) { //即使找到了现已经存在设备，如果设置了共享，那么直接使用它
4. file->private_data = tun;
5. tun->attached = 1;
6. tun->users++; //增加引用计数。原则上应该使用atomic_inc
7. strcpy(ifr->ifr_name, tun->dev->name);
8. return 0;
9. }
10. if (tun->attached)
11. return -EBUSY;
12. /* Check permissions */
13. if (tun->owner != -1 &&
14. current->euid != tun->owner && !capable(CAP_NET_ADMIN))
15. return -EPERM;
16. }
```

在第一次建立设备时需要递增users计数：

```
1. file->private_data = tun;
2. tun->attached = 1;
3. strcpy(ifr->ifr_name, tun->dev->name);
4. return 0;
5. err_free_dev:
6. free_netdev(dev);
```
修改为：
```
1. file->private_data = tun;
2. tun->attached = 1;
3. tun->users++;
4. 
5. strcpy(ifr->ifr_name, tun->dev->name);
6. return 0;
7. 
8. free_dev:
9. free_netdev(dev);
```

第三步：修改tun\_chr\_close函数：

```
1. /* Detach from net device */
2. file->private_data = NULL;
3. tun->attached = 0;
4. /* Drop read queue */
5. skb_queue_purge(&tun->readq);
6. if (!(tun->flags & TUN_PERSIST)) {
7. list_del(&tun->list);
8. unregister_netdevice(tun->dev);
9. }
```

修改为：
```
1. tun->users--; //递减引用计数，原则上该使用atomic_dec
2. if (!tun->users) {
3. /* Detach from net device */
4. file->private_data = NULL;
5. tun->attached = 0;
6. /* Drop read queue */
7. skb_queue_purge(&tun->readq);
8. if (!(tun->flags & TUN_PERSIST)) {
9. list_del(&tun->list);
10. ==unregister_netdevice(tun->dev);
11. }
12. }
```

修改完毕，重新加载tun.ko即可看到效果。
实验很简单，在服务器端和客户端分别准备N个配置文件，服务器端的N个配置文件除了端口需要不同外其它配置一律一样，客户端的N个配置文件除了remote参数中的端口分别对应于服务器的服务端口外其它完全一样，注意都需要指定一个特定的虚拟网卡，比如dev tap0，然后启动它们，马上见效果。

8.总结于展望
OpenVPN很不错，但是我还是觉得它最大的好处在于可以随心定制。最近在OpenVPN社区也没有发现其有开发多线程版本的迹象，不知道是不是受tun驱动限制，但是不管怎么说，在其现有的源代码中，存在一个函数：

```
1. void
2. tunnel_server_udp (struct context *top)
3. {
4. tunnel_server_udp_single_threaded (top);
5. }
```

从它的名子来看，thread也是可以期待的。我们没有必要自己实现thread，而只是等待即可，但是如果你有更好的主意，千万别独吞。现如今，通过外部脚本去自动化生成不同的配置文件以及启动不同的OpenVPN实例是最简单的方式。


-----



-----


