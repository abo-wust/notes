
BFD（Bidirectional Forwarding Detection，双向转发检测）是一种基于RFC 5880标准的高速故障检测机制，两个系统建立BFD会话后，在它们之间的通道上周期性地发送BFD报文，如果一方在协商的检测时间内没有接收到BFD报文，则认为这条双向通道上发生了故障。上层协议通过BFD感知到链路故障后可以及时采取措施，进行故障恢复。

### 为什么要用BFD？

为了减小设备故障对业务的影响，提高网络的可靠性，网络设备需要能够尽快检测到与相邻设备间的通信故障，以便及时采取措施，保证业务继续进行。

在现有网络中，有些链路通过硬件检测信号检测链路故障，如SDH（Synchronous Digital Hierarchy，同步数字体系）告警，但并不是所有的介质都能提供硬件检测。此时，应用就要依靠上层协议自身的Hello报文机制来进行故障检测。上层协议的检测时间通常在秒级，当数据传输速率达到GB级时，秒级检测时间内，大量数据将会丢失。在三层网络中，Hello报文检测机制无法针对所有路由来检测故障，如静态路由。这对系统间互联互通定位故障造成困难。

BFD协议就是在这种背景下产生的，BFD提供了一个通用的标准化的介质无关和协议无关的快速故障检测机制，它具有以下优点：

- 提供轻负荷、短周期的故障检测，故障检测时间可达到毫秒级，可靠性更高。
- 支持多种故障检测，如接口故障、数据链路故障、转发引擎本身故障等。
- 不依赖硬件，能够对任何介质、任何协议层进行实时检测。

### BFD典型应用场景

通常，BFD不能独立运行，而是作为辅助与接口状态或与路由协议（如静态路由、OSPF、IS-IS、BGP等）联动使用，此处介绍两种典型应用，其他应用场景请参见[BFD应用场景](https://support.huawei.com/enterprise/zh/doc/EDOC1100197869/df878e97)。

**BFD与接口状态联动**
BFD与接口状态联动提供一种简单的机制，使得BFD检测行为可以关联接口状态，提高了接口感应链路故障的灵敏度，减少了非直连链路故障导致的问题。BFD检测到链路故障会立即上报Down消息到相应接口，使接口进入一种特殊的Down状态：BFD Down状态。该状态等效于链路协议Down状态，在该状态下只有BFD的报文可以正常处理，从而使接口也可以快速感知链路故障。
![](images/Pasted%20image%2020230426102805.png)
*BFD与接口状态联动*

如图所示，链路中间存在其他设备，虽然三层仍是直连，但由于实际物理线路分段，一旦链路故障，两端设备需要比较长的时间才能检测到，导致直连路由收敛慢，网络中断时间长。在SwitchA和SwitchB上配置BFD会话，配置接口联动后，当BFD检测到链路出现故障，立即上报Down消息到相应接口，使接口进入BFD Down状态。

**BFD与OSPF联动**
网络上的链路故障或拓扑变化都会导致路由重新计算，要提高网络可用性，缩短路由协议收敛时间非常重要。由于链路故障无法完全避免，因此，加快故障感知速度并将故障快速通告给路由协议是一种可行的方案。

BFD与OSPF联动就是将BFD和OSPF协议关联起来，通过BFD对链路故障的快速感应进而通知OSPF协议，从而加快OSPF协议对于网络拓扑变化的响应。下图显示了OSPF协议是否绑定BFD时收敛速度的数据。
![](images/Pasted%20image%2020230426102840.png)
*OSPF协议收敛速度*

![](images/Pasted%20image%2020230426102915.png)
*BFD与OSPF联动*

如上图所示，SwitchA分别与SwitchC和SwitchD建立OSPF邻居关系，SwitchA到SwitchB的路由出接口为Interface 1，经过SwitchC到达SwitchB。邻居状态到达FULL状态时通知BFD建立BFD会话。

当SwitchA和SwitchC之间链路出现故障，BFD首先感知到并通知SwitchA。SwitchA处理邻居Down事件，重新计算路由，新的路由出接口为Interface 2，经过SwitchD到达SwitchB。

### BFD是如何工作的？

BFD的检测机制是两个系统建立BFD会话，并沿它们之间的路径周期性发送BFD报文，如果一方在既定的时间内没有收到BFD报文，BFD会话状态变为Down，则认为路径上发生了故障。

下面从BFD故障检测机制、BFD会话建立流程、BFD会话建立方式三个方面介绍BFD是如何工作的。

**BFD故障检测机制**
BFD在两台网络设备上建立会话，用来检测网络设备间的双向转发路径，为上层应用服务。BFD本身并没有邻居发现机制，而是靠被服务的上层应用通知其邻居信息以建立会话。会话建立后会周期性地快速发送BFD报文，如果设备在检测时间内没有收到BFD报文则认为该双向转发路径发生了故障，通知被服务的上层应用进行相应的处理。

下面以OSPF与BFD联动为例，简单介绍会话工作流程。
![](images/Pasted%20image%2020230426103026.png)
*BFD会话建立流程图*

上图所示是一个简单的网络组网，两台设备上同时配置了OSPF与BFD，BFD会话建立过程如下所示：

- OSPF通过自己的Hello机制发现邻居并建立连接。
- OSPF在建立了新的邻居关系后，将邻居信息（包括目的地址和源地址等）通告给BFD。
- BFD根据收到的邻居信息建立会话。
- 会话建立以后，BFD开始检测链路故障，并做出快速反应。

![](images/Pasted%20image%2020230426103140.png)
*BFD故障发现处理流程图*

如上图所示：
- 被检测链路出现故障。
- BFD快速检测到链路故障，BFD会话状态变为Down。
- BFD通知本地OSPF进程BFD邻居不可达。
- 本地OSPF进程中断OSPF邻居关系。

**BFD会话建立流程**

BFD会话有以下四种状态，会话状态变化通过BFD报文的State字段传递，系统根据本地会话状态和接收到对端BFD报文驱动状态改变。
- Down：会话处于Down或刚刚创建状态。
- Init：已经能够与对端系统通信，本端希望使会话进入Up状态。
- Up：会话已经建立成功。
- AdminDown：会话处于管理性Down状态。

BFD状态机的建立和拆除都采用三次握手机制，以确保两端系统都能知道状态的变化。以BFD会话建立为例，简单介绍状态机的迁移过程。
![](images/Pasted%20image%2020230426103254.png)
*BFD会话建立流程图*

- SwitchA和SwitchB各自启动BFD状态机，初始状态为Down，发送状态为Down的BFD报文。对于静态配置BFD会话，报文中Remote Discriminator的值是用户指定的；对于动态创建BFD会话，Remote Discriminator的值是0。

- SwitchB收到状态为Down的BFD报文后，状态切换至Init，并发送状态为Init的BFD报文。

- SwitchB本地BFD状态为Init后，不再处理接收到的状态为Down的报文。

- SwitchA的BFD状态变化同SwitchB。

- SwitchB收到状态为Init的BFD报文后，本地状态切换至Up。

- SwitchA的BFD状态变化同SwitchB。

**BFD会话建立方式**

BFD有两种会话建立方式，即静态方式和动态方式。静态和动态创建BFD会话的主要区别在于本地标识符（Local Discriminator）和远端标识符（Remote Discriminator）的配置方式不同。BFD通过控制报文中的Local Discriminator和Remote Discriminator区分不同的会话。

- 静态建立BFD会话
- 静态建立BFD会话是指通过命令行手工配置BFD会话参数，包括配置本地标识符和远端标识符等，然后手工下发BFD会话建立请求。
- 动态建立BFD会话
- 动态建立BFD会话时，系统对本地标识符和远端标识符的处理方式如下：
- 动态分配本地标识符
    当应用程序触发动态创建BFD会话时，系统分配属于动态会话标识符区域的值作为BFD会话的本地标识符。然后向对端发送Remote Discriminator的值为0的BFD报文，进行会话协商。
- 自学习远端标识符
    当BFD会话的一端收到Remote Discriminator的值为0的BFD报文时，判断该报文是否与本地BFD会话匹配，如果匹配，则学习接收到的BFD报文中Local Discriminator的值，获取远端标识符。

### BFD单臂回声功能

BFD除了与其他协议联动使用，还有一种特殊场景，即BFD单臂回声功能。

单臂回声功能是指通过BFD报文的环回操作检测转发链路的连通性。在两台直连设备中，其中一台设备支持BFD功能，另一台设备不支持BFD功能，只支持基本的网络层转发，此时可以使用BFD单臂回声功能来检测链路。

为了能够快速检测两台设备之间的故障，可以在支持BFD功能设备上创建单臂回声功能的BFD会话，支持BFD功能的设备主动发起回声请求报文，不支持BFD功能的设备接收到该报文后直接将其环回，从而实现转发链路的连通性检测功能。单臂回声功能只适用于单跳BFD会话。
![](images/Pasted%20image%2020230426103510.png)
*BFD单臂回声功能*

如上图所示，SwitchA支持BFD功能，SwitchB不支持BFD功能。在SwitchA上配置单臂回声功能的BFD会话，检测SwitchA到SwitchB之间的单跳路径。SwitchB接收到SwitchA发送的BFD报文后，直接在网络层将该报文环回，从而快速检测SwitchA和SwitchB之间的直连链路的连通性。

参考资源
1 [了解BFD帧格式](https://support.huawei.com/hedex/hdx.do?docid=EDOC1000105967&id=ZH-CN_CONCEPT_0254009266)
2 [RFC 5880](https://datatracker.ietf.org/doc/html/rfc5880)
3 [BFD配置指南（S2700, S5700, S6700系列以太网交换机）](https://support.huawei.com/enterprise/zh/doc/EDOC1100197869/8b9b8863)
4 [BFD典型配置（S2700, S5700, S6700系列以太网交换机）](https://support.huawei.com/enterprise/zh/doc/EDOC1000069491/5b32e0a9)


