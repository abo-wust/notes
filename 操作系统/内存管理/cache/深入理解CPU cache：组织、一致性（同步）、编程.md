深入理解CPU cache：组织、一致性（同步）、编程

收录于合集

以下文章来源于Linux阅码场 ，作者宋宝华



**CACHE 基础**



对cache的掌握，对于Linux工程师（其他的非Linux工程师也一样）写出高效能代码，以及优化Linux系统的性能是至关重要的。简单来说，cache快，内存慢，硬盘更慢。在一个典型的现代CPU中比较接近改进的哈佛结构，cache的排布大概是这样的：

![](images/Pasted%20image%2020221202001807.png)



L1速度\>  L2速度\> L3速度\> RAM
L1容量<  L2容量< L3容量< RAM

**现代CPU，通常L1 cache的指令和数据是分离的。**
**这样可以实现2条高速公路并行访问，CPU可以同时load指令和数据。当然，cache也不一定是一个core独享，现代很多CPU的典型分布是这样的，比如多个core共享一个L3。**

比如这台的Linux里面运行 **lstopo** 命令：

![](images/Pasted%20image%2020221202001811.png)

人们也常常称呼L2cache为 **MLC （MiddleLevel Cache）**, **L3cache为 LLC（Last LevelCache）**。

**这些Cache究竟有多块呢？**

我们来看看Intel的数据，具体配置：**Intel i7-4770 (Haswell), 3.4 GHz (Turbo Boostoff), 22 nm. RAM: 32 GB (PC3-12800 cl11 cr2)**

访问延迟：

L1 Data Cache Latency = 4 cycles for simple access via pointer
L1 Data Cache Latency = 5 cycles for access with complex address calculation (size\_t n, \*p; n = p\[n\]).
L2 Cache Latency = 12 cycles
L3 Cache Latency = 36 cycles (3.4 GHz i7\-4770)
L3 Cache Latency = 43 cycles (1.6 GHz E5\-2603 v3)
L3 Cache Latency = 58 cycles (core9) - 66 cycles (core5) (3.6 GHz E5\-2699 v3 - 18 cores)
RAM Latency = 36 cycles + 57 ns (3.4 GHz i7\-4770)
RAM Latency = 62 cycles + 100 ns (3.6 GHz E5\-2699 v3 dual)

数据来源：https://www.7-cpu.com/cpu/Haswell.html

由此我们可以知道，我们应该尽可能追求cache的命中率高，以避免延迟， **最好是低级cache的命中率越高越好。**

**CACHE 的组织**



**SET、WAY、TAG、INDEX**



现代的cache基本按照这个模式来组织：**SET、WAY、TAG、INDEX**
**，这几个概念是理解Cache的关键。随便打开一个数据手册，就可以看到这样的字眼：**

![](images/Pasted%20image%2020221202001826.png)

但是它的执行时间，则远远不到后者的8倍：

![](images/Pasted%20image%2020221202001829.png)

**16KB** 的cache是 **4way** 的话，每个set包括 **4\*64B** ，则整个cache分为 **16KB/64B/4 = 64set**
**，也即2的6次方。当CPU从cache里面读数据的时候，它会用地址位的BIT6-BIT11来寻址set，BIT0-BIT5是cacheline内的offset。**

![](images/Pasted%20image%2020221202001833.png)

比如CPU访问地址

0 000000 XXXXXX

或者

1 000000 XXXXXX

或者

YYYY 000000 XXXXXX

由于它们红色的6位都相同，所以他们全部都会找到第0个set的cacheline。第0个set里面有4个way，之后硬件会用地址的高位如0,1，YYYY作为tag，去检索这4个way的tag是否与地址的高位相同，而且cacheline是否有效，如果tag匹配且cacheline有效，则cache命中。

**所以地址YYYYYY 000000XXXXXX全部都是找第0个set，YYYYYY000001XXXXXX全部都是找第1个set，YYYYYY111111XXXXXX全部都是找第63个set。**每个set中的4个way，都有可能命中。

中间红色的位就是INDEX，前面YYYY这些位就是TAG。

具体的实现可以是用虚拟地址或者物理地址的相应位做TAG或者INDEX。

1. **如果用虚拟地址做TAG，我们叫VT；**
2. **如果用物理地址做TAG，我们叫PT；**
3. **如果用虚拟地址做INDEX，我们叫VI；**
4. **如果用物理地址做INDEX，我们叫PI。**

工程中碰到的cache可能有这么些组合：**VIVT、VIPT、PIPT。**

**VIVT、VIPT、PIPT**



具体的实现可以是用虚拟地址或者物理地址的相应位做TAG或者INDEX。

1. **如果用虚拟地址做TAG，我们叫VT；**
2. **如果用物理地址做TAG，我们叫PT；**
3. **如果用虚拟地址做INDEX，我们叫VI；**
4. **如果用物理地址做INDEX，我们叫PI。**

VIVT的硬件实现开销最低，但是软件维护成本高；PIPT的硬件实现开销最高，但是软件维护成本最低；VIPT介于二者之间，但是有些硬件是VIPT，但是behave
as PIPT，这样对软件而言，维护成本与PIPT一样。

在VIVT的情况下，CPU发出的虚拟地址，不需要经过MMU的转化，直接就可以去查cache。

![](images/Pasted%20image%2020221202001841.png)

而在VIPT和PIPT的场景下，都涉及到虚拟地址转换为物理地址后，再去比对cache的过程。VIPT如下：

![](images/Pasted%20image%2020221202001844.png)

PIPT如下：

![](images/Pasted%20image%2020221202001848.png)

从图上看起来，VIVT的硬件实现效率很高，不需要经过MMU就可以去查cache了。不过，对软件来说，这是个灾难。因为VIVT有严重的歧义和别名问题。

- **歧义：一个虚拟地址先后指向两个（或者多个）物理地址**
- **别名：两个（或者多个）虚拟地址同时指向一个物理地址**

**Cache别名问题**



这里我们重点看别名问题。比如2个虚拟地址对应同一个物理地址，基于VIVT的逻辑，无论是INDEX还是TAG，2个虚拟地址都是可能不一样的(尽管他们的物理地址一样，但是物理地址在cache比对中完全不掺和)，这样它们完全可能在2个cacheline同时命中。

![](images/Pasted%20image%2020221202001853.png)

由于2个虚拟地址指向1个物理地址，这样CPU写过第一个虚拟地址后，写入cacheline1。CPU读第2个虚拟地址，读到的是过时的cacheline2，这样就出现了不一致。所以，为了避免这种情况，软件必须写完虚拟地址1后，对虚拟地址1对应的cache执行clean，对虚拟地址2对应的cache执行invalidate。

**而PIPT完全没有这样的问题，因为无论多少虚拟地址对应一个物理地址，由于物理地址一样，我们是基于物理地址去寻找和比对cache的，所以不可能出现这种别名问题。**

![](images/Pasted%20image%2020221202001856.png)

**那么VIPT有没有可能出现别名呢？答案是有可能，也有可能不能。** 如果VI恰好对于PI，就不可能，这个时候，VIPT对软件而言就是PIPT了：



VI\=PI
PT=PT

那么什么时候VI会等于PI呢？这个时候我们来回忆下虚拟地址往物理地址的转换过程，它是以页为单位的。假设一页是4K，那么地址的低12位虚拟地址和物理地址是完全一样的。回忆我们前面的地址：

YYYYY000000XXXXXX

其中红色的000000是INDEX。在我们的例子中，红色的6位和后面的XXXXXX（cache内部偏移）加起来正好12位，所以这个000000经过虚实转换后，其实还是000000的，这个时候
VI=PI ，VIPT没有别名问题。

我们原先假设的cache是：16KB大小的cache，假设是4路组相联，cacheline的长度是 **64字节**
**，这样我们正好需要红色的6位来作为INDEX。但是如果我们把cache的大小增加为32KB，这样我们需要**
**32KB/4/64B=128=2^7，也即7位来做INDEX。**

YYYY0000000XXXXXX

这样VI就可能不等于PI了，因为红色的最高位超过了2^12的范围，完全可能出现如下2个虚拟地址，指向同一个物理地址：

![](images/Pasted%20image%2020221202001901.png)

**这样就出现了别名问题，**
**我们在工程里，可能可以通过一些办法避免这种别名问题，比如软件在建立虚实转换的时候，把虚实转换往2^13而不是2^12对齐，让物理地址的低13位而不是低12位与物理地址相同，这样强行绕开别名问题，下图中，2个虚拟地址指向了同一个物理地址，但是它们的INDEX是相同的，这样VI=PI，就绕开了别名问题。这通常是PAGE**
**COLOURING技术中的一种技巧。**

![](images/Pasted%20image%2020221202001907.png)

如果这种PAGE
COLOURING的限制对软件仍然不可接受，而我们又想享受VIPT的INDEX不需要经过MMU虚实转换的快捷？有没有什么硬件技术来解决VIPT别名问题呢？确实是存在的，现代CPU很多都是把L1
CACHE做成VIPT，但是表现地（behave as）像PIPT。这是怎么做到的呢？

这要求VIPT的cache，硬件上具备alias detection的能力。比如，硬件知道YYYY **0000000** XXXXXX既有可能出现在第
0000000 ，又可能出现在 **1000000**
**这2个set，然后硬件自动去比对这2个set里面是否出现映射到相同物理地址的cacheline，并从硬件上解决好别名同步，那么软件就完全不用操心了。**

下面我们记住一个简单的规则：

- 对于VIPT，如果cache的size除以WAY数，小于等于1个page的大小，则天然VI=PI，无别名问题；
- 对于VIPT，如果cache的size除以WAY数，大于1个page的大小，则天然VI≠PI，有别名问题；这个时候又分成2种情况:



- 硬件不具备alias detection能力，软件需要pagecolouring；
- 硬件具备alias detection能力，软件把cache当成PIPT用。


比如cache大小64KB，4WAY，PAGE SIZE是4K，显然有别名问题；这个时候，如果cache改为16WAY，或者PAGE
SIZE改为16K，不再有别名问题。为什么？感觉小学数学知识也能算得清😊

**CACHE 的一致性**



Cache的一致性有这么几个层面

- 1.一个CPU的icache和dcache的同步问题
- 2.多个CPU各自的cache同步问题
- 3.CPU与设备（其实也可能是个异构处理器，不过在Linux运行的CPU眼里，都是设备，都是 **DMA** ）的cache同步问题

![](images/Pasted%20image%2020221202001912.png)

**cache中的映射**



**1\. 直接映射**

一个内存地址能被映射到的Cache line是固定的。就如每个人的停车位是固定分配好的，可以直接找到。缺点是：因为人多车位少，很可能几个人争用同一个车位，导致Cache淘汰换出频繁，需要频繁的从主存读取数据到Cache，这个代价也较高。

**2\. 全相联映射**

主存中的一个地址可被映射进任意cache line，问题是：当寻找一个地址是否已经被cache时，需要遍历每一个cache line来寻找，这个代价很高。就像停车位可以大家随便停一样，停的时候简单，找车的时候需要一个一个停车位的找了。
主存中任何一块都可以映射到Cache中的任何一块位置上。
全相联映射方式比较灵活，主存的各块可以映射到Cache的任一块中，Cache的利用率高，块冲突概率低，只要淘汰Cache中的某一块，即可调入主存的任一块。但是，由于Cache比较电路的设计和实现比较困难，这种方式只适合于小容量Cache采用。

**3\. 组相联映射**

组相联映射实际上是直接映射和全相联映射的折中方案，其组织结构如图（3）所示。
主存和Cache都分组，主存中一个组内的块数与Cache中的分组数相同，组间采用直接映射，组内采用全相联映射。也就是说，将Cache分成2^u组，每组包含2^v块，主存块存放到哪个组是固定的，至于存到该组哪一块则是灵活的。即主存的某块只能映射到Cache的特定组中的任意一块。主存的某块b与Cache的组k之间满足以下关系：k=b%(2^u).

**icache、dcache同步 - 指令流（ icache ）和数据流（ dcache ）**



先看一下 **ICACHE** 和 **DCACHE** 同步问题。由于程序的运行而言， **指令流的都流过icache** ，而指令中涉及到的**数据流经过dcache** 。所以对于自修改的代码（Self-Modifying Code）而言，比如我们修改了内存p这个位置的代码（典型多见于JIT
compiler），这个时候我们是通过store的方式去写的p，所以新的指令会进入dcache。但是我们接下来去执行p位置的指令的时候，icache里面可能命中的是修改之前的指令。

![](images/Pasted%20image%2020221202001917.png)

所以这个时候软件需要把dcache的东西clean出去，然后让icache invalidate，这个开销显然还是比较大的。

但是，比如ARM64的N1处理器，它支持硬件的icache同步，详见文档：The Arm Neoverse N1 Platform: Building Blocks for the Next-Gen Cloud-to-Edge Infrastructure SoC

![](images/Pasted%20image%2020221202001921.png)

特别注意画红色的几行。软件维护的成本实际很高，还涉及到icache的invalidation向所有核广播的动作。

接下来的一个问题就是多个核之间的cache同步。下面是一个简化版的处理器，CPU\_A和B共享了一个L3，CPU\_C和CPU\_D共享了一个L3。实际的硬件架构由于涉及到NUMA，会比这个更加复杂，但是这个图反映层级关系是足够了。

![](images/Pasted%20image%2020221202001924.png)

比如CPU\_A读了一个地址p的变量？CPU\_B、C、D又读，难道B,C,D又必须从RAM里面经过L3,L2,L1再读一遍吗？这个显然是没有必要的，在硬件上，cache的snooping控制单元，可以协助直接把CPU\_A的p地址cache拷贝到CPU\_B、C和D的cache

![](images/Pasted%20image%2020221202001928.png)

这样A-B-C-D都得到了相同的p地址的棕色小球。

假设CPU B这个时候，把棕色小球写成红色，而其他CPU里面还是棕色，这样就会不一致了：

![](images/Pasted%20image%2020221202001930.png)

这个时候怎么办？**这里面显然需要一个 协议**，典型的多核cache同步协议有MESI和MOESI。MOESI相对MESI有些细微的差异，不影响对全局的理解。下面我们重点看MESI协议。

**MESI 协议**



MESI协议定义了4种状态：

**M（Modified）** :
当前cache的内容有效，数据已被修改而且与内存中的数据不一致，数据只在当前cache里存在；类似RAM里面是棕色球，B里面是红色球（CACHE与RAM不一致），A、C、D都没有球。

![](images/Pasted%20image%2020221202001935.png)

**E（Exclusive** 独有的 **）**
**：当前cache的内容有效，数据与内存中的数据一致，数据只在当前cache里存在；类似RAM里面是棕色球，B里面是棕色球（RAM和CACHE一致），A、C、D都没有球。**

![](images/Pasted%20image%2020221202001939.png)

**S（Shared）** ：当前cache的内容有效，数据与内存中的数据一致，数据在多个cache里存在。类似如下图，在CPU A-B-C里面cache的棕色球都与RAM一致。

![](images/Pasted%20image%2020221202001946.png)

**I（Invalid）** ：当前cache无效。前面三幅图里面cache没有球的那些都是属于这个情况。

**然后它有个状态机**

![](images/Pasted%20image%2020221202001949.png)

这个状态机比较难记，死记硬背是记不住的，也没必要记，它讲的cache原先的状态，经过一个硬件在本cache或者其他cache的读写操作后，各个cache的状态会如何变迁。所以，硬件上不仅仅是监控本CPU的cache读写行为，还会监控其他CPU的。只需要记住一点：这个状态机是为了保证多核之间cache的一致性，比如一个干净的数据，可以在
多个CPU的cache share ，这个没有一致性问题；但是，假设其中一个CPU写过了，比如A-B-C本来是这样：

![](images/Pasted%20image%2020221202001952.png)

然后B被写过了：

![](images/Pasted%20image%2020221202001956.png)

这样A、C的cache实际是过时的数据，这是不允许的。这个时候，硬件会自动把A、C的cache
invalidate掉，不需要软件的干预，A、C其实变地相当于不命中这个球了：

![](images/Pasted%20image%2020221202002002.png)

这个时候，你可能会继续问，如果C要读这个球呢？它目前的状态在B里面是modified的，而且与RAM不一致，这个时候，硬件会把红球clean，然后B、C、RAM变地一致，B、C的状态都变化为S（Shared）：

![](images/Pasted%20image%2020221202002006.png)

这一系列的动作虽然由硬件完成，但是对软件而言不是免费的，因为它耗费了时间。如果编程的时候不注意，引起了硬件的大量cache同步行为，则程序的效率可能会急剧下降。

为了让大家直观感受到这个cache同步的开销，下面我们写一个程序，这个程序有2个线程，一个写变量，一个读变量：

![](images/Pasted%20image%2020221202002106.png)

这个程序里，x和y都是cacheline对齐的，这个程序的thread1的写，会不停地与thread2的读，进行cache同步。

它的执行时间为：

$ time ./a.out 
real  0m3.614s
user  0m7.021s
sys  0m0.004s

它在2个CPU上的userspace共运行了7.021秒，累计这个程序从开始到结束的对应真实世界的时间是3.614秒（就是从命令开始到命令结束的时间）。

如果我们把程序改一句话，把thread2里面的c = x改为c =
y，这样2个线程在2个CPU运行的时候，读写的是不同的cacheline，就没有这个硬件的cache同步开销了：

![](images/Pasted%20image%2020221202002208.png)

它的运行时间：

$ time ./b.out 
real  0m1.820s
user  0m3.606s
sys  0m0.008s

现在只需要1.8秒，几乎减小了一半。

感觉前面那个a.out，双核的帮助甚至都不大。如果我们改为单核跑呢？

$ time taskset -c 0 ./a.out 
real  0m3.299s
user  0m3.297s
sys  0m0.000s

它单核跑，居然只需要3.299秒跑完，而双核跑，需要3.614s跑完。单核跑完这个程序，甚至比双核还快，有没有惊掉下巴？！！！因为单核里面没有cache同步的开销。

下一个cache同步的重大问题，就是设备与CPU之间。如果设备感知不到CPU的cache的话（下图中的红色数据流向不经过cache），这样，做DMA前后，CPU就需要进行相关的cacheclean和invalidate的动作，软件的开销会比较大。

![](images/Pasted%20image%2020221202002212.png)

这些软件的动作，若我们在Linux编程的时候，使用的是 **streaming DMA APIs** 的话，都会被类似这样的API自动搞定：

dma\_map\_single()
dma\_unmap\_single()
dma\_sync\_single\_for\_cpu()
dma\_sync\_single\_for\_device()
dma\_sync\_sg\_for\_cpu()
dma\_sync\_sg\_for\_device()

如果是使用的 **dma\_alloc\_coherent** ()API呢，则设备和CPU之间的buffer是cache一致的，不需要每次DMA进行同步。对于不支持硬件cache一致性的设备而言，很可能dma\_alloc\_coherent()会把CPU对那段DMA
buffer的访问设置为uncachable的。

这些API把底层的硬件差异封装掉了，如果硬件不支持CPU和设备的cache同步的话，延时还是比较大的。那么，对于底层硬件而言，更好的实现方式，应该仍然是硬件帮我们来搞定。比如我们需要修改总线协议，延伸红线的触角：

![](images/Pasted%20image%2020221202002217.png)

当设备访问RAM的时候，可以去snoop CPU的cache：

- 如果做内存到外设的DMA，则直接从CPU的cache取modified的数据；
- 如果做外设到内存的DMA，则直接把CPU的cache invalidate掉。

这样，就实现硬件意义上的cache同步。当然，硬件的cache同步，还有一些其他方法，原理上是类似的。注意，这种同步仍然不是免费的，它仍然会消耗bus
cycles的。实际上，cache的同步开销还与距离相关，可以说距离越远，同步开销越大，比如下图中A、B的同步开销比A、C小。

![](images/Pasted%20image%2020221202002220.png)

对于一个NUMA服务器而言，跨NUMA的cache同步开销显然是要比NUMA内的同步开销大。

**意识到 CACHE 的编程**



通过上一节的代码，读者应该意识到了cache的问题不处理好，程序的运行性能会急剧下降。所以意识到cache的编程，对程序员是至关重要的。



**后台回复“加群”，带你进入高手如云交流群**



**推荐阅读：**

[**__Go高性能编程技法解读__**](http://mp.weixin.qq.com/s?__biz=MzI1OTY2MzMxOQ==&mid=2247499710&idx=1&sn=f3e710297b8099c2c8be43cf09ef55bb&chksm=ea77cb06dd004210f5dab97e227cf9be9253b4dfcb1cdff918e446bbe2970c165562749b9465&scene=21#wechat_redirect)

[**__容器网络|深入理解Cilium__**](http://mp.weixin.qq.com/s?__biz=MzI1OTY2MzMxOQ==&mid=2247498724&idx=1&sn=d33d04212563f4596ff08c0c6f81533d&chksm=ea77cf5cdd00464a4e4a34adfc8916f701fa6db789ec0d7e7aaea898711dec43ecd4c7d2adbe&scene=21#wechat_redirect)

[**__Linux下的TCP测试工具__**](http://mp.weixin.qq.com/s?__biz=MzI1OTY2MzMxOQ==&mid=2247498690&idx=1&sn=c5a176e6c8e9f1c7258abbfb06b53cb7&chksm=ea77cf7add00466c51d525833a9b22101a2da60a640740cb1e243eb88830e9eceaaa59cf5ea7&scene=21#wechat_redirect)

[**__深入理解 Cache 工作原理__**](http://mp.weixin.qq.com/s?__biz=MzI1OTY2MzMxOQ==&mid=2247497507&idx=1&sn=91327167900834132efcbc6129d26cd0&chksm=ea77c39bdd004a8de63a542442b5113d5cde931a2d76cff5c92b7e9f68cd561d3ed6ef7a1aa5&scene=21#wechat_redirect)

[**__Cilium 容器网络的落地实践__**](http://mp.weixin.qq.com/s?__biz=MzI1OTY2MzMxOQ==&mid=2247497237&idx=1&sn=d84b91d9e416bb8d18eee409b6993743&chksm=ea77c2addd004bbb0eda5815bbf216cff6a5054f74a25122c6e51fafd2512100e78848aad65e&scene=21#wechat_redirect)

[**__【中断】的本质__**](http://mp.weixin.qq.com/s?__biz=MzI1OTY2MzMxOQ==&mid=2247496751&idx=1&sn=dbdb208d4a9489981364fa36e916efc9&chksm=ea77c097dd004981e7358d25342f5c16e48936a2275202866334d872090692763110870136ad&scene=21#wechat_redirect)**__
__**

[**__图解 | Linux内存回收之LRU算法__**](http://mp.weixin.qq.com/s?__biz=MzI1OTY2MzMxOQ==&mid=2247496417&idx=1&sn=4267d317bb0aa5d871911f255a8bf4ad&chksm=ea77c659dd004f4f54a673830560f31851dfc819a2a62f248c7e391973bd14ab653eaf2a63b8&scene=21#wechat_redirect)**__
__**

[**__Linux 应用内存调试神器- ASan__**](http://mp.weixin.qq.com/s?__biz=MzI1OTY2MzMxOQ==&mid=2247496414&idx=1&sn=897d3d39e208652dcb969b5aca221ca1&chksm=ea77c666dd004f70ebee7b9b9d6e6ebd351aa60e3084149bfefa59bca570320ebcc7cadc6358&scene=21#wechat_redirect)

[**__深入理解 Cilium 的 eBPF 收发包路径__**](http://mp.weixin.qq.com/s?__biz=MzI1OTY2MzMxOQ==&mid=2247496345&idx=1&sn=22815aeadccc1c4a3f48a89e5426b3f3&chksm=ea77c621dd004f37ff3a9e93a64e145f55e621c02a917ba0901e8688757cc8030b4afce2ef63&scene=21#wechat_redirect)

[**__Page Cache和Buffer Cache关系__**](http://mp.weixin.qq.com/s?__biz=MzI1OTY2MzMxOQ==&mid=2247495951&idx=1&sn=8bc76e05a63b8c9c9f05c3ebe3f99b7a&chksm=ea77c5b7dd004ca18c71a163588ccacd33231a58157957abc17f1eca17e5dcb35147b273bc52&scene=21#wechat_redirect)

[**__深入理解DPDK程序设计|Linux网络2.0__**](http://mp.weixin.qq.com/s?__biz=MzI1OTY2MzMxOQ==&mid=2247495791&idx=1&sn=5d9f3bdc29e8ae72043ee63bc16ed280&chksm=ea77c4d7dd004dc1eb0cee7cba6020d33282ead83a5c7f76a82cb483e5243cd082051e355d8a&scene=21#wechat_redirect)**__
__**

[**__一文读懂基于Kubernetes打造的边缘计算__**](http://mp.weixin.qq.com/s?__biz=MzI1OTY2MzMxOQ==&mid=2247495291&idx=1&sn=0aebc6ee54af03829e15ac659db923ae&chksm=ea77dac3dd0053d5cd4216e0dc91285ff37607c792d180b946bc09783d1a2032b0dffbcb03f0&scene=21#wechat_redirect)

[**__网络方案 Cilium 入门教程__**](http://mp.weixin.qq.com/s?__biz=MzI1OTY2MzMxOQ==&mid=2247495170&idx=1&sn=54d6c659853f296fd6e6e20d44b06d9b&chksm=ea77dabadd0053ac7f72c4e742942f1f59d29000e22f9e31d7146bcf1d7d318b68a0ae0ef91e&scene=21#wechat_redirect)

[**__Docker  容器技术使用指南__**](http://mp.weixin.qq.com/s?__biz=MzI1OTY2MzMxOQ==&mid=2247494756&idx=1&sn=f7384fc8979e696d587596911dc1f06b&chksm=ea77d8dcdd0051ca7dacde28306c535508b8d97f2b21ee9a8a84e2a114325e4274e32eccc924&scene=21#wechat_redirect)

[**__云原生/云计算发展白皮书（附下载）__**](http://mp.weixin.qq.com/s?__biz=MzI1OTY2MzMxOQ==&mid=2247494647&idx=1&sn=136f21a903b0771c1548802f4737e5f8&chksm=ea77df4fdd00565996a468dac0afa936589a4cef07b71146b7d9ae563d11882859cc4c24c347&scene=21#wechat_redirect)

[**__使用 GDB+Qemu 调试 Linux 内核__**](http://mp.weixin.qq.com/s?__biz=MzI1OTY2MzMxOQ==&mid=2247493336&idx=1&sn=268fae00f4f88fe27b24796644186e9e&chksm=ea77d260dd005b76c10f75dafc38428b8357150f3fb63bc49a080fb39130d6590ddea61a98b5&scene=21#wechat_redirect)

[**__防火墙双机热备__**](http://mp.weixin.qq.com/s?__biz=MzI1OTY2MzMxOQ==&mid=2247493173&idx=1&sn=53975601d927d4a89fe90d741121605b&chksm=ea77d28ddd005b9bdd83dac0f86beab658da494c4078af37d56262933c866fcb0b752afcc4b9&scene=21#wechat_redirect)

[**__常见的几种网络故障案例分析与解决__**](http://mp.weixin.qq.com/s?__biz=MzI1OTY2MzMxOQ==&mid=2247493157&idx=1&sn=de0c263f74cb3617629e84062b6e9f45&chksm=ea77d29ddd005b8b6d2264399360cfbbec8739d8f60d3fe6980bc9f79c88cc4656072729ec19&scene=21#wechat_redirect)

[**__Kubernetes容器之间的通信浅谈__**](http://mp.weixin.qq.com/s?__biz=MzI1OTY2MzMxOQ==&mid=2247493145&idx=1&sn=c69bd59a40281c2d7e669a639e1a50cd&chksm=ea77d2a1dd005bb78bf499ea58d3b6b138647fc995c71dcfc5acaee00cd80209f43db878fdcd&scene=21#wechat_redirect)

[**__kube-proxy 如何与 iptables 配合使用__**](http://mp.weixin.qq.com/s?__biz=MzI1OTY2MzMxOQ==&mid=2247492982&idx=1&sn=2b842536b8cdff23e44e86117e3d940f&chksm=ea77d1cedd0058d82f31248808a4830cbe01077c018a952e3a9034c96badf9140387b6f011d6&scene=21#wechat_redirect)

[**__完美排查入侵__**](http://mp.weixin.qq.com/s?__biz=MzI1OTY2MzMxOQ==&mid=2247492931&idx=1&sn=523a985a5200430a7d4c71333efeb1d4&chksm=ea77d1fbdd0058ed83726455c2f16c9a9284530da4ea612a45d1ca1af96cb4e421290171030a&scene=21#wechat_redirect)

[**__QUIC也不是万能的__**](http://mp.weixin.qq.com/s?__biz=MzI1OTY2MzMxOQ==&mid=2247491959&idx=1&sn=61058136134e7da6a1ad1b9067eebb95&chksm=ea77d5cfdd005cd9261e3dc0f9689291895f0c9764aa1aa740608c0916405a5b94302f659025&scene=21#wechat_redirect)

[**__网络排错大讲解~__**](http://mp.weixin.qq.com/s?__biz=MzI1OTY2MzMxOQ==&mid=2247488574&idx=1&sn=68df1982e9f23ce42457d00ce529b012&chksm=ea742086dd03a9902adb16d2b7648149209fed6c9811c6dd05be5101b42d462cb48e269b6e9d&scene=21#wechat_redirect)

[**__OVS 和 OVS-DPDK 对比__**](http://mp.weixin.qq.com/s?__biz=MzI1OTY2MzMxOQ==&mid=2247488294&idx=2&sn=303d9baefa768f887ba36213df793025&chksm=ea74279edd03ae88bc91300a1666066881dd763879b714941120fb9e41e787c557c2930ff5db&scene=21#wechat_redirect)

[**__微软出品的最新K8S学习指南3.0__**](http://mp.weixin.qq.com/s?__biz=MzI1OTY2MzMxOQ==&mid=2247488271&idx=1&sn=a52af9aba2ea8bbc5fd4440f85ece458&chksm=ea7427b7dd03aea1dbf106326168333c651b403dd44abf4e652d5a1912377d64becabac5051a&scene=21#wechat_redirect)**__下载__**



