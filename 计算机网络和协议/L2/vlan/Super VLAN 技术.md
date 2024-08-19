已剪辑自: [http://www.h3c.com/cn/d\_200911/655248\_97665\_0.htm](http://www.h3c.com/cn/d_200911/655248_97665_0.htm)

1               Super VLAN 产生的背景

在LanSwitch网络中，VLAN技术以其对广播域的灵活控制（可跨物理设备）、部署方便而得到了广泛的应用。但是在一般的三层交换机中，通常是采用一个VLAN对应一个三层接口的方式来实现广播域之间的互通的，这在某些情况下导致了对IP地址的较大浪费。

我们来看下面这个例子，设备内VLAN划分如图1所示。

![](images/Pasted%20image%2020221202000109.png)

|VLAN|IP Subnet|Gateway Address|Usable Hosts|Customer Hosts|Needed Hosts|
|----|---------|---------------|------------|--------------|------------|
|21|1.1.1.0/28|1.1.1.1|14|13|10|
|22|1.1.1.16/29|1.1.1.17|6|5|5|
|23|1.1.1.24/30|1.1.1.25|2|1|1|



表1列出了地址的划分情况。VLAN 21预计未来要有10个主机，给其分配一个掩码长28的子网——1.1.1.0 /28。网段内子网号1.1.1.0和子网定向广播地址1.1.1.15不能用作主机地址，1.1.1.1作为子网缺省网关地址也不可作为主机地址，剩下地址范围在1.1.1.2～1.1.1.14的主机地址可以被主机使用，这部分地址共有13个（

![](images/Pasted%20image%2020221202000134.png)

）。这样，尽管VLAN 21只需要10个地址就可以满足需求了，但是按照子网划分却要分给它13个地址。

依此类推，VLAN 22预计未来有5个主机地址需求，至少需要分配一个29位掩码的子网（1.1.1.16 /29）才能满足要求。VLAN 23只有1个主机，则占用子网1.1.1.24 /30。

这三个VLAN总共需要10+5+1＝16个地址，按照普通VLAN的编址方式，即使最优化的方案也需要占用

![](images/Pasted%20image%2020221202000139.png)

个地址，地址浪费了将近一半。

而且，如果VLAN 21后来并没有增长到10台主机，而只增长到了3台主机，本来分一个掩码长29的子网就够用了，但之前却分了一个子网掩码长28的子网给它，多出来的地址都因不能再被其他VLAN使用而被浪费掉了。

同时，这种划分也给后续的网络升级带来了很大不便。假设VLAN23所在的客户一段时间后需要增加2台主机，又不愿意改变已经分配的IP地址。在1.1.1.24后面的地址已经分配给了其他人的情况下，那没有办法，只能额外再给他分配一个的29位掩码的子网和一个新的VLAN给他。这样VLAN23的客户只有3台主机，却被迫分配了两个子网，不在同一个VLAN，造成管理上的极大不便。

由此我们可以看出，被诸如子网号、子网定向广播地址、子网缺省网关地址消耗掉的IP地址数量是相当可观。同时，这种地址分配的固有约束也严重降低了编址的灵活性，使许多闲置地址被浪费。为了解决这一问题，Super VLAN应运而生。

2               Super VLAN 的实现

Super VLAN技术（也称为VLAN Aggregation ，即VLAN聚合）针对上述缺陷作了改进，它引入super-VLAN和sub-VLAN的概念。一个super-VLAN可以包含一个或多个保持着不同广播域的sub-VLAN。sub-VLAN不再占用一个独立的子网网段。在同一个super-VLAN中，无论主机属于哪一个sub-VLAN，它的IP地址都在super-VLAN对应的子网网段内。

这样，通过sub-VLAN间共用同一个三层接口，既减少了一部分子网号、子网缺省网关地址和子网定向广播地址的消耗，又实现了不同广播域使用同一子网网段地址，消除了子网差异，增加了编址的灵活性，减少了闲置地址浪费。从而，在保证了各个sub-VLAN作为一个独立广播域实现了广播隔离的同时，将从前使用普通VLAN浪费掉的IP地址节省下来。

仍以前面的例子进行说明。用户需求不变。仍旧是VLAN 21预计有10个主机，VLAN 22 预计有5个，VLAN 23 预计1个。按照Super VLAN的实现方式，令VLAN 2为super-VLAN，分配子网1.1.1.0/24，子网缺省网关地址1.1.1.1，子网定向广播地址1.1.1.255，如图2所示。则sub-VLAN——VLAN 21、VLAN 22、VLAN 23的地址划分如表2所示。

![](images/Pasted%20image%2020221202000152.png)

|VLAN|IP Subnet|Gateway Address|Usable Hosts|Customer Hosts|
|----|---------|---------------|------------|--------------|
|21|1.1.1.0/24|1.1.1.1|10|.2-.11|
|22|1.1.1.0/24|1.1.1.1|5|.12-.16|
|23|1.1.1.0/24|1.1.1.1|1|.17|



从表中我们可以看到，VLAN 21、VLAN 22、VLAN 23共用同一个子网号1.1.1.0、子网缺省网关地址1.1.1.1和子网定向广播地址1.1.1.255。这样，普通VLAN实现方式中用到的1.1.1.16、1.1.1.24这样的子网号（1.1.1.0仍被作为子网号被占用）和1.1.1.17、1.1.1.25这样的子网缺省网关（1.1.1.1仍作为子网缺省网关被占用），以及1.1.1.15、1.1.1.23、1.1.1.27这样的子网定向广播地址就都可以用来作为主机IP地址使用了。

Super VLAN的实现中，各sub-VLAN间的界线也不再是从前的子网界线了，它们可以根据其各自主机的需求数目在super-VLAN对应子网内灵活的划分地址范围。譬如，原来在分配给VLAN 21的子网1.1.1.0/28中的1.1.1.12～1.1.1.14这3个地址是被闲置浪费掉的。用Super VLAN来实现，就可以把这些地址划分给VLAN 22。如果VLAN 21最终只用了3个地址的情况，那么剩下的地址还可以分配给其他的sub-VLAN。

这样，3个VLAN一共需要16个地址，实际上在这个子网里就刚好分配了16个地址给它们。这16个主机地址加上子网号、子网缺省网关、子网定向广播地址，一共用去了19个IP地址，网段内仍剩余237的地址可以被任意sub-VLAN内的主机使用。

3               super-VLAN 和sub-VLAN

在Super VLAN中，引入super-VLAN和sub-VLAN的概念。这是两种与以往的VLAN不同的VLAN类型。

super-VLAN和通常意义上的VLAN不同，它只建立三层接口，而不包含物理端口。因此，可以把它看作一个逻辑的三层概念——若干sub-VLAN的集合。与一般没有物理端口的VLAN不同的是，它的三层虚接口的UP状态不依赖于其自身物理端口的UP，而是只要它所含sub-VLAN中存在UP状态的物理端口。

sub-VLAN 则只包含物理端口，但不能建立三层VLAN虚接口。它与外部的三层交换是靠super-VLAN的三层虚接口来实现的。

我们可以这样看：每一个普通VLAN都有一个三层逻辑接口和若干物理端口。而Super VLAN把这两部分剥离开来：sub-VLAN只映射若干物理端口，负责保留各自独立的广播域；而用一个super-VLAN来实现所有sub-VLAN共享同一个三层接口的需求，使不同sub-VLAN内的主机可以共用同一个super-VLAN的网关，在super-VLAN对应的子网里分配地址；然后再通过建立super-VLAN和sub-VLAN间的映射关系，把三层逻辑接口和物理端口这两部分有机的结合起来，并用ARP Proxy来实现sub-VLAN间的三层互访，从而在实现普通VLAN的功能的同时，达到节省IP地址的目的。

4               Super VLAN的通信

4.1               super-VLAN内部通信

4.1.1             VLAN聚合带来的问题

VLAN聚合（即Super VLAN）在实现不同VLAN间共用同一子网网段地址的同时，也给sub-VLAN间的三层转发带来了问题。原本在三层交换机上，VLAN间是可以通过走各自不同的上行网关进行三层转发的。但是super-VLAN内的主机使用的是同一个网段的地址，共用同一个上行网关，即使是属于不同的sub-VLAN的主机，由于其地址同属一个子网，设备会认为它们是二层互通的，会做二层转发，而不会送网关进行三层转发。而实际上sub-VLAN间在二层是相互隔离的，这就造成了sub-VLAN间无法通信的局面（具体过程参见4.1.3中的举例）。

解决之道就是，用ARP Proxy来实现sub-VLAN间的互通。

4.1.2             ARP Proxy原理

一个物理网络的子网（subnet）中的源主机向另一个物理网络的子网中的目的主机发ARP request，和源主机直连的网关用自己接口的MAC地址代替目的主机回ARP reply，这个过程称为ARP 代理。 也称作混合ARP(ARP promiscuous)、ARP 出租(ARP hack)，或者叫(透明)子网网关((transparent) subnet ARP gateways)。

PROXY ARP 的基本过程如下：

源主机向另一物理网络的子网的目的主机发ARP请求；

与源主机网络相连的网关已经使能ARP PROXY功能，如果存在到达目的主机的正常路由，则代替目的主机REPLY自己接口的MAC地址；

源主机向目的主机发送的IP报文都发给了路由器；

路由器对报文做正常的IP路由转发；

发往目的主机的IP报文通过网络，最终到达目的主机。

4.1.3             用ARP Proxy实现不同sub-VLAN间的三层互通

例如，super-VLAN—VLAN 2包含sub-VLAN—VLAN 21和VLAN 22。IP地址和MAC地址如图3所示。

![](images/Pasted%20image%2020221202000206.png)

VLAN 21内的主机A与VLAN 22内的主机B的过程如下（假设主机A的ARP表中无B对应表项）：A要发送报文给B，则将B的IP地址——1.1.1.15 和A自己所在网段1.1.1.0 /24 进行比较，由于VLAN 21 和VLAN 22 同属一个super-VLAN所在子网，于是发广播ARP请求B的MAC，而B并不在VLAN 21的广播域内，无法接收到这个ARP。此时，ARP Proxy所做的就是，当A在二层发出的ARP Request在其广播域内没有回应时，网关开始在路由表查找，发现下一跳为直连路由接口，则在该接口发一个广播ARP请求B的MAC地址；得到B的回应后，就把自己的MAC地址00-e0-fc-00-00-11 当作B的MAC地址（实际上是00-e0-fc-00-00-15 ）回给A（不同设备的实现略有不同）。之后，主机A要发给B的报文都发给了交换机，由三层交换机做正常的三层转发。

B回送给A的报文转发过程和上述的A到B的报文流程类似。

4.2               Super VLAN与其他设备间的二层通信

Super VLAN并不包含物理端口，可以把它看作一个逻辑的三层概念——若干sub VLAN的集合。所以，在我们通常使用的基于端口的VLAN中，二层通信时，无论是入还是出都不会有针对Super VLAN的报文。举个例子来说，如图4所示。

![](images/Pasted%20image%2020221202000217.png)

从port 1进入设备的报文会打VLAN 21的tag，在设备中这个tag不会变为VLAN 2的tag。它从trunk口port 3出去时，依然是VLAN 21的报文。

也就是说，设备本身不会发出VLAN 2 的报文。就算上层设备有VLAN 2 的报文发到该设备上，也会因为没有对应物理端口而被丢弃。而事实上，port3这个trunk口上根本就不允许Super VLAN——VLAN 2，无论是passing还是allowed（在8.2中将做详细讨论），也就根本不可能有VLAN 2 的报文通过。对于其他设备而言，有效的VLAN只有VLAN 21 和VLAN 22，报文都是在这两个VLAN中转发的。

4.3               super-VLAN与外部的三层通信

如图5。S3526E上配置了super-VLAN 2，sub-VLAN 21和22，并有一个正常的VLAN10，S3526上有两个正常的VLAN10和20。假设super-VLAN2下sub-VLAN21下的主机A想访问S3526下连的主机C，下面简述一下上下行报文的转发流程。

![](images/Pasted%20image%2020221202000223.png)

4.3.1             报文上行流程

主机A （1.1.1.2/24）要访问主机C（1.1.3.2/24），因为IP地址不在一个网段，所以A发送ARP Request给自己的网关，请求网关的MAC地址。sub-VLAN 21 收到ARP报文后，将其送上CPU。CPU查找sub-VLAN和super-VLAN的对应关系，知道应该回应super-VLAN 2 的MAC（不同设备实现上可能略有不同），并且知道回应报文应该从sub-VLAN 21发送给主机A。

主机A学到网关MAC地址后，开始发送目的MAC为super-VLAN2、目的IP为1.1.3.2的报文。sub-VLAN21 收到报文后，检测到目的MAC，知道应该进行三层转发，于是查找FIB表，发现下一跳地址为1.1.2.2，出接口为VLAN10，并通过ARP表项和MAC表项确定出端口，把报文发送给S3526，S3526根据正常的转发流程把报文发送给主机C。

4.3.2             报文下行流程

主机C回应的报文，在S3526上按正常的转发流程到达S3526E，这时S3526E会查找FIB表项，发现目的地址为主机A（1.1.1.2）对应的出接口为super-VLAN2，但是super-VLAN2根本没有包含任何端口，那么究竟应该如何确定报文的出端口呢？

仔细查看可以发现，尽管在FIB表中1.1.1.2/24网段的出接口为super-VLAN2，但是在ARP表中IP地址1.1.1.2对应的出接口却为sub-VLAN 21。而芯片真正用来构建转发表时所用的出接口应该是ARP表中对应的sub-VLAN接口，这样就可以通过在sub-VLAN下查找目的MAC找到正确的出端口。于是，主机C的回应报文就能正常到达主机A了。

5               Super VLAN的配置

Super VLAN是一个三层概念，只在三层交换机上实现。现在以S3526E为例，说明Super VLAN的配置步骤如下：（VLAN2作为super-VLAN，VLAN21、VLAN22、VLAN23作为sub-VLAN）

l          在交换机上建立sub-VLAN

\[H3C\]vlan 21

\[H3C-vlan21\]vlan 22

\[H3C -vlan22\]vlan 23

l          配置 VLAN 2

\[H3C -vlan23\]vlan 2

l          在VLAN视图下，设置super-VLAN

\[H3C -vlan2\]supervlan

l          接下来，在VLAN视图下，设置sub-VLAN与super-VLAN的映射关系

\[H3C -vlan2\]subvlan 21 22 23                      （列表形式添加）

或

\[H3C -vlan2\]subvlan 21 to 23                      （范围形式添加）

l          查看super-VLAN状态

\[H3C\]display supervlan

 Supervlan ID :  2

 Subvlan ID :  21-23



 VLAN ID: 2

 VLAN Type: static

 It is a Super VLAN.

 ARP proxy disabled.

 Route Interface: not configured

 Description: VLAN 0002

 Tagged   Ports: none

 Untagged Ports: none



 VLAN ID: 21

 VLAN Type: static

 It is a Sub-vlan.

 Route Interface: not configured

 Description: VLAN 0021

 Tagged   Ports:

             Ethernet0/23

 Untagged Ports:

             Ethernet0/21



 VLAN ID: 22

 VLAN Type: static

 It is a Sub-vlan.

 Route Interface: not configured

 Description: VLAN 0022

 Tagged   Ports:

             Ethernet0/23

 Untagged Ports:

             Ethernet0/22



 VLAN ID: 23

 VLAN Type: static

 It is a Sub-vlan.

 Route Interface: not configured

 Description: VLAN 0023

 Tagged   Ports:

             Ethernet0/23

 Untagged Ports: none

l          配置Vlan-interface2后，ARP Proxy自动生效（在我司交换机上对SuperVLAN而言是默认打开，且不可关闭）

 \[H3C\]interface Vlan-interface 2

 \[Quidway-Vlan-interface2\]display supervlan

 Supervlan ID :  2

 Subvlan ID :  21-23



 VLAN ID: 2

 VLAN Type: static

 It is a Super VLAN.

 ARP proxy enabled.

……

[6               SuperVLAN](http://www.h3c.com/cn/d_200911/655248_97665_0.htm)典型组网应用

![](images/Pasted%20image%2020221202000245.png)

[7               优缺点](http://www.h3c.com/cn/d_200911/655248_97665_0.htm)

主要优点：

节约大量的IP地址：子网号、广播地址、网关地址等；

编址灵活，扩展容易

主要缺点： 允许在不同的广播域间任意分配IP地址，带来安全隐患；

建议：严格地址范围分配

8               相关问题讨论

8.1               Super VLAN与trunk端口的相互影响

前面说过，Super VLAN中是不存在物理端口的，这种限制是强制的，表现在：

如果先配好了Super VLAN，在配置trunk端口时，trunk的VLAN allowed项里就自动滤除了Super VLAN。即使trunk允许所有VLAN通过，VLAN passing项里也永远不会有Super VLAN。

如果先配好了trunk端口，并permit vlan all，则在此设备上将无法配置Super VLAN。本质原因是有物理端口的VLAN都不能被配置为Super VLAN。而permit vlan all的trunk端口是所有VLAN的tagged端口，当然任何VLAN都不能被配置为Super VLAN。

8.2               Super VLAN在GVRP中的表现

因为trunk口将super-VLAN滤除的缘故，super-VLAN的信息不会被发送到其他设备上，也就不会被其他的设备学到。



