已剪辑自: [http://www.h3c.com/cn/d\_200911/922199\_30005\_0.htm](http://www.h3c.com/cn/d_200911/922199_30005_0.htm)

Isolated-User VLAN 产生的背景

以太网的快速发展对传统VLAN网络提出了更高的要求，从安全性考虑出发，运营商一般要求接入用户互相隔离。VLAN是天然的隔离手段，很自然的一个想法是每个用户一个VLAN。但是，对于运营商来说，4094个VLAN远远不够，而且VLAN是需要三层终结的，为每个只包含一个用户的VLAN进行三层终结，将耗费大量的IP地址和部署成本。

为了解决上述问题，Isolated-User VLAN（Cisco 称PVLAN--Private VLAN） 技术应运而生。

1         Isolated-User VLAN 的实现

Isolated-User VLAN 采用了二层的VLAN 结构，第一层为Primary VLAN ，第二层为Secondary VLAN。

如图1和表1所示，Isolated-User VLAN 将多个Secondary VLAN （VLAN 12、13、14）映射到一个Primary VLAN（VLAN 11）。第一层的Primary VLAN 用于上行，对于上层设备来说，只需识别下层交换机的Primary VLAN而不必关心Primary VLAN 中包含的Secondary VLAN，这样简化了网络配置，节省了VLAN资源。第二层的Secondary VLAN 用于接入用户，不同的Secondary VLAN间通过传统的VLAN 技术实现二层隔离。

H3C交换机产品创造性地利用Hybrid类型端口的灵活性以及VLAN间MAC地址的同步技术，实现了Isolated-User VLAN功能。

![](images/Pasted%20image%2020221202000336.png)

|VLAN|Role|Port|
|----|----|----|
|11|Primary VLAN|1 2 3 4|
|12|Secondary VLAN|1 2|
|13|Secondary VLAN|1 3|
|14|Secondary VLAN|1 4|



2         Isolated-User VLAN 的通信

2.1        Isolated-User VLAN 用户访问外部网络

以图2为例，对Isolated-User VLAN 用户（PC 1）访问外部网络（LSW1的网关）的过程进行阐述。



![](images/Pasted%20image%2020221202000344.png)

  图2 Isolated-User VLAN 用户访问外部网络示意图

|VLAN|Role|Port|
|----|----|----|
|11|Primary VLAN|1 2 3|
|12|Secondary VLAN|1 2|
|13|Secondary VLAN|1 3|



这里作如下假设：PC1的MAC地址为MAC1，PC2的MAC地址为MAC2，网关的MAC地址为MAC3。

刚开始PC1发送ARP请求报文请求网关192.168.1.1的MAC地址，该广播报文从VLAN 12进入到LanSwitch 2后，LanSwitch 2根据源地址学习规则学习到PC1在VLAN 12的记录，同时复制一份到Primary VLAN（VLAN 11）进行记录：

|MAC|VLAN|Port|
|---|----|----|
|MAC1|12|2|
|MAC1|11|2|



然后该报文从VLAN 12的端口广播出去，以untag 方式送到了LanSwitch 1。LanSwitch 1的网关收到这个ARP请求报文后回一个单播ARP应答报文，这个报文从LanSwitch 2的端口1进来，于是LanSwitch 2把网关的MAC地址MAC3学习在端口1上，同时复制到所有的Secondary VLAN（VLAN 12、13）进行记录：

|MAC|VLAN|Port|
|---|----|----|
|MAC1|12|2|
|MAC1|11|2|
|MAC2|11|1|
|MAC2|12|1|
|MAC2|13|1|



最后LanSwitch 2根据端口1的PVID=11，将报文在VLAN 11中转发，同时根据MAC地址表，报文从端口2出来送到PC 1。

后续的报文转发过程都类似上述的过程。

2.2        Isolated-User VLAN 用户之间互访

同样以图2为例对Isolated-User VLAN 用户（PC1和PC2）之间的互访过程进行阐述。

刚开始PC1发送ARP广播（请求PC2），ARP的转发和MAC地址表的学习和3.1 Isolated-User VLAN 用户访问外部网络小节中提到的相同。LanSwitch 1的网关收到这个ARP报文后，因为网关使能了ARP代理，其会在VLAN 11广播ARP请求（请求PC2）；当PC2收到之后，将自己的MAC地址回复给网关；网关学习PC2的ARP之后，以网关的MAC为源MAC回复ARP应答给PC1，欺骗PC1说PC2的MAC为自己网关的MAC（这里值得指出的是，对于V5设备来说，收到这个ARP报文后，会根据其中目的IP查询路由表，如果该路由条目存在，网关立刻以自己的MAC地址为源MAC回复给PC1,然后才会去执行请求解析PC2的ARP请求）。后续的报文转发都会经过LanSwitch 1的网关进行转发。PC2访问PC1同上述过程。

3         Isolated-User VLAN 配置

以95为例，根据图2和表2 Isolated-User VLAN 关系示例进行配置：



\# 配置Isolate-user-VLAN 。
```
[H3C]vlan 11

[H3C-vlan11]isolate-user-vlan enable

[H3C-vlan11]int g5/1/1

[H3C-vlan11]port Gigabit Ethernet 5/1/1
```


\# 配置Secondary VLAN。
```
[H3C]vlan 12

[H3C-vlan12]port GigabitEthernet 5/1/2

[H3C-vlan12]vlan 13

[H3C-vlan12]port GigabitEthernet 5/1/3
```


\# 配置Isolate-user-VLAN 和Secondary VLAN间的映射关系。
```
[H3C]isolate-user-vlan 11 secondary 12 to 13
```


\# 在完成上述配置后，在所有视图下执行display命令可以显示配置后的Isolate-user-VLAN的运行情况
```
[H3C]display isolate-user-vlan

 Isolate-user-VLAN VLAN ID : 11

 Secondary VLAN ID : 12-13



 VLAN ID: 11

 VLAN Type: static

 Isolate-user-VLAN type: isolate-user-VLAN

 Route Interface: configured

 IP Address: 192.168.1.1

 Subnet Mask: 255.255.255.0

 Description: VLAN 0011

 Tagged   Ports: none

 Untagged Ports:

    GigabitEthernet5/1/1    GigabitEthernet5/1/2    GigabitEthernet5/1/3



 VLAN ID: 12

 VLAN Type: static

 Isolate-user-VLAN type: secondary

 Route Interface: configured

 Description: VLAN 0012

 Tagged   Ports: none

 Untagged Ports:

    GigabitEthernet5/1/1    GigabitEthernet5/1/2



 VLAN ID: 13

 VLAN Type: static

 Isolate-user-VLAN type: secondary

 Route Interface: not configured

 Description: VLAN 0013

 Tagged   Ports: none

 Untagged Ports:

    GigabitEthernet5/1/1    GigabitEthernet5/1/3

```


\# 同时查看端口的配置，可以发现配置Isolate-user-VLAN后，端口自动变为Hybrid属性
```
[H3C]dis current-configuration interface g5/1/1

interface GigabitEthernet5/1/1

 port link-type hybrid

 undo port hybrid vlan 1

 port hybrid vlan 11 to 13 untagged

 port hybrid pvid vlan 11

return


[H3C]dis current-configuration interface g5/1/2

interface GigabitEthernet5/1/2

 port link-type hybrid

 undo port hybrid vlan 1

 port hybrid vlan 11 to 12 untagged

 port hybrid pvid vlan 12

return


[H3C]dis current-configuration interface g5/1/3

interface GigabitEthernet5/1/3

 port link-type hybrid

 undo port hybrid vlan 1

 port hybrid vlan 11 13 untagged

 port hybrid pvid vlan 13

return
```


4         Isolated-User VLAN 的优缺点

4.1        Isolated-User VLAN 的优点

Isolated-User VLAN 优点在于在节省VLAN资源的基础上实现了二层隔离。

4.2        Isolated-User VLAN 的缺点

Isolated-User VLAN 缺点主要集中在两个方面，首先二层用户的通信必须依靠代理ARP来完成，一定程度上增加上层设备的负担；其次部署Isolated-User VLAN设备的上行口必须untag上行，欠缺灵活性。

5         其他讨论

Cisco Private VLAN技术的实现和我司的类似，但是更为细致。Cisco将Secondary VLAN 又进行细分，分为community VLAN和isolated VLAN。对于community VLAN和我司普通的Secondary VLAN一致，同一个VLAN内两两可以相互通信。对于isolated VLAN 内部采用类似我司端口隔离的技术进行同一个VLAN内部两两隔离。

Cisco 的PVLAN技术结构层次如图3。



![](images/Pasted%20image%2020221202000405.png)

                     图3 Cisco PVLAN 示意图

