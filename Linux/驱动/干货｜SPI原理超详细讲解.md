已剪辑自: [https://view.inews.qq.com/a/20220824A00UYK00](https://view.inews.qq.com/a/20220824A00UYK00)

什么是SPI

SPI 是英语Serial Peripheral interface的缩写，顾名思义就是串行外围设备接口。是Motorola(摩托罗拉)首先在其MC68HCXX系列处理器上定义的。

SPI，是一种高速的，全双工，同步的通信总线，并且在芯片的管脚上只占用四根线，节约了芯片的管脚，同时为PCB的布局上节省空间，提供方便，主要应用在 EEPROM，FLASH，实时时钟，AD转换器，还有数字信号处理器和数字信号解码器之间。

SPI主从模式

SPI分为主、从两种模式，一个SPI通讯系统需要包含一个（且只能是一个）主设备，一个或多个从设备。提供时钟的为主设备（Master），接收时钟的设备为从设备（Slave），SPI接口的读写操作，都是由主设备发起。当存在多个从设备时，通过各自的片选信号进行管理。

SPI是全双工且SPI没有定义速度限制，一般的实现通常能达到甚至超过10 Mbps

SPI信号线

SPI接口一般使用四条信号线通信：

SDI（数据输入），SDO（数据输出），SCK（时钟），CS（片选）

MISO：主设备输入/从设备输出引脚。该引脚在从模式下发送数据，在主模式下接收数据。

MOSI：主设备输出/从设备输入引脚。该引脚在主模式下发送数据，在从模式下接收数据。

SCLK：串行时钟信号，由主设备产生。

CS/SS：从设备片选信号，由主设备控制。它的功能是用来作为“片选引脚”，也就是选择指定的从设备，让主设备可以单独地与特定从设备通讯，避免数据线上的冲突。

硬件上为4根线。

SPI一对一

SPI一对多

![](images/Pasted%20image%2020221202001544.png)

SPI设备选择

SPI是［单主设备（ single-master ）］通信协议，这意味着总线中的只有一支中心设备能发起通信。当SPI主设备想读/写［从设备］时，它首先拉低［从设备］对应的SS线（SS是低电平有效），接着开始发送工作脉冲到时钟线上，在相应的脉冲时间上，［主设备］把信号发到MOSI实现“写”，同时可对MISO采样而实现“读”，如下图：

![](images/Pasted%20image%2020221202001548.png)

SPI数据发送接收

SPI主机和从机都有一个串行移位寄存器，主机通过向它的SPI串行寄存器写入一个字节来发起一次传输。

首先拉低对应SS信号线，表示与该设备进行通信

主机通过发送SCLK时钟信号，来告诉从机写数据或者读数据

这里要注意，SCLK时钟信号可能是低电平有效，也可能是高电平有效，因为SPI有四种模式，这个我们在下面会介绍

主机(Master)将要发送的数据写到发送数据缓存区(Menory)，缓存区经过移位寄存器(0~7)，串行移位寄存器通过MOSI信号线将字节一位一位的移出去传送给从机，，同时MISO接口接收到的数据经过移位寄存器一位一位的移到接收缓存区。

从机(Slave)也将自己的串行移位寄存器(0~7)中的内容通过MISO信号线返回给主机。同时通过MOSI信号线接收主机发送的数据，这样，两个移位寄存器中的内容就被交换。

![](images/Pasted%20image%2020221202001552.png)

SPI只有主模式和从模式之分，没有读和写的说法，外设的写操作和读操作是同步完成的。如果只进行写操作，主机只需忽略接收到的字节；反之，若主机要读取从机的一个字节，就必须发送一个空字节来引发从机的传输。也就是说，你发一个数据必然会收到一个数据；你要收一个数据必须也要先发一个数据。

SPI通信的四种模式

SPI的四种模式，简单地讲就是设置SCLK时钟信号线的那种信号为有效信号

SPI通信有4种不同的操作模式，不同的从设备可能在出厂是就是配置为某种模式，这是不能改变的；但我们的通信双方必须是工作在同一模式下，所以我们可以对我们的主设备的SPI模式进行配置，通过CPOL（时钟极性）和CPHA（时钟相位）来

控制我们主设备的通信模式，具体如下：

时钟极性(CPOL)定义了时钟空闲状态电平：

CPOL=0，表示当SCLK=0时处于空闲态，所以有效状态就是SCLK处于高电平时

CPOL=1，表示当SCLK=1时处于空闲态，所以有效状态就是SCLK处于低电平时

时钟相位(CPHA)定义数据的采集时间。

CPHA=0，在时钟的第一个跳变沿（上升沿或下降沿）进行数据采样。，在第2个边沿发送数据

CPHA=1，在时钟的第二个跳变沿（上升沿或下降沿）进行数据采样。，在第1个边沿发送数据

例如：

Mode0：CPOL=0，CPHA=0：此时空闲态时，SCLK处于低电平，数据采样是在第1个边沿，也就是SCLK由低电平到高电平的跳变，所以数据采样是在上升沿(准备数据），（发送数据）数据发送是在下降沿。

Mode1：CPOL=0，CPHA=1：此时空闲态时，SCLK处于低电平，数据发送是在第1个边沿，也就是SCLK由低电平到高电平的跳变，所以数据采样是在下降沿，数据发送是在上升沿。

Mode2：CPOL=1，CPHA=0：此时空闲态时，SCLK处于高电平，数据采集是在第1个边沿，也就是SCLK由高电平到低电平的跳变，所以数据采集是在下降沿，数据发送是在上升沿。

Mode3：CPOL=1，CPHA=1：此时空闲态时，SCLK处于高电平，数据发送是在第1个边沿，也就是SCLK由高电平到低电平的跳变，所以数据采集是在上升沿，数据发送是在下降沿。

![](images/Pasted%20image%2020221202001558.png)

![](images/Pasted%20image%2020221202001601.png)

它们的区别是定义了在时钟脉冲的哪条边沿转换（toggles）输出信号，哪条边沿采样输入信号，还有时钟脉冲的稳定电平值（就是时钟信号无效时是高还是低）。每种模式由一对参数刻画，它们称为时钟极（clock polarity）CPOL与时钟期（clock phase）CPHA。

SPI的通信协议

![](images/Pasted%20image%2020221202001606.png)

主从设备必须使用相同的工作模式——SCLK、CPOL 和 CPHA，才能正常工作。如果有多个从设备，并且它们使用了不同的工作模式，那么主设备必须在读写不同从设备时需要重新修改对应从设备的模式。以上SPI总线协议的主要内容。

是不是感觉，这就完了？SPI就是如此，他没有规定最大传输速率，没有地址方案，也没规定通信应答机制，没有规定流控制规则。

只要四根信号线连接正确，SPI模式相同，将CS/SS信号线拉低，即可以直接通信，一次一个字节的传输，读写数据同时操作，这就是SPI

些通信控制都得通过SPI设备自行实现，SPI并不关心物理接口的电气特性，例如信号的标准电压。

PS:这也是SPI接口的一个缺点：没有指定的流控制，没有应答机制确认是否接收到数据。

SPI的三种模式

SPI工作在3中模式下，分别是运行、等待和停止。

运行模式（Run Mode）

这是基本的操作模式

等待模式（Wait Mode）

SPI工作在等待模式是一种可配置的低功耗模式，可以通过SPICR2寄存器的SPISWAI位进行控制。在等待模式下，如果SPISWAI位清0，SPI操作类似于运行模式。如果SPISWAI位置1，SPI进入低功耗状态，并且SPI时钟将关闭。如果SPI配置为主机，所有的传输将停止，但是会在CPU进入运行模式后重新开始。如果SPI配置为从机，会继续接收和传输一个字节，这样就保证从机与主机同步。

停止模式（Stop Mode）

为了降低功耗，SPI在停止模式是不活跃的。如果SPI配置为主机，正在进行的传输会停止，但是在CPU进入运行模式后会重新开始。如果SPI配置为从机，会继续接受和发送一个字节，这样就保证了从机与主机同步。

SPI原理图连接

![](images/Pasted%20image%2020221202001611.png)

STM32中SPI初始化配置

1.初始化GPIO口，配置相关引脚的复用功能，使能SPIx时钟。调用函数：void GPIO\_Init()；

2.使能SPI时钟总线:RCC\_APB2PeriphClockCmd(RCC\_APB2Periph\_SPI1,ENABLE)

3.配置SPI初始化的参数，设置SPI工作模式:SPI\_Init(SPI1,&SPI\_Initstructure)

4.使能SPI外设：SPI\_Cmd(SPI1,ENABLE);

SPI配置设置

![](images/Pasted%20image%2020221202001615.png)

![](images/Pasted%20image%2020221202001621.png)

![](images/Pasted%20image%2020221202001626.png)

SPI发送函数(标准库/HAL库)

![](images/Pasted%20image%2020221202001631.png)

