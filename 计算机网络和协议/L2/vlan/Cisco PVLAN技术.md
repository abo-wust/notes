
VLAN的局限性
随着网络的迅速发展，用户对于网络数据通信的安全性提出了更高的要求，诸如防范黑客攻击、控制病毒传 播等，都要求保证网络用户通信的相对安全性；传统的解决方法是给每个客户分配一个VLAN和相关的IP子网，通过使用VLAN，每个客户被从第2层隔离 开，可以防止任何恶意的行为和Ethernet的信息探听。然而，这种分配每个客户单一VLAN和IP子网的模型造成了巨大的可扩展方面的局限。这些局限 主要有下述几方面。
（1）VLAN的限制：交换机固有的VLAN数目的限制；
（2）复杂的STP：对于每个VLAN，每个相关的Spanning Tree的拓扑都需要管理；
（3）IP地址的紧缺：IP子网的划分势必造成一些IP地址的浪费；
（4）路由的限制：每个子网都需要相应的默认网关的配置。

PVLAN技术
现在有了一种新的VLAN机制，所有服务器在同一个子网中，但服务器只能与自己的默认网关通信。这一新的VLAN特性就是专用 VLAN（Private VLAN）。在Private VLAN的概念中，交换机端口有三种类型：Isolated port，Community port， Promisc-uous port；它们分别对应不同的VLAN类型：Isolated port属于Isolated PVLAN，Community port属于Community PVLAN，而代表一个Private VLAN整体的是Primary VLAN，前面两类VLAN需要和它绑定在一起，同时它还包括Promiscuous port.在Isolated PVLAN中，Isolated port只能和Promiscuous port通信，彼此不能交换流量；在Community PVLAN中，Community port不仅可以和Promiscuous port通信，而且彼此也可以交换流量。Promiscuous port 与路由器或第3层交换机接口相连，它收到的流量可以发往Isolated port和Communityport。PVLAN的应用对于保证接入网络的数据通信的安全性是非常有效的，用户只需与自己的默认网关连接，一个PVLAN不需要多个VLAN和IP子 网就提供了具备第2层数据通信安全性的连接，所有的用户都接入PVLAN，从而实现了所有用户与默认网关的连接，而与PVLAN内的其他用户没有任何访 问。PVLAN功能可以保证同一个VLAN中的各个端口相互之间不能通信，但可以穿过Trunk端口。这样即使同一VLAN中的用户，相互之间也不会受到 广播的影响。

PVLAN的作用与要点
PVLAN（Private VLAN,私有VLAN）是指一个VLAN中，各成员虽然处于同一个子网中，但是各自只能与自己的默认网关通信，相互之间不能通信，相当于一个VLAN内部实现了VLAN本身所具有的隔离特性。PVLAN的应用对于保证接入网络的数据通信的安全性是很有效的，所有的用户都接入PVLAN,从而实现了所有用户与默认网关的连接，而与PVLAN内的其他用户没有任何访问。PVLAN功能可以保证同一个VLAN中各个端口之间不能通信，但可以穿透Trunk端口，这样即使同一个VLAN的用户，相互之间也不会受到广播的影响。

PVLAN中有3种类型的端口
**混杂端口（Promiscuous Port）**：一个混杂类型的端口助于主VLAN,可以与所有的接口通信，包括与主VLAN关联的从VLAN中的共有和隔离VLAN中的端口。
**隔离端口（Isolated Port)**：一个隔离端口是一个属于隔离VLAN中的主机端口。这个端口与一个PVLAN域中的其他端口完全的二层隔离，除了混杂端口外，但是PVLAN会阻止所有从混杂端口到达隔离端口的通信，从隔离端口接收到的通信仅可以转发到混杂端口上。
**共有端口（Community Port）**：一个共有端口是一个属于公共VLAN的主机端口。共有端口可以与同一个公共VLAN中的其他端口通信。这些端口与所有其他公共VLAN上的端口，以及同一个PVLAN中的其他隔离端口之间都是二层隔离的。
注：因为中继可以支持VLAN在隔离、共有和混杂端口承载通信，所以隔离和共有端口通信可能通过中继端口进入或者流出交换机。

与三类端口对应，PLAN也有三种不同类型的VLAN：主VLAN（Primary VLAN）、隔离VLAN（Isolated VLAN）和公共VLAN（Community VLAN） ，后两种都属于从VLAN（Secondary VLAn）。

主vlan和从vlan具有以下特征：
**主vlan**：主VLAN承载从混在端口到隔离和共有主机端口的以及到其他混杂端口的单向通信。
**隔离VLAN**：一个PVLAN域中仅有一个隔离VLAN。一个隔离VLAN是一个承载从主机到混在端口和网关之间单向通信的从VLAN。
**公共VLAN**： 一个公共VLAN是一个承载从共有端口到混在端口、网关和其他在一个公共VLAN中的主机端口之间单向通信的从VLAN。

一个混杂端口仅可以服务于一个主VLAN、一个隔离VLAN和多个公共VLAN之中。三层网关是典型的通过混杂端口与交换机直接连接的情形。一个混杂端口可以连接大范围内的设备作为PVLAN的访问点。


Catalyst3560， 45、65系列支持
Cisco-SW(config)#vlan 100
Cisco-SW(config-vlan)#private-vlan primary

!设置主VLAN 100
Cisco-SW(config)#vlan 200
Cisco-SW(config-vlan)#private-vlan community

!设置团体VLAN 200
Cisco-SW(config)#vlan 300
Cisco-SW(config-vlan)#private-vlan isolated
Cisco-SW(config-vlan)#private-vlan association 200,300

!将辅助VLAN关联到主VLAN
Cisco-SW(config)#interface vlan 100
Cisco-SW(config-if)#private-vlan mapping add 200,300

!将辅助VLAN映射到主VLAN接口，允许pVLAN入口流量的三层交换
Cisco-SW(config)# interface fastethernet 0/2
Cisco-SW(config-if)#switchport mode private-vlan host
Cisco-SW(config-if)#switchport private-vlan host-association 100 200

!2号口划入团体VLAN 200
Cisco-SW(config)# interface fastethernet 0/3
Cisco-SW(config-if)#switchport mode private-vlan host
Cisco-SW(config-if)#switchport private-vlan host-association 100 300
!3号口划入隔离VLAN 300

Cisco-SW(config)# interface fastethernet 0/1
Cisco-SW(config-if)#switchport mode private-vlan promiscuous
Cisco-SW(config-if)#switchport private-vlan mapping 100 add 200-300
!1号口杂合模式


关于端口隔离(Cisco)
Cisco-SW(config)# interface gigabitethernet1/0/2
Cisco-SW(config-if)# switchport protected

PVLAN通常用于企业内部网，用来防止连接到某些接 口或接口组的网络设备之间的相互通信，但却允许与默认网关进行通信。 尽管各设备处于不同的pVLAN中，它们可以使用相同的IP子网。 pVLAN 的3种port： isolated，community ，promiscuous。 PVLAN 的3种子VLAN。

![](images/Pasted%20image%2020231027105502.png)


PVLAN 的3种port：isolated，community ，promiscuous。
PVLAN 的3种子VLAN ：primary VLAN ， isolated VLAN（second vlan）， community VLAN（second vlan）。
PVLAN通 信范围：primary VLAN：可以和所有他所关联的isolated VLAN，community VLAN通信。
community VLAN：可以同那些处于相同community VLAN内的community port通信，也可以与PVLAN中的promiscuous端口通信。（每个PVLAN可以有多个community VLAN）
isolated VLAN：不可以和处于相同isolated VLAN内的其它isolated port通信，只可以与promisuous端口通信。（每个PVLAN中只能有 一个isolated VLAN）

PVLAN限制：
PVLAN要求使用VTPv1或VTPv2。
PVLAN必须工作于Transparent模式。
禁止将第 3层VLAN接口配置为辅助VLAN。
EtherChannel或SPAN目标端口不支持pVLAN。

PVLAN使用方案：
1. 把需要2层隔离的主机放到同一个isolated VLAN，或者不同的community VLAN。
2. 把需要2层通信的主机放到同一个community VLAN。
3. 公 共的服务器或者上联端口放到primary VLAN，和本primaryVLAN和所有secondaryVLAN里的主机是2层连通的。
4. 在primary VLAN上来看，所有的主机在同一个primary VLAN里，在secondary VLAN里的主机互相访问的时候，才和private VLAN的配置有关。
5. 网关可以是primary VLAN上配一个3层地址或者在primary VLAN上连一个路由器。所有主机在3层都是在primary VLAN的这个网段里，所以还是可以通信的，只是2层隔离了，因为3层接口不转发广播，广播也就被隔离了，网上邻居之类基于广播的应用就互相看不到了。











