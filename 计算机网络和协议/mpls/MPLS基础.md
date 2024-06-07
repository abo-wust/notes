
**MPLS简介**

多协议标签交换MPLS（Multiprotocol Label Switching）是一种IP（Internet Protocol）骨干网技术。MPLS在无连接的IP网络上引入面向连接的标签交换概念，将第三层路由技术和第二层交换技术相结合，充分发挥了IP路由的灵活性和二层交换的简捷性。

MPLS起源于IPv4（Internet Protocol version 4），其核心技术可扩展到多种网络协议，包括IPv6（Internet Protocol version 6）、IPX（Internet Packet Exchange）和CLNP（Connectionless Network Protocol）等。MPLS中的“Multiprotocol”指的就是支持多种网络协议。

由此可见，MPLS并不是一种业务或者应用，它实际上是一种隧道技术。这种技术不仅支持多种高层协议与业务，而且在一定程度上可以保证信息传输的安全性。

**MPLS基本结构**

**网络结构**

MPLS网络的典型结构如下图所示。MPLS基于标签进行转发，下图中进行MPLS标签交换和报文转发的网络设备称为标签交换路由器LSR（Label Switching Router）；由LSR构成的网络区域称为MPLS域（MPLS Domain）。位于MPLS域边缘、连接其他网络的LSR称为边缘路由器LER（Label Edge Router），区域内部的LSR称为核心LSR（Core LSR）。

![](images/Pasted%20image%2020221201233601.png)

图：MPLS网络结构图

IP报文进入MPLS网络时，MPLS入口的LER分析IP报文的内容并且为这些IP报文添加合适的标签，所有MPLS网络中的LSR根据标签转发数据。当该IP报文离开MPLS网络时，标签由出口LER弹出。

IP报文在MPLS网络中经过的路径称为标签交换路径LSP（Label Switched Path）。LSP是一个单向路径，与数据流的方向一致。

如上图，LSP的入口LER称为入节点（Ingress）；位于LSP中间的LSR称为中间节点（Transit）；LSP的出口LER称为出节点（Egress）。一条LSP可以有0个、1个或多个中间节点，但有且只有一个入节点和一个出节点。

根据LSP的方向，MPLS报文由Ingress发往Egress，则Ingress是Transit的上游节点，Transit是Ingress的下游节点。同理，Transit是Egress上游节点，Egress是Transit的下游节点。

**体系结构：**

MPLS的体系结构如下图所示，它由控制平面（Control Plane）和转发平面（Forwarding Plane）组成。

![](images/Pasted%20image%2020221201233608.png)

图：MPLS体系结构图

- 控制平面：负责产生和维护路由信息以及标签信息。
  - 路由信息表RIB（Routing Information Base）：由IP路由协议（IP Routing Protocol）生成，用于选择路由。
  - 标签分发协议LDP（Label Distribution Protocol）：负责标签的分配、标签转发信息表的建立、标签交换路径的建立、拆除等工作。
  - 标签信息表LIB（Label Information Base）：由标签分发协议生成，用于管理标签信息。

- 转发平面：即数据平面（Data Plane），负责普通IP报文的转发以及带MPLS标签报文的转发。
  - 转发信息表FIB（Forwarding Information Base）：从RIB提取必要的路由信息生成，负责普通IP报文的转发。
  - 标签转发信息表LFIB（Label Forwarding Information Base）：简称标签转发表，由标签分发协议在LSR上建立LFIB，负责带MPLS标签报文的转发。


**MPLS标签**

**转发等价类：**

MPLS将具有相同特征的报文归为一类，称为转发等价类FEC（Forwarding Equivalence Class）。属于相同FEC的报文在转发过程中被LSR以相同方式处理。

FEC可以根据源地址、目的地址、源端口、目的端口、VPN等要素进行划分。例如，在传统的采用最长匹配算法的IP转发中，到同一条路由的所有报文就是一个转发等价类。

**标签：**

标签（Label）是一个短而定长的、只具有本地意义的标识符，用于唯一标识一个分组所属的FEC。在某些情况下，例如要进行负载分担，对应一个FEC可能会有多个入标签，但是一台设备上，一个标签只能代表一个FEC。

MPLS报文与普通的IP报文相比增加了MPLS标签信息，MPLS标签的长度为4个字节。MPLS标签封装在链路层和网络层之间，可以支持任意的链路层协议。MPLS标签的封装结构如下图所示：

![](images/Pasted%20image%2020221201233625.png)

图：MPLS标签封装结构

标签共有4个字段：

- Label：20bit，标签值域。
- Exp：3bit，用于扩展。现在通常用做CoS（Class of Service），当设备阻塞时，优先发送优先级高的报文。
- S：1bit，栈底标识。MPLS支持多层标签，即标签嵌套。S值为1时表明为最底层标签。
- TTL：8bit，和IP报文中的TTL（Time To Live）意义相同。
- TTL：8bit，和IP报文中的TTL（Time To Live）意义相同。

标签栈（Label Stack）是指标签的排序集合。靠近二层首部的标签称为栈顶MPLS标签或外层MPLS标签（Outer MPLS label）；靠近IP首部的标签称为栈底MPLS标签或内层MPLS标签（Inner MPLS label）。理论上，MPLS标签可以无限嵌套。目前MPLS标签嵌套主要应用在MPLS VPN、TE FRR（Traffic Engineering Fast ReRoute）中。

标签栈按后进先出方式组织标签，从栈顶开始处理标签。

**MPLS报文抓包示例：**

![](images/Pasted%20image%2020221201233630.png)

图：MPLS报文抓包示例

**标签空间：**

标签空间就是指标签的取值范围。标签空间划分如下：

- 0～15：特殊标签。
- 16～1023：静态LSP和静态CR-LSP（Constraint-based Routed Label Switched Path）共享的标签空间。
- 1024及以上：LDP、RSVP-TE（Resource Reservation Protocol-Traffic Engineering）、MP-BGP（MultiProtocol Border Gateway Protocol）等动态信令协议的标签空间。

**特殊标签表：**

|**标签值**|**含义**|**描述**|
|-------|------|------|
|0|IPv4 Explicit NULL Label|表示该标签必须被弹出（即标签被剥掉），且报文的转发必须基于IPv4。如果出节点分配给倒数第二跳节点的标签值为0，则倒数第二跳LSR需要将值为0的标签正常压入报文标签值顶部，转发给最后一跳。最后一跳发现报文携带的标签值为0，则将标签弹出。|
|1|Router Alert Label|只有出现在非栈底时才有效。类似于IP报文的“Router Alert Option”字段，节点收到Router Alert Label时，需要将其送往本地软件模块进一步处理。实际报文转发由下一层标签决定。如果报文需要继续转发，则节点需要将Router Alert Label压回标签栈顶。|
|2|IPv6 Explicit NULL Label|表示该标签必须被弹出，且报文的转发必须基于IPv6。如果出节点分配给倒数第二跳节点的标签值为2，则倒数第二跳节点需要将值为2的标签正常压入报文标签值顶部，转发给最后一跳。最后一跳发现报文携带的标签值为2，则直接将标签弹出。|
|3|Implicit NULL Label|倒数第二跳LSR进行标签交换时，如果发现交换后的标签值为3，则将标签弹出，并将报文发给最后一跳。最后一跳收到该报文直接进行IP转发或下一层标签转发。|
|4～13|保留|\-|
|14|OAM Router Alert Label|MPLS OAM（Operation Administration & Maintenance）通过发送OAM报文检测和通告LSP故障。OAM报文使用MPLS承载。OAM报文对于Transit LSR和倒数第二跳LSR（penultimate LSR）是透明的。|
|15|保留|\-|

**LSP的建立**

MPLS需要为报文事先分配好标签，建立一条LSP，才能进行报文转发。LSP分为静态LSP和动态LSP两种。

**静态LSP的建立：**

静态LSP是用户通过手工为各个转发等价类分配标签而建立的。由于静态LSP各节点上不能相互感知到整个LSP的情况，因此静态LSP是一个本地的概念。

静态LSP不使用标签发布协议，不需要交互控制报文，因此消耗资源比较小，适用于拓扑结构简单并且稳定的小型网络。但通过静态方式分配标签建立的LSP不能根据网络拓扑变化动态调整，需要管理员干预。

配置静态LSP时，管理员需要为各LSR手工分配标签，需要遵循的原则是：前一节点出标签的值等于下一个节点入标签的值。

**动态LSP的建立：**

**动态LSP的标签发布协议**

动态LSP通过标签发布协议动态建立。标签发布协议是MPLS的控制协议（也可称为信令协议），负责FEC的分类、标签的分发以及LSP的建立和维护等一系列操作。

**动态LSP的基本建立过程**

标签由下游LSR分配，按从下游到上游的方向分发。如下图，由下游LSR在IP路由表的基础上进行FEC的划分，并根据FEC分配标签，通告给上游的LSR，以便建立标签转发表和LSP。

![](images/Pasted%20image%2020221201233650.png)

图：动态LSP的基本建立过程

**MPLS转发**

**MPLS基本转发过程：**

**基本概念**

在MPLS基本转发过程中涉及的相关概念如下：

标签操作类型包括标签压入（Push）、标签交换（Swap）和标签弹出（Pop），它们是标签转发的基本动作。

- Push：当IP报文进入MPLS域时，MPLS边界设备在报文二层首部和IP首部之间插入一个新标签；或者MPLS中间设备根据需要，在标签栈顶增加一个新的标签（即标签嵌套封装）。
- Swap：当报文在MPLS域内转发时，根据标签转发表，用下一跳分配的标签，替换MPLS报文的栈顶标签。
- Pop：当报文离开MPLS域时，将MPLS报文的标签剥掉。

在最后一跳节点，标签已经没有使用价值。这种情况下，可以利用倒数第二跳弹出特性PHP（Penultimate Hop Popping），在倒数第二跳节点处将标签弹出，减少最后一跳的负担。最后一跳节点直接进行IP转发或者下一层标签转发。

默认情况下，设备支持PHP特性，支持PHP的Egress节点分配给倒数第二跳节点的标签值为3。

**基本转发过程：**

以支持PHP的LSP为例，说明MPLS基本转发过程。

![](images/Pasted%20image%2020221201233655.png)

图：MPLS基本转发过程

如上图所示，MPLS标签已分发完成，建立了一条LSP，其目的地址为4.4.4.2/32。则MPLS基本转发过程如下：

1. Ingress节点收到目的地址为4.4.4.2的IP报文，压入标签Z并转发。
2. Transit节点收到该标签报文，进行标签交换，将标签Z换成标签Y。
3. 倒数第二跳Transit节点收到带标签Y的报文。因为Egress分给它的标签值为3，所以进行PHP操作，弹出标签Y并转发报文。从倒数第二跳转发给Egress的报文以IP报文形式传输。
4. Egress节点收到该IP报文，将其转发给目的地4.4.4.2/32。

**MPLS详细转发过程：**

**基本概念：**

在MPLS详细转发过程中涉及的相关概念如下：

- Tunnel ID

为了给使用隧道的上层应用（如VPN、路由管理）提供统一的接口，系统自动为隧道分配了一个ID，也称为Tunnel ID。该Tunnel ID的长度为32比特，只是本地有效。

- NHLFE

下一跳标签转发表项NHLFE（Next Hop Label Forwarding Entry）用于指导MPLS报文的转发。

NHLFE包括：Tunnel ID、出接口、下一跳、出标签、标签操作类型等信息。

FEC到一组NHLFE的映射称为FTN（FEC-to-NHLFE）。通过查看FIB表中Tunnel ID值不为0x0的表项，能够获得FTN的详细信息。FTN只在Ingress存在。

- ILM

入标签到一组下一跳标签转发表项的映射称为入标签映射ILM（Incoming Label Map）。

ILM包括：Tunnel ID、入标签、入接口、标签操作类型等信息。

ILM在Transit节点的作用是将标签和NHLFE绑定。通过标签索引ILM表，就相当于使用目的IP地址查询FIB，能够得到所有的标签转发信息。


**详细转发过程：**

![](images/Pasted%20image%2020221201233707.png)

图：MPLS详细转发过程

MPLS的详细转发过程如上图所示：

当IP报文进入MPLS域时，首先查看FIB表，检查目的IP地址对应的Tunnel ID值是否为0x0。

- 如果Tunnel ID值为0x0，则进入正常的IP转发流程。
- 如果Tunnel ID值不为0x0，则进入MPLS转发流程。

在MPLS转发过程中，FIB、ILM和NHLFE表项是通过Tunnel ID关联的。

- Ingress的处理：通过查询FIB表和NHLFE表指导报文的转发。
  - 查看FIB表，根据目的IP地址找到对应的Tunnel ID。
  - 根据FIB表的Tunnel ID找到对应的NHLFE表项，将FIB表项和NHLFE表项关联起来。
  - 查看NHLFE表项，可以得到出接口、下一跳、出标签和标签操作类型。
  - 在IP报文中压入出标签，同时处理TTL，然后将封装好的MPLS报文发送给下一跳。

- Transit的处理：通过查询ILM表和NHLFE表指导MPLS报文的转发。
  - 根据MPLS的标签值查看对应的ILM表，可以得到Tunnel ID。
  - 根据ILM表的Tunnel ID找到对应的NHLFE表项。
  - 查看NHLFE表项，可以得到出接口、下一跳、出标签和标签操作类型。
  - MPLS报文的处理方式根据不同的标签值而不同。
    - 如果标签值>＝16，则用新标签替换MPLS报文中的旧标签，同时处理TTL，然后将替换完标签的MPLS报文发送给下一跳。
    - 如果标签值为3，则直接弹出标签，同时处理TTL，然后进行IP转发或下一层标签转发。

- Egress的处理：通过查询ILM表指导MPLS报文的转发或查询路由表指导IP报文转发。
  - 如果Egress收到IP报文，则查看路由表，进行IP转发。
  - 如果Egress收到MPLS报文，则查看ILM表获得标签操作类型，同时处理TTL。
    - 如果标签中的栈底标识S=1，表明该标签是栈底标签，直接进行IP转发。
    - 如果标签中的栈底标识S=0，表明还有下一层标签，继续进行下一层标签转发。


**MPLS对TTL的处理：**

MPLS对TTL的处理包括MPLS对TTL的处理模式和ICMP响应报文这两个方面。

**MPLS对TTL的处理模式：**

MPLS标签中包含一个8比特的TTL字段，其含义与IP头中的TTL域相同。MPLS对TTL的处理除了用于防止产生路由环路外，也用于实现Traceroute功能。

RFC3443中定义了两种MPLS对TTL的处理模式：Uniform和Pipe。缺省情况下，MPLS对TTL的处理模式为Uniform。

- **Uniform模式**

IP报文经过MPLS网络时，在入节点，IP TTL减1映射到MPLS TTL字段，此后报文在MPLS网络中按照标准的TTL处理方式处理。在出节点将MPLS TTL减1后映射到IP TTL字段。如下图所示：

![](images/Pasted%20image%2020221201233714.png)

图：Uniform模式下入方向TTL的处理

- **Pipe模式**

在入节点，IP TTL值减1，MPLS TTL字段为固定值，此后报文在MPLS网络中按照标准的TTL处理方式处理。在出节点会将IP TTL字段的值减1。即IP分组经过MPLS网络时，无论经过多少跳，IP TTL只在入节点和出节点分别减1。如下图所示：

![](images/Pasted%20image%2020221201233721.png)

图：Pipe模式下入方向TTL的处理

- 在MPLS VPN应用中，出于网络安全的考虑，需要隐藏MPLS骨干网络的结构，这种情况下，对于私网报文，Ingress上使用Pipe模式。

**ICMP响应报文：**

在MPLS网络中，当LSR收到TTL为1的含有标签的MPLS报文时，LSR生成ICMP的TTL超时消息。

- 如果LSR上存在到达报文发送者的路由，则可以通过IP路由，直接向发送者回应TTL超时消息。
- 如果LSR上不存在到达报文发送者的路由，则ICMP响应报文将按照LSP继续传送，到达LSP出节点后，由Egress节点将该消息返回给发送者。

通常情况下，收到的MPLS报文只带一层标签时，LSR可以采用第一种方式回应TTL超时消息；收到的MPLS报文包含多层标签时，LSR采用第二种方式回应TTL超时消息。

但是，在MPLS VPN中，ASBR（Autonomous System Boundary Router，自治系统边界路由器）和HoVPN组网应用中的SPE（Superstratum PE or Sevice Provider-end PE，上层PE或运营商侧PE），接收到的承载VPN报文的MPLS报文可能只有一层标签，此时，这些设备上并不存在到达报文发送者的路由，则采用第二种方法回应TTL超时消息。

**LSP连通性检测**

在MPLS网络中，如果通过LSP转发数据失败，负责建立LSP的MPLS控制平面将无法检测到这种错误，加大了网络维护的难度。MPLS Ping/MPLS Tracert为用户提供了发现LSP错误、并及时定位失效节点的机制。

MPLS Ping主要用于检查LSP的连通性。MPLS Tracert在检查LSP的连通性的同时，还可以分析网络什么地方发生了故障。类似于普通IP的Ping/Tracert，MPLS Ping/MPLS Tracert使用MPLS回显请求（Echo Request）报文和MPLS回显应答（Echo Reply）报文检测LSP的可用性。这两种消息都以UDP报文格式发送，其中Echo Request的UDP端口号为3503，该端口号只有使能MPLS功能的设备才能识别。

MPLS Echo Request中携带需要检测的FEC信息，和其他属于此FEC的报文一样沿LSP发送，从而实现对LSP的检测。MPLS Echo Request报文通过MPLS转发给目的端，而MPLS Echo Reply报文则通过IP转发给源端。另外为了防止LSP断路时Echo Request进行IP转发，从而保证LSP的连通性测试，将Echo Request消息的IP头中目的地址设置为127.0.0.1/8（本机环回地址），IP头中的TTL值为1。

**MPLS Ping：**

![](images/Pasted%20image%2020221201233729.png)

图：MPLS网络

如上图，LSR\_1上建立了一条目的地为LSR\_4的LSP。从LSR\_1对该LSP进行MPLS Ping时的处理如下：

1. LSR\_1查找该LSP是否存在（对于TE隧道，查找Tunnel接口是否存在且CR-LSP是否建立成功）。如果不存在，返回错误信息，停止Ping。如果存在，则继续进行以下操作。
2. LSR\_1构造MPLS Echo Request报文，IP头中的目的地址为127.0.0.1/8，IP头中的TTL值为1，同时将4.4.4.4填入Echo Request报文中的目的FEC中。然后查找相应的LSP，压入LSP的标签，将报文发送给LSR\_2。
3. 中间节点LSR\_2和LSR\_3对MPLS Echo Request报文进行普通MPLS转发。如果中间节点MPLS转发失败，则中间节点返回带有错误码的MPLS Echo Reply报文。
4. 当MPLS转发路径无故障，则MPLS Echo Request报文到达LSP的出节点LSR\_4。然后检查目的FEC中包含的目的地址4.4.4.4是否为自己的Loopback接口地址，以此来确认LSR\_4是该FEC的真正出口后，返回正确的MPLS Echo Reply报文。至此整个MPLS Ping过程结束。

**MPLS Tracert:**

从LSR\_1对4.4.4.4/32进行MPLS Tracert时的处理如下：

1. LSR\_1检查LSP是否存在（对于TE隧道，查找Tunnel接口是否存在且CR-LSP是否建立成功）。如果不存在，返回错误信息，停止Tracert，否则继续进行如下处理。
2. LSR\_1构造MPLS Echo Request报文，IP头中的目的地址为127.0.0.1/8，同时将4.4.4.4填入MPLS Echo Request报文中的目的FEC中，然后查找相应的LSP，压入LSP的标签并且将MPLS TTL设置为1，将报文发送给LSR\_2。此MPLS Echo Request报文中包含Downstream Mapping TLV（用来携带LSP在当前节点的下游信息，主要包括下一跳地址、出标签等）。
3. LSR\_2收到LSR\_1发送来的报文后，将MPLS Echo Request中MPLS TTL减1为0后发现TTL超时，然后LSR\_2需要检查是否存在该LSP，同时检查报文中Downstream Mapping TLV的下一跳地址、出标签是否正确，如果两项检查都正确，返回正确的MPLS Echo Reply报文，并且报文中必须携带LSR\_2本身的包含下一跳和出标签的Downstream Mapping TLV给LSR\_1。如果检查有不正确，则返回错误的MPLS Echo Reply报文。
4. LSR\_1收到正确的MPLS Echo Reply报文后再次发送MPLS Echo Request报文，报文的封装方式跟步骤2类似，只是将LSP标签的MPLS TTL设置为2，此时的MPLS Echo Request报文中的Downstream Mapping TLV是从MPLS Echo Reply报文中复制过来的。然后LSR\_2收到该报文后进行普通MPLS转发。LSR\_3收到此报文，标签的TTL超时，跟步骤3同样的处理方式后返回MPLS Echo Reply报文。
5. LSR\_1收到正确的MPLS Echo Reply报文后重复步骤4把LSP标签的MPLS TTL设置为3，复制Downstream Mapping TLV后发送MPLS Echo Request报文。LSR\_2和LSR\_3对该报文进行普通MPLS转发。LSR\_4收到此报文，重复步骤3处理方式对报文进行处理，同时检查目的FEC中包含的目的IP 4.4.4.4为自己的Loopback接口地址，以此来发现已经是该LSP的出节点，因此返回不带下游信息的MPLS Echo Reply报文，至此整个MPLS Tracert过程结束。

通过上述步骤中返回携带下游信息的MPLS Echo Reply报文，在LSR\_1上就获取了该LSP沿途每一个节点信息。





