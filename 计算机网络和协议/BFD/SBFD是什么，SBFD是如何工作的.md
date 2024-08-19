
BFD（Bidirectional Forwarding Detection）技术提供了一个通用的标准化的与介质和协议无关的快速故障检测机制，用于快速检测系统之间的通信故障。BFD技术相对成熟，但当配置大量BFD会话进行链路检测时，BFD会话的协商时间会变长，成为整个系统的一个瓶颈。SBFD（Seamless Bidirectional Forwarding Detection）是BFD的一种简化机制，简化了BFD的状态机，缩短了BFD会话协商时间，提高了整个网络节点路径检测的灵活性。


### 为什么需要SBFD？

BFD（Bidirectional Forwarding Detection）技术提供了一个通用的标准化的与介质和协议无关的快速故障检测机制，用于快速检测系统之间的通信故障。

BFD检测机制是两个系统建立BFD会话，并通过BFD报文中携带的参数进行会话协商。BFD会话协商采用三次握手机制，协商成功后，以协商的报文收发时间在彼此的路径上周期性发送BFD报文。当配置大量BFD会话进行链路检测时，BFD会话协商时间会变长，成为整个系统的一个瓶颈。

SBFD（Seamless Bidirectional Forwarding Detection）简化了BFD会话协商机制。SBFD分为发起端和反射端，发起端作为检测端，向反射端发送SBFD报文触发会话协商，反射端仅环回发起端发送的SBFD报文，因此缩短了SBFD会话的协商时间，为网络节点路径检测带来灵活性。


### SBFD是如何工作的

SBFD发起端会话状态只有Up和Down状态，发出的报文也只有Up和Down状态。反射端不会主动发送SBFD报文，仅校验SBFD报文的参数构造环回SBFD报文，返回的报文状态只有Up或Admin Down状态，反射端不处于工作状态，则会将报文状态置为Admin Down。
- 初始状态：SBFD报文由发起端首先向反射端发送，发起端初始状态为Down。
- 状态迁移：发起端收到反射端返回Up报文则将本地状态置为Up。发起端收到反射端返回Admin Down报文，则将本地状态置为Down。发起端在定时器超时前收不到返回报文，也将状态置为Down。
- 状态保持：发起端处于Up状态，如果收到反射端返回的Up报文，则本地状态持续维持在Up状态。发起端处于Down状态，如果收到反射端返回的Admin Down报文或者在定时器超时前收不到返回报文，则本地状态持续维持在Down状态。

SBFD会话状态协商过程如下图所示。发起端和反射端通过发送SBFD报文进行会话协商。
- 发起端做为检测端，向反射端发送SBFD报文，报文初始状态为Down。

- 反射端无SBFD状态机，无检测机制，不会主动发送SBFD报文，仅用于构造环回SBFD报文。反射端接收到发起端的SBFD报文，检查报文中SBFD描述符是否与本地配置的全局SBFD描述符匹配，不匹配则丢弃；如果匹配并且反射端处于工作状态，则构造环回SBFD报文，反射端不处于工作状态，则将报文状态置为Admin Down。

![](images/Pasted%20image%2020230426101238.png)
*SBFD会话协商*

SBFD发起端状态机只有Up和Down两个状态，也只能在这两个状态间转变，具体如下图所示。
![](images/Pasted%20image%2020230426101302.png)
*SBFD发起端状态机*

- 初始状态：SBFD报文由发起端首先向反射端发送，发起端初始状态为Down。
- 状态迁移：发起端收到反射端发回的Up报文则将本地状态置为Up。发起端收到反射端返回Admin Down报文，则将状态置为Down。发起端在定时器超时前收不到返回报文，也将状态置为Down。
- 状态保持：发起端处于Up状态，如果收到反射端返回的Up报文，则本地状态持续维持在Up状态。发起端处于Down状态，如果收到反射端返回的Admin Down报文或者在定时器超时前收不到返回报文，则本地状态持续维持在Down状态。

### SBFD与BFD有何不同？

SBFD和BFD通过UDP端口号区分两种报文。UDP数据部分SBFD和BFD报文格式是一样的，协议版本号目前都为1，只是部分字段要求有所区分。

SBFD和BFD主要区别如下表所示。

表1-1 SBFD与BFD主要区别
|类别|BFD|SBFD|
|---|---|---|
|UDP端口号|3784/4784|7784|
|标识符|BFD会话本端的本地标识符与对端的远端标识符需要一致。|反射端配置一个标识符可以与发射端N个BFD会话配对。|
|状态机|BFD两端会话都有四种会话状态，设备两端都需要建立状态机，相互发送BFD报文协商会话状态。|SBFD报文分为发起端和反射端，只有两个会话状态，只有发起端需要建立状态机，反射端无状态机。|
|应用场景|BFD主要应用的场景，包含：<br> IP链路 <br> IP-Trunk <br> Eth-Trunk <br> VLANIF <br> [MPLS](https://info.support.huawei.com/info-finder/encyclopedia/zh/MPLS.html) LSP <br> PW <br> Segment Routing: <br>  BFD for [SR-MPLS](https://info.support.huawei.com/info-finder/encyclopedia/zh/SR-MPLS.html) BE <br>  BFD for SR-MPLS TE LSP <br>  BFD for SR-MPLS TE Tunnel |SBFD主要应用的场景，包含：<br> SBFD for SR-MPLS BE <br> SBFD for SR-MPLS TE LSP <br> SBFD for SR-MPLS TE Tunnel<br> SBFD For SR-MPLS TE Policy<br> SBFD for [SRv6](https://info.support.huawei.com/info-finder/encyclopedia/zh/SRv6.html) TE Policy <br>


### SBFD是如何应用的？

SBFD典型的应用场景为Segment Routing场景，包含SBFD for SR-MPLS BE、SBFD for SR-MPLS TE LSP、SBFD For SR-MPLS TE Policy、SBFD for SRv6 TE Policy。

如下图所示是一个简单的网络组网，为了提升网络可靠性，在PE1和PE2上部署了SBFD特性。SBFD根据上层应用通知的邻居信息建立会话。当SBFD检测到故障时，可以触发应用进行快速流量切换，以减少对业务的影响。

SBFD故障检测过程为：

- 被检测链路出现故障。
- SBFD快速检测到链路故障，SBFD会话状态变为Down。
- SBFD通知上层应用协议邻居不可达。
- 上层应用协议中断邻居关系，重新计算路由，流量切换到备份链路上。

图1-4 SBFD故障检测示意图
![](images/Pasted%20image%2020230426102414.png)

