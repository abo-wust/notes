已剪辑自: [https://info.support.huawei.com/info-finder/encyclopedia/zh/Super+VLAN.html](https://info.support.huawei.com/info-finder/encyclopedia/zh/Super+VLAN.html)

Super VLAN，也叫VLAN聚合（VLAN Aggregation）指在一个物理网络内，用多个[VLAN](https://info.support.huawei.com/info-finder/encyclopedia/zh/VLAN.html)（称为Sub-VLAN）隔离广播域，并将这些Sub-VLAN聚合成一个逻辑的VLAN（称为Super-VLAN），这些Sub-VLAN使用同一个IP子网和缺省网关，达到节约IP地址资源的目的。

为什么需要Super VLAN?

交换网络中，[VLAN](https://info.support.huawei.com/info-finder/encyclopedia/zh/VLAN.html)技术以其对广播域的灵活控制和部署方便而得到了广泛的应用。但是在一般的三层交换机中，通常是采用一个VLAN对应一个VLANIF接口的方式实现广播域之间的互通，这在某些情况下导致了IP地址的浪费。因为一个VLAN对应的子网中，子网号、子网定向广播地址、子网缺省网关地址不能用作VLAN内的主机IP地址，且子网中实际接入的主机可能少于编址数，多出来的IP地址也会因不能再被其他VLAN使用而被浪费掉。

如下图所示的VLAN规划中，VLAN2预计未来有10个主机地址的需求，但按编址方式，至少需要给其分配一个掩码长度是28的子网10.1.1.0/28，其中10.1.1.0为子网号，10.1.1.15为子网定向广播地址，10.1.1.1为子网缺省网关地址，这三个地址都不能用作主机地址，剩下范围在10.1.1.2～10.1.1.14的地址可以被主机使用，共13个。

这样，VLAN2子网至少浪费3个IP地址，三个VLAN子网一起至少浪费9个IP地址。同时，VLAN2子网实际地址需求只有10个，剩余的3个也不能再被其他VLAN使用。网络中的VLAN越多，浪费的IP地址也就越多。

![](images/Pasted%20image%2020221201235958.png)



*普通VLAN网络示意图*

为了解决上述问题，VLAN聚合应运而生。它通过引入Super-VLAN和Sub-VLAN的概念，使每个Sub-VLAN对应一个广播域，并让多个Sub-VLAN和一个Super-VLAN关联，只给Super-VLAN分配一个IP子网，所有Sub-VLAN都使用Super-VLAN的IP子网和缺省网关进行三层通信。

这样，多个Sub-VLAN共享一个网关地址，节约了子网号、子网定向广播地址、子网缺省网关地址，且各Sub-VLAN间的界线也不再是从前的子网界线了，它们可以根据各自主机的需求数目在Super-VLAN对应子网内灵活的划分地址范围，从而即保证了各个Sub-VLAN作为一个独立广播域实现广播隔离，又节省了IP地址资源，提高了编址的灵活性。

Super VLAN应用场景是什么？

如下图所示，某公司拥有多个部门，为了提升业务安全性，将不同部门划分到不同[VLAN](https://info.support.huawei.com/info-finder/encyclopedia/zh/VLAN.html)中。各部门均有访问Internet需求，且由于业务需要，部门1与部门2间需要互通，部门3与部门4间需要互通，但公司IP地址有限。

![](images/Pasted%20image%2020221202000006.png)

*VLAN聚合应用组网图*

可通过部署VLAN聚合实现公司的需求，在Switch上部署Super VLAN 2和Super VLAN 3，将Sub VLAN 21和Sub VLAN 22聚合到Super VLAN 2中，将Sub VLAN 31和Sub VLAN 32聚合到Super VLAN 3中。这样，只需在Switch上为Super VLAN 2和Super VLAN 3分配IP地址，部门1和部门2的用户可通过Super VLAN 2的IP地址访问Internet，部门3和部门4的用户可通过Super VLAN 3的IP地址访问Internet，既实现了各部门访问Internet的需求，又节约了IP地址资源。

同时，分别在Switch的Super VLAN 2、Super VLAN 3上配置[Proxy ARP](https://info.support.huawei.com/info-finder/encyclopedia/zh/ARP.html)，即可实现部门1和部门2间的互通、部门3和部门4间的互通。

Super VLAN是如何工作的?

VLAN聚合通过定义Super-VLAN和Sub-VLAN，使Sub-VLAN只包含物理接口，负责保留各自独立的广播域；Super-VLAN不包含物理接口，只用来建立三层VLANIF接口。然后再通过建立Super-VLAN和Sub-VLAN间的映射关系，把三层VLANIF接口和物理接口两部分有机的结合起来，实现所有Sub-VLAN共用一个网关与外部网络通信，并用[Proxy ARP](https://info.support.huawei.com/info-finder/encyclopedia/zh/ARP.html)实现Sub-VLAN间的三层通信，从而在实现普通[VLAN](https://info.support.huawei.com/info-finder/encyclopedia/zh/VLAN.html)的隔离广播域的同时，达到节省IP地址的目的。

工作原理

相对每一个普通VLAN都有一个三层逻辑接口和若干物理接口，VLAN聚合定义的Super-VLAN和Sub-VLAN比较特殊：

- Sub-VLAN：只包含物理接口，不能建立三层VLANIF接口，用于隔离广播域。每个Sub-VLAN内的主机与外部的三层通信是靠Super-VLAN的三层VLANIF接口来实现的。
- Super-VLAN：只建立三层VLANIF接口，不包含物理接口，与子网网关对应。与普通VLAN不同的是，它的VLANIF接口的Up不依赖于自身物理接口的Up，而是只要它所含Sub-VLAN中存在Up的物理接口就Up。

一个Super-VLAN可以包含一个或多个Sub-VLAN。Sub-VLAN不再占用一个独立的子网网段。在同一个Super-VLAN中，无论主机属于哪一个Sub-VLAN，它的IP地址都在Super-VLAN对应的子网网段内。

这样，Sub-VLAN间共用同一个网关，既减少了一部分子网号、子网缺省网关地址和子网定向广播地址的消耗，又实现了不同广播域使用同一子网网段地址的目的，消除了子网差异，增加了编址的灵活性，减少了闲置地址浪费。

如下图所示，按照VLAN聚合的实现方式，令VLAN10为Super-VLAN，分配子网10.1.1.0/24，VLAN2～VLAN4作为Super-VLAN10的Sub-VLAN。

![](images/Pasted%20image%2020221202000013.png)

*VLAN聚合实现示意图*

可以看出，Sub-VLAN2、Sub-VLAN3和Sub-VLAN4共用一个子网10.1.1.0/24，这样，该网络中就只有一个子网号10.1.1.0、一个子网缺省网关地址10.1.1.1和一个子网定向广播地址10.1.1.255共三个IP地址不能被主机使用，其余都可以被主机使用。而且，各Sub-VLAN间的界线也不再是从前的子网界线了，它们可以根据其各自主机的需求数目在Super-VLAN对应子网内灵活的划分地址范围，比如Sub-VLAN2实际需要10个，就给它分配10.1.1.2～10.1.1.11的地址段。

Sub-VLAN之间的通信

VLAN聚合在实现不同VLAN共用同一子网网段地址的同时，也给Sub-VLAN间的三层转发带来了问题。普通VLAN中，不同VLAN内的主机可以通过各自不同的网关进行三层互通。但是Super-VLAN中，所有Sub-VLAN内的主机使用的是同一个网段的地址，共用同一个网关地址，主机只会做二层转发，而不会送网关进行三层转发。即实际上，不同Sub-VLAN的主机在二层是相互隔离的，这就造成了Sub-VLAN间无法通信的问题。

解决这一问题的方法就是使用Proxy ARP。

如下图所示，假设Sub-VLAN2内的主机Host\_1与Sub-VLAN3内的主机Host\_2要通信，在Super-VLAN10的VLANIF接口上启用Proxy ARP。

![](images/Pasted%20image%2020221202000024.png)

*
Proxy ARP实现不同Sub-VLAN间的三层通信组网图*

Host\_1与Host\_2的通信过程如下（假设Host\_1的[ARP表](https://info.support.huawei.com/info-finder/encyclopedia/zh/ARP.html)中无Host\_2的对应表项）：

1. Host\_1将Host\_2的IP地址（10.1.1.12）和自己所在网段10.1.1.0/24进行比较，发现Host\_2和自己在同一个子网，但是Host\_1的ARP表中无Host\_2的对应表项。
2. Host\_1发送[ARP](https://info.support.huawei.com/info-finder/encyclopedia/zh/ARP.html)广播报文，请求Host\_2的MAC地址，目的IP为10.1.1.12。
3. 网关L3 Switch收到Host\_1的ARP请求，由于网关上使能Sub-VLAN间的Proxy ARP，开始使用报文中的目的IP地址在路由表中查找，发现匹配了一条路由，下一跳为直连网段（VLANIF10的10.1.1.0/24），VLANIF10对应Super-VLAN10，则向Super-VLAN10的所有Sub-VLAN接口发送一个ARP广播，请求Host\_2的MAC地址。
4. Host\_2收到网关发送的ARP广播后，对此请求进行ARP应答。
5. 网关收到Host\_2的应答后，就把自己的MAC地址回应给Host\_1。
6. Host\_1之后要发给Host\_2的报文都先发送给网关，由网关做三层转发。

Host\_2发送报文给Host\_1的过程和上述的Host\_1发送报文给Host\_2的过程类似，不再赘述。

Sub-VLAN与其他网络的三层通信

如下图所示，用户主机与服务器处于不同的网段中，Switch\_1上配置了Sub-VLAN2、Sub-VLAN3、Super-VLAN4和VLAN10，Switch\_2上配置了VLAN10和VLAN20。

![](images/Pasted%20image%2020221202000030.png)

*
Sub-VLAN与其他网络的三层通信组网图*

假设Sub-VLAN2下的主机Host\_1想访问与Switch\_2相连的Server，报文转发流程如下（假设Switch\_1上已配置了去往10.1.2.0/24网段的路由，Switch\_2上已配置了去往10.1.1.0/24网段的路由，但两交换机没有任何三层转发表项）：

1. Host\_1将Server的IP地址（10.1.2.2）和自己所在网段10.1.1.0/24进行比较，发现和自己不在同一个子网，发送ARP请求给自己的网关，请求网关的MAC地址，目的MAC为全F，目的IP为10.1.1.1。
2. Switch\_1收到该请求报文后，查找Sub-VLAN和Super-VLAN的对应关系，知道应该回应Super-VLAN4对应的VLANIF4的MAC地址，并知道从Sub-VLAN2的接口回应给Host\_1。
3. Host\_1学习到网关的MAC地址后，开始发送目的MAC为Super-VLAN4对应的VLANIF4的MAC地址、目的IP为10.1.2.2的报文。
4. Switch\_1收到该报文后，根据Sub-VLAN和Super-VLAN的对应关系以及目的MAC判断进行三层转发，查三层转发表项没有找到匹配项，上送CPU查找路由表，得到下一跳地址为10.1.10.2，出接口为VLANIF10，并通过ARP表项和MAC表项确定出接口，把报文发送给Switch\_2。
5. Switch\_2根据正常的三层转发流程把报文发送给Server。

Server收到Host\_1的报文后给Host\_1回应，回应报文的目的IP为10.1.1.2，目的MAC为Switch\_2上VLANIF20接口的MAC地址，回应报文的转发流程如下：

1. Server给Host\_1的回应报文按照正常的三层转发流程到达Switch\_1。到达Switch\_1时，报文的目的MAC地址为Switch\_1上VLANIF10接口的MAC地址。
2. Switch\_1收到该报文后根据目的MAC地址判断进行三层转发，查三层转发表项没有找到匹配项，上送CPU，CPU查路由表，发现目的IP为10.1.1.2对应的出接口为VLANIF4，查找Sub-VLAN和Super-VLAN的对应关系，并通过ARP表项和MAC表项，知道报文应该从Sub-VLAN2的接口发送给Host\_1。
3. 回应报文到达Host\_1。

Sub-VLAN与其他设备的二层通信

如下图所示，Switch\_1上配置了Sub-VLAN2、Sub-VLAN3和Super-VLAN4，Switch\_1的IF\_1和IF\_2配置为Access接口，IF\_3接口配置为Trunk接口，并允许VLAN2和VLAN3通过；Switch\_2连接Switch\_1的接口配置为Trunk接口，并允许VLAN2和VLAN3通过。

![](images/Pasted%20image%2020221202000038.png)

*Sub-VLAN与其他设备的二层通信组网图*

从Host\_1进入Switch\_1的报文会被打上VLAN2的Tag。在Switch\_1中这个Tag不会因为VLAN2是VLAN4的Sub-VLAN而变为VLAN4的Tag。该报文从Switch\_1的Trunk接口IF\_3出去时，依然是携带VLAN2的Tag。

也就是说，Switch\_1本身不会发出VLAN4的报文。就算其他设备有VLAN4的报文发送到该设备上，这些报文也会因为Switch\_1上没有VLAN4对应的物理接口而被丢弃。因为Switch\_1的IF\_3接口上根本就不允许Super-VLAN4通过。对于其他设备而言，有效的VLAN只有Sub-VLAN2和Sub-VLAN3，所有的报文都是在这些VLAN中交互的。

这样，Switch\_1上虽然配置了VLAN聚合，但与其他设备的二层通信，不会涉及到Super-VLAN，与正常的二层通信流程一样，此处不再赘述。

