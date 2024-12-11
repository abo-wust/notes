
**MPLS LDP简介**

标签分发协议LDP（Label Distribution Protocol）是多协议标签交换MPLS的一种控制协议，相当于传统网络中的信令协议，负责转发等价类FEC（Forwarding Equivalence Class）的分类、标签的分配以及标签交换路径LSP（Label Switched Path）的建立和维护等操作。LDP规定了标签分发过程中的各种消息以及相关处理过程。

**目的：**

MPLS支持多层标签，并且转发平面面向连接，故具有良好的扩展性，使在统一的MPLS/IP基础网络架构上为客户提供各类服务成为可能。通过LDP协议，标签交换路由器LSR（Label Switched Router）可以把网络层的路由信息直接映射到数据链路层的交换路径上，动态建立起网络层的LSP。

目前，LDP广泛地应用在VPN服务上，具有组网、配置简单、支持基于路由动态建立LSP、支持大容量LSP等优点。

**LDP基本概念：**

**LDP对等体：**

LDP对等体是指相互之间存在LDP会话、使用LDP来交换标签消息的两个LSR。LDP对等体通过它们之间的LDP会话获得对方的标签。

**LDP邻居体：**

当一台LSR接收到对端发送过来的Hello消息后LDP邻接体建立。LDP邻接体存在两种类型：

- 本地邻接体（Local Adjacency）：以组播形式发送Hello消息（即链路Hello消息）发现的邻接体叫做本地邻接体。
- 远端邻接体（Remote Adjacency）：以单播形式发送Hello消息（即目标Hello消息）发现的邻接体叫做远端邻接体。

LDP通过邻接体来维护对等体的存在，对等体的类型取决于维护它的邻接体的类型。一个对等体可以由多个邻接体来维护，如果由本地邻接体和远端邻接体两者来维护，则对等体类型为本远共存对等体。

**LDP会话：**

LDP会话用于LSR之间交换标签映射、释放等消息。只有存在对等体才能建立LDP会话，LDP会话分为两种类型：

- 本地LDP会话（Local LDP Session）：建立会话的两个LSR之间是直连的。
- 远端LDP会话（Remote LDP Session）：建立会话的两个LSR之间可以是直连的，也可以是非直连的。

本地LDP会话和远端LDP会话可以共存。

**LDP工作机制**

LDP协议规定了标签分发过程中的各种消息以及相关的处理过程。通过LDP，LSR可以把网络层的路由信息映射到数据链路层的交换路径上，进而建立起LSP。

**LDP消息类型：**

LDP协议主要使用四类消息：

- 发现（Discovery）消息：用于通告和维护网络中LSR的存在，如Hello消息。
- 会话（Session）消息：用于建立、维护和终止LDP对等体之间的会话，如Initialization消息、Keepalive消息。
- 通告（Advertisement）消息：用于创建、改变和删除FEC的标签映射。
- 通知（Notification）消息：用于提供建议性的消息和差错通知。

为保证LDP消息的可靠发送，除了Discovery消息使用UDP（User Datagram Protocol）传输外，LDP的Session消息、Advertisement消息和Notification消息都使用TCP（Transmission Control Protocol）传输。

**LDP消息作用：**



![](images/Pasted%20image%2020221201233800.png)



![](images/Pasted%20image%2020221201233807.png)

**图：LDP消息作用**



![](images/Pasted%20image%2020221201233812.png)



![](images/Pasted%20image%2020221201233816.png)

**图：Label mapping消息报文**

**LDP报文消息抓包示例:**

![](images/Pasted%20image%2020221201233821.png)

**图：LDP的hello报文抓包示例**

![](images/Pasted%20image%2020221201233829.png)
![](images/Pasted%20image%2020221201233848.png)

**图：LDP的init报文抓包示例**

![](images/Pasted%20image%2020221201233859.png)
![](images/Pasted%20image%2020221201233906.png)

**图：KeepAlive消息和Address消息抓包示例**

**LDP工作过程：**

LDP工作过程主要分为两个阶段：

1. LDP会话的建立
通过Hello消息发现邻居后，LSR之间开始建立LDP会话。会话建立后，LDP对等体之间通过不断地发送Hello消息和Keepalive消息来维护这个会话。
  - LDP对等体之间，通过周期性发送Hello消息表明自己希望继续维持这种邻接关系。如果Hello保持定时器超时仍没有收到新的Hello消息，则删除Hello邻接关系。邻接关系被删除后，本端LSR将发送Notification消息，结束该LDP会话。
  - LDP对等体之间通过LDP会话连接上传送的Keepalive消息来维持LDP会话。如果会话保持定时器(Keepalive保持定时器)超时仍没有收到任何Keepalive消息，则关闭TCP连接，本端LSR将发送Notification消息，结束LDP会话。

2. LDP LSP的建立
会话建立后，LDP通过发送标签请求和标签映射消息，在LDP对等体之间通告FEC和标签的绑定关系，从而建立LSP。

**LDP会话的建立：**

通过LDP发现机制发现LDP对等体用来建立LDP会话。只有建立了LDP会话后，才能建立LDP LSP来承载业务。

**LDP发现机制：**

LDP发现机制用于LSR发现潜在的LDP对等体。LDP有两种发现机制：

- 基本发现机制：用于发现链路上直连的LSR。
LSR通过周期性地发送LDP链路Hello消息（LDP Link Hello），实现LDP基本发现机制，建立本地LDP会话。
LDP链路Hello消息使用UDP报文，目的地址是组播地址224.0.0.2。如果LSR在特定接口接收到LDP链路Hello消息，表明该接口存在LDP对等体。
- 扩展发现机制：用于发现链路上非直连LSR。
LSR周期性地发送LDP目标Hello消息（LDP Targeted Hello）到指定IP地址，实现LDP扩展发现机制，建立远端LDP会话。
LDP目标Hello消息使用UDP报文，目的地址是指定IP地址。如果LSR接收到LDP目标Hello消息，表明该LSR存在LDP对等体。

**LDP会话的建立过程：**

两台LSR之间交换Hello消息触发LDP会话的建立。

LDP会话的建立过程如下图所示：

![](images/Pasted%20image%2020221201233923.png)


![](images/Pasted%20image%2020221201233927.png)

图：LDP会话的建立过程

1. 两个LSR之间互相发送Hello消息。
Hello消息中携带传输地址（即设备的IP地址），双方使用传输地址建立LDP会话。
2. **传输地址较大的一方作为主动方，发起建立TCP连接**。
如上图所示，LSR\_1作为主动方发起建立TCP连接，LSR\_2作为被动方等待对方发起连接。
3. TCP连接建立成功后，由主动方LSR\_1发送初始化消息，协商建立LDP会话的相关参数。
LDP会话的相关参数包括LDP协议版本、标签分发方式、Keepalive保持定时器的值、最大PDU长度和标签空间等。
4. 被动方LSR\_2收到初始化消息后，LSR\_2接受相关参数，则发送初始化消息，同时发送Keepalive消息给主动方LSR\_1。
如果被动方LSR\_2不能接受相关参数，则发送Notification消息终止LDP会话的建立。
初始化消息中包括LDP协议版本、标签分发方式、Keepalive保持定时器的值、最大PDU长度和标签空间等。
5. 主动方LSR\_1收到初始化消息后，接受相关参数，则发送Keepalive消息给被动方LSR\_2。
如果主动方LSR\_1不能接受相关参数，则发送Notification消息给被动方LSR\_2终止LDP会话的建立。

当双方都收到对端的Keepalive消息后，LDP会话建立成功。

**LDP LSP的建立：**

LDP通过发送标签请求和标签映射消息，在LDP对等体之间通告FEC和标签的绑定关系来建立LSP，而标签的发布和管理由标签发布方式、标签分配控制方式和标签保持方式来决定。

**标签的发布和管理：**

**标签发布方式（Label Advertisement Mode）**

在MPLS体系中，由下游LSR决定将标签分配给特定FEC，再通知上游LSR，即标签由下游指定，标签的分配按从下游到上游的方向分发。

标签发布方式有两种方式。具有标签分发邻接关系的上游LSR和下游LSR必须对使用的标签发布方式达成一致。



![](images/Pasted%20image%2020221201233937.png)

![](images/Pasted%20image%2020221201233942.png)

图：标签发布方式



|**标签发布方式**|**含义**|**描述**|
|----------|------|------|
|下游自主方式DU（Downstream Unsolicited）|对于一个特定的FEC，LSR无需从上游获得标签请求消息即进行标签分配与分发。|如上图所示，对于目的地址为192.168.1.1/32的FEC，下游（Egress）通过标签映射消息主动向上游（Transit）通告自己的主机路由192.168.1.1/32的标签。|
|下游按需方式DoD（Downstream on Demand）|对于一个特定的FEC，LSR获得标签请求消息之后才进行标签分配与分发。|如上图所示，对于目的地址为192.168.1.1/32的FEC，上游（Ingress）向下游发送标签请求消息，下游（Egress）收到标签请求消息后，才会向上游发送标签映射消息。|

**标签分配控制方式（Label Distribution Control Mode）**

标签分配控制方式是指在LSP的建立过程中，LSR分配标签时采用的处理方式。

- 独立标签分配控制方式（Independent）：本地LSR可以自主地分配一个标签绑定到某个FEC，并通告给上游LSR，而无需等待下游的标签。
- 有序标签分配控制方式（Ordered）：对于LSR上某个FEC的标签映射，只有当该LSR已经具有此FEC下一跳的标签映射消息、或者该LSR就是此FEC的出节点时，该LSR才可以向上游发送此FEC的标签映射。

**标签分配控制方式和标签发布方式的组合：**

|**标签分配控制方式**|**下游自主方式DU（Downstream Unsolicited）**|**下游按需方式DoD（Downstream on Demand）**|
|------------|------------------------------------|-----------------------------------|
|**独立标签分配控制方式（Independent）**|DU ＋ Independent：LSR（Transit）无需等待下游（Egress）的标签，就会直接向上游（Ingress）分发标签。|DoD ＋ Independent：发送标签请求的LSR（Ingress）的直连下游（Transit）会直接回应标签，而不必等待来自最终下游（Egress）的标签。|
|**有序标签分配控制方式（Ordered）**|DU ＋ Ordered：LSR（Transit）只有收到下游（Egress）的标签映射消息，才会向上游（Ingress）分发标签。|DoD ＋ Ordered：发送标签请求的LSR（Ingress）的直连下游（Transit）只有收到最终下游（Egress）的标签映射消息，才会向上游（Ingress）分发标签。|

**标签保持方式（Label Retention Mode）**

标签保持方式是指LSR对收到的、但目前暂时不需要的标签映射的处理方式。

LSR收到的标签映射可能来自下一跳，也可能来自非下一跳。

标签保持方式：

1. 自由标签保持方式（Liberal）
对于从邻居LSR收到的标签映射，无论邻居LSR是不是自己的下一跳都保留。
2. 保守标签保持方式（Conservative）
对于从邻居LSR收到的标签映射，只有当邻居LSR是自己的下一跳时才保留。

**目前设备支持如下组合方式：**

- 下游自主方式（DU）＋ 有序标签分配控制方式（Ordered）＋ 自由标签保持方式（Liberal），该方式为缺省方式。
- 下游按需方式（DoD）＋ 有序标签分配控制方式（Ordered）＋ 保守标签保持方式（Conservative）。
- 下游自主方式（DU）＋ 独立标签分配控制方式（Independent）＋ 自由标签保持方式（Liberal）。
- 下游按需方式（DoD）＋ 独立标签分配控制方式（Independent）＋ 保守标签保持方式（Conservative）。

**LDP LSP的建立过程：**

LSP的建立过程实际就是将FEC和标签进行绑定，并将这种绑定通告LSP上相邻LSR的过程。如下图所示，下面结合下游自主标签发布方式和有序标签控制方式来说明其主要步骤。

![](images/Pasted%20image%2020221201234003.png)

![](images/Pasted%20image%2020221201234008.png)

图：LDP LSP的建立过程

1. 缺省情况下，网络的路由改变时，如果有一个边缘节点（Egress）发现自己的路由表中出现了新的主机路由，并且这一路由不属于任何现有的FEC，则该边缘节点需要为这一路由建立一个新的FEC。
2. 如果MPLS网络的Egress有可供分配的标签，则为FEC分配标签，并主动向上游发出标签映射消息，标签映射消息中包含分配的标签和绑定的FEC等信息。
3. Transit收到标签映射消息后，判断标签映射的发送者（Egress）是否为该FEC的下一跳。若是，则在其标签转发表中增加相应的条目，然后主动向上游LSR发送对于指定FEC的标签映射消息。
4. Ingress收到标签映射消息后，判断标签映射的发送者（Transit）是否为该FEC的下一跳。若是，则在标签转发表中增加相应的条目。这时，就完成了LSP的建立，接下来就可以对该FEC对应的数据报文进行标签转发。

上面介绍的是普通LDP LSP的建立，还有一种代理Egress LSP。代理Egress（Proxy Egress）是能够针对非本地路由触发建立LSP的Egress节点，当路由器使能倒数第二跳弹出时，倒数第二跳节点实际上就是一种特殊的代理Egress。一般情况下，代理Egress由配置产生。代理Egress可以应用于网络中有不支持MPLS特性的路由器场景，也可用于解决BGP路由负载分担问题。

**LDP的安全机制：**

为了提高LDP报文的安全性，MPLS提供了三种保护机制：LDP MD5认证、LDP Keychain认证和LDP GTSM。

LDP Keychain认证是比LDP MD5认证更安全的加密认证，对于同一邻居，只能选择其中一个加密认证；而LDP GTSM用来防止设备受到非法LDP报文的攻击，其可以与前面两个配合使用。

**LDP MD5认证：**

MD5（Message-Digest Algorithm 5）是RFC1321定义的国际标准摘要密码算法。MD5的典型应用是针对一段信息计算出对应的信息摘要，从而防止信息被篡改。MD5信息摘要是通过不可逆的字符串变换算法产生的，结果唯一。因此，不管信息内容在传输过程中发生任何形式的改变，只要重新计算就会产生不同的信息摘要，接收端就可以由此判定收到的是一个不正确的报文。

LDP MD5应用其对同一信息段产生唯一摘要信息的特点来实现LDP报文防篡改校验，比一般意义上TCP校验和更为严格。其实现过程如下：

1. LDP会话消息在经TCP发出前，会在TCP头后面填充一个唯一的信息摘要再发出。而这个信息摘要就是把TCP头、LDP会话消息以及用户设置的密码一起作为原始信息，通过MD5算法计算出的。
2. 当接收端收到这个TCP报文时，首先会取得报文的TCP头、信息摘要、LDP会话消息，并结合TCP头、LDP会话消息以及本地保存的密码，利用MD5计算出信息摘要，然后与报文携带的信息摘要进行比较，从而检验报文是否被篡改过。

**LDP Keychain认证：**

Keychain是一种增强型加密算法，类似于MD5，Keychain也是针对同一段信息计算出对应的信息摘要，实现LDP报文防篡改校验。

Keychain允许用户定义一组密码，形成一个密码串，并且分别为每个密码指定加解密算法（包括MD5，SHA-1等）及密码使用的有效时间。在收发报文时，系统会按照用户的配置选出一个当前有效的密码，并按照与此密码相匹配的加密解密算法以及密码的有效时间，进行发送时加密和接收时解密报文。此外，系统可以依据密码使用的有效时间，自动完成有效密码的切换，避免了长时间不更改密码导致的密码易破解问题。

Keychain的密码、所使用的加解密算法以及密码使用的有效时间可以单独配置，形成一个Keychain配置节点，每个Keychain配置节点至少需要配置一个密码，并指定加解密算法。

**LDP GTSM：**

通用TTL安全保护机制GTSM（Generalized TTL Security Mechanism）是一种通过检查IP报文头中的TTL值是否在一个预先定义好的范围内来实现对IP业务进行保护的机制。使用GTSM的两个前提：

- 设备之间正常报文的TTL值确定
- 报文的TTL值很难被修改

LDP GTSM是GTSM在LDP方面的具体应用。

GTSM通过判定报文的TTL值，确定报文是否有效，从而保护设备免受攻击。LDP GTSM是对相邻或相近（基于只要跳数确定的原则）设备间的LDP消息报文应用此种机制。用户预先在各设备上设定好针对其他设备报文的有效范围，使能GTSM，这样当相应设备之间应用LDP时，如果LDP消息报文的TTL不符合之前设置的范围要求，设备就认为此报文为非法攻击报文予以丢弃，进而实现对上层协议的保护。

**LDP扩展**

**LDP跨域扩展：**

LDP跨域扩展通过使能LDP按最长匹配原则查找路由，使LDP能够依据聚合后的路由建立起跨越多个IGP区域的LDP LSP。

**产生原因：**

当网络规模比较大时，通常需要部署多个IGP区域来达到灵活部署和快速收敛的目的。在这种情况下，IGP区域间进行路由通告时，为了避免路由数量多而引起的对资源的过多占用，区域边界路由器（ABR）需要将区域内路由聚合，再通告给相邻的IGP区域。然而，LDP在建立LSP的时候，会在路由表中查找与收到的标签映射消息中携带的FEC精确匹配的路由，对于聚合路由，LDP只能建立Liberal LSP，无法建立跨越IGP区域的LDP LSP。因此，引入LDP跨域扩展来解决这个问题。

**注：已经被分配标签，但是没有建立成功的LSP叫做Liberal LSP。**

**实现过程：**

如下图所示,存在Area10和Area20两个IGP区域。在Area10区域边缘的LSR\_2的路由表中，存在到LSR\_3和LSR\_4的两条主机路由，为了避免路由数量多而引起的对资源的过多占用，在LSR\_2上通过ISIS路由协议将这两条路由聚合为1.3.0.0/24发送到Area20区域。



![](images/Pasted%20image%2020221201234026.png)



![](images/Pasted%20image%2020221201234030.png)

图：LDP跨域扩展组网拓扑

LDP在建立LSP的时候，会在路由表中查找与收到的标签映射消息中携带的FEC精确匹配的路由，对于上图中的情况，LSR\_1的路由表中只有这条聚合后的路由，而没有32位的主机路由。

对于聚合路由，LDP只能建立Liberal LSP，无法建立跨越IGP区域的LDP LSP，以至于无法提供必要的骨干网隧道。

因此，在LSR\_1上需要按照最长匹配方式查找路由建立LSP。在LSR\_1的路由表中，已经存在聚合路由1.3.0.0/24。当LSR\_1收到Area10区域的标签映射消息时（例如携带的FEC为1.3.0.1/32），按照最长匹配的查找方式，LSR\_1能够找到聚合路由1.3.0.0/24的信息，把该路由的出接口和下一跳作为到FEC 1.3.0.1/32的出接口和下一跳。这样，LDP就可以建立跨越IGP区域的LDP LSP。

**LDP学习笔记**

LDP是用来在LSR之间建立LDP Session并交换Label/FEC映射信息的协议。

LDP用UDP（协议ID=17）发现邻居，用TCP（协议ID=6）建立邻接（LDP协议的目的端口号：646）

LDP协议将hello报文发往224.0.0.2（所有开启组播功能的路由器）的组播组，用于发现直连链路上的LDP邻居。

LDP的hello报文发送间隔是5s，hold time是3倍的hello时间15s。

LDP的keepalive报文的发送间隔是15s，old timer是3倍的keepalive时间45s。

##LDP的环路检测机制：

LDP路由向量法和最大跳数法分别通过两类TLV实现：Path Vector TLV和Hop Count TLV。如果配置使用者两类方法检测环路，那么在标签请求消息（Label Requset Message）和标签映射消息（Label Mapping MEssage）中都会携带者两种TLv。

距离向量法：每个LSR在发送标签请求消息（标签映射消息）中，包含一个Path Vector TLV，并且入口（出口）LSR产生的路由长度值为1，并且把自己的LSR ID加入到TLV的列表中，标签请求消息（标签映射消息）每经过一跳长度值加1，接收端LSR如果收到标签请求消息（标签映射消息），发现长度值达到预先设定的最大值或者发现LSR ID列表中有自己的LSR ID，认为发生环路，发出通告消息，拒绝LSP的建立。

**LDP可以为以下协议分配标签：**

- LDP：可以为直连、静态和IGP路由分配标签。
- RSVR-TE：为TE预留资源和分配标签。
- MPBGP：可以为私网路由分配标签。
- BGP：可以为BGP路由分配标签。

**FEC简介：**

转发等价类FEC（Forwarding Equivalence Class）是一组具有某些共性的数据流的集合。这些数据流在转发过程中被LSR以相同的方式出。

FEC可以根据地质、业务类型、QoS等要素进行划分。例如，在传统的采用最常匹配算法的IP转发中，到同一条路由的所有报文就是一个转发等价类。

Cisco定义：

- LFIB：标签转发表。
- LIB：标签信息数据库。
- FIB：转发信息数据库。

华为定义：

- FTN（FEC to NHLFE）：FIB。
- NHLFE（Next hop lable forward entry）
- ILM（Incoming lable map）：LFIB+LIB。

**LDP配置命令**

```
fec-list
//来创建动态BFD检测LDP LSP的FEC列表。
fec-node
//增加BFD会话的FEC节点。
gtsm peer valid-ttl-hops
//在指定的LDP对等体上配置GTSM功能。
label advertise { explicit-null | implicit-null | non-null }
//配置出节点向倒数第二跳分配的标签。
//explicit-null:不支持PHP特性，出节点向倒数第二跳分配显式空标签。显式空标签的值为0。
//implicit-null:支持PHP特性，出节点向倒数第二跳分配隐式空标签。隐式空标签的值为3。
//non-null:不支持PHP特性，出节点向倒数第二跳正常分配标签。分配的标签值不小于16。
label distribution control-mode { independent | ordered }
//配置LDP标签分配控制方式。
//缺省情况下，LDP标签分配控制方式为有序标签分配控制（Ordered）。
label-withdraw-delay
//使能Label Withdraw消息延迟发送功能。
//执行此命令后,Label Withdraw消息延迟发送的时间默认为5秒，
label-withdraw-delay timer 5
//设置Label Withdraw消息延迟发送的时间。
ldp-sync enable
//使能IS-IS进程下所有接口的LDP和IS-IS联动功能。
longest-match
//配置LDP跨域扩展功能，使能LDP按照最长匹配方式查找路由建立LSP。
//缺省情况下，LDP按照精确匹配方式查找路由建立LSP。
lsp-trigger bgp-label-route
//配置LDP为带标签的公网BGP路由分标签的能力。
lsp-trigger
//设置触发建立LSP的策略。
lsr-id
//配置LDP实例的LSR ID。
md5-password
//配置在建立LDP会话时，TCP连接所使用的密码。
md5-password all
//对所有对等体批量配置LDP MD5认证。
mpls
//使能本节点的全局MPLS能力
mpls bfd enable
//在LDP LSP的源端设备上使能动态创建BFD会话的功能。
mpls bfd
//设置BFD会话的相关参数
mpls ldp advertisement { dod | du }
//配置标签发布模式。
//缺省情况下，标签发布模式为下游自主标签分发（Downstream Unsolicited）。
mpls ldp
//来使能本节点的LDP能力
mpls ldp remote-peer
//创建远端对等体并进入远端对等体视图。
mpls ldp timer hello-hold 45
//设置Hello保持定时器的值。
mpls ldp timer hello-send 15
//配置Hello发送定时器的值。
mpls ldp timer igp-sync-delay 10
//配置LDP会话建立后等待LSP建立的时间间隔。
mpls ldp timer keepalive-hold 45
//配置Keepalive保持定时器的值。
mpls ldp timer keepalive-send 15
//配置Keepalive发送定时器的值。
mpls ldp transport-address
//配置LDP传输地址。
mpls-passive
//在LSP的目的端设备上使能被动动态创建BFD会话功能。
remote-ip
//配置LDP远端对等体的IP地址。
remote-ip auto-dod-request
//配置在采用DoD的标签发布方式下，自动向下游指定的远端对等体请求标签映射消息。
remote-peer pwe3
//配置禁止向所有远端邻居分发公网标签。
route recursive-lookup tunnel
//使能迭代隧道功能。
static-lsp egress
//在出口节点配置静态LSP。
static-lsp ingress
//为入口节点配置静态LSP。
static-lsp transit
//为中间转发节点配置静态LSP。
ttl expiration pop
//配置MPLS TTL超时后ICMP响应报文沿本地IP路由转发。
undo ttl expireation pop
//ICMP超时应答报文会沿着LSP继续往下传，由边界设备回送给发送源
ttl propagate
//配置MPLS报文中TTL传播模式为uniform。
//缺省情况下，ttl propagate命令使能，MPLS报文中TTL传播模式是uniform。 
ttl propagate public
//使能MPLS公网报文的IP TTL复制功能。
//缺省情况下，对公网报文使能TTL复制功能。
```


参考资料：华为HedEx文档


来自 <[https://cshihong.github.io/2018/02/23/MPLS-LDP%E5%9F%BA%E7%A1%80/](https://cshihong.github.io/2018/02/23/MPLS-LDP%E5%9F%BA%E7%A1%80/)\>

