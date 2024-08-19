
**BGP/MPLS IP VPN简介**

BGP/MPLS IP VPN是一种L3VPN（Layer 3 Virtual Private Network）。它使用BGP（Border Gateway Protocol）在服务提供商骨干网上发布VPN路由，使用MPLS（Multiprotocol Label Switch）在服务提供商骨干网上转发VPN报文。这里的IP是指VPN承载的是IP（Internet Protocol）报文。



![](images/Pasted%20image%2020221201234333.png)



![](images/Pasted%20image%2020221201234336.png)

图：BGP/MPLS IP VPN模型

BGP/MPLS IP VPN的基本模型由三部分组成：CE、PE和P。

- CE（Customer Edge）：用户网络边缘设备，有接口直接与服务提供商网络相连。CE可以是路由器或交换机，也可以是一台主机。通常情况下，CE“感知”不到VPN的存在，也不需要支持MPLS。
- PE（Provider Edge）：是服务提供商网络的边缘设备，与CE直接相连。在MPLS网络中，对VPN的所有处理都发生在PE上，对PE性能要求较高。
- P（Provider）：服务提供商网络中的骨干设备，不与CE直接相连。P设备只需要具备基本MPLS转发能力，不维护VPN信息。

PE和P设备仅由服务提供商管理；CE设备仅由用户管理，除非用户把管理权委托给服务提供商。

一台PE设备可以接入多台CE设备。一台CE设备也可以连接属于相同或不同服务提供商的多台PE设备。

**目的：**

传统的VPN通过在所有站点间建立全连接隧道或者永久虚链路PVC（Permanent Virtual Circuit）的方式实现，不易维护和扩展，尤其是向已有的VPN加入新的站点时，需要同时修改所有接入此VPN站点的边缘节点的配置。

BGP/MPLS IP VPN基于对等体模型，这种模型使得服务提供商和用户可以交换路由，服务提供商转发用户站点间的数据而不需要用户的参与。相比较传统的VPN，BGP/MPLS IP VPN更容易扩展和管理。新增一个站点时，只需要修改提供该站点业务的边缘节点的配置。

BGP/MPLS IP VPN支持地址空间重叠、支持重叠VPN、组网方式灵活、可扩展性好，并能够方便地支持MPLS TE，成为在IP网络运营商提供增值业务的重要手段，因此得到越来越多的应用。

**MPLS VPN的原理描述**

**基本概念:**

**Site:**

- Site是指相互之间具备IP连通性的一组IP系统，并且，这组IP系统的IP连通性不需通过运营商网络实现。
- Site的划分是根据设备的拓扑关系，而不是地理位置，尽管在大多数情况下一个Site中的设备地理位置相邻。地理位置隔离的两组IP系统，如果它们使用专线互联，不需要通过运营商网络就可以互通，这两组IP系统也组成一个Site。
- 一个Site中的设备可以属于多个VPN，换言之，一个Site可以属于多个VPN。
- Site通过CE连接到运营商网络，一个Site可以包含多个CE，但一个CE只属于一个Site。
根据Site的情况，建议CE设备选择方案如下：
如果Site只是一台主机，则这台主机就作为CE设备；
如果Site是单个子网，则使用交换机作为CE设备；
如果Site是多个子网，则使用路由器作为CE设备。
对于多个连接到同一运营商网络的Site，通过制定策略，可以将它们划分为不同的集合（set），只有属于相同集合的Site之间才能通过运营商网络互访，这种集合就是VPN。

**地址空间重叠：**

VPN是一种私有网络，不同的VPN独立管理自己的地址范围，也称为地址空间（address space）。不同VPN的地址空间可能会在一定范围内重合，例如，VPN1和VPN2都使用10.110.10.0/24网段地址，这就发生了地址空间的重叠（address spaces overlapping）。

以下两种情况允许VPN使用重叠的地址空间：

- 两个VPN没有共同的Site
- 两个VPN有共同的Site，但此Site中的设备不与两个VPN中使用重叠地址空间的设备互访

**VPN示例：**

在BGP/MPLS IP VPN中，不同VPN之间的路由隔离通过VPN实例（VPN-instance）实现。

PE为每个直接相连的Site建立并维护专门的VPN实例，VPN实例中包含对应Site的VPN成员关系和路由规则。具体来说，VPN实例中的信息包括：IP路由表、标签转发表、与VPN实例绑定的接口以及VPN实例的管理信息。VPN实例的管理信息包括RD（Route Distinguisher，路由标识符）、路由过滤策略、成员接口列表等。

VPN、Site、VPN实例之间的关系如下：

- VPN是多个Site的组合。一个Site可以属于多个VPN。
- 每一个Site在PE上都关联一个VPN实例。VPN实例综合了它所关联的Site的VPN成员关系和路由规则。多个Site根据VPN实例的规则组合成一个VPN。
- VPN实例与VPN不是一一对应的关系，VPN实例与Site之间存在一一对应的关系。

VPN实例也称为VPN路由转发表VRF（VPN Routing and Forwarding table）。PE上存在多个路由转发表，包括一个公网路由转发表，以及一个或多个VPN路由转发表。

**公网路由转发表与VPN实例存在以下不同：**

- 公网路由表包括所有PE和P设备的IPv4路由，由骨干网的路由协议或静态路由产生。
- VPN路由表包括属于该VPN实例的所有Site的路由，通过CE与PE之间或者两个PE之间的VPN路由信息交互获得。
- 公网转发表是根据路由管理策略从公网路由表提取出来的转发信息；而VPN转发表是根据路由管理策略从对应的VPN路由表提取出来的转发信息。
可以看出，PE上的各VPN实例之间相互独立，并与公网路由转发表相互独立。
可以将每个VPN实例看作一台虚拟的设备，维护独立的地址空间并有连接到私网的接口。

**RD和VPN-IPv4地址：**

传统BGP无法正确处理地址空间重叠的VPN的路由。假设VPN1和VPN2都使用了10.110.10.0/24网段的地址，并各自发布了一条去往此网段的路由。虽然本端PE通过不同的VPN实例可以区分地址空间重叠的VPN的路由，但是这些路由发往对端PE后，由于不同VPN的路由之间不进行负载分担，因此对端PE将根据BGP选路规则只选择其中一条VPN路由，从而导致去往另一个VPN的路由丢失。

PE之间使用MP-BGP（Multiprotocol Extensions for BGP-4，BGP-4的多协议扩展）来发布VPN路由，并使用VPN-IPv4地址来解决上述问题。

VPN-IPv4地址共有12个字节，包括8字节的路由标识符RD（Route Distinguisher）和4字节的IPv4地址前缀，地址格式如下：



![](images/Pasted%20image%2020221201234346.png)

![](images/Pasted%20image%2020221201234350.png)

图：VPN-IPv4地址结构

RD用于区分使用相同地址空间的IPv4前缀，增加了RD的IPv4地址称为VPN-IPv4地址（即VPNv4地址）。PE从CE接收到IPv4路由后，转换为全局唯一的VPN-IPv4路由，并在公网上发布。

RD的结构使得每个服务供应商可以独立地分配RD，但为了在CE双归属的情况下保证路由正常，必须保证PE上的RD全局唯一。

**VPN Target：**

BGP/MPLS IP VPN使用BGP扩展团体属性－VPN Target（也称为Route Target）来控制VPN路由信息的发布。

每个VPN实例关联一个或多个VPN Target属性。有两类VPN Target属性：

- Export Target：本地PE从直接相连Site学到IPv4路由后，转换为VPN-IPv4路由，并为这些路由设置Export Target属性。Export Target属性作为BGP的扩展团体属性随路由发布。
- Import Target：PE收到其它PE发布的VPN-IPv4路由时，检查其Export Target属性。当此属性与PE上某个VPN实例的Import Target匹配时，PE就把路由加入到该VPN实例中。

在BGP/MPLS IP VPN网络中，通过VPN Target属性来控制VPN路由信息在各Site之间的发布和接收。VPN Export Target和Import Target的设置相互独立，并且都可以设置多个值，能够实现灵活的VPN访问控制，从而实现多种VPN组网方案。

**基本原理：**

**私网标签分配：**

在BGP/MPLS IP VPN中，PE通过MP-BGP发布私网路由给骨干网的其他相关的PE前，需要为私网路由分配MPLS标签（私网标签）。当数据包在骨干网传输时，携带私网标签。

PE上分配私网标签的方法有如下两种：

- 基于路由的MPLS标签分配：为VPN路由表的每一条路由分配一个标签（one label per route）。这种方式的缺点是：当路由数量比较多时，设备入标签映射表ILM（Incoming Label Map）需要维护的表项也会增多，从而提高了对设备容量的要求。
- 基于VPN实例的MPLS标签分配：为整个VPN实例分配一个标签，该VPN实例里的所有路由都共享一个标签。使用这种分配方法的好处是节约了标签。

MP-BGP为私网路由分配标签的前提是PE上使能MPLS功能。

**私网路由交叉：**

两台PE之间通过MP-BGP传播的路由是VPNv4路由。当接收到VPNv4路由，PE先进行如下处理：

- 检查其下一跳是否可达。如果下一跳不可达，该路由被丢弃。
- 对于RR发送过来的VPNv4路由，如果收到的路由中cluster\_list包含自己的cluster\_id，则丢弃这条路由。
- 进行BGP的路由策略过滤，如果不通过，则丢弃该路由。

之后，PE把没有丢弃的路由与本地的各个VPN实例的Import Target属性匹配。**VPNv4路由与本地VPN实例的Import VPN-Target进行匹配的过程称为私网路由交叉**。

PE上有种特殊的路由，即来自本地CE的属于不同VPN的路由。对于这种路由，如果其下一跳直接可达或可迭代成功，PE也将其与本地的其他VPN实例的Import Target属性匹配，该过程称为本地交叉。例如：CE1所在的Site属于VPN1，CE2所在的Site属于VPN2，且CE1和CE2同时接入PE1。当PE1收到来自CE1的VPN1的路由时，也会与VPN2对应的VPN实例的Import Target属性匹配。

**公网隧道迭代：**

为了将私网流量通过公网传递到另一端，需要有一条公网隧道承载这个私网流量。因此私网路由交叉完成后，需要根据目的IPv4前缀进行路由迭代，查找合适的隧道（本地交叉的路由除外）；只有隧道迭代成功，该路由才被放入对应的VPN实例路由表。将路由迭代到相应的隧道的过程叫做隧道迭代。

隧道迭代成功后，保留该隧道的标识符（Tunnel ID），供后续转发报文时使用。Tunnel ID用于唯一标识一条隧道。VPN报文转发时根据Tunnel ID查找对应的隧道，然后从隧道上发送出去。

**私网路由的选择规则：**

经过路由交叉和隧道迭代的路由并不是全部被放入VPN实例路由表。从本地CE收到的路由和本地交叉路由也不是全部被放入VPN实例路由表。

对于到同一目的地址的多条路由，如果不进行路由的负载分担，按如下规则选择其中的一条：

- 同时存在直接从CE收到的路由和交叉成功后的同一目的地址路由，则优选从CE收到的路由。
- 同时存在本地交叉路由和从其他PE接收并交叉成功后的同一目的地址路由，则优选本地交叉路由。

对于到同一目的地址的多条路由，如果进行路由的负载分担，则：

- 优先选择从本地CE收到的路由。只有一条从本地CE收到的路由而有多条交叉路由的情况下，也只选择从本地CE收到的路由。
- 只在从本地CE收到的路由之间分担或只在交叉路由之间分担，不会在本地CE收到的路由和交叉路由之间分担。
- 负载分担的AS\_PATH属性必须完全相同。

**BGP/MPLS IP VPN的路由发布：**

基本BGP/MPLS IP VPN组网中，VPN路由信息的发布涉及CE和PE，P设备只维护骨干网的路由，不需要了解任何VPN路由信息。PE设备一般维护所有VPN路由。

VPN路由信息的发布过程包括三部分：本地CE到入口PE、入口PE到出口PE、出口PE到远端CE。完成这三部分后，本地CE与远端CE之间建立可达路由，VPN路由信息能够在骨干网上发布。

下面分别对这三部分进行介绍。

- 本地CE到入口PE的路由信息交换
CE与直接相连的PE建立邻居或对等体关系后，把本站点的IPv4路由发布给PE。CE与PE之间可以使用静态路由、RIP（Routing Information Protocol）、OSPF（Open Shortest Path First）、IS-IS（Intermediate System to Intermediate System）或BGP（Border Gateway Protocol）。无论使用哪种路由协议，CE发布给PE的都是标准的IPv4路由。
- 入口PE到出口PE的路由信息交换
  - PE从CE学到VPN路由信息后，存放到VPN实例中。同时，为这些标准IPv4路由增加RD，形成VPN-IPv4路由。
  - 入口PE通过MP-BGP的Update报文把VPN-IPv4路由发布给出口PE。Update报文中携带Export VPN Target属性及MPLS标签。
  - 出口PE收到VPN-IPv4路由后，在下一跳可达的情况下进行路由交叉、隧道迭代和路由优选，决定是否将该路由加入到VPN实例的路由表。被加入到VPN路由表的路由，本地PE为其保留如下信息以供后续转发报文时使用：
    - MP-BGP Update消息中携带的MPLS标签值
    - Tunnel ID

- 出口PE到远端CE的路由信息交换
远端CE有多种方式可以从出口PE学习VPN路由，包括静态路由、RIP、OSPF、IS-IS和BGP，与本地CE到入口PE的路由信息交换相同。此处不再赘述。值得注意的是，出口PE发布给远端CE的路由是普通IPv4路由。

以下图为（PE-CE之间使用BGP，公网隧道为LSP）为例，说明将CE2的一条路由发送到CE1的过程。



![](images/Pasted%20image%2020221201234403.png)


![](images/Pasted%20image%2020221201234408.png)

图：CE2->CE1的路由发布过程

1. 在CE2的BGP IPv4单播地址族下引入IGP（Interior Gateway Protocol）路由。
2. CE2将该路由随EBGP的Update消息一起发布给Egress PE。Egress PE从连接CE2的接口收到Update消息，把该路由转化为VPN IPv4路由，加入对应的VPN实例路由表。
3. Egress PE为该路由分配MPLS标签，并将标签和VPN IPv4路由信息加入MP-IBGP的Update消息中的NLRI字段中，Export-RT属性加入MP-BGP Update消息的扩展团体属性字段中，将Update消息发送给Ingress PE。
4. Ingress PE对该路由进行路由交叉。交叉成功则根据路由目的IPv4地址进行隧道迭代，查找合适的隧道。如果迭代成功，则保留该隧道的Tunnel ID和标签，并将路由加入该VPN实例路由表。
5. Ingress PE把该路由通过BGP Update消息发布给CE1。此时路由是普通IPv4路由。
6. CE1收到该路由后，把该路由加入BGP路由表。通过在IGP中引入BGP路由的方法可使CE1把该路由加入IGP路由表。

上面过程只是将CE2的路由发布给CE1。要实现CE1与CE2的互通，还需要将CE1的路由发布给CE2，其过程与上面的步骤类似，此处不再赘述。

**BGP/MPLS IP VPN的报文转发：**

在基本BGP/MPLS IP VPN应用中（不包括跨域的情况），VPN报文转发采用两层标签方式：

- 第一层（公网）标签在骨干网内部进行交换，指示从PE到对端PE的一条LSP。VPN报文利用这层标签，可以沿LSP到达对端PE；
- 第二层（私网）标签在从对端PE到达CE时使用，指示报文应被送到哪个Site，或者更具体一些，到达哪一个CE。这样，对端PE根据内层标签可以找到转发报文的接口。

特殊情况下，属于同一个VPN的两个Site连接到同一个PE，这种情况下只需要知道如何到达对端CE。

以下图为例说明BGP/MPLS IP VPN报文的转发过程。下图是CE1发送报文给CE2的过程。其中，I-L表示内层标签，O-L表示外层标签。



![](images/Pasted%20image%2020221201234416.png)


![](images/Pasted%20image%2020221201234420.png)

图：VPN报文转发过程

1. CE1发送一个VPN报文。
2. Ingress PE从绑定了VPN实例的接口上接收VPN数据包后进行如下操作：
  - 先根据绑定的VPN实例的RD查找对应VPN的转发表。
  - 匹配目的IPv4前缀，查找对应的Tunnel ID。
  - 将报文打上对应的标签（I-L），根据Tunnel-ID找到隧道。
  - 将报文从隧道发送出去。此例的隧道是LSP，则打上公网（外层）MPLS标签头（O-L1）。


接着，该报文携带两层MPLS标签穿越骨干网。骨干网的每台P设备都对该报文进行外层标签交换。

3. Egress PE收到该携带两层标签的报文，交给MPLS协议处理。MPLS协议将去掉外层标签（此例最后的外层标签是O-L2，但如果应用了倒数第二跳弹出，则此标签会在到达Egress PE之前的一跳弹出，Egress PE只能收到带有内层标签的报文）。
4. 此时Egress PE就可以看见内层标签，发现该标签处于栈底，将内层标签剥离。
5. Egress PE将报文从对应出接口发送给CE2。此时报文是个纯IP报文。
这样，报文就成功地从CE1传到CE2了。CE2按照普通的IP转发过程将报文传送到目的地。

**MPLS VPN学习笔记**

**PE与CE之间为通过OSPF多实例接入时防环机制：**

**在MP-BGP中传递路由信息时，会在扩展团体属性中携带IGP的路由信息。在OSPF中：**

**OSPF DOMAIN ID < 0.0.0.0 : 0 >:**

- 0..0.0.0 : 代表domain ID，默认值为0.0.0.0，可以修改。
- 0：代表vlaue 值，默认为0，可以修改。

**OSPF RT < 0.0.0.0 : 1 : 0 >:**

- 0.0.0.0 :代表区域ID。
- 1 ：代表LSA的类型（1：域内路由、3：域间路由、5：外部路由、7：外部路由（NSSA））
- 0：代表外部路由的类型（0=类型1,1=类型2）

**OSPF ROUTER ID < 37.1.1.3 : 0 >:**

- 37.1.1.1:代表通告OSPF路由设备的router-id。

**将BGP路由重发布进OSPF以哪种类型的路由通告，需要看OSPF的扩展参数。判断原则如下：**

1. 如果扩展参数中的domain-id和设备本地的somain-id不一致，直接通告成外部路由（5类LSA）。
2. 如果domain-id一致，需要看OSPF RT中的LSA类型，如果LSA的类型为1或者3，则通告类型为域间路由（3类LSA），如果LSA的类型为5则通告为外部路由，并标识外部路由类型。如果LSA的类型为7则通告为NSSA外部路由，并标识外部路由类型。

注：在修改domain id时，domain id和value两端必须一致，type可以不一致。如果domain id两端不一致，OSPF路由会直接以外部路由方式（5类LSA）通告。

**OSPF在VPN中有两种防环机制：TAG和Downbit位（DN位）。**

DN位主要用于3类LSA防环，而TAG主要用于5类、7类LSA防环。（在华为中5/7类同时也会将DN为置位）。

注：TAG长度是32个bit，其中最后16个bit用于标识AS number。PE设备收到OSPF外部 LSA后需要查看外部路由的TAG中是否包含本地的AS number，如果包含不计算该路由。

**Sham-Link：**

建立Sham-link的条件：

1. 建立sham-link的路由必须是32位路由。
2. 建立sham-link的路由必须通过MPBGP通告。

注：sham-link在华为中的实现并不是按需电路，会定期通告hello和lsa维护邻居状态和LSA的状态。

sham-link需要在两端PE的OSPF私有进程下配置。

**防环机制：**

ISIS中通过UP/DOWN位实现防环，PE设备不会讲DOWN位置位的路由重分发进BGP.

RIP中通过在单点双向重分发时做过标记，过滤进行防环。

**在MPLS VPN中访问Internet有三种方式：**

1. 通过在CE和PE之间拉单独的专线实现上网。
2. 路由泄露。
路由泄露配置（主要在PE上配置）
ip router-static 14.1.1.1.0 255.255.255.0 vpn-instance HCIE 14.1.1.4
ip router-static vpn-instance HCIE 0.0.0.0 0 12,1,1,2 public
3. 利用Tunnel实现CE访问Internet。
CE端配置：
interface Tunnel0/0/0
ip address 100.1.1.1 24
tunnel-protocol gre
source 14.1.1.4
destination 14.1.1.1
nat outbound 20000
PE端配置：
interface Tunnel
0/0/0
ip address 100.1.1.2 24
tunnrl-protocol gre
source 14.1.1.1
destination vpn-instance HCIE 14.1.1.4//在PE端配置目的地址需要指定VPN实例
ospf enable 1 area 0 //需要将tunnel夹克宣告进公网路由。

**MPLS IP VPN配置命令行**
```
apply-label per-instance
//配置当前VPN实例地址族下所有发往对端PE的路由都使用同一个标签值。
apply-label per-nexthop
//使能ASBR按下一跳为VPNv4路由分标签。
apply-label per-route
//使能每路由每标签的标签分配方式，即配置当前VPN实例地址
//族下的所有发往对端PE的每条路由使用单独的标签值。
//缺省情况下，同一VPN实例IPv4地址族下所有发往对端PE的路由都使用同一个标签值。
display ip vpn-instance
//查看VPN实例的配置信息。
display ip vpn-instance import-vt
//显示所有具备指定入口vpn-target属性的VPN实例。
display interface tunnel
//查看Tunnel接口的所有信息。
display tunnel-info
//查看隧道信息。
ingress-lsp trigger
//配置基于路由策略为BGP标签路由创建Ingress LSP的功能。
ip binding vpn-instance
//将PE上的接口与VPN实例绑定。
//配置接口与VPN实例绑定后，或取消接口与VPN实例的绑定，
//都会清除该接口的IP地址、三层特性和IP相关的路由协议，如果需要应重新配置。
ip vpn-instance
//创建VPN实例，并进入VPN实例视图。
peer soo
//为BGP VPN实例下的EBGP对等体配置Site-of-Origin（SoO）。
peer substitute-as
//使能AS号替换功能，即用本地AS号替换AS_Path属性中指定对等体的AS号。
route-distinguisher
//为VPN实例地址族配置路由标识RD（Route Distinguisher）。
traffic-statistics enable
//使能基于VPN实例的流量统计功能。
vpn-target
//配置VPN实例地址族入方向或出方向的VPN-Target扩展团体属性。
```


参考资料：华为HedEx文档



来自 <[https://cshihong.github.io/2018/03/03/MPLS-VPN/](https://cshihong.github.io/2018/03/03/MPLS-VPN/)\>

