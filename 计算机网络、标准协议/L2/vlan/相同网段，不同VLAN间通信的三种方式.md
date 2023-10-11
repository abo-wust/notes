
参考资料：

[https://forum.huawei.com/enterprise/zh/thread/580894324915322880](https://forum.huawei.com/enterprise/zh/thread/580894324915322880)



### 1、纯二层网络实现

  拓扑如下，PC1属于vlan 10，IP地址是1.1.1.1/16，PC2属于vlan 20，IP地址是1.1.2.2/16。

  两台交换机互连接口（都为GE0/0/3口）为trunk模式（或者access模式，只要LSW1和LSW2的GE0/0/3口收发报文都是untagged就可以），交换机LSW1的GE0/0/3口的pvid是vlan 10，交换机LSW2的GE0/0/3口pvid是vlan 20。

  ![](images/Pasted%20image%2020221201234839.png)




#### 通信原理：

1. PC1访问PC2（例如：ping 1.1.2.2），判断目标主机是否同网段，是
2. PC1发出针对PC2的ARP请求，ARP报文进入LSW1的GE0/0/1口，并给报文打上vlan 10的标签
3. LSW1做二层vlan的交换，ARP报文从GE0/0/3口发出，由于GE0/0/3口的pvid是vlan 10，所以发出的ARP报文不带tag
4. LSW2的GE0/0/3口收到untagged的ARP报文，打上GE0/0/3口的pvid，也就是vlan 20
5. LSW2做二层vlan的交换，ARP报文从GE0/0/1口发出，由于GE0/0/1口的pvid是vlan 20，所以发出的ARP报文不带tag
6. PC2收到ARP报文后，向PC1发送ARP响应报文，经过上述流程的反向处理，PC1收到PC2的ARP响应报文建立ARP表项
7. 根据ARP表项封装数据报文，实现双方的通信


#### 配置过程：

```
LSW1：

Interface GigabitEthernet0/0/1
port link-type access
Port default vlan 10


Interface GigabitEthernet0/0/3
port link-type trunk
port trunk pvid vlan 10
port trunk allow-pass vlan 10


LSW2:

Interface GigabitEthernet0/0/1
port link-type access
Port default vlan 20


Interface GigabitEthernet0/0/3
port link-type trunk
port trunk pvid vlan 20
port trunk allow-pass vlan 20
```

  


### 2、代理ARP实现

拓扑如下，PC1属于vlan 10，IP地址是1.1.1.1/16，PC2属于vlan 20，IP地址是1.1.2.2/16。

![](images/Pasted%20image%2020221201234845.png)

在交换机LSW1上配置vlanif接口，两个vlanif接口的IP地址网段不同。

然后在vlanif接口使能代理ARP，即可实现PC1和PC2之间的通信。


#### 通信原理：

1. PC1访问PC2（例如：ping 1.1.2.2），判断目标主机是否同网段，是
2. PC1发出针对PC2的ARP请求，因为两台PC属于不同vlan，所以交换机LSW1的GE0/0/1收到PC1发出的ARP请求报文后，将报文送到接口vlanif 10
3. LSW1的vlanif 10接口开启了代理ARP功能，所以vlanif 10会响应PC1的ARP请求，ARP响应报文封装的mac地址实际是vlanif 10接口的mac
4. PC1收到ARP响应后，建立ARP表项，然后就可以给正常的数据报文封装目的mac（也就是vlanif 10接口的mac）
5. LSW1的接口GE0/0/1收到数据报文后，查找路由表，根据路由表将数据报文从vlanif 20接口发出
6. PC2收到PC1发出的正常数据报文，PC2发给PC1的过程类似，这样就完成了两台PC间的通信。


#### 配置过程：

```
LSW1：

interface GigabitEthernet0/0/1
port link-type access
port default vlan 10


interface GigabitEthernet0/0/2
port link-type access
port default vlan 20


interface vlanif 10
ip address 1.1.1.254 255.255.255.0
arp-proxy enable


interface vlanif 20
ip address 1.1.2.254 255.255.255.0
arp-proxy enable
```


### 3、vxlan实现

  拓扑如下图

  ![](images/Pasted%20image%2020221201234851.png)

  vxlan技术的本质是在三层网络上构建一个逻辑的二层网络。PC1和PC2分别属于vlan 10和vlan 20，把PC1和PC2关联到一个相同的vxlan 广播域BD，这里使用BD 5。

  

#### 通信原理：

1. PC1访问PC2（例如：ping 1.1.2.2），判断目标主机是否同网段，是
2. PC1发出针对PC2的ARP请求，交换机LSW1的GE0/0/1口收到ARP报文后打上vlan 10的tag，然后经二层转发从GE0/0/2口发出带vlan 10的ARP请求报文
3. 交换机CE1的GE1/0/0口收到带vlan 10的报文，根据vlan到BD的映射配置，将vlan 10的报文映射到BD 5
4. CE1根据vxlan的隧道配置，通过VTEP1将报文送到CE2上的VTEP2
5. CE2的VTEP2收到报文后，根据BD到vlan的映射，将BD 5的报文映射到vlan 20，经二层转发从CE2的GE1/0/0口发出
6. LSW2的GE0/0/2口收到vlan 20的ARP请求报文，二层转发到PC2，PC2响应该ARP请求报文，实现两台不同vlan的通信。

  

#### 配置过程：

vlan和接口的基本配置参考前面两种方式的配置，这里主要看vxlan的配置。

1. 实现两个VTEP的连通性（VTEP1的loopback地址为172.16.1.1，VTEP2的loopback地址为172.16.1.2），VTEP1和VTEP2可以互相ping通对方。
2. 配置vxlan交换机VTEP1，重点是将vlan 10的数据包关联到BD 5，其中BD 5的VNI也是5
```
interface GE1/0/0.1 mode l2

encapsulation dot1q vid 10

bridge-domain 5
```
说明：GE1/0/0收到的vlan 10报文关联到BD 5

```
bridge-domain 5

vxlan vni 5
```
说明：BD 5分配的VNI是5

```
interface nve1

source 172.16.1.1

vni 5 head-end-peer-list 172.16.1.2
```
说明：将vni 5的vxlan报文转发到172.16.1.2，也就是VTEP2

3. 配置vxlan交换机VTEP2，思路和VTEP1类似，只是将BD 5和vlan 20关联
```
interface GE1/0/0.1 mode l2

encapsulation dot1q vid 20

bridge-domain 5


bridge-domain 5

vxlan vni 5


interface nve1

source 172.16.1.2

vni 5 head-end-peer-list 172.16.1.1
```



