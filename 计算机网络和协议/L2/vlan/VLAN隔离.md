已剪辑自: [https://forum.huawei.com/enterprise/zh/thread/580895560787968001](https://forum.huawei.com/enterprise/zh/thread/580895560787968001)

小编在上期为大家介绍了多种VLAN互通场景和方式，之后读者又有了新的问题：如何对同一VLAN下用户进行隔离呢，如果实现部分VLAN互通、部分VLAN隔离呢，如何针对某个用户、或某个网段用户进行隔离呢，下面就听小编一一道来 。

### 场景一、同一VLAN下用户进行隔离

如果不希望同一VLAN下某些用户进行互通，可以通过配置端口隔离实现。

下面小编通过一个实验来详细讲解如何实现端口隔离：

如下图所示，三台PC属于同一VLAN、同一网段，配置完成后，三台PC都可以互访，现在要求PC1和PC2之间不能互通，PC1和PC3能够互通、PC2和PC3能够互通。

![](images/Pasted%20image%2020221201235720.png)



配置过程如下：

![](images/Pasted%20image%2020221201235724.png)

验证配置结果：

PC1 ping PC2，无法ping通。

![](images/Pasted%20image%2020221201235734.png)

PC1 ping PC3，可以ping通。

![](images/Pasted%20image%2020221201235753.png)



OK！配置成功。



### 场景二、部分VLAN间可以互通、部分VLAN间隔离、VLAN内用户隔离――通过MUX VLAN实现

MUX VLAN只适用于二层网络中、对同一网段的用户进行互通和隔离。

MUX VLAN分为Principal VLAN和Subordinate VLAN，Subordinate VLAN又分为Separate VLAN和Group VLAN。

- Principal VLAN可以和所有VLAN互通。

- Group VLAN可以和Principal VLAN和本VLAN内互通。

- Separate VLAN只能和Principal VLAN互通，本VLAN内不能互通。


下面小编通过一个例子详解介绍通过MUX VLAN进行VLAN互通和隔离。如下图所示，由于不同的PC属于不同的部门，需要对用户进行互通和隔离。

l 要求所有PC都可以访问服务器（Server），即VLAN20和VLAN30可以访问VLAN10。

l PC1和PC2之间可以互访，和PC3、PC4不能互访，即VLAN20和VLAN30不能互访。

l PC3和PC4之间隔离，不能互访，即VLAN30内用户不能互访。

![](images/Pasted%20image%2020221201235803.png)



详细配置步骤如下：

![](images/Pasted%20image%2020221201235811.png)



OK，配置完成，让我们来验证一下配置结果吧。

所有PC都可以和Principal VLAN中的Server互通。

    PC1 ping Server

![](images/Pasted%20image%2020221201235823.png)

PC3 ping Server

![](images/Pasted%20image%2020221201235829.png)



所有Group VLAN中的PC可以互通，但是不可以和Separate VLAN中的PC互通。

PC1 ping PC2

![](images/Pasted%20image%2020221201235834.png)


PC1 ping PC3

![](images/Pasted%20image%2020221201235840.png)



Separate VLAN的PC隔离，不能互通。

      PC3 ping PC4

![](images/Pasted%20image%2020221201235843.png)



### 场景三：不同VLAN互通后，如何对部分VLAN或部分用户进行隔离――通过流策略实现

流策略技术原理，小编就不做过多介绍了，想了解详细信息，请参见QoS手册，通过下面的例子介绍如何实现VLAN隔离。



![](images/Pasted%20image%2020221201235856.png)

如上图所示，FTP Server属于192.168.1.0/24网段，PC1和PC2属于192.168.10.0/24，PC3和PC4属于192.168.20.0/24网段。

假设所有VLAN已经通过VLANIF接口实现VLAN间互通，详细配置方法不做赘述，请参加上期《交换机在江湖之初窥门径\-VLAN通信篇》。

将FTP Server的网关设置为192.168.1.1，将PC1和PC2的网关设置为192.168.10.1，将PC2和PC4的网关设置为192.168.20.1。VLAN10、VLAN20和VLAN100内所有设备能够互通，例如：在PC1上ping FTP Server，能够ping通。

现在要求PC3和PC4所在网段设备能够访问FTP Server，PC1和PC2所在网段设备禁止访问FTP Server。

在SwitchA上配置ACL和流策略进行隔离，192.168.10.0/24网段的设备禁止访问FTP Server，详细配置步骤如下：

![](images/Pasted%20image%2020221201235902.png)



配置完之后，我们再来验证一下PC1是否能够ping通FTP Server呢？

![](images/Pasted%20image%2020221201235912.png)


但是PC3任然可以ping同FTP Server。

![](images/Pasted%20image%2020221201235931.png)


OK，配置成功，大功告成。


PS：通过ACL和流策略对VLAN进行隔离，是一种非常灵活的方式，也可以对单个用户进行隔离，ACL只要匹配单个用户的IP即可。

