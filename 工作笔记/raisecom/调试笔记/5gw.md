
## 1、编译
编译脚本修改，需要编译 ros、ritp 和 5g 模块的代码。其中 ros 主要提供进程间通信的功能，ritp 实现具体的 rtl9617 芯片的配置功能，5g 模块实现对模块的操作。

当前 ros 和 ritp 的编译已经修改完成，能正常编译版本，产品能正常调用的 sdk api 函数。5g 模块的代码放在驱动目录，目前还未进行编译适配。

> 当前存在问题：
> （1）当前编译版本超过40多兆较多时无法正常升级，这个再跟方兆良下周定位
> （2）文件系统缺少初始化启动配置，无法调用到产品的启动脚本，应用进程无法启动，这个下周跟方兆良一起改一下

## 2、功能梳理

### 板卡热插拔
框式设备通过 cpld 中断通知主控板卡上下线，5gw 子卡没有 cpld，这个通知应该是 gpio 方式实现，硬件手册有提供 5GW板卡系统启动正常信号输出到主控的 GPIO。


### 复位
包括 9617、5g模块，以及其他器件的复位，看硬件文档都是有对应的 gpio 实现功能，具体使用与硬件沟通。


### 设备管理
- 板卡信息查询
- 5G 模块信息查询（主控上通过 IBC 查询）
- EEPROM 由板卡 IIC 读写
- 电压和温度监控，板上 CT80 提供信息，主控上通过 IBC 查询
- 5G 模块的管理，模块热拔插在主控上怎么处理？模块异常主控上怎么处理？告警？
- 5G 模块的 WIFI 信号状态，主控也要进行监控，信号异常时需要业务倒换到有线


### 接口管理
- 背板的 10G PON 口连接 5118 的 XFI，初始化需要设置为 10G XFI 模式，在主控上呈现为 10GE 端口，这个 10GE 端口的 link 状态应该要根据背板的 10G XFI 端口和接 5G 模块的 USB3.0 端口状态一起判断。

- 另外几个 rgmii 和 GPHY 接口通过 RTL8211 接到背板，需要提供调试手段，进行配置和查询。
	- P6 SDS口连接背板数据通道接口
	- GPHY0外接RTL8211（地址为0x06），实现UTP转SGMII，作为背板数据通道接口
	- RGMII接口外接RTL8211（地址为0x07），实现RGMII转SGMII，作为带外管理通道接口
	- GPHY1外接RTL8211（地址为0x07），实现UTP转SGMII，作为带外管理通道接口

- 主控上呈现的 10GE 端口提供可配置项，这些是配置到 5G 模块？

- 主控上呈现的 10GE 端口可配置选项需要梳理，如果是配置到 5G 模块，前置任务要梳理模块的配置项，以及终端的代码实现。


### IBC 通信
这个按照框式设备的板卡间通信移植过来，应该没有其他问题。


### 在线升级
同 IBC 通信，移植框式设备的实现。需要验证 system、boot 的升级。


### LED 指示灯
与硬件沟通有哪些部分需要软件参与实现。


### 业务倒换
倒换动作都由主控完成，5GW 子卡只需要提供 5G 模块的 WIFI 信号是否正常，保证 5G 模块收到 WIFI 信号后能正确通过 USB3.0 接口到背板的 10G XFI 接口送到主控。


## 3、5G 模块可配置项
（待梳理）
主要的工作量应该是在 RTL9617 和 5G 模块之间的通信适配上，终端给的代码只有实现逻辑能参考。


**配置到 5G 模块：**
- 用户名、密码
- APN
- 拨号网络类型，auto/nr5g/lte/cdma/gsm
- 流控
- 

**配置到 RTL9617 侧接 5G 模块的网口：**
- IP
- dns
- nat

**配置到平台：**
- mtu
- 拨号方式，手动/自动（代码需要梳理配置过程）
- 

**查询配置：**
- 板卡信息
- 模块信息
- 配置信息



## 4、工作内容
开发任务分为三个部分：
### RTL9617
在前面的“功能梳理”里面已经列出

### 5G 模块
- 按终端给的文档和代码，把 5G 模块的命令行调试走一遍，熟悉基本内容
- 5G 模块的代码详细梳理，需要支持的配置项配置逻辑
- 驱动适配 5G 模块的配置，新增文件后的编译问题

### 主控侧
- 板卡管理，板卡插拔动作主控侧怎么感知
- 接口管理，5GW 子卡应该只呈现一个 10GE 端口
- 命令行新增，具体需要哪些命令行待定
- IBC 通信


## 5、调试方法
两台设备 WIFI 组网



---


## 调试笔记


### 5G 模块

#### 模块上电

上电后进入 uboot，配置5G模块上电，给 gpio 104 配置为高电平，目的是给 5G 模块上电
```
gpio_test 104 1
```
执行完后敲`r`启动 system，注意要断电进入 uboot 执行上面命令。

> 这个后续需要写进 boot，启动时自动上电


#### 检查 USB 驱动挂载
板卡启动后，检查 5G 模块是否正常挂载：
执行lsusb，检查是否检测到模组USB, 如果找到对应模式的VID/PID，表示USB已正常检测到：
```
# lsusb
Bus 001 Device 001: ID 1d6b:0002
Bus 001 Device 002: ID 2cb7:0a05
Bus 002 Device 001: ID 1d6b:0003
# 
# 
```

5G 模块未上电时，文件系统下只能扫到两个设备：
```
# lsusb
Bus 001 Device 001: ID 1d6b:0002
Bus 002 Device 001: ID 1d6b:0003
# 
```

再通过 `ls /dev/ttyUSB*`  检查USB串口驱动是否正常加载
```
# 
# ls /dev/ttyUSB* -l
crw-rw----    1 root     root      188,   0 Jan  1 00:07 /dev/ttyUSB0
crw-rw----    1 root     root      188,   1 Jan  1 00:00 /dev/ttyUSB1
crw-rw----    1 root     root      188,   2 Jan  1 00:00 /dev/ttyUSB2
crw-rw----    1 root     root      188,   3 Jan  1 00:00 /dev/ttyUSB3
# 
# 
```


```
# 
# 
# ls -al /sys/bus/usb/devices/
total 0
drwxr-xr-x    2 root     root             0 Jan  1 00:03 .
drwxr-xr-x    4 root     root             0 Jan  1 00:03 ..
lrwxrwxrwx    1 root     root             0 Jan  1 00:03 1-0:1.0 -> ../../../devices/platform/f0200000.usb3-xhci/usb1/1-0:1.0
lrwxrwxrwx    1 root     root             0 Jan  1 00:03 1-1 -> ../../../devices/platform/f0200000.usb3-xhci/usb1/1-1
lrwxrwxrwx    1 root     root             0 Jan  1 00:03 1-1:1.0 -> ../../../devices/platform/f0200000.usb3-xhci/usb1/1-1/1-1:1.0
lrwxrwxrwx    1 root     root             0 Jan  1 00:03 1-1:1.1 -> ../../../devices/platform/f0200000.usb3-xhci/usb1/1-1/1-1:1.1
lrwxrwxrwx    1 root     root             0 Jan  1 00:03 1-1:1.2 -> ../../../devices/platform/f0200000.usb3-xhci/usb1/1-1/1-1:1.2
lrwxrwxrwx    1 root     root             0 Jan  1 00:03 1-1:1.3 -> ../../../devices/platform/f0200000.usb3-xhci/usb1/1-1/1-1:1.3
lrwxrwxrwx    1 root     root             0 Jan  1 00:03 1-1:1.4 -> ../../../devices/platform/f0200000.usb3-xhci/usb1/1-1/1-1:1.4
lrwxrwxrwx    1 root     root             0 Jan  1 00:03 1-1:1.5 -> ../../../devices/platform/f0200000.usb3-xhci/usb1/1-1/1-1:1.5
lrwxrwxrwx    1 root     root             0 Jan  1 00:03 2-0:1.0 -> ../../../devices/platform/f0200000.usb3-xhci/usb2/2-0:1.0
lrwxrwxrwx    1 root     root             0 Jan  1 00:03 usb1 -> ../../../devices/platform/f0200000.usb3-xhci/usb1
lrwxrwxrwx    1 root     root             0 Jan  1 00:03 usb2 -> ../../../devices/platform/f0200000.usb3-xhci/usb2
# 
# 
# 
# 
# ls -al /sys/bus/usb/drivers
total 0
drwxr-xr-x   26 root     root             0 Jan  1 00:12 .
drwxr-xr-x    4 root     root             0 Jan  1 00:03 ..
drwxr-xr-x    2 root     root             0 Jan  1 00:12 asix
drwxr-xr-x    2 root     root             0 Jan  1 00:12 ax88179_178a
drwxr-xr-x    2 root     root             0 Jan  1 00:12 cdc_acm
drwxr-xr-x    2 root     root             0 Jan  1 00:12 cdc_ether
drwxr-xr-x    2 root     root             0 Jan  1 00:12 cdc_ncm
drwxr-xr-x    2 root     root             0 Jan  1 00:12 cdc_subset
drwxr-xr-x    2 root     root             0 Jan  1 00:12 cdc_wdm
drwxr-xr-x    2 root     root             0 Jan  1 00:12 ch341
drwxr-xr-x    2 root     root             0 Jan  1 00:12 cp210x
drwxr-xr-x    2 root     root             0 Jan  1 00:12 cypress_m8
drwxr-xr-x    2 root     root             0 Jan  1 00:12 ftdi_sio
drwxr-xr-x    2 root     root             0 Jan  1 00:12 hub
drwxr-xr-x    2 root     root             0 Jan  1 00:12 net1080
drwxr-xr-x    2 root     root             0 Jan  1 00:12 option
drwxr-xr-x    2 root     root             0 Jan  1 00:12 pl2303
drwxr-xr-x    2 root     root             0 Jan  1 00:12 qmi_wwan_simcom
drwxr-xr-x    2 root     root             0 Jan  1 00:12 rndis_host
drwxr-xr-x    2 root     root             0 Jan  1 00:12 uas
drwxr-xr-x    2 root     root             0 Jan  1 00:12 usb
drwxr-xr-x    2 root     root             0 Jan  1 00:12 usb-storage
drwxr-xr-x    2 root     root             0 Jan  1 00:12 usbfs
drwxr-xr-x    2 root     root             0 Jan  1 00:12 usblp
drwxr-xr-x    2 root     root             0 Jan  1 00:12 usbserial_generic
drwxr-xr-x    2 root     root             0 Jan  1 00:12 zaurus
# 
# 
# ls -al /sys/bus/usb-serial/devices/
total 0
drwxr-xr-x    2 root     root             0 Jan  1 00:12 .
drwxr-xr-x    4 root     root             0 Jan  1 00:12 ..
lrwxrwxrwx    1 root     root             0 Jan  1 00:12 ttyUSB0 -> ../../../devices/platform/f0200000.usb3-xhci/usb1/1-1/1-1:1.2/ttyUSB0
lrwxrwxrwx    1 root     root             0 Jan  1 00:12 ttyUSB1 -> ../../../devices/platform/f0200000.usb3-xhci/usb1/1-1/1-1:1.3/ttyUSB1
lrwxrwxrwx    1 root     root             0 Jan  1 00:12 ttyUSB2 -> ../../../devices/platform/f0200000.usb3-xhci/usb1/1-1/1-1:1.4/ttyUSB2
lrwxrwxrwx    1 root     root             0 Jan  1 00:12 ttyUSB3 -> ../../../devices/platform/f0200000.usb3-xhci/usb1/1-1/1-1:1.5/ttyUSB3
# 
# 
# 
# ls -al /sys/bus/usb-serial/drivers
total 0
drwxr-xr-x   11 root     root             0 Jan  1 00:13 .
drwxr-xr-x    4 root     root             0 Jan  1 00:12 ..
drwxr-xr-x    2 root     root             0 Jan  1 00:13 ch341-uart
drwxr-xr-x    2 root     root             0 Jan  1 00:13 cp210x
drwxr-xr-x    2 root     root             0 Jan  1 00:13 cyphidcom
drwxr-xr-x    2 root     root             0 Jan  1 00:13 earthmate
drwxr-xr-x    2 root     root             0 Jan  1 00:13 ftdi_sio
drwxr-xr-x    2 root     root             0 Jan  1 00:13 generic
drwxr-xr-x    2 root     root             0 Jan  1 00:13 nokiaca42v2
drwxr-xr-x    2 root     root             0 Jan  1 00:13 option1
drwxr-xr-x    2 root     root             0 Jan  1 00:13 pl2303
# 
# 
```


#### 查看网口 cell

这个网口是沿用了终端的 sdk 修改，在文件系统启动后自动创建的。
```
# 
# ifconfig -a
br0       Link encap:Ethernet  HWaddr 00:0E:5E:62:EE:EE  
          inet addr:192.168.1.254  Bcast:192.168.1.255  Mask:255.255.255.0
          inet6 addr: fe80::20e:5eff:fe62:eeee/64 Scope:Link
          UP BROADCAST RUNNING MULTICAST  MTU:1500  Metric:1
          RX packets:0 errors:0 dropped:0 overruns:0 frame:0
          TX packets:23 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:1000 
          RX bytes:0 (0.0 B)  TX bytes:3162 (3.0 KiB)

cell      Link encap:Ethernet  HWaddr 0E:37:6F:6A:DC:C8  
          BROADCAST MULTICAST  MTU:1500  Metric:1
          RX packets:0 errors:0 dropped:0 overruns:0 frame:0
          TX packets:0 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:1000 
          RX bytes:0 (0.0 B)  TX bytes:0 (0.0 B)

eth0      Link encap:Ethernet  HWaddr 00:13:25:00:00:01  
          UP BROADCAST RUNNING MULTICAST  MTU:1500  Metric:1
          RX packets:0 errors:0 dropped:0 overruns:0 frame:0
          TX packets:0 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:1000 
          RX bytes:0 (0.0 B)  TX bytes:0 (0.0 B)
          Interrupt:32 

eth0.2    Link encap:Ethernet  HWaddr 00:0E:5E:62:EE:EE  
          UP BROADCAST MULTICAST  MTU:1500  Metric:1
          RX packets:0 errors:0 dropped:0 overruns:0 frame:0
          TX packets:0 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:1000 
          RX bytes:0 (0.0 B)  TX bytes:0 (0.0 B)
          Interrupt:12 

eth0.2.0  Link encap:Ethernet  HWaddr 00:0E:5E:62:EE:EE  
          UP BROADCAST MULTICAST  MTU:1500  Metric:1
          RX packets:0 errors:0 dropped:0 overruns:0 frame:0
          TX packets:0 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:1000 
          RX bytes:0 (0.0 B)  TX bytes:0 (0.0 B)

eth0.3    Link encap:Ethernet  HWaddr 00:0E:5E:62:EE:EE  
          UP BROADCAST MULTICAST  MTU:1500  Metric:1
          RX packets:0 errors:0 dropped:0 overruns:0 frame:0
          TX packets:0 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:1000 
          RX bytes:0 (0.0 B)  TX bytes:0 (0.0 B)
          Interrupt:12 

eth0.3.0  Link encap:Ethernet  HWaddr 00:0E:5E:62:EE:EE  
          UP BROADCAST RUNNING MULTICAST  MTU:1500  Metric:1
          RX packets:0 errors:0 dropped:0 overruns:0 frame:0
          TX packets:23 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:1000 
          RX bytes:0 (0.0 B)  TX bytes:3162 (3.0 KiB)

eth0.4    Link encap:Ethernet  HWaddr 00:0E:5E:62:EE:EE  
          UP BROADCAST MULTICAST  MTU:1500  Metric:1
          RX packets:0 errors:0 dropped:0 overruns:0 frame:0
          TX packets:0 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:1000 
          RX bytes:0 (0.0 B)  TX bytes:0 (0.0 B)
          Interrupt:12 

eth0.4.0  Link encap:Ethernet  HWaddr 00:0E:5E:62:EE:EE  
          UP BROADCAST MULTICAST  MTU:1500  Metric:1
          RX packets:0 errors:0 dropped:0 overruns:0 frame:0
          TX packets:0 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:1000 
          RX bytes:0 (0.0 B)  TX bytes:0 (0.0 B)

eth0.5    Link encap:Ethernet  HWaddr 00:0E:5E:62:EE:EE  
          UP BROADCAST MULTICAST  MTU:1500  Metric:1
          RX packets:0 errors:0 dropped:0 overruns:0 frame:0
          TX packets:0 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:1000 
          RX bytes:0 (0.0 B)  TX bytes:0 (0.0 B)
          Interrupt:12 

eth0.5.0  Link encap:Ethernet  HWaddr 00:0E:5E:62:EE:EE  
          UP BROADCAST MULTICAST  MTU:1500  Metric:1
          RX packets:0 errors:0 dropped:0 overruns:0 frame:0
          TX packets:0 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:1000 
          RX bytes:0 (0.0 B)  TX bytes:0 (0.0 B)

eth0.6    Link encap:Ethernet  HWaddr 00:0E:5E:62:EE:EE  
          UP BROADCAST MULTICAST  MTU:1500  Metric:1
          RX packets:0 errors:0 dropped:0 overruns:0 frame:0
          TX packets:0 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:1000 
          RX bytes:0 (0.0 B)  TX bytes:0 (0.0 B)
          Interrupt:12 

eth0.6.0  Link encap:Ethernet  HWaddr 00:0E:5E:62:EE:EE  
          UP BROADCAST MULTICAST  MTU:1500  Metric:1
          RX packets:0 errors:0 dropped:0 overruns:0 frame:0
          TX packets:0 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:1000 
          RX bytes:0 (0.0 B)  TX bytes:0 (0.0 B)

eth0.7    Link encap:Ethernet  HWaddr 00:00:01:00:02:00  
          BROADCAST MULTICAST  MTU:1500  Metric:1
          RX packets:0 errors:0 dropped:0 overruns:0 frame:0
          TX packets:0 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:1000 
          RX bytes:0 (0.0 B)  TX bytes:0 (0.0 B)
          Interrupt:12 

eth0.8    Link encap:Ethernet  HWaddr 00:00:01:00:02:00  
          BROADCAST MULTICAST  MTU:1500  Metric:1
          RX packets:0 errors:0 dropped:0 overruns:0 frame:0
          TX packets:0 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:1000 
          RX bytes:0 (0.0 B)  TX bytes:0 (0.0 B)
          Interrupt:12 

ip6tnl0   Link encap:UNSPEC  HWaddr 00-00-00-00-00-00-00-00-00-00-00-00-00-00-00-00  
          NOARP  MTU:1452  Metric:1
          RX packets:0 errors:0 dropped:0 overruns:0 frame:0
          TX packets:0 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:1000 
          RX bytes:0 (0.0 B)  TX bytes:0 (0.0 B)

lo        Link encap:Local Loopback  
          inet addr:127.0.0.1  Mask:255.0.0.0
          inet6 addr: ::1/128 Scope:Host
          UP LOOPBACK RUNNING  MTU:65536  Metric:1
          RX packets:0 errors:0 dropped:0 overruns:0 frame:0
          TX packets:0 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:1000 
          RX bytes:0 (0.0 B)  TX bytes:0 (0.0 B)

nas0      Link encap:Ethernet  HWaddr 00:0E:5E:62:EE:EE  
          UP BROADCAST MULTICAST  MTU:1500  Metric:1
          RX packets:0 errors:0 dropped:0 overruns:0 frame:0
          TX packets:0 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:1000 
          RX bytes:0 (0.0 B)  TX bytes:0 (0.0 B)
          Interrupt:33 

nas0_0    Link encap:Ethernet  HWaddr 00:0E:5E:62:EE:EF  
          UP BROADCAST MULTICAST  MTU:1500  Metric:1
          RX packets:0 errors:0 dropped:0 overruns:0 frame:0
          TX packets:0 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:10 
          RX bytes:0 (0.0 B)  TX bytes:0 (0.0 B)

sit0      Link encap:IPv6-in-IPv4  
          NOARP  MTU:1480  Metric:1
          RX packets:0 errors:0 dropped:0 overruns:0 frame:0
          TX packets:0 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:1000 
          RX bytes:0 (0.0 B)  TX bytes:0 (0.0 B)

# 
```



#### USB 端口信息

前面检查 usb 的设备时会显示出唯一的 ID 信息：
```
# lsusb
Bus 001 Device 001: ID 1d6b:0002
Bus 001 Device 002: ID 2cb7:0a05
Bus 002 Device 001: ID 1d6b:0003
# 
```

从终端给的文档描述的 USB 端口信息可以知道，当前 5G 模块的模式是 36：

| **USB Mode** | **VID/PID**   | **Compositions**                     |
| ------------ | ------------- | ------------------------------------ |
| 34           | 0x2CB7/0x0A04 | ECM + AT + DIAG + LOG                |
| 35           | 0x2CB7/0x0A04 | ECM + AT + DIAG + LOG +ADB           |
| 36           | 0x2CB7/0x0A05 | NCM + AT + MODEM + DIAG + LOG        |
| 37           | 0x2CB7/0x0A05 | NCM + AT + MODEM + DIAG + LOG + ADB  |
| 38           | 0x2CB7/0x0A06 | RNDIS + AT + MODEM + DIAG + LOG      |
| 39           | 0x2CB7/0x0A06 | RNDIS + AT + MODEM + DIAG + LOG +ADB |

FG650支持ECM/NCM/RNDIS 三种模式，三种模式的主要特性如下表：

|           | **ECM** | **NCM** | **RNDIS** |
| --------- | ------- | ------- | --------- |
| 支持Linux   | Y       | Y       | Y         |
| 支持Windows | N       | N       | Y         |
| 支持硬件加速    | N       | Y       | Y         |

客户可以根据自己的需求，选择对应的端口模式。推荐使用RNDIS或NCM方式拨号。**模组默认是NCM模式**。

**所以可以知道四个 `ttyUSB` 设备对应的端口分别是 AT、Modem、Diag 和 Log。**


--- 

**ECM模式**端口枚举顺序：

| **Vendor ID: 0X2CB7 Product ID:0x0A04 (USBMODE 34/35)** |                        |                    |
| ------------------------------------------------------- | ---------------------- | ------------------ |
| **Interface Number**                                    | **Interface Function** | **Interface Name** |
| 0-1                                                     | USB NET                | CDC ECM            |
| 2                                                       | USB Serial             | Fibocom AT         |
| 3                                                       | USB Serial             | Fibocom Diag       |
| 4                                                       | USB Serial             | Fibocom Log        |

**NCM模式**端口枚举顺序：

| **Vendor ID: 0X2CB7 Product ID:0x0A05(USBMODE 36/37)** |                    |     |
| ------------------------------------------------------ | ------------------ | --- |
| **Interface Function**                                 | **Interface Name** |     |
| USB NET                                                | CDC NCM            |     |
| USB Serial                                             | Fibocom AT         |     |
| USB Serial                                             | Fibocom Modem      |     |
| USB Serial                                             | Fibocom Diag       |     |
| USB Serial                                             | Fibocom Log        |     |

**RNDIS模式**端口枚举顺序

| **Vendor ID: 0X2CB7 Product ID:0x0A06(USBMODE 38/39)** |                        |                    |
| ------------------------------------------------------ | ---------------------- | ------------------ |
| **Interface Number**                                   | **Interface Function** | **Interface Name** |
| 0-1                                                    | USB NET                | RNDIS              |
| 2                                                      | USB Serial             | Fibocom AT         |
| 3                                                      | USB Serial             | Fibocom Modem      |
| 4                                                      | USB Serial             | Fibocom Diag       |
| 5                                                      | USB Serial             | Fibocom Log        |

---


#### FIBOCOM 5G 模组 USB 虚拟端口说明

根据 FIBCOM 5G 模组（如 FG150/FM150 系列）的技术文档和应用实践，其 USB 虚拟端口中 AT、Modem、Diag 和 Log 端口的核心区别如下表所示：

| 端口类型     | 功能定位   | 典型操作                           | 应用场景           |
| -------- | ------ | ------------------------------ | -------------- |
| AT 端口    | 指令控制通道 | 执行配置命令（如 AT+GTUSBMODE=33 切换模式） | 参数配置、网络管理、功能控制 |
| Modem 端口 | 数据传输通道 | 承载 PPP/NCM/ECM 协议的网络数据包        | 联网后的实际数据传输     |
| Diag 端口  | 底层诊断接口 | 捕获基带芯片原始通信数据（如信令交互）            | 深度调试、协议栈分析     |
| Log 端口   | 系统日志输出 | 输出模组内核运行日志（如启动状态、异常报告          | 故障追踪、运行监控      |

##### AT端口
标识：通常为 /dev/ttyUSB0
作用：发送标准 AT 指令（如 AT+CSQ 查询信号强度、AT+CGDCONF 设置 APN），接收 OK/ERROR 响应。
特征：支持交互式命令，是模组功能配置的主入口。

##### Modem端口
标识：可能为 /dev/ttyUSB1 或 /dev/usb0（网卡形态）
作用：在 ECM/NCM 模式下承载IP数据流量；自动拨号成功后，系统将其识别为网络接口。（如 usb0）
数据协议：不解析AT指令，仅传输网络层数据包。

##### Diag端口
标识：如 /dev/ttyUSB2
作用：输出二进制诊断数据（如 QCDM 日志），需专用工具（如 QPST、QCSuper）解析。
应用：分析信号接入失败、基站交互异常等底层问题。

##### Log端口
标识：如 /dev/ttyUSB3
作用：实时输出模组内部运行日志（包括驱动加载、线程状态等），可直接使用串口工具查看。
调试价值：定位系统级错误（如驱动崩溃、资源不足）。


> **端口分配差异**
	具体映射关系因模组型号和固件版本而异（如 FG150 的 Diag 可能占用 ttyUSB2、而 FM150 可能合并 Log 功能）。



#### AT 命令说明

1、定义与作用
AT(Attention) 命令是用于控制通信模块（5G模组，调制解调器）的标准指令集。通过串口发送文本指令实现模块配置，网络管理等功能。
前缀要求：每条命令必须以AT或者AT开头。
响应格式：模块返回`OK,ERROR`或`+CME ERROR:<err>`, 后跟`<CR><LF>`换行符。

2、AT命令分类：
测试命令： `AT+<CMD>=?`，查询模块支持的参数范围
查询命令： `AT+<CMD>? `，获取当前参数配置
设置命令： `AT+<CMD>=p1,p2,...`，修改模块参数（如APN, 频段）
执行命令： `AT+<CMD>` ，触发特定操作（如拨号）


3、常用指令详解
（1）基础功能指令
通信测试： AT -> 返回OK表示串口连通。
信号强度： AT+CSQ -> 返回 +CSQ:<rssi>,<ber> (数值越大越好)
SIM卡状态： AT+CPIN? -> 返回 READY 表示识别成功。

（2）网络控制指令
模式切换：
AT+CFUN=0 # 进入飞行模式
AT+CFUN=1 # 恢复全功能模式：ml-citation{ref="9,13" data="citationList"}

5G频段锁定： AT+QNWPREFCFG="nr5g_band",1:78
APN设置： AT+CGDCONT=1,"IPV4V6","cmnet" （移动默认APN）

（3）高级功能指令
IP过滤关闭：
AT^WHITELISTCTL=0
AT^IPFILTERSWITCH=0 #允许非终端IP数据包转发：ml-citation{ref="6,10" data="citationList"}

自动拨号： AT+GTAUTOCONNECT=1(广和通模组专用)








