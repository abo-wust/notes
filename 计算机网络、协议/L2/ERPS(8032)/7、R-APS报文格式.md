
R-APS报文具体包含在R-APS PDU（Protocol Data Unit）即协议数据单元中，属于以太网OAM消息的一种。

R-APS使用MAC地址传送消息。协议建议使用目的MAC地址：01-19-A7-00-00-01。（？？？）

R-APS PDU的报文格式如下图：

![](images/Image%201.png)

上图中各字段的说明：
MEL：标识维护实例（MEG）的等级。
Version：版本。目前协议固定为１。
OpCode：图中显示的值为40，即0x28。
Flags：固定值为0，即0x00。该字段在接受的过程中被忽略。
TLV Offset：固定值32，即0x20。
R-APS Specific Information：R-APS消息的核心字段，包含核心的消息。共32位。
TLV：该字段用来描述报文中需要加载的信息，长度无限制。而没有需要加载的消息时，设置为0。

R-APS Specific Information报文格式如下：

![](images/Image%202.png)





