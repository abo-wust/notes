已剪辑自: [https://cshihong.github.io/2017/11/14/MUX-VLAN/](https://cshihong.github.io/2017/11/14/MUX-VLAN/)

产生背景：

MUX VLAN（Multiplex VLAN）提供了一种通过VLAN进行网络资源控制的机制。

例如，在企业网络中，企业员工和企业客户可以访问企业的服务器。对于企业来说，希望企业内部员工之间可以互相交流，而企业客户之间是隔离的，不能够互相访问。

为了实现所有用户都可访问企业服务器，可通过配置VLAN间通信实现。如果企业规模很大，拥有大量的用户，那么就要为不能互相访问的用户都分配VLAN，这不但需要耗费大量的VLAN ID，还增加了网络管理者的工作量同时也增加了维护量。

通过MUX VLAN提供的二层流量隔离的机制可以实现企业内部员工之间互相交流，而企业客户之间是隔离的。

基本概念：

MUX VLAN分为主VLAN和从VLAN，从VLAN又分为隔离型从VLAN和互通型从VLAN。

- 主VLAN（Principal VLAN）：Principal port可以和MUX VLAN内的所有接口进行通信。
- 从VLAN（Subordinate VLAN）：
    - 隔离型从VLAN（Separate VLAN）：Separate port只能和Principal port进行通信，和其他类型的接口实现完全隔离。
    每个隔离型从VLAN必须绑定一个主VLAN。
    
    - 互通型从VLAN（Group VLAN）：Group port可以和Principal port进行通信，在同一组内的接口也可互相通信，但不能和其他组接口或Separate port通信。
    每个互通型从VLAN必须绑定一个主VLAN。

通信原理：（[support.huawei.com/enterprise/zh/doc/EDOC1000178154/bd237d28](https://support.huawei.com/enterprise/zh/doc/EDOC1000178154/bd237d28)）

如图7-1所示，根据MUX VLAN特性，企业可以用Principal port连接企业服务器，Separate port连接企业客户，Group port连接企业员工。这样就能够实现企业客户、企业员工都能够访问企业服务器，而企业员工内部可以通信、企业客户间不能通信、企业客户和企业员工之间不能互访的目的。

**图7-1**  MUX VLAN应用场景图（接入层）

![](images/Pasted%20image%2020221201235528.png)

对于汇聚层设备，可以为Principal VLAN创建VLANIF接口，VLANIF接口的IP地址可以作为Host或Server的网关地址。如==图7-2==所示，在汇聚设备Switch1上配置MUX VLAN，可以灵活实现接入流量的隔离或者互通。

**图7-2**  MUX VLAN应用组网（汇聚层）

![](images/Pasted%20image%2020221201235540.png)





配置MUX VLAN:

配置注意事项：

- 如果指定VLAN已经用于主VLAN或从VLAN，那么该VLAN不能再用于创建VLANIF接口，或者在VLAN Mapping、VLAN Stacking、Super-VLAN、Sub-VLAN的配置中使用。
- 禁止接口MAC地址学习功能或限制接口MAC地址学习数量会影响MUX VLAN功能的正常使用。
- 不能在同一接口上配置MUX VLAN和接口安全功能。
- 不能在同一接口上配置MUX VLAN和MAC认证功能。
- 不能在同一接口上配置MUX VLAN和802.1x认证功能。
- 当同时配置DHCP Snooping和MUX VLAN时，如果DHCP Server在MUX VLAN的从VLAN侧，而DHCP Client在主VLAN侧，则会导致DHCP Client不能正常获取IP地址。因此请将DHCP Server配置在主VLAN侧。
- 接口使能MUX VLAN功能后，该接口不可再配置VLAN Mapping、VLAN Stacking。

配置MUX VLAN：

拓扑：

PC1,2为组VLAN，组内成员可以实现互通，同时可以与主VLAN互通，而与隔离VLAN不能互通。

PC3为主VLAN成员，可以与所用组VLAN和隔离VLAN内所有成员通信。

PC6,7属于隔离VLAN组，隔离VLAN内成员不能互相通信，只能与主VLAN内成员通信。

![](images/Pasted%20image%2020221201235549.png)

图：MUX VLAN拓扑图

配置命令：
```
<SW>dis current-configuration  
#  
sysname SW  
#  
vlan batch 10 20 30  
#  
vlan 10  
 mux-vlan //配置vlan10位 MUX valn  
 subordinate separate 30 //配置隔离型从VLAN  
 subordinate group 20 //配置互通型从VLAN  
#  
interface GigabitEthernet0/0/1  
 port link-type access  
 port default vlan 20  
 port mux-vlan enable //使能接口的MUX VLAN功能。  
#  
interface GigabitEthernet0/0/2  
 port link-type access  
 port default vlan 20  
 port mux-vlan enable  
#  
interface GigabitEthernet0/0/3  
 port link-type access  
 port default vlan 30  
 port mux-vlan enable  
#  
interface GigabitEthernet0/0/4  
 port link-type access  
 port default vlan 10  
 port mux-vlan enable  
#  
interface GigabitEthernet0/0/5  
 port link-type access  
 port default vlan 30  
 port mux-vlan enable  
#
```


配置跨设备MUX VLAN示列：

在企业网络中，企业希望某些部门之间的员工是互相隔离的，某些部门之间的员工是可以互相访问的，并且所有部门的员工都可以访问公司的某些服务器。

为了解决上述问题，可在设备上部署MUX VLAN功能。将企业服务器划分在主VLAN内，需要互相访问的部门员工划分在互通型从VLAN内，需要互相隔离的部门员工划分在隔离型从VLAN内，即可解决上述问题，且不会耗费大量的VLAN ID。

拓扑需求：

如下图所示，PC1,2和PC5,6是连接在不同交换机上的主机，属于同一个VLAN3，VLAN3为组VALN。

PC3,4 和PC 7,8是连接在不同交换机上的主机，属于同一个VLAN4，VLAN4为隔离VALN。

PC9为交换机SW1上的一台服务器A，PC10为交换机SW2上的一台服务器B。PC9，10同属于VLAN2，VLAN2为主VLAN。

![](images/Pasted%20image%2020221201235628.png)

图： 跨设备MUX VLNA通信

配置命令行：

SW1和SW2配置完全相同。
```
<SW1>dis current-configuration  
#  
sysname SW1  
#  
vlan batch 2 to 4  
#  
vlan 2  
 mux-vlan  
 subordinate separate 4  
 subordinate group 3  
#  
interface GigabitEthernet0/0/1  
 port link-type access  
 port default vlan 3  
 port mux-vlan enable  
#  
interface GigabitEthernet0/0/2  
 port link-type access  
 port default vlan 3  
 port mux-vlan enable  
#  
interface GigabitEthernet0/0/3  
 port link-type access  
 port default vlan 4  
 port mux-vlan enable  
#  
interface GigabitEthernet0/0/4  
 port link-type access  
 port default vlan 4  
 port mux-vlan enable  
#  
interface GigabitEthernet0/0/5  
 port link-type access  
 port default vlan 2  
 port mux-vlan enable  
#  
interface GigabitEthernet0/0/6  
 port link-type trunk  
 port trunk allow-pass vlan 2 to 4  
#
```


参考资料：华为HedEx文档

