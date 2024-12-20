已剪辑自: [https://mp.weixin.qq.com/s?\_\_biz=MzIwNDQwMjIwNQ==&mid=2247484368&idx=1&sn=0af552051a01299b9e23db15b0e7f602&chksm=96c1f8a2a1b671b496fc2d0de57275d70f22b2fa8f308e8b3b4742938517976b877e62cdaca9#rd](https://mp.weixin.qq.com/s?__biz=MzIwNDQwMjIwNQ==&mid=2247484368&idx=1&sn=0af552051a01299b9e23db15b0e7f602&chksm=96c1f8a2a1b671b496fc2d0de57275d70f22b2fa8f308e8b3b4742938517976b877e62cdaca9#rd)

I2C(**IIC**)属于两线式串行总线，由飞利浦公司开发用于微控制器(MCU)和外围设备(从设备)进行通信的一种总线，属于**一主多从(一个主设备(Master)，多个从设备(Slave))的总线结构**，**总线上的每个设备都有一个特定的设备地址，以区分同一I2C总线上的其他设备**。



物理I2C接口有两根双向线，**串行时钟线（SCL）**和**串行数据线（SDA）**组成，可用于发送和接收数据，但是通信都是由主设备发起，从设备被动响应，实现数据的传输。

![](images/Pasted%20image%2020221202000805.png)



I2C主设备与从设备的一般通信过程



一. 主设备给从设备发送/写入数据：

1. 主设备发送起始（START）信号

2\. 主设备发送设备地址到从设备

3\. 等待从设备响应(ACK)

4\. 主设备发送数据到从设备，一般发送的每个字节数据后会跟着等待接收来自从设备的响应(ACK)

5. 数据发送完毕，主设备发送停止(STOP)信号终止传输

![](images/Pasted%20image%2020221202000811.png)



二. 主设备从从设备接收/读取数据

1. 设备发送起始（START）信号

2\. 主设备发送设备地址到从设备

3\. 等待从设备响应(ACK)

4. 主设备接收来自从设备的数据，一般接收的每个字节数据后会跟着向从设备发送一个响应(ACK)

5. 一般接收到最后一个数据后会发送一个无效响应(NACK)，然后主设备发送停止(STOP)信号终止传输

![](images/Pasted%20image%2020221202000815.png)



注：具体通信过程需视具体时序图而定







就是使用芯片上的I2C外设，也就是硬件I2C，它有相应的I2C驱动电路，有专用的IIC引脚，效率更高，写代码会相对简单，**只要调用I2C的控制函数即可**，**不需要用代码去控制SCL、SDA的各种高低电平变化来实现I2C协议**，只需要将I2C协议中的可变部分（如：从设备地址、传输数据等等）通过函数传参给控制器，控制器自动按照I2C协议实现传输，但是如果出现问题，就只能通过示波器看波形找问题。

二. 使用GPIO通过软件模拟实现

**软件模拟I2C比较重要**，**因为软件模拟的整个流程比较清晰**，**哪里出来bug**，**很快能找到问题**，**模拟一遍会对I2C通信协议更加熟悉**。



如果芯片上没有IIC控制器，或者控制接口不够用了，通过使用**任意IO口去模拟实现IIC通信协议**，手动写代码去控制IO口的电平变化，模拟IIC协议的时序，实现IIC的信号和数据传输，**下面会讲到根据通信协议如何用软件去模拟**。





IIC总线协议无非就是几样东西：**起始信号**、**停止信号**、**应答信号**、以及**数据有效性**。





**时钟线（SCL）和数据线（SDA）接上拉电阻**，**默认高电平**，**表示总线是空闲状态**。





从设备地址用来区分总线上不同的从设备，一般发送从设备地址的时候会在最低位加上读/写信号，比如设备地址为0x50，0表示读，1表示写，则读数据就会发送0x50，写数据就会发送0x51。





I2C通信的起始信号由主设备发起，SCL保持高电平，SDA由高电平跳变到低电平。

![](images/Pasted%20image%2020221202000822.png)

// 起始信号
void IIC\_start(void)
{
    // 1.首先把数据线设置为输出模式
    // 总线空闲, SCL和SDA输出高
    SCL = 1;  
    SDA = 1;
    delay\_us(5);
    
    // SDA由高变低
    SDA = 0;
    delay\_us(5);
    
    // 拉低SCL开始传输数据
    SCL = 0;
}





I2C通信的停止信号由主设备终止，SCL保持高电平，SDA由低电平跳变到高电平。

![](images/Pasted%20image%2020221202000826.png)

// 停止信号
void IIC\_stop(void)
{
    // 1.首先把数据线设置为输出模式
    
    // 拉高时钟线
    SDA = 0;
    delay\_us(5);
    SCL = 1;
    delay\_us(5);
    
    // SDA由低变高
    SDA = 1;
}





I2C总线进行数据传送时，在SCL的每个时钟脉冲期间传输一个数据位，**时钟信号SCL为高电平期间，数据线SDA上的数据必须保持稳定**，只有在时钟线SCL上的信号为低电平期间，数据线SDA上的高电平或低电平状态才允许变化，因为当SCL是高电平时，数据线SDA的变化被规定为**控制命令**（**START**或**STOP**，也就是前面的**起始信号**和**停止信号**）。

![](images/Pasted%20image%2020221202000830.png)



六. 应答信号(ACK：有效应答，NACK：无效应答)



**接收端收到有效数据后向对方响应的信号，发送端每发送一个字节(8位)数据，在第9个时钟周期释放数据线去接收对方的应答。**

当SDA是低电平为有效应答(ACK)，表示对方**接收成功**；

当SDA是高电平为无效应答(NACK)，表示对方**没有接收成功**。

发送数据需要等待接收方的应答：

// 等待ACK   1-无效    0-有效
u8 IIC\_wait\_ack(void)
{
    u8 ack = 0;
    
    // 数据线设置为输入
    
    // 拉高时钟线
    SCL = 1;
    delay\_us(5);
    // 获取数据线的电平
    if(SDA)
    {   // 无效应答
        ack = 1;
        IIC\_stop();
    }
    else
    {   // 有效应答
        ack = 0;
        // 拉低SCL开始传输数据
        SCL = 0;
        delay\_us(5);
    }
    
    return ack;
}

接收数据需要向发送方发送应答：

void IIC\_ack(u8 ack)
{
    // 数据线设置为输出
    
    SCL = 0;
    delay\_us(5);
    
    if(ack)
        SDA = 1; // 无效应答
    else
        SDA = 0; // 有效应答      
    delay\_us(5);
    SCL = 1;
    // 保持数据稳定
    delay\_us(5);
    // 拉低SCL开始传输数据
    SCL = 0;
}



注：后台发送 “**IIC**” 即可获取基于STM32上实现软件模拟IIC的完整代码。



  关注微信公众号『**混说Linux**』，后台点击 **关于混说** 即可添加作者微信。



