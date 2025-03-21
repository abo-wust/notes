
![](4b6a328db4124cfd9188afb6132f4b4f.png)
## 1、以太网标准规范简介

802.3标准定义了以太网PHY，约定其支持的速率、互联介质类型(媒体)以及信号编码方式等。802.3标准适用哪种速率、互联介质，采用哪种编码方式，多少通道，从标准的名称上即可以识别。

![800](ad16933f4e8247959f5097e66e415aad.png)

KR,CR,SR,DR,LR,ER,ZR的意思。
K表示背板互连，
C表示铜线互连，
S表示短距100m以上，多模光纤，
D表示500m，并行单模光纤，
F表示2km，通常是CWDM单模，
L一般表示长距10km，单模光纤，
E表示延长距离到40km,
ZR表示80km级互连，通常要用相干探测了。

字母R后面的数字一般表示并行光纤或者WDM通道的数量。

以太网根据传输媒体和传输速率组合分类标准，传输媒体分为双绞线缆、多模光纤和单模光纤，背板，传输速率分为10Mbps、100Mbps、1Gbps、和10Gbps等。

|类型|说明 |
|---|---|
|标准以太网|传输速率为10Mbps|
|快速以太网|传输速率为100Mbps|
|千兆以太网|传输速率为1000Mbps|
|万兆以太网|传输速率为10000Mbps|

### 1.1、10Mbps以太网标准

**10BASE5**
  10BASE5是用粗同轴电缆作为传输媒体的以太网标准，10代表10Mbps，BASE代表基带传输方式，5代表单段电缆的长度限制为500m，超过500m需要由中继器互连的两段电缆组成，这个标准已经淘汰。

**10BASE2**
  10BASE2是用细同轴电缆作为传输媒体的以太网标准，10和BASE的含义和10BASE5相同，2代表单段电缆的长度限制为200m，超过200m需要由中继器互连的两段电缆组成，这个标准已经淘汰。

**10BASE-T**
  10BASE T是用双绞线作为传输媒体的以太网标准，10和BASE的含义和10BASE5相同。它采用4对双绞线组成的双绞线电缆，用其中一对双绞线发送数据，另一对双绞线接收数据，因此，可以实现全双工通信。

  10BASE-T用于以集线器或以太网交换机为组网设备的以太网中，网络设备之间、网络设备和终端之间的距离必须小于100m。10BASE-T可以采用3类双绞线缆。

**10BASE-F**
10表示10Mbit/s的传输速度，BASE表示使用基带传输，F表示光纤，使用双工光缆，一条光缆用于发送数据，另一条用于接收；

### 1.2、100Mbps、1000Mbps、10Gbps以太网标准

![800](工作笔记/资料/以太网接口/images/200563f28ebb4bc9a69de45517252d88.png)
PCI Express、串行ATA (SATA) 和USB等串行接口的每个数据速率都有一个规范，而以太网针对相同的数据速率有多个不同规范。例如，10GBASE-ER和10GBASE-KR是10 Gbps以太网规范，但它们描述的是不同的互连介质接口。

## MDI接口

### 三种双工模式区别

![500](a1c689b60876444e8084987acb26557f.png)
**单工模式：** 数据传输是单向的，一方固定为发送端，另一方固定为接收端。例如，收音机只能接收发射塔发送的数据，而不能向发射塔发送数据。

**半双工模式：** 双方都具有发送和接收能力，但同一时刻只能有一个设备处于发送状态，另一个设备处于接收状态。这种模式在对讲机等设备中常见。

**全双工模式：** 双方能同时发送和接收数据，通常需要两根可以在不同站点同时发送和接收的传输线。

在全双工双绞线网络中，接收线对和发送线对同时在工作，每一线对的带宽都是100Mbps，这样在双绞线网络中，实际传送的带宽是200Mbps。就像双向车道一样，车辆流量的计算应是两个方向的车辆流量之和，网络带宽的计算也是如此。

在半双工双绞线网络中，在某一时刻，如果发送线对处于发送状态，则接收线对只能处于闲置状态，即使在物理线路上允许接收也不行，因为是不是全双工状态取决于与双绞线连接的网络设备是否支持全双工,而双绞线只是提供了物理介质而已。打个浅显的比方，高速公路都是双向(即双工)的，但是如果高速公路两端的收费站都只有一个车道，同一时刻只能有一辆车驶入或驶出，那么双向车道又有什么用呢?所以，100Mbps的半双工网络的带宽是100Mbps，而不是200Mbps。

10/100/1000 Base以太网中，除100 Base T4之外，均具有全双工能力，但在实际应用中，似乎只有Gb以太（即千兆以太）才使用全双工方式。

如果希望工作在全双工方式下，首先要有硬件的支持。全双工以太的主要优势在于它能够在二个独立的信道上同时实现二个方向上的数据传输，借以提高链路的总带宽。对于一般只进行单向数据传输的站点，全双工以太并无优势可言，所以全双工以太在应用上有很大的局限性。全双工以太主要用在交换机互连的场合，尤其是Gb以太交换机。

### 百兆以太网

在100BASE-T以太网中，制定的三种有关传输介质的标准是100BASE-TX、100BASE-T4、100BASE-FX。

100BASE-TX：
   支持半/全双工模式，使用两对非屏蔽双绞线，一组发送，一组接收，使用4B/5B曼彻斯特编码机制。数据传输速率为100Mbit/s，则实际传输速率为125Mbit/s。在全双工模式下，速率最大200Mbit/s。

   所谓的曼彻斯特（Manchester）编码方法，即由“+”跳变到“-”为0，由“-”跳变到“+”为1。不论是“0”或是“1”, 都有跳变。

100BASE-T4：
   只采用半双工模式，使用4对双绞线，其中的三对线用以传输数据（每对线的数据传输率为33.3Mbps），一对线进行冲突检验和控制信号的发送接受。采用8B/6T的编码方式，即每8位作为一组的数据转换为每6位一组的三元码组。

100BASE-FX：
   支持半/全双工模式，使用光纤，其中F指示光纤，IEEE标准为802.3u。为了实现时钟/数据恢复（CDR）功能，100Base-FX使用4B/5B编码机制。

### 千兆以太网

1000BASE-T，基于非屏蔽双绞线传输介质，传输距离为100米。在传输中使用了全部4对双绞线，采用全双工模式，该模式下可以实现2Gb/ s的传输速率。这种设计采用 PAM-5 (5级脉冲放大调制) 编码在每个线对上传输 250Mbps。1000BASE-T的编码方式比较复杂，最终落实到每一路的基频时钟是31.25MHz。

1000BASE-TX，也是基于4对双绞线，但却是以两对线发送，两对线接收( 类似于100Base-TX的一对线发送一对线接收)。支持半/全双工模式, 由于每对线缆本身不进行双向的传输，线缆之间的串扰就大大降低，同时其编码方式也相对简单，无需回声消除技术。

   由于使用线缆的效率降低了（两对线收，两对线发），要达到1000mbps的传输速率，要求带宽就超过100mhz，也就是说在五类和超五类的系统中不能支持该类型的网络。一定需要六类系统的支持。

1000BASE-X，支持半/全双工模式, 基于光纤介质，X代表单模或多模光纤.-X表示-CX、-SX以及-LX或（非标准化的）-ZX。1000BASE-X是非标准叫法。采用8b/10b编码，实际传输速率为1.25Gbit/s。

   -CX：采用均衡屏蔽的150Ω屏蔽双绞线（STP）, 最大传输距离25米。
   -SX：多模光纤上的短波激光。(Single-mode Fiber)
   -LX：多模光纤上的长波激光。(Multi-mode Fiber)
   -ZX：Cisco 指定的千兆以太网通信标准。运行在平常的链接跨度达43.5英里(70千米)的单模光纤上。


### 万兆以太网

10G以太网的速率很高，一般都是以光口形式出现，统称为10G Base-R，又可细分为SR、LR、ZR、LRM等多种形式，这部分与光模块联系很深，相关内容这里不细讲。

10G PHY连接光口时，一般都是XFI、SFI形式。XFI(接XFP光模块)接口相对SFI(接SFP+光模块)来说 一个很大的区别是：Mac端SFI接口需要包括预加重/均衡模块，同时SFP+光模块处不再拥有CDR模块，CDR由MAC端完成，这样SFP+相对XFP光模块来说，功耗少了一大半，体积也小。XFP光模块的大小和QSFP+、QSFP28差不多大。

10G Base-CR/KR也是经常见到的标准，这两个都是跑在铜线上，但是一个是电缆、一个是背板。直观地可以认为，接光模块时走线短，信号衰减比接电缆、背板要小，CR和KR就要想办法加强信号强度和抗干扰能力。

10G以太网还有支持双绞线的10G Base-T标准：
   10G Base-T采用的仍然是RJ-45网络接口，传输双绞线采用六类线，支持的传输距离最远可达100米，仍然采用四个差分对同时传输，全双工，但传输的总速率高达10Gbps，每对线的速率高达2.5Gbps。

   虽然采用了六类线的4个差分同时传输，把每个差分对的速率降到了2.5 Gbps，但要在双绞线上跑这个速率仍然是一个非常具有挑战性的工作。10G Base-T采用了非常复杂的编码方案来达到目标。最终结果是每个线对的码元传输率就是800M/s，用不算高的时钟速度实现了10G。这里就不展开说了。


## 2、MII分类

MII（Medium Independent Interface）即媒体独立接口，也叫介质无关接口。它是IEEE-802.3定义的以太网行业标准。它包括MAC和PHY之间的一个数据接口和一个管理接口。数据接口包括分别用于发送器和接收器的两条独立信道。每条信道都有自己的数据、时钟和控制信号。 

MII 接口是 MAC 与 PHY 连接的标准接口，提供了 MAC 与 PHY 之间、PHY 与 STA（Station Management）之间的互联技术。

类型有MII、RMII、SMII 、SSMII、SSSMII(S3MII)、GMII、RGMII、SGMII、QSGMII、PSGMII、TBI、RTBI、XGMII、XAUI、XLGMII、XLAUI、CGMII、RXAUI、 CAUI、HIGIG(Bro[adc](http://www.elecfans.com/tags/adc/ "adc")om Specification)、XL、Interlaken，所有这些接口都从MII而来，不用考虑媒体是铜轴、光纤、电缆等，因为这些媒体处理的相关工作都有PHY或者叫做MAC的芯片完成。

在10Mb/s时叫MII，速率≥100Mb/s时叫xMII，这个x表示自定义加的前缀。比如速率是Gb/s时叫GMII，速率是10Gb/s时叫XGMII，速率100Mb/s时还叫MII。

### 2.1、根据总线形式

a、并行总线

b、串行总线   有的MII采用Serdes技术；每条lane包括tx+，tx-，rx+，rx-（收发各1对差分总线）

![](1e363a12933b49eeb2386c929a45569d.png)

### 2.2、根据接口介质

#### a、基于双绞线、光纤、铜轴

**MII：** Medium Independent Interface
25MHz；４位并行通路，信号不经过编码，总带宽最高为25* ４=100 Mbps。
2.5MHz；４位并行通路，信号不经过编码，总带宽最高为2.5* ４=10Mbps。

**RMII：** Reduced Media Independant Interface
50MHz时；2位并行通路，信号不经过编码，带宽最高为50* 2=100 Mbps。
5MHz时；2位并行通路，信号不经过编码，带宽最高为5* 2=10Mbps。

**SMII：** Serial Media Independent Interface
125MHz时，1位串行通路，信号含控制与状态信息，故带宽最高125 * 1 * 80% = 100Mbps。
12.5MHz时,1位串行通路，信号含控制与状态信息，故带宽最高12.5 * 1 * 80% = 10Mbps

**SSMII：** Serial Sync MII
125MHz时，1位串行通路，信号包含控制与状态信息，故带宽最高125 * 1 * 80% = 100Mbps。
12.5MHz时,1位串行通路，信号包含控制与状态信息，故带宽最高12.5 * 1 * 80% = 10Mbps

**SSSMII：** Source Sync Serial MII
125MHz时，1位串行通路，信号包含控制与状态信息，故带宽最高125 * 1 * 80% = 100Mbps。
12.5MHz时,1位串行通路，信号包含控制与状态信息，故带宽最高12.5 * 1 * 80% = 10Mbps。

**GMII：** Gigabit MII
125MHz时 ，8位并行通路，信号不经过编码，故带宽最高125* 8=1000 Mbps。
25MHz时，4位并行通路，信号不经过编码，故带宽最高25* 4=100 Mbps。
2.5MHz时 ，4位并行通路，信号不经过编码，故带宽最高为2.5* 4=10 Mbps。

**RGMII：** Reduce Gigabit MII
125MHz时 ，时钟上升/下降都采样(DDR)，4位并行通路，信号不经过编码，故带宽最高125`*` 4`*`2=1000 Mbps。
25MHz时，采用单边沿传输，4位并行通路，信号不经过编码，故带宽最高25* 4=100 Mbps。
2.5MHz时 ，采用单边沿传输，4位并行通路，信号不经过编码，故带宽最高为2.5* 4=10 Mbps。

**SGMII：** Serial Gigabit MII
625MHz时钟，收和发各1对差分总线，时钟上升/下降都采样(DDR)，采用 8b/10b 线路编码，总带宽625 * 2 * 1=1.25Gbps,有效带宽1Gbps。**10/100Mbps** 工作模式下，1.25Gbit/s速率，分别重复发送 **100/10** 次，不降时钟速率。有时和外部接口1000base-X复用（不支持自协商）。

**QSGMII：** Quad Serial Gigabit MII
625MHz时钟，时钟上升/下降采样(DDR)，收和发各1对差分总线,，内部4路SGMII，采用8b/10b 线路编码，总带宽625 * 2 * 4=5Gbps, 有效带宽4Gbps。

**PSGMII：** Penta-Serial Gigabit Media Independent Interface 
625MHz时钟，时钟上升/下降采样(DDR)，收和发各1对差分总线，内部5路SGMII，采用8b/10b 线路编码，总带宽625 * 2 * 5=6.25Gbps, 有效带宽5Gbps。

**XSGMII：** 10 Gigabit Serial Gigabit MII
625MHz时钟，时钟上升/下降采样(DDR)，收和发各1对差分总线,，内部8路SGMII，采用 8b/10b 线路编码，总带宽625 * 2 * 8=12.5Gbps, 有效带宽10Gbps。

**USGMII：** Universal Serial Gigabit MII，
是SGMII和QSGMII的扩展规范，可向后兼容，还支持连接4端口或8端口千兆PHY和MAC，采用 8b/10b 线路编码, serdes速率5Gbps(4x1GE)或10Gbps(8x1GE) 。

**USXGMII：** Universal Serial 10 Gigabit MII
收发各1对差分对，采用64B/66B线路编码，最大serdes速率10Gbps，支持连接多端口、多速率PHY和MAC。

接单端口PHY，支持端口速率从10M到10G；连接2端口PHY，支持端口速率从10M到5G；连接4端口PHY，支持端口速率从10M到2.5G；连接8端口PHY，支持端口速率从10M到2.5G，等组合。

**XGMII：** 10 Gigabit MII (旧）
156.25MHz时钟，时钟上升/下降都采样(DDR)， 32位并行通道，信号不经过编码，总带宽156.25MHz * 2 * 32 = 10Gbps，有效带宽10Gbps。

**XLGMII：** 40 Gigabit  MII (旧）
156.25MHz时钟，4条lanes, 64位并行通道/lanes，采用 64b/66b 线路编码，总带宽156.25x 4 x66= 41.250G，有效带宽40Gbps。

**CGMII：** 100 Gigabit  MII (旧）
156.25MHz时钟，10条lanes，64位并行通道/lanes, 采用 64b/66b 线路编码，总带宽156.25x 10x66=103.125Gbps，有效带宽100Gbps。

**XAUI：** 10 Gigabit Attachment Unit Interface （新）（replace xgmii)
1.5625GHz时钟，时钟上升/下降采样(DDR)，速率3.125Gbps/差分对, 收和发各4对差分总线，采用 8b/10b线路编码，总带宽1.5625 `*` 2 `*` 4 = 12.5Gbps，有效带宽10Gbps。

**RXAUI：** reduced XAUI
3.125GHz时钟，时钟上升/下降采样(DDR)，速率6.25Gbps/差分对, 收和发各2对差分总线，采用 8b/10b 线路编码，总带宽3.125 `*` 2 `*`2 = 12.5Gbps，有效带宽10Gbps。

**DXAUI：** dual speed XAUI
3.125GHz时钟，时钟上升/下降采样(DDR)，速率6.25Gbps/差分, 收和发各4对差分总线, 采用 8b/10b 线路编码，总带宽3.125 * 2 * 4 =25Gbps，有效带宽20Gbps。

**XLAUI：** 40 Gigabit Attachment Unit Interface（新）（replace xlgmii)
5.15625GHz时钟, 时钟上升/下降采样(DDR)，速率10.3125Gbps/差分对,收和发各4对差分总线，采用 64b/66b 线路编码，总带宽5.15625 `*` 2 `*` 4 = 41.2540G，有效带宽40Gbps。

**CAUI：** 100 Gigabit Attachment Unit Interface（新）（replace cgmi)
12.890625GHz时钟，时钟上升/下降采样(DDR)， 速率25.78125Gbps/差分对，收和发各4对差分总线，采用 64b/66b 线路编码，总带宽12.890625`*`2 `*`4=103.125Gbps，有效带宽100Gbps。

**25GAUI：** 25 Gigabit Attachment Unit Interface
12.890625GHz时钟，时钟上升/下降采样(DDR)，速率25.78125Gbps/差分对,收和发各1对差分总线，采用 64b/66b 线路编码，总带宽12.890625 `*` 2 `*`1 = 25.78125Gbps，有效带宽25Gbps。

**XFI：** 10 Gigabit framer interface
5.15625GHz时钟, 时钟上升/下降采样(DDR)，速率10.3125Gbps/差分,收和发各1对差分总线，采用 64b/66b 线路编码，总带宽5.1562 * 2 * 1 = 10.3125Gbps   有效带宽10G。**XFP**（10 Gigabit Small Form Factor Pluggable）是指应用XFI接口的光模块。XFP光模块的尺寸略大于 **SFP** 和 **SFP+** 光模块。

**SFI：** scalable SERDES framer interface
5.15625GHz时钟, 时钟上升/下降采样(DDR)，速率10.3125Gbps/差分,收和发各1对差分总线，采用 64b/66b 线路编码，总带宽5.15625 `*` 2 `*` 1 = 10.3125Gbps，有效带宽10G。

**TBI：** Ten Bit Interface
62.5MHz时钟，2个半频时钟RX_CLK+/- , 10位并行通路，采用 8b/10b 线路编码，总带宽62.5* 2 10=1.25Gbps, 有效带宽10G 。

**RTBI：** Reduced TBI
125MHz时钟，时钟上升/下降采样(DDR)，5位并行通路，采用 8b/10b 线路编码，故带宽最高125`*`5`*`2=1.25Gbps, 有效带宽10G 。

**HIGIG：** (Broadcom Specification)
1.5625GHz时钟，时钟上升/下降采样(DDR)，3.125Gbps/差分对,收和发各4对差分总线，采用 8b/10b 线路编码，总带宽1.5625 * 2 * 4 = 12.5Gbps，有效带宽10Gbps。物理层电气特性和XAUI端口相同。

**HIGIG+**
1.875GHz时钟，时钟上升/下降采样(DDR)，速率3.75Gbps/差分对,收和发各4对差分总线，采用 8b/10b 线路编码，总带宽1.875 * 2 * 4=15Gbps，有效带宽12Gbps，与HiGig完全一样，所以对HiGig接口完全兼容。

**Interlaken**
0.5GHz ~ 3.125GHz时钟，时钟上升/下降采样(DDR)，速率1~6.25Gbps/差分对，收和发各n对差分总线，采用 64b/67b 线路编码，有效带宽不存在上限，主要用于10Gbps~100Gbps, 最高可达200Gbps。属于芯片到芯片高速互连协议，不局限于固定serdes通道数。

#### b、基于背板走线

目前应用背板标准有：
  1000BASE-KX for 1 Gb/s operation over a single lane
  25GBASE-KR and 25GBASE-KR-S for 25Gb/s operation over a single lane
  40GBASE-KR4 for 40 Gb/s operation over four lanes
  100GBASE-KR4 and 100GBASE-KP4 for 100 Gb/s operation over four lanes

注意：10G背板目前存在并行和串行两种版本。

**10GBASE-KR：并行**
5.15625GHz时钟, 时钟上升/下降采样(DDR)，速率10.3125Gbps/差分,收和发各1对差分总线，采用 64b/66b 线路编码，总带宽5.15625 * 2 * 1 = 10.3125Gbps，有效带宽10Gbps。

**10GBASE-KX4：串行**
1.5625GHz时钟，时钟上升/下降采样(DDR)，速率3.125Gb/差分, 收和发各4对差分总线，采用 8b/10b 线路编码，总带宽1.5625 `*` 2 `*` 4 = 10.3125Gbps，有效带宽10Gbps。


## 3、SGMII和MII/RMII/SMII.. 有什么区别？

SGMII是PHY与MAC之间的媒体接口（单工），类似与GMII和RGMII，只不过GMII和RGMII都是并行的，而且需要随路时钟，PCB布线相对麻烦，而且不适应背板应用。

SGMII是串行的，不需要提供另外的时钟，MAC和PHY都需要CDR去恢复时钟。另外SGMII是有8B/10b编码的，速率是1.25G。

## 4、SGMII和SERDES有什么区别？

相同点：

1、SerDes模式和SGMII模式在硬件连线上是完全兼容的，都是一对接收、发送差分信号，GE端口速率都为1.25Gbps。

不同点：
1、SGMII是以太网MAC与PHY之间的媒体接口（单工）,SerDes是通用可编程串行接口（双工）
2、SGMII有参考时钟信号硬件接口，如果MAC和PHY各自拥有独立的参考时钟，则可以忽略。SerDes是CDR时钟
3、SGMII模式下，MAC和电模块之间需要接PHY芯片，同时支持自协商模式。Serdes 外面不接PHY芯片，不支持自协商。
4、
![](9c0824b2908642cca7de54ae4b630c32.png)

其实，大多数MAC芯片的SGMII接口都可以配置成SerDes接口（双工）(在物理上完全兼容，只需配置寄存器即可)，直接外接光模块，而不需要PHY层芯片，此时时钟速率仍旧是625MHz。

### 5、XGMII和XAUI的区别？    

万兆以太网MII接口的端口速率为10Gbps，主要有XGMII和XAUI两种，另外还有HIGIG，不过HIGIG是Broadcom公司的私有标准，这里暂不介绍。

XAUI接口 x 代表10，aui（Ethernet attachment unit interface）其实它扩展的接口就是XGMII（与介质无关的万兆）

XFI（接XFP光模块）接口相对 SFI（接SFP+光模块）来说 一个很大的区别是：Mac端SFI接口需要包括预加重/均衡模块，同时SFP+光模块处不再拥有CDR模块，CDR由MAC端完成，这样SFP+相对XFP光模块来说，功耗少了一大半，也不会那么烫了。

### 6、背板以太网是如何兼容其他版本的？

背板以太网是兼容光口以太网的。比如10GBASE-KR兼容10GBASE-R。协议层上背板协议增加了FEC和自协商等功能。这些功能是可以关闭的。这样底层编解码处理上就没有什么区别了。

1000BASE-X与SGMII在线速率上是相同的，区别主要是自协商方式不同。SGMII采用CL37定义的自协商，速率可以向下兼容10/100M而1000BASE-X不支持更低速率。值得注意的是，SGMII即使协商为低速模式，serdes侧的速率仍然为1.25G。

### 7、基于场景和标准的以太网方案设计

数据中心服务器和交换机、交换机和交换机，以及交换机和路由器的连接，首选确定他们的连接采用电口，还是光口，这个决定需要不需要采用PHY芯片。

PHY芯片的作用主要是在电口之间进行不同接口的转换(SGMII/GMII/RGMII等)，因此如果需要外接其他设备，那么一般是要接PHY芯片的，如果是级联口，可以不用PHY。

**当交换机端口作电口用时，配为SGMII模式，需要连接PHY芯片；当端口作光口用时，配为SERDES模式，不需要接PHY芯片。**

服务器和交换机连接，现实的做法是从现有的10Gbps向25Gbps/50Gbps升级，直接上到100GE太贵了，40GE需要多个物理通道(4个10Gbps)，25GE享有单一通道的成本优势，并且25Gbps和50Gbps每通道技术将是未来100Gbps(4个25Gbps)和400Gbps(8个50Gbps)以太网标准的基础。

通过使用单一交换机/NIC SerDes通道，25Gbps以太网链路所提供的带宽相当于目前使用2对双轴铜缆SFP+直连式铜缆(DAC)的10Gbps链路所提供带宽的2.5倍。通过使用2条以25Gbps运行的交换机/NIC SerDes通道，50Gbps以太网链路能够提供比40千兆以太网链路多25%的带宽。