
**1、lldp简介**

**定义**

LLDP（Link Layer Discovery Protocol）是IEEE 802.1ab中定义的链路层发现协议。

LLDP是一种标准的二层发现方式，可以将本端设备的管理地址、设备标识、接口标识等信息组织起来，并发布给自己的邻居设备，邻居设备收到这些信息后将其以标准的管理信息库MIB（Management Information Base）的形式保存起来，以供网络管理系统查询及判断链路的通信状况。



**目的**

随着网络规模越来越大，网络设备种类繁多，并且各自的配置错综复杂，对网络管理能力的要求也越来越高。传统网络管理系统多数只能分析到三层网络拓扑结构，无法确定网络设备的详细拓扑信息、是否存在配置冲突等。因此需要有一个标准的二层信息交流协议。

LLDP提供了一种标准的链路层发现方式。通过LLDP获取的设备二层信息能够快速获取相连设备的拓扑状态；显示出客户端、交换机、路由器、应用服务器以及网络服务器之间的路径；检测设备间的配置冲突、查询网络失败的原因。企业网用户可以通过使用网管系统，对支持运行LLDP协议的设备进行链路状态监控，在网络发生故障的时候快速进行故障定位。



**2、工作原理**

LLDP可以将本地设备的信息组织起来并发布给自己的远端设备，本地设备将收到的远端设备信息以标准MIB的形式保存起来。工作原理如图所示。

![](images/Pasted%20image%2020221201234624.png)

*LLDP原理框图*

**LLDP基本实现原理为：**

1. ==LLDP模块通过LLDP代理与设备上物理拓扑MIB、实体MIB、接口MIB以及其他类型MIB的交互，来更新自己的LLDP本地系统MIB，以及本地设备自定义的LLDP扩展MIB。==
2. ==将本地设备信息封装成LLDP帧发送给远端设备。==
3. ==接收远端设备发过来的LLDP帧，更新自己的LLDP远端系统MIB，以及远端设备自定义的LLDP扩展MIB。==
4. ==通过LLDP代理收发LLDP帧，设备就很清楚地知道远端设备的信息，包括连接的是远端设备的哪个接口、远端设备的MAC地址等信息。==

==LLDP本地系统MIB用来保存本地设备信息。包括设备ID、接口ID、系统名称、系统描述、接口描述、网络管理地址等信息。==

==LLDP远端系统MIB用来保存远端设备信息。包括设备ID、接口ID、系统名称、系统描述、接口描述、网络管理地址等信息。==

**LLDP代理完成下列任务：**

- ==维护LLDP本地系统MIB和LLDP远端系统MIB。==
- ==在本地状态发生变化的情况下，提取LLDP本地系统MIB信息并向远端设备发送。在本地设备状态信息没有变化的情况下，按照一定的周期提取LLDP本地系统MIB信息向远端设备发送。==
- ==识别并处理收到的LLDP帧。==
- ==LLDP本地系统MIB或LLDP远端系统MIB的状态发生变化的情况下，向网管发送LLDP告警。==



**3、LLDP报文**

封装有LLDP数据单元LLDPDU（LLDP Data Unit）的报文称为LLDP报文，其封装格式有两种：Ethernet II和SNAP（Subnetwork Access Protocol，子网访问协议）。

**3.1 Ethernet II格式封装的LLDP报文**

![](images/Pasted%20image%2020221201234631.png)

  *图 1 Ethernet II格式封装的LLDP报文*


如图 1所示，是以Ethernet II格式封装的LLDP报文，其中各字段的含义如下：

- Destination MAC address：目的MAC地址，为固定的组播MAC地址0x0180-C200-000E。
- Source MAC address：源MAC地址，为端口MAC地址或设备桥MAC地址（如果有端口地址则使用端口MAC地址，否则使用设备桥MAC地址）。
- Type：报文类型，为0x88CC。
- Data：数据，为LLDPDU。
- FCS：帧检验序列。



**3.2 SNAP格式封装的LLDP报文**

![](images/Pasted%20image%2020221201234636.png)

*图 2 SNAP格式封装的LLDP报文*

如图 2所示，是以SNAP格式封装的LLDP报文，其中各字段的含义如下：

Destination MAC address：目的MAC地址，为固定的组播MAC地址0x0180-C200-000E。

Source MAC address：源MAC地址，为端口MAC地址或设备桥MAC地址（如果有端口地址则使用端口MAC地址，否则使用设备桥MAC地址）。

Type：报文类型，为0xAAAA-0300-0000-88CC。

Data：数据，为LLDPDU。

FCS：帧检验序列。



**3.3 LLDPDU**

LLDPDU就是封装在LLDP报文中本地信息的数据单元。在组成LLDPDU之前，先将本地信息封装成TLV(Type/Length/Value)格式，再由若干个TLV组合成一个LLDPDU封装在LLDP报文的数据部分进行传送。

![](images/Pasted%20image%2020221201234641.png)

*图3 LLDPDU*

如图 3所示，每个LLDPDU共可携带28种TLV，其中深蓝色的Chasis ID TLV、Port ID TLV、TTL TLV和End TLV这四种是必须携带的，其余的TLV则为可选携带。

当接口的状态发生变化（去使能LLDP、接口shutdown）时，接口会向邻居设备发送一个LLDP报文，其中**Time To Live TLV字段的Value值为0，这个报文称为shutdown报文**。shutdown报文不包含任何可选TLV。



**3.4 TLV**

**3.4.1 TLV结构**

TLV是组成LLDPDU的单元，每个TLV都代表一个信息。

TLV的结构如图4所示。



![](images/Pasted%20image%2020221201234651.png)

*图4 TLV结构*

各字段含义如下：

- TLV Type：TLV的类型，每个TLV的类型值不同，比如End of LLDPDU TLV的类型值为0，Chassis ID TLV的类型值为1等。
- TLV Length：TLV的长度，占9个bit。
- TLV Value：TLV的值，第一个字节指此TLV的子类型，剩余的字节为TLV真正的值。



**3.4.2 TLV类型**

LLDP可以封装的TLV类型包括基本TLV、802.1组织定义的TLV、802.3组织定义的TLV和媒体终端发现MED（Media Endpoint Discovery）TLV。基本TLV是一组对设备进行管理的基础TLV，802.1组织定义的TLV、802.3组织定义的TLV和MED TLV则是由标准组织或其他机构定义的TLV，用于增强对设备的管理功能，可根据实际需要选择是否在LLDPDU中发送。

- **基本TLV**

在基本TLV中，有4种类型的TLV对于实现LLDP功能是必选的，即必须在LLDPDU中发布。

|**TLV名称**|**说明**|**是否必须发布**|
|---------|------|----------|
|Chassis ID TLV|发送设备的桥MAC地址。|是|
|Port ID TLV|标识LLDPDU发送端的端口，内容为端口名称。|是|
|Time To Live TLV|本设备信息在邻居设备上的存活时间。|是|
|End of LLDPDU TLV|标志LLDPDU结束。|是|
|Port Description TLV|以太网端口的描述字符串。|否|
|System Name TLV|设备名称。|否|
|System Description TLV|系统描述。|否|
|System Capabilities TLV|系统的主要功能以及有哪些主要功能被使能。|否|
|Management Address TLV|供网管系统标识网络设备并进行管理的地址。管理地址可以明确地标识一台设备，有利于网络拓扑的绘制，便于网络管理。|否|



- **IEEE 802.1组织定义的TLV**

|**TLV名称**|**说明**|
|---------|------|
|Port VLAN ID TLV|端口VLAN ID。|
|Port And Protocol VLAN ID TLV|端口的协议VLAN ID。|
|VLAN Name TLV|端口VLAN名称。|
|Protocol Identity TLV|端口支持的协议类型。|



- **IEEE 802.3组织定义的TLV**

|**TLV名称**|**说明**|
|---------|------|
|EEE TLV|端口是否支持EEE（Energy Efficient Ethernet）功能。|
|Link Aggregation TLV|端口是否支持链路聚合以及是否已使能链路聚合。|
|MAC/PHY Configuration/Status TLV|端口的速率和双工状态、是否支持端口速率自动协商、是否已使能自动协商功能以及当前的速率和双工状态。|
|Maximum Frame Size TLV|端口支持的最大帧长度，取端口最大传输单元MTU（Max Transmission Unit）。|
|Power Via MDI TLV|端口的供电能力，比如是否支持PoE，是供电设备还是受电设备。|



- **MED TLV**

MED TLV为VoIP（Voice over IP）提供了许多高级的应用，包括基本配置、网络策略配置、地址信息以及目录管理等，满足了语音设备的不同生产厂商在成本有效、易部署性、易管理性等方面的要求，并解决了在以太网中部署语音设备的问题，为语音设备的生产者、销售者以及使用者提供便利。

  >**说明：**
  当交换机识别到其某一接口的LLDP邻居发送的LLDP报文中包含任一类型的MED TLV时，交换机会将该接口能发布的所有MED TLV均发布给该LLDP邻居。但该LLDP邻居可能仅支持交换机发布的部分MED TLV，此时会导致LLDP协商失败。可以通过执行**undo lldp tlv-enable med-tlv**命令配置该接口不发布其LLDP邻居不支持的MED TLV。例如，某终端不支持802.3af标准，即无法识别Extended Power-via-MDI TLV，则必须在连接该终端的接口下执行**undo lldp tlv-enable med-tlv** **power-over-ethernet**命令配置该接口不发布Extended Power-via-MDI TLV。

  


|**TLV名称**|**说明**|
|---------|------|
|LLDP-MED Capabilities TLV|当前设备的设备类型以及在LLDPDU中可封装的LLDP-MED TLV类型。|
|Inventory TLV|设备的制造厂商。|
|Location Identification TLV|位置标识信息，供其它设备发现设备的位置。|
|Network Policy TLV|Voice VLAN的VLAN ID、二层优先级以及DSCP值等。|
|Extended Power-via-MDI TLV|当前设备的供电能力。|
|Hardware Revision TLV|媒体终端ME（Media Endpoint）设备的硬件版本。|
|Firmware Revision TLV|ME设备的硬件版本。|
|Software Revision TLV|ME设备的软件版本。|
|Serial Number TLV|ME设备的序列号。|
|Model Name TLV|ME设备的Model Name。|
|Asset ID TLV|ME设备的资产标识符。|



**4、LLDP报文收发机制**

**4.1 LLDP的工作模式**

LLDP有以下四种工作模式：

- TxRx：既发送也接收LLDP报文。
- Tx：只发送不接收LLDP报文。
- Rx：只接收不发送LLDP报文。
- Disable：既不发送也不接收LLDP报文。

当端口的LLDP工作模式发生变化时，端口将对协议状态机进行初始化操作。为了避免端口工作模式频繁改变而导致端口不断执行初始化操作，可配置端口初始化延迟时间，当端口工作模式改变时延迟一段时间再执行初始化操作。



**4.2 LLDP报文的发送机制**

当端口工作在TxRx或Tx模式时，设备会周期性地向邻居设备发送LLDP报文。如果设备的本地配置发生变化则立即发送LLDP报文，以将本地信息的变化情况尽快通知给邻居设备。但为了防止本地信息的频繁变化而引起LLDP报文的大量发送，每发送一个LLDP报文后都需延迟一段时间后再继续发送下一个报文。

当设备的工作模式由Disable/Rx切换为TxRx/Tx，或者发现了新的邻居设备（即收到一个新的LLDP报文且本地尚未保存发送该报文设备的信息）时，该设备将自动启用快速发送机制，即将LLDP报文的发送周期缩短为1秒，并连续发送指定数量的LLDP报文后再恢复为正常的发送周期。



**4.3 LLDP报文的接收机制**

当端口工作在TxRx或Rx模式时，设备会对收到的LLDP报文及其携带的TLV进行有效性检查，通过检查后再将邻居信息保存到本地，并根据TTL（Time To Live，生存时间） TLV中TTL的值来设置邻居信息在本地设备上的老化时间，若该值为零，则立刻老化该邻居信息。









