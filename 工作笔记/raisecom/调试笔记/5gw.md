
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


### 5gw 子卡


#### 驱动适配

1、初始化
先查询模块信息，判断模块正常上电
查询 sim 卡信息，正常后配置 sim 卡相关默认配置（目前就是 apn），板卡上线后具体的 apn 由主控下发保存的配置
使能后路由
使能 ippass
开启 arp 代理
锁定入网模式为 5g
自动拨号，这个需要主控下发正确的 apn 后执行

2、at 指令的配置和返回信息处理
at 指令下发参考终端代码封装一个公共接口
at 指令下发后需要获取模组返回的信息，看是否正确配置，这需要解析返回的内容
at 指令下发的冲突检测，应该是一次下发一个 at 指令，在上一个 at 指令没有返回时，后续的 at 指令执行等待，或者是都下发，但是需要检测每个指令的返回值




#### 擦除 flash

```
setenv mtdparts mtdparts=ca_spinand_flash:2816K(boot),256K@0x340000(env),256K@0x380000(env2),256K@0x3c0000(static_conf),245760K@0x400000(ubi_device)
setenv bootargs ubi.mtd=4 earlycon=serial,0xf43291b0 console=ttyS0,9600 swiotlb=noforce mtdparts=ca_spinand_flash:2816K(boot),256K@0x340000(env),256K@0x380000(env2),256K@0x3c0000(static_conf),245760K@0x400000(ubi_device)
saveenv
saveenv 
nand erase.part ubi_device 
执行完后重新设备
ubi part ubi_device 
ubi create ubi_Config  0xa00000 dynamic
ubi create ubi_k0  0x5a00000 dynamic
ubi create ubi_k1 0x5a00000 dynamic
ubi create ubi_user 0x1400000 dynamic
```

##### 1、解除ubi设备和mtd分区绑定：

ubi detach -m <mtd_device_number> // 如 ubi detach -m 5 或 ubi detach ubi_device
```
# ubi detach ubi_device
ubi0: detaching mtd5
ubi0: mtd5 is detached
# 
```

##### 2、擦除mtd分区

`nand erase.part <partition_name>`

```
# nand erase.part ubi_device

NAND erase.part: device 0 offset 0x400000, size 0x1f000000
Erasing at 0x1f3c0000 -- 100% complete.
OK
#
```

##### 3、创建ubi设备

`ubi part <partition_name>`

```
# ubi part ubi_device
ubi0: default fastmap pool size: 95
ubi0: default fastmap WL pool size: 47
ubi0: attaching mtd5
ubi0: scanning is finished
ubi0: empty MTD device detected
ubi0: attached mtd5 (name "ubi_device", size 496 MiB)
ubi0: PEB size: 262144 bytes (256 KiB), LEB size: 253952 bytes
ubi0: min./max. I/O unit sizes: 4096/4096, sub-page size 4096
ubi0: VID header offset: 4096 (aligned 4096), data offset: 8192
ubi0: good PEBs: 1984, bad PEBs: 0, corrupted PEBs: 0
ubi0: user volume: 0, internal volumes: 1, max. volumes count: 128
ubi0: max/mean erase counter: 0/0, WL threshold: 4096, image sequence number: 0
ubi0: available PEBs: 1938, total reserved PEBs: 46, PEBs reserved for bad PEB handling: 40
# 
```

##### 4、创建ubi卷
`ubi create <volume_name> <size> <-> `

```
# ubi create ubi_Config  0x3200000 dynamic
Creating dynamic volume ubi_Config of size 52428800

# ubi create ubi_k0  0x9600000 dynamic
Creating dynamic volume ubi_k0 of size 157286400
# 

# ubi create ubi_k1 0x9600000 dynamic
Creating dynamic volume ubi_k1 of size 157286400
# 

# ubi create ubi_user 0x6400000 dynamic
Creating dynamic volume ubi_user of size 104857600
# 
```


**异常情况需要看一下是不是5和6 的问题**
##### 5、关于 ubi part ubi_device 报错

```
# ubi part ubi_device       
ubi0: default fastmap pool size: 100
ubi0: default fastmap WL pool size: 50
ubi0: attaching mtd5
ubi0: scanning is finished
ubi0 error: ubi_read_volume_table: the layout volume was not found
ubi0 error: ubi_attach_mtd_dev: failed to attach mtd5, error -22
UBI error: cannot attach mtd5
UBI error: cannot initialize UBI, error -22
UBI init error 22
Please check, if the correct MTD partition is used (size big enough?)

需要做以下修改：
修改前：
bootargs=earlycon=serial,0xf43291b0 ubi.mtd=4 console=ttyS0,9600 swiotlb=noforce mtdparts=ca_spinand_flash:2816K(boot),256K@0x340000(env),256K@0x380000(env2),256K@0x3c0000(static_conf),-(ubi_device)
mtdparts=mtdparts=ca_spinand_flash:2816K(boot),256K@0x340000(env),256K@0x380000(env2),256K@0x3c0000(static_conf),-(ubi_device)

修改后：
setenv mtdparts mtdparts=ca_spinand_flash:2816K(boot),256K@0x340000(env),256K@0x380000(env2),256K@0x3c0000(static_conf),507904K@0x400000(ubi_device)
setenv bootargs ubi.mtd=4 earlycon=serial,0xf43291b0 console=ttyS0,9600 swiotlb=noforce mtdparts=ca_spinand_flash:2816K(boot),256K@0x340000(env),256K@0x380000(env2),256K@0x3c0000(static_conf),507904K@0x400000(ubi_device)
```

ubi part ubi_device执行结果：
```
# ubi part ubi_device
ubi0: default fastmap pool size: 95
ubi0: default fastmap WL pool size: 47
ubi0: attaching mtd5
ubi0: scanning is finished
ubi0: empty MTD device detected
ubi0: attached mtd5 (name "ubi_device", size 496 MiB)
ubi0: PEB size: 262144 bytes (256 KiB), LEB size: 253952 bytes
ubi0: min./max. I/O unit sizes: 4096/4096, sub-page size 4096
ubi0: VID header offset: 4096 (aligned 4096), data offset: 8192
ubi0: good PEBs: 1984, bad PEBs: 0, corrupted PEBs: 0
ubi0: user volume: 0, internal volumes: 1, max. volumes count: 128
ubi0: max/mean erase counter: 0/0, WL threshold: 4096, image sequence number: 0
ubi0: available PEBs: 1938, total reserved PEBs: 46, PEBs reserved for bad PEB handling: 40

```
##### 6、移除ubi卷
1）创建ubi卷
```
# ubi create ubi_Config  0xa00000 dynamic
Creating dynamic volume ubi_Config of size 10485760
```
2）创建错了，移除卷
```
# ubi remove ubi_Config
Remove UBI volume ubi_Config (id 0)
#
``` 
3）重新创建ubi卷



##### 7、查看ubi设备信息
`ubi info`
##### 8、查看ubi卷布局
`ubi info l`


#### btest 工具

##### yt8531 读写

读写 gphy0 连接的 yt8531 :
```
btest mdio1 r 0 0x0
bsp_mdio1_read unit(0) offset(0x0) value(0x1140)

btest mdio1 w 0 0x0 0x9140
```

扩展寄存器读写方法：
```
# 读 0x27 地址
btest mdio1 w 0 0x1e 0x27
btest mdio1 r 0 0x1f

# 给 0x27 地址写 0xAAA
btest mdio1 w 0 0x1e 0x27
btest mdio1 w 0 0x1f 0xAAA

```

```
btest mdio1 w 0 0x1e 0xa001
btest mdio1 r 0 0x1f

btest mdio1 w 0 0x1e 0xa001
btest mdio1 w 0 0x1f 0x8147
```



##### 环回配置

```
btest mdio1 w 0 0x1e 0x27
btest mdio1 w 0 0x1f 
```


##### UTP 收发包计数

打开收发包统计：
```
ext reg 0xA0.[15:14]=2’b10
```

关闭收发包统计：
```
ext reg 0xA0.[15:14]=2’b01
```

先读 0xA0 地址值：
```
btest mdio1 w 0 0x1e 0xA0
btest mdio1 r 0 0x1f

# 读值如下
bsp_mdio1_read unit(0) offset(0x1f) value(0x68d0)
```

然后打开收发包统计：
```
btest mdio1 w 0 0x1e 0xA0
btest mdio1 w 0 0x1f 0xA8D0
```

停止打流，然后关闭收发包统计：
```
btest mdio1 w 0 0x1e 0xA0
btest mdio1 w 0 0x1f 0x68D0
```


从 MDI 收到的正确报文数量（0xA3~0xA8），CRC 错误报文数量（0xA9~0xAC），寄存器读清：
```
btest mdio1 w 0 0x1e 0xA3
btest mdio1 r 0 0x1f

btest mdio1 w 0 0x1e 0xA4
btest mdio1 r 0 0x1f

btest mdio1 w 0 0x1e 0xA5
btest mdio1 r 0 0x1f

btest mdio1 w 0 0x1e 0xA6
btest mdio1 r 0 0x1f

btest mdio1 w 0 0x1e 0xA7
btest mdio1 r 0 0x1f

btest mdio1 w 0 0x1e 0xA8
btest mdio1 r 0 0x1f




btest mdio1 w 0 0x1e 0xA9
btest mdio1 r 0 0x1f

btest mdio1 w 0 0x1e 0xAA
btest mdio1 r 0 0x1f

btest mdio1 w 0 0x1e 0xAB
btest mdio1 r 0 0x1f

btest mdio1 w 0 0x1e 0xAC
btest mdio1 r 0 0x1f

```

从 RGMII 收到的正确报文数量（0xAD~0xB2），CRC 错误报文数量（0xB3~0xB6），寄存器读清：
```
btest mdio1 w 0 0x1e 0xAD
btest mdio1 r 0 0x1f

btest mdio1 w 0 0x1e 0xAE
btest mdio1 r 0 0x1f

btest mdio1 w 0 0x1e 0xAF
btest mdio1 r 0 0x1f

btest mdio1 w 0 0x1e 0xB0
btest mdio1 r 0 0x1f

btest mdio1 w 0 0x1e 0xB1
btest mdio1 r 0 0x1f

btest mdio1 w 0 0x1e 0xB2
btest mdio1 r 0 0x1f




btest mdio1 w 0 0x1e 0xB3
btest mdio1 r 0 0x1f

btest mdio1 w 0 0x1e 0xB4
btest mdio1 r 0 0x1f

btest mdio1 w 0 0x1e 0xB5
btest mdio1 r 0 0x1f

btest mdio1 w 0 0x1e 0xB6
btest mdio1 r 0 0x1f
```




##### FIBER 收发包计数

打开收发包统计：
```
ext reg 0x1A0.[15:14]=2’b10
```

关闭收发包统计：
```
ext reg 0x1A0.[15:14]=2’b01
```

先读 0xA0 地址值：
```
btest mdio1 w 0 0x1e 0x1A0
btest mdio1 r 0 0x1f

```

然后打开收发包统计：
```
btest mdio1 w 0 0x1e 0x1A0
btest mdio1 w 0 0x1f 0x8000
```

停止打流，然后关闭收发包统计：
```
btest mdio1 w 0 0x1e 0x1A0
btest mdio1 w 0 0x1f 0x0
```


从 MDI 收到的正确报文数量（0x1A3~0x1A8），CRC 错误报文数量（0x1A9~0x1AC），寄存器读清：
```
btest mdio1 w 0 0x1e 0x1A3
btest mdio1 r 0 0x1f

btest mdio1 w 0 0x1e 0x1A4
btest mdio1 r 0 0x1f

btest mdio1 w 0 0x1e 0x1A5
btest mdio1 r 0 0x1f

btest mdio1 w 0 0x1e 0x1A6
btest mdio1 r 0 0x1f

btest mdio1 w 0 0x1e 0x1A7
btest mdio1 r 0 0x1f

btest mdio1 w 0 0x1e 0x1A8
btest mdio1 r 0 0x1f




btest mdio1 w 0 0x1e 0x1A9
btest mdio1 r 0 0x1f

btest mdio1 w 0 0x1e 0x1AA
btest mdio1 r 0 0x1f

btest mdio1 w 0 0x1e 0x1AB
btest mdio1 r 0 0x1f

btest mdio1 w 0 0x1e 0x1AC
btest mdio1 r 0 0x1f

```

从 RGMII 收到的正确报文数量（0x1AD~0x1B2），CRC 错误报文数量（0x1B3~0x1B6），寄存器读清：
```
btest mdio1 w 0 0x1e 0x1AD
btest mdio1 r 0 0x1f

btest mdio1 w 0 0x1e 0x1AE
btest mdio1 r 0 0x1f

btest mdio1 w 0 0x1e 0x1AF
btest mdio1 r 0 0x1f

btest mdio1 w 0 0x1e 0x1B0
btest mdio1 r 0 0x1f

btest mdio1 w 0 0x1e 0x1B1
btest mdio1 r 0 0x1f

btest mdio1 w 0 0x1e 0x1B2
btest mdio1 r 0 0x1f




btest mdio1 w 0 0x1e 0x1B3
btest mdio1 r 0 0x1f

btest mdio1 w 0 0x1e 0x1B4
btest mdio1 r 0 0x1f

btest mdio1 w 0 0x1e 0x1B5
btest mdio1 r 0 0x1f

btest mdio1 w 0 0x1e 0x1B6
btest mdio1 r 0 0x1f
```




#### 背板端口验证

- 子卡插在 3 槽（左边槽位）上时，7118 对应 serdes 8、9、10、11，gport 分别是24、25、26、27，硬件连接对应的 RTL9617 分别是GPHY1、GPHY0、10GPON、P6 SDS；
- 子卡插在 4 槽（右边槽位）上时，7118 对应 serdes 4、5、6、7；

四个 serdes 模式分别是 SGMII、SGMII、XFI、2.5G，交换芯片的限制是 8 个 serdes 一组时，速率模式不能超过 3 种，分两个小组，4个一组中不能超过 2 种速率。


GPHY0-3 --> 0~3
10GPON--> 7
PX SDS 就是 port X

LAN 口可以看這個
```
diag rt_port get status port all
```

插拔有 log，可以用 eth0.x 對照
```
cat /proc/ni/dev_port_mapping

# cat /proc/ni/dev_port_mapping
DEV ability:
 eth0.2
 eth0.3
 eth0.4
 eth0.5
 eth0.6
 eth0.7
 eth0.8
 nas0

rx: phyPort -> dev[the packet rx from phyPort will send to kernel using dev]
Root DEV: eth0
port0 -> eth0.6 , status=1
port1 -> eth0.3 , status=255
port2 -> eth0.4 , status=255
port3 -> eth0.5 , status=255
port4 -> eth0.7 , status=255
port5 -> eth0.8 , status=255
port6 -> eth0.2 , status=255
port7 -> nas0 , status=255

Root DEV: nas0
port0 -> eth0.6 , status=1
port1 -> eth0.3 , status=255
port2 -> eth0.4 , status=255
port3 -> eth0.5 , status=255
port4 -> eth0.7 , status=255
port5 -> eth0.8 , status=255
port6 -> eth0.2 , status=255
port7 -> nas0 , status=255

tx:dev -> txPortMask[when tx from dev, we will use this txPortMask]
eth0.2 -> port 6
eth0.3 -> port 0
eth0.4 -> port 0
eth0.5 -> port 0
eth0.6 -> port 0
eth0.7 -> port 4
eth0.8 -> port 5
nas0 -> port 7

=============================================================
Purpose: Configure NI driver multiple LAN device port mapping
READ Usage: cat dev_port_mapping
WRITE Usage: echo [num_of_port dev_name] > dev_port_mapping
=============================================================
# 
```



接口强制down：
```
port set phy-force-power-down port 0 state enable
```

###### 10GPON

配置命令配置 10GPON 口模式为 XFI：
```
rt_ponmisc init
rt_ponmisc set mode 255 speed 0
rt_ponmisc set mode 3 speed 4
```

第二条命令配置后会周期性打印错误，原因是 OMCI 没有关闭，需要修改启动脚本。这条命令是必须的，否则第三条直接配置 xfi 的命令会返回参数invalid，配置前获取 10GPON 口模式：
```
RTK.0> rt_ponmisc get mode 
============================================================
 PON MODE : XG-PON
============================================================
RTK.0> 
```

配置后获取的接口模式正确：
```
RTK.0> rt_ponmisc get mode            
============================================================
 PON MODE : XFI : 10G
============================================================
RTK.0>
```

查看接口状态，需要配置10GPON 口模式为 XFI 后才能 show 出接口状态，否则提示非以太口报错：
```
# 
# app-cli 
                            

       .o88b.  .d88b.  d8888b. d888888b d888888b d8b   db  .d8b.  
      d8P  Y8 .8P  Y8. 88  `8D `  88  '   `88'   888o  88 d8' `8b 
      8P      88    88 88oobY'    88       88    88V8o 88 88ooo88 
      8b      88    88 88`8b      88       88    88 V8o88 88   88 
      Y8b  d8 `8b  d8' 88 `88.    88      .88.   88  V888 88   88 
       `Y88P'  `Y88P'  88   YD    YP    Y888888P VP   V8P YP   YP 


Cortina>
Cortina>en
Cortina>enable 
Cortina#
Cortina#config 
Cortina#aal 
Cortina(config-aal)#
Cortina(config-aal)#psds
Cortina(config-aal-psds)#show eth_link_status 0 
link_up                       :  0x00000001 
Cortina(config-aal-psds)#
```


命令行配置模式时，对应的 mode 和 speed 参数参考：
```
extern int32
rt_ponmisc_modeSpeed_get(rt_ponmisc_ponMode_t *pPonMode,rt_ponmisc_ponSpeed_t *pPonSpeed);

typedef enum{
    RT_GPON_MODE,
    RT_EPON_MODE,
    RT_NGPON2_MODE,
    RT_XFI_MODE,                /* For XFI/10G */
    RT_FIBER_MODE,              /* For 100BASE-X, 1000BASE-X, 2500BASE-X */
    RT_SGMII_MODE,              /* For SGMII/1G, HiSGMII/2.5G */
    RT_USXGMII_MODE,    /* For USXGMII/10G */
    RT_PONMODE_END,
}rt_ponmisc_ponMode_t;

typedef enum{
    RT_100M_SPEED,                      /* Speed: 100Mbps */
    RT_1G_SPEED,                        /* Speed: 1Gbps */
    RT_2DOT5G_SPEED,            /* Speed: 2.5Gbps */
    RT_5G_SPEED,                        /* Speed: 5Gbps */
	RT_10G_SPEED,                       /* Speed: 10Gbps */
    RT_10M_SPEED,                       /* Speed: 10Mbps */
    RT_500M_SPEED,                      /* Speed: 500Mbps */
    RT_ETHSPEED_END,
}rt_ponmisc_ethSpeed_t;

```


修改启动脚本，避免设置 PON mode，rtk给的XFI範例: 
	PON_MODE=3
	PON_SUB_MODE=0
	PON_SPEED=4
	啟動文件（modutils.sh/runsdk.sh）會去設定PON mode, 帶起相關的APP

编译文件系统时，会拷贝生成/etc/scripts/modutils.sh，这里面修改ca_rtk_pon_set_mode() 中的 rt_pon_mode 值，系统启动后，10GPON 口模式变成 XFI。
PON_MODE 的值在这里时是 1，也就是 GPON 模式，这个哪里配置的？可以在 PON_MODE=1 条件下修改 rt_pon_mode = 3，也就是 XFI 模式，pon_speed 在这里时本来已经是 1，也就是 10G-asym，不需要修改。（这个地方修改不是最开始的，因为modutils.sh 里面获取的 PON_MODE 已经是 GPON 模式了，所以需要最原始的模式配置）

```
# cat /proc/ca_rtk/ponmisc 
echo init > /proc/ca_rtk/ponmisc : diag rt_ponmisc init
echo bstPolRvs $state > /proc/ca_rtk/ponmisc : diag rt_ponmisc set burstPolarityReverse state [enable|disable]
  $state=1 means enable, $state=0 means disable
echo modSpd $pon_mode $pon_speed > /proc/ca_rtk/ponmisc : diag rt_ponmisc set mode $pon_mode speed $pon_speed
  $pon_mode=0/1/2/3/4/5/6 means GPON/EPON/NGPON2/XFI/FIBER/SGMII/USXGMII
  $pon_speed=0/1/2/3/4 means 2.5G/10G-asym/10G-sym/NGPON-asym/NGPON-sym
```


**默认模式的配置有两种方法**
（1）產測時候用 MIB 命令配置，系统已经启动并且跑了软件
```
mib set PON_MODE 3
mib commit hs
```

（2）preconfig 內的 config_default_hs.xml 裡面修改。這樣改指針對乾淨的 flash 有效。

这个 xml 文件路径在：`os_rtl/sdk/sdk/vendors/Realtek/luna/conf510/9617C_demo_Board-CTC_8832b_8192xb/`，对照下面的数值进行修改：
```
PON_MODE配合9607系列做一下同步調整:
         PON_MODE = 0 <-- None, 用來當作不啟用PON port的設定
         PON_MODE = 1 <-- GPON type WAN
         PON_MODE = 2 <-- EPON type WAN
         PON_MODE = 3 <-- Ether type WAN
EPON/GPON type WAN的speed:
           PON_SPEED = 0 <-- EPON/GPON
           PON_SPEED = 1 <-- 下行10G, 非對稱(XGPON, 10G/1G EPON)
           PON_SPEED = 2 <-- 上下行皆10G (XGSPON, 10G/10G EPON)
針對Ether type WAN新增加PON_SUB_MODE來識別, 後續針對NG-PON2亦可用來作為GPON mode的擴增:
           PON_SUB_MODE = 0 <-- XFI mode, 目前只有10G, (traditional GPON modes)
           PON_SUB_MODE = 1 <-- FIBER mode, for 100BASE-X, 1000BASE-X, 2500BASE-X, (NG-PON2)
           PON_SUB_MODE = 2 <-- SGMII mode, for SGMII/1G/100M/10M, HiSGMII/2.5G
           PON_SUB_MODE = 3 <-- USXGMII mode, 10G/5G/2.5G/1G/100M/10M
Ether type WAN的speed:
           PON_SPEED = 0  <-- 100M
           PON_SPEED = 1  <-- 1G
           PON_SPEED = 2  <-- 2.5G
           PON_SPEED = 3  <-- 5G
           PON_SPEED = 4  <-- 10G
           PON_SPEED = 5  <-- 10M

```


> 修改 config_default_hs.xml 后，擦掉 flash，重新升级 system 版本，起来后配置 `mib set PON_MODE 3` 和 `mib commit hs`，然后重启，再看板卡启动后就没有打印刷屏了。


###### GPHY0~1

gphy0 接 yt8531，7118 侧三槽插子卡时，对应 ctcport 24，需要配置为 sgmii 自协商模式，gphy0 配置自协商。通过 mdio1 读写 phy 寄存器，对应unit=0：
```
btest mdio1 r 0 0x0
btest mdio1 r 0 0x1
```

启动脚本里面对 phy 0x6 进行复位，但是起来后 mdio1 读不到，手动复位后正常：
```
btest gpio w 15 0
btest gpio w 15 1
```


gphy1 接yt8211，7118 侧三槽插子卡时，对应 ctcport 25，需要配置为 sgmii 自协商模式，gphy1 配置自协商。通过 mdio1 读写 phy 寄存器，对应unit=1：
```
btest mdio1 r 1 0x0
btest mdio1 r 1 0x1
```

测试板卡上gphy1 没有接到 8211，而是直出的 RJ45 网口用来调试。


配置端口 7 收包直接从 port0 转发：
```
rt_cls clear ext-rule
rt_cls set ext-rule filter igrPort data 7 mask 0x3f
rt_cls set ext-rule action fwd ldpid 0
rt_cls add ext-rule index 0
```

如果要配置反向转发，增加另一个index：
```
rt_cls clear ext-rule
rt_cls set ext-rule filter igrPort data 0 mask 0x3f
rt_cls set ext-rule action fwd ldpid 7
rt_cls add ext-rule index 10
```





rt_port get unitype port all
rt_l2-table get limit-learning port all count
l2-table get lookup-miss port all unicast action




###### P6 SDS

支持 1G（SGMII） 和 2.5G（HSGMII） 两种速率，ctc7118 侧查看 serdes 8、9、10、11 对应端口 24、25、26、27；serdes 4、5、6、7 对应端口 8、9、10、11。配置 port 27 为 2.5G 模式：
```
port 27 if-mode 2500 2500X
port 27 port-en enable
port 27 mac enable
```

rtl9617 侧找到 P6 SDS 对应的端口 6，配置 hsgmii 模式：
```
port set serdes port 6 hsgmii-mac n-way auto
或者强制
port set serdes port 6 hsgmii-mac n-way force

# 设置千兆sgmii模式
port set serdes port 6 sgmii-mac n-way force
```

然后查看两侧端口 link 状态，rtl9617 查看端口 P6 状态：
```
RTK.0> port get status port 6
Port Status Speed    Duplex TX_FC RX_FC
---- ------ -----    ------ ----- -----
6    Up     2.5G     Full   Dis   Dis  
```

ctc7118 侧查看 port 27 状态：
```
CTC_CLI(ctc-sdk)# show port mac-link 
 ----------------------------------------------------------------------------------------------------
 GPort   LPort MAC   Link   MAC-EN   Speed  Fec     Duplex  Auto-Neg  Interface     Rx-Rate   Tx-Rate
 ----------------------------------------------------------------------------------------------------
 0x0000  0     0     down   TRUE     1G     -      FD      TRUE      QSGMII         0 bps     0 bps   
 0x0001  1     1     down   TRUE     1G     -      FD      TRUE      QSGMII         0 bps     0 bps   
 0x0002  2     2     down   TRUE     1G     -      FD      TRUE      QSGMII         0 bps     0 bps   
 0x0003  3     3     down   TRUE     1G     -      FD      TRUE      QSGMII         0 bps     0 bps   
 0x0004  4     4     up     TRUE     1G     -      FD      TRUE      QSGMII         0 bps     0 bps   
 0x0005  5     5     up     TRUE     1G     -      FD      TRUE      QSGMII         0 bps     0 bps   
 0x0006  6     6     up     TRUE     1G     -      FD      TRUE      QSGMII         0 bps     0 bps   
 0x0007  7     7     up     TRUE     1G     -      FD      TRUE      QSGMII         0 bps     0 bps   
 0x0008  8     8     up     TRUE     10G    None   FD      FALSE     XFI            0 bps     0 bps   
 0x0009  9     9     up     TRUE     10G    None   FD      FALSE     XFI            0 bps     0 bps   
 0x000A  10    10    down   TRUE     10G    None   FD      FALSE     XFI            0 bps     0 bps   
 0x000B  11    11    down   TRUE     10G    None   FD      FALSE     XFI            0 bps     0 bps   
 0x000C  12    12    up     TRUE     10G    None   FD      FALSE     XFI            0 bps     0 bps   
 0x000D  13    13    up     TRUE     10G    None   FD      FALSE     XFI            0 bps     0 bps   
 0x000E  14    14    down   TRUE     10G    None   FD      FALSE     XFI            0 bps     0 bps   
 0x000F  15    15    down   TRUE     10G    None   FD      FALSE     XFI            0 bps     0 bps   
 0x0010  16    16    up     TRUE     1G     -      FD      TRUE      QSGMII         0 bps     0 bps   
 0x0011  17    17    up     TRUE     1G     -      FD      TRUE      QSGMII         0 bps     0 bps   
 0x0012  18    18    up     TRUE     1G     -      FD      TRUE      QSGMII         0 bps     0 bps   
 0x0013  19    19    up     TRUE     1G     -      FD      TRUE      QSGMII         0 bps     0 bps   
 0x0014  20    20    up     TRUE     1G     -      FD      TRUE      QSGMII         0 bps     0 bps   
 0x0015  21    21    up     TRUE     1G     -      FD      TRUE      QSGMII         0 bps     0 bps   
 0x0016  22    22    up     TRUE     1G     -      FD      TRUE      QSGMII         0 bps     0 bps   
 0x0017  23    23    up     TRUE     1G     -      FD      TRUE      QSGMII         0 bps     0 bps   
 0x0018  24    24    down   FALSE    10G    None   FD      FALSE     XFI            0 bps     0 bps   
 0x0019  25    25    down   FALSE    10G    None   FD      FALSE     XFI            0 bps     0 bps   
 0x001A  26    26    down   TRUE     10G    None   FD      FALSE     XFI            0 bps     0 bps   
 0x001B  27    27    up     TRUE     2.5G   -      FD      TRUE      2500X          0 bps     0 bps   
 0x003C  60    60    down   TRUE     10G    None   FD      FALSE     XFI            0 bps     0 bps   
 0x003D  61    61    down   TRUE     10G    None   FD      FALSE     XFI            0 bps     0 bps   
 ----------------------------------------------------------------------------------------------------

```



```
ca_event_port_link_t link_event;
/* Send port link change event */
    memset(&link_event, 0, sizeof(ca_event_port_link_t));
    link_event.port_id = port;
    link_event.status  = status.link_up ? CA_PORT_LINK_UP : CA_PORT_LINK_DOWN ;
    ca_event_send(0, CA_EVENT_ETH_PORT_LINK, (void *)&link_event, sizeof(ca_event_port_link_t));
```

##### 收发包统计

查看指定端口的所有收发包统计：
```
RTK.0> mib dump counter port 6 
Port: 6
ifInOctets                         :                   2427178
ifInUcastPkts                      :                     28223
ifInMulticastPkts                  :                         0
ifInBroadcastPkts                  :                         0
ifInDiscards                       :                         0
ifOutOctets                        :                         0
ifOutDiscards                      :                         0
ifOutUcastPkts                     :                         0
ifOutMulticastPkts                 :                         0
ifOutBroadcastPkts                 :                         0
dot1dPortDelayExceedDiscards       :                         0
......
RTK.0>
```


只查看指定接口的收包或者发包计数：
```
RTK.0> mib dump counter port 6 ifInUcastPkts
Port: 6
ifInUcastPkts: 49266
RTK.0> 
RTK.0> 
RTK.0> mib dump counter port 7 ifOutUcastPkts
Port: 6
ifOutUcastPkts: 0
```


##### 透传配置（指定端口到端口）

配置端口 7 收包直接从端口 6 转发：
```
rt_cls clear ext-rule
rt_cls set ext-rule filter igrPort data 7 mask 0x3f
rt_cls set ext-rule action fwd ldpid 6
rt_cls add ext-rule index 0
```

如果要配置反向转发，增加另一个index：
```
rt_cls clear ext-rule
rt_cls set ext-rule filter igrPort data 6 mask 0x3f
rt_cls set ext-rule action fwd ldpid 7
rt_cls add ext-rule index 10
```


需要注意系统启动已经配置的规则，避免 index 冲突，查看已有规则：
```
rt_cls get ext-rule index valid
```

删除规则：
```
rt_cls delete ext-rule index 1
rt_cls delete ext-rule index 2
rt_cls delete ext-rule index 3
rt_cls delete ext-rule index 4
rt_cls delete ext-rule index 510
rt_cls delete ext-rule index 511
```


另外，还需要关掉未知单播的限速，否则超过 1.5M 会有丢包：
```
rt_rate set storm-control unknown-unicast port all state disable
```


只配置指定端口转发会出现256字节几分钟丢一个包，其他字节长度报文开始就丢将近50%包，配置下面两个设置：
```
l2-table set limit-learning port 6 count 2000
l2-table set limit-learning port 7 count 2000

app-cli
enable
config
aal
fdb
ctrl 0 lrn_mode 0
```

或者配置下面两个命令：（跟上面类似，解决mac漂移导致的丢包）
```
app-cli
enable
config
aal
fdb
lrn_fwd_ctrl 0 sm_tmp_entry_fwd 1
lrn_fwd_ctrl 0 sm_sm_entry_fwd 1
```




##### 调试命令

1、查看丢包，这个命令是读清的，在确定有丢包时，重复打流前先 cat 清空计数，打流后再 cat 看结果：
```
cat /proc/fc/hw_dump/dropcount
```


2、设置 mac 学习limit：
```
l2-table set limit-learning port 6 count 2000
```



##### 打流验证

![](Pasted%20image%2020250717185559.png)

9617 配置
```
port set serdes port 6 hsgmii-mac n-way auto

rt_ponmisc init
rt_ponmisc set mode 255 speed 0
rt_ponmisc set mode 3 speed 4

rt_cls clear ext-rule
rt_cls set ext-rule filter igrPort data 7 mask 0x3f
rt_cls set ext-rule action fwd ldpid 6
rt_cls add ext-rule index 0

rt_cls set ext-rule filter igrPort data 6 mask 0x3f
rt_cls set ext-rule action fwd ldpid 7
rt_cls add ext-rule index 10

rt_rate set storm-control unknown-unicast port all state disable

app-cli
enable
config
aal
fdb
lrn_fwd_ctrl 0 sm_tmp_entry_fwd 1
lrn_fwd_ctrl 0 sm_sm_entry_fwd 1

```


7118 配置：
```
port 27 if-mode 2500 2500X
port 27 port-en enable
port 27 mac enable

port 26 port-en enable

vlan add port 27 vlan 10

show nexthop brguc port 61
Gport:0x003d L2Uc Nexthop Id:2147483770

show nexthop brguc port 26
Gport:0x001a L2Uc Nexthop Id:2147483700

port 61 port-cross-connect nhid 2147483700
port 26 port-cross-connect nhid 2147483770

```


#### IBC 验证

先确认背板硬件连接关系：
- 子卡插在 3 槽（左边槽位）上时，7118 对应 serdes 8、9、10、11，gport 分别是24、25、26、27，硬件连接对应的 RTL9617 分别是GPHY1、GPHY0、10GPON、P6 SDS；
- 子卡插在 4 槽（右边槽位）上时，7118 对应 serdes 4、5、6、7；
```
# 7118 侧三个接口都配置sgmii模式
port 24 if-mode 1G SGMII
port 24 port-en enable
port 24 mac enable

port 25 if-mode 1G SGMII
port 25 port-en enable
port 25 mac enable

port 27 if-mode 1G SGMII
port 27 port-en enable
port 27 mac enable


```

4xg上已经配置了 snmp 口通过 knet 上 cpu，ibc 需要配置另外一个交换口通过 knet 上 cpu，ibc 通信使用 port 25 与 RTL9617 GPHY0 连接的通道：
```
packet netif create eth-ibc port 25 000e.5e00.0011

qos cpu-reason 601 map-to queue-id 2 reason-group 15
qos cpu-reason 601 dest-to local-cpu

nexthop add misc cpu-reason 0x5402 reason-id 601

port 25 port-cross-connect nhid 0x5402


# 配置网口 ibc0
vconfig add eth-ibc 1026
ip link set eth-ibc.1026 down
ip link set eth-ibc.1026 name ibc0
ip link set ibc0 up

```

RTL9617 上删除规则：
```
rt_cls delete ext-rule index 1
rt_cls delete ext-rule index 2
rt_cls delete ext-rule index 3
rt_cls delete ext-rule index 4
rt_cls delete ext-rule index 510
rt_cls delete ext-rule index 511
```

##### 验证硬件通道数据收发

1、验证从 RTL9617 的 GPHY0 出去的报文是否能通过 7118 的 port 25 从 knet 上送 7118 的 cpu，报文流向，7118 port27 -> 9617 P6 SDS -> 9617 GPHY0 -> 7118 port25 -> 7118 cpu （上面创建的网口eth-ibc）

配置端口 6 收包直接从端口 0 转发出去：
```
rt_cls clear ext-rule
rt_cls set ext-rule filter igrPort data 6 mask 0x3f
rt_cls set ext-rule action fwd ldpid 0
rt_cls add ext-rule index 0
```

另外，还需要关掉未知单播的限速，否则超过 1.5M 会有丢包：
```
rt_rate set storm-control unknown-unicast port all state disable
```

2、验证 RTL9617 GPHY0 收到的报文是否能正常上送 9617 的 cpu：

通过下面的命令查看 GPHY0 对应的网口是 eth 0.6：
```
cat /proc/ni/dev_port_mapping
```

报文流向：7118 port25 -> 9617 GPHY0 -> 9617 cpu（ifconfig eth0.6查看收发计数）


##### tipc 配置

主控上需要注意tipc 的配置要在创建 netif 口，配置好 ibc0 网口之后。

RTL9617 上需要删除默认的 cls 规则，不然 tipc 报文到 GPHY0 口后由于没有 hit 上 cls 规则走默认的丢弃规则 cls 511
```
[2025-09-10 14:25:07] RTK.0>
[2025-09-10 14:25:07] RTK.0>rt_cls get ext-rule index valid
[2025-09-10 14:25:07] ***************************************
[2025-09-10 14:25:07] Index: 1
[2025-09-10 14:25:07] Valid: 1
[2025-09-10 14:25:07] Filter:
[2025-09-10 14:25:07]     Direction: Upstream
[2025-09-10 14:25:07]     Egress Port: (0,0x3f)
[2025-09-10 14:25:07] Action:
[2025-09-10 14:25:08]     Stag Action: Transparent
[2025-09-10 14:25:08]     Ctag Action: Transparent
[2025-09-10 14:25:08]     Flow ID action  : Nop
[2025-09-10 14:25:08]     Group ID action : Nop
[2025-09-10 14:25:08]     Forward action  : Nop
[2025-09-10 14:25:08]     COS action      : Nop
[2025-09-10 14:25:08] ***************************************
[2025-09-10 14:25:08] Index: 2
[2025-09-10 14:25:08] Valid: 1
[2025-09-10 14:25:08] Filter:
[2025-09-10 14:25:08]     Direction: Upstream
[2025-09-10 14:25:08]     Egress Port: (1,0x3f)
[2025-09-10 14:25:08] Action:
[2025-09-10 14:25:08]     Stag Action: Transparent
[2025-09-10 14:25:08]     Ctag Action: Transparent
[2025-09-10 14:25:08]     Flow ID action  : Nop
[2025-09-10 14:25:08]     Group ID action : Nop
[2025-09-10 14:25:08]     Forward action  : Nop
[2025-09-10 14:25:08]     COS action      : Nop
[2025-09-10 14:25:08] ***************************************
[2025-09-10 14:25:08] Index: 3
[2025-09-10 14:25:08] Valid: 1
[2025-09-10 14:25:08] Filter:
[2025-09-10 14:25:08]     Direction: Upstream
[2025-09-10 14:25:08]     Egress Port: (2,0x3f)
[2025-09-10 14:25:08] Action:
[2025-09-10 14:25:08]     Stag Action: Transparent
[2025-09-10 14:25:08]     Ctag Action: Transparent
[2025-09-10 14:25:08]     Flow ID action  : Nop
[2025-09-10 14:25:08]     Group ID action : Nop
[2025-09-10 14:25:08]     Forward action  : Nop
[2025-09-10 14:25:08]     COS action      : Nop
[2025-09-10 14:25:08] ***************************************
[2025-09-10 14:25:08] Index: 4
[2025-09-10 14:25:08] Valid: 1
[2025-09-10 14:25:08] Filter:
[2025-09-10 14:25:08]     Direction: Upstream
[2025-09-10 14:25:08]     Egress Port: (3,0x3f)
[2025-09-10 14:25:08] Action:
[2025-09-10 14:25:08]     Stag Action: Transparent
[2025-09-10 14:25:09]     Ctag Action: Transparent
[2025-09-10 14:25:09]     Flow ID action  : Nop
[2025-09-10 14:25:09]     Group ID action : Nop
[2025-09-10 14:25:09]     Forward action  : Nop
[2025-09-10 14:25:09]     COS action      : Nop
[2025-09-10 14:25:09] ***************************************
[2025-09-10 14:25:09] Index: 510
[2025-09-10 14:25:09] Valid: 1
[2025-09-10 14:25:09] Filter:
[2025-09-10 14:25:09]     Direction: Upstream
[2025-09-10 14:25:09]     Packet Type: Unknown Unicast
[2025-09-10 14:25:09] Action:
[2025-09-10 14:25:09]     Stag Action: Transparent
[2025-09-10 14:25:09]     Ctag Action: Transparent
[2025-09-10 14:25:09]     Flow ID action  : Nop
[2025-09-10 14:25:09]     Group ID action : Nop
[2025-09-10 14:25:09]     Forward action  : MCGID 0x1
[2025-09-10 14:25:09]     COS action      : Nop
[2025-09-10 14:25:09] ***************************************
[2025-09-10 14:25:09] Index: 511
[2025-09-10 14:25:09] Valid: 1
[2025-09-10 14:25:09] Filter:
[2025-09-10 14:25:09] Action:
[2025-09-10 14:25:09]     Stag Action: Transparent
[2025-09-10 14:25:09]     Ctag Action: Transparent
[2025-09-10 14:25:09]     Flow ID action  : Nop
[2025-09-10 14:25:09]     Group ID action : Nop
[2025-09-10 14:25:09]     Forward action  : Drop 0x0
[2025-09-10 14:25:09]     COS action      : Nop
[2025-09-10 14:25:10] RTK.0>
[2025-09-10 14:25:10] RTK.0>
```


##### 使用 P6 SDS 口作为管理口时 ibc 不通

网卡 ibc0 是在网卡 eth0.2 上创建的子接口，通过ifconfig 查看网卡都是正常 up，但是实际使用时无法正常收发报文。这个时候就需要使用 `ip -d link show`命令查看详细的网卡状态信息：
```
# 
# ip -d link show eth0.2
7: eth0.2: <NO-CARRIER,BROADCAST,MULTICAST,UP> mtu 1500 qdisc pfifo_fast state DOWN mode DEFAULT group default qlen 1000
    link/ether 00:0e:5e:62:ee:ee brd ff:ff:ff:ff:ff:ff permaddr 00:00:01:00:02:00 promiscuity 1 minmtu 60 maxmtu 9022 addrgenmode eui64 numtxqueues 1 numrxqueues 1 gso_max_size 65536 gso_max_segs 65535 
# 
# ip -d link show ibc0
18: ibc0@eth0.2: <NO-CARRIER,BROADCAST,MULTICAST,UP> mtu 1500 qdisc noqueue state LOWERLAYERDOWN mode DEFAULT group default qlen 1000
    link/ether 20:20:02:25:15:03 brd ff:ff:ff:ff:ff:ff promiscuity 0 minmtu 0 maxmtu 65535 
    vlan protocol 802.1Q id 1026 <REORDER_HDR> addrgenmode eui64 numtxqueues 1 numrxqueues 1 gso_max_size 65536 gso_max_segs 65535
```

可以看到子接口 ibc0 和父接口 eth0.2 的状态都有问题，NO-CARRIER 表示物理链路有问题。最后定位是 P6 SDS 这个口没有通知 nic driver carrier on，需要在接口 link 时通知：
```
ca_event_port_link_t link_event;
/* Send port link change event */
memset(&link_event, 0, sizeof(ca_event_port_link_t));
link_event.port_id = CA_PORT_ID(CA_PORT_TYPE_ETHERNET, 6);
link_event.status  = 1;//status.link_up ? CA_PORT_LINK_UP : CA_PORT_LINK_DOWN ;
ca_event_send(0, CA_EVENT_ETH_PORT_LINK, (void *)&link_event, sizeof(ca_event_port_link_t));
```

这个口起来后硬件需要始终保持 link，所以可以在 sdk 初始化时直接通知驱动 carrier on 即可。



#### 主控读取中断和 ready 信号

子卡1  sys_ok 读，子卡1（slot3）给出 gpio 97 低有效 sys_ok 信号
```
btest spi firmware w 0 0xC5 1 0x00 
btest spi firmware r 0 0xC4 1 
```


子卡2  sys_ok 读，子卡2（slot4）给出 gpio 97 低有效 sys_ok 信号
```
btest spi firmware w 0 0xC8 1 0x00 
btest spi firmware r 0 0xC7 1 
```


```
btest spi firmware w 0 0x28 1 0xff
btest spi firmware r 0 0x2b 1 
```

中断信号，bit0 和 bit1 低有效，子卡1（slot3）给出 gpio 96 低有效的中断信号后，主控 bit0 读到期望为 0
```
btest spi firmware r 0 0x56 1 
```

子卡写 gpio 96 是触发中断信号给主控，写 gpio 97 是触发 sys_ok 信号给主控
```
btest gpio w 96 0
btest gpio w 97 0
```


#### 子卡通过主控 cpld 判断所在槽位

B7  GPIO3[6]，gpio num 102
D3   GPIO3[2]，gpio num 98

Slot1，左边槽位

16'h009B ;      写1  表示输出
16'h009C ;      写1 表示输出低电平    CPU_GPIO3[6]为低电平 ;   写2 表示输出高电平  CPU_GPIO3[6]为高电平

16'h00D1 ;    写1  表示输出
16'h00D2 ;    写1 表示输出低电平      CPU_GPIO3[2]为低电平 ;   写2 表示输出高电平  CPU_GPIO3[2]为高电平

```
btest spi firmware w  0 0x9b 1 0x1
btest spi firmware w  0 0x9c 1 0x1

btest spi firmware w  0 0xd1 1 0x1
btest spi firmware w  0 0xd2 1 0x1
```


Slot2，右边槽位

16'h009E ;     写1  表示输出
16'h009F ;     写1 表示输出低电平    CPU_GPIO3[6]为低电平 ;   写2 表示输出高电平   CPU_GPIO3[6]为高电平

16'h00D4 ;   写1  表示输出
16'h00D5 ;   写1 表示输出低电平      CPU_GPIO3[2]为低电平 ;   写2 表示输出高电平  CPU_GPIO3[2]为高电平

```
btest spi firmware w  0 0x9e 1 0x1
btest spi firmware w  0 0x9f 1 0x1

btest spi firmware w  0 0xd4 1 0x1
btest spi firmware w  0 0xd5 1 0x1
```

以上寄存器是通过写iTN331-E-4XG 主控的cpld 来输出高低电平,  5GW子卡通过这些状态来判断


#### USB3.0 和 10GPON 口透传配置

**USB加速**

- 參考 linux-4.4.x/drivers/net/usb/usbnet.c

	修改rx data path接入FC driver

	Key word:
	```
	#ifdef CONFIG_HWNAT_FLEETCONNTRACK
	    extern int rtk_fc_fastfwd_netif_rx(struct sk_buff *skb);
	    status = rtk_fc_fastfwd_netif_rx(skb);
	#else
	    status = netif_rx (skb);
	#endif
	```

- 參考: Realtek Fleet Conntrack Driver programming guide

	其中的6. Wlan/USB Acceleration

	Key word: 
	```
	echo del dev {devname} > /proc/fc/sw_dump/wlan_devmap
	
	# 5gw上网卡是 cell
	echo del dev cell > /proc/fc/sw_dump/wlan_devmap
	
	# 清除配置
	echo flush > /proc/fc/sw_dump/wlan_devmap
	
	```

	需注意要重新down/up netdev

- 以上若usb driver的skb data沒有其他私有header, 即可與FC快速建立加速關聯

	cat /proc/fc/sw_dump/wlan_devmap 要可以看到netdev
	基本封包要可以流通


**資料透傳**

- Plan A: 推薦最基本做法是HGU的流表加速, 上層軟轉可通, FC driver及學習
	BC/MC, unknown UC等需要額外探討軟轉or 特殊加速控制

- Plan B: ACL redirect rule進行全透傳，不修改封包，不需要其他細微Qos控制
	WIFI to ETH:
	```
	Key:
	RTK.0> rt_acl set pattern ingress_port_mask … 指定from 0x40000 (port 0x12, 參考 wlan_devmap 設定)
	
	Action:
	RTK.0> rt_acl set action FORWARD redirect port 7 指定to ETH port number
	```
	
	ETH to WIFI:
	```
	Key:
	RTK.0> rt_acl set pattern ingress_port_mask 0x80 … 指定from 0x80 (port7, XFI port)
	
	Action:
	RTK.0> rt_acl set action FORWARD redirect port 0x12 … 指定cpu port e.g. 0x12
	RTK.0> rt_acl set action FORWARD redirect flowid … 指定cpu port上的 dev mapping idx e.g. 11 (參考 wlan_devmap 設定)
	
	
	rt_cls clear ext-rule
	rt_cls set ext-rule filter igrPort data 7 mask 0x3f
	rt_cls set ext-rule action fwd ldpid 0x18
	rt_cls add ext-rule index 0
	
	```


**Note: assign flowid** **需patch FC driver**才可支援
```
[FC][77 Series] RT ACL support redirect with flowid

 - Special usage case for SFU
   - force redirect to wifi fast forward with wlan device index
   - force redirect to pon with pon stream id
 - Usage
   [DIAG]
    rt_acl set action FORWARD redirect flowid N
   [API]
    action_fields |= RT_ACL_ACTION_FORWARD_GROUP_REDIRECT_FLOWID_BIT
	action_forward_group_redirect_flowid
```

**若不熟悉RT ACL API**，可先參考API **文件**



```
# 这一条可以修改代码实现，不用 echo 配置
echo del dev cell > /proc/fc/sw_dump/wlan_devmap

diag
rt_acl set pattern ingress_port_mask 0x40000
rt_acl set action FORWARD redirect port 7
rt_acl add entry


rt_acl set pattern ingress_port_mask 0x80
rt_acl set action FORWARD redirect port 0x12
rt_acl set action FORWARD redirect flowid 54
rt_acl add entry

rt_cls clear ext-rule
rt_cls set ext-rule filter igrPort data 7 mask 0x3f
rt_cls set ext-rule action fwd ldpid 0x18
rt_cls add ext-rule index 100

# 查看配置的 acl 表项
cat /proc/fc/sw_dump/acl

```


```
echo 1 > /proc/fc/sw_dump/fwd_statistic
echo 1 > /proc/fc/sw_dump/smp_statistic

#打流
cat /proc/fc/sw_dump/fwd_statistic
cat /proc/fc/sw_dump/smp_statistic
```

抓測試包相關 debug 資訊，包含 header I & nic rx log
```
cat /proc/fc/hw_dump/headeri
echo 0x8 > /proc/ni/ni_debug ; sleep 3; echo 0 > /proc/ni/ni_debug
```

```
cat /proc/fc/ctrl/wan_port_mask
cat /proc/fc/hw_dump/acl
```


####  gre 配置验证

##### 基本业务配置

```

                     -----                                    -----
                    |     |                                  |     |
           gi 1/2/1 |     | ten 1/3/1             ten 1/3/1  |     | gi 1/2/1
        ----------->| 184 | -------------------------------> | 185 | ----------->
                    |     |                                  |     |
                    |     |                                  |     |
                     -----                                    -----

```


184 设备配置
```
mpls lsr-id 11.11.11.11
mpls enable

interface loopback 2
ip address 11.11.11.11 255.255.255.255
exit

interface tunnel 1/1/1
tunnel mode gre
source 10.128.1.184
destination 10.128.1.185
gre key plain 2000
exit

interface tengigabitethernet 1/3/1
ip address 10.128.1.184 255.255.255.0
exit

ip route 22.22.22.22 255.255.255.255 10.128.1.185 tengigabitethernet 1/3/1

interface gigaethernet 1/2/1
mode l2
mpls static-l2vc destination 22.22.22.22 raw vc-id 2 in-label 1000 out-label 2000 tunnel 1/1/1
exit




###################### delete config ######################

interface gigaethernet 1/2/1
no mpls static-l2vc
exit

no interface tunnel 1/1/1
no interface loopback 2
no ip route 22.22.22.22 255.255.255.255

mpls disable
no mpls lsr-id

```


185 设备配置
```
mpls lsr-id 22.22.22.22
mpls enable

interface loopback 2
ip address 22.22.22.22 255.255.255.255
exit

interface tunnel 1/1/1
tunnel mode gre
source 10.128.1.185
destination 10.128.1.184
gre key plain 2000
exit

interface tengigabitethernet 1/3/1
ip address 10.128.1.185 255.255.255.0
exit

ip route 11.11.11.11 255.255.255.255 10.128.1.184 tengigabitethernet 1/3/1

interface gigaethernet 1/2/1
mode l2
mpls static-l2vc destination 11.11.11.11 raw vc-id 2 in-label 2000 out-label 1000 tunnel 1/1/1
exit
```


##### 主备业务配置

```

                     -----                                    -----
                    |     | ten 1/3/1           ten 1/3/1    |     |
           gi 1/2/1 |     | -------------------------------> |     |  gi 1/2/1
        ----------->| 184 |                                  | 185 | ----------->
                    |     | -------------------------------> |     |
                    |     | gi 1/2/3            gi 1/2/3     |     |
                     -----                                    -----

```

184 设备配置
```
mpls lsr-id 11.11.11.11
mpls enable


interface loopback 2
ip address 11.11.11.11 255.255.255.255
exit

interface tunnel 1/1/1
tunnel mode gre
source 10.128.1.184
destination 10.128.1.185
gre key plain 2000
exit

interface tunnel 1/1/2
tunnel mode mpls
destination 22.22.22.22
mpls tunnel-id 1
mpls te commit
exit

interface tengigabitethernet 1/3/1
ip address 10.128.1.184 255.255.255.0
exit

interface gigaethernet 1/2/3
ip address 10.10.1.184 255.255.255.0
exit

### ip route 22.22.22.22 255.255.255.255 10.128.1.185 tengigabitethernet 1/3/1
### ip route 22.22.22.22 255.255.255.255 10.10.1.185 gigaethernet 1/2/3 distance 100

interface gigaethernet 1/2/1
mode l2
mpls static-l2vc destination 22.22.22.22 raw vc-id 3 in-label 1001 out-label 2001 tunnel 1/1/2
mpls static-l2vc destination 22.22.22.22 raw vc-id 2 in-label 1000 out-label 2000 tunnel 1/1/1 backup
mpls l2vpn redundancy master switch-mode revertive wtr-time 60
exit


mpls bidirectional static-lsp ingress A-B lsr-id 22.22.22.22 tunnel-id 1
forward 22.22.22.22 255.255.255.255 nexthop 10.10.1.185 out-label 10021
backward in-label 10011
exit



###################### delete config ######################

interface gigaethernet 1/2/1
no mpls l2vpn redundancy master
no mpls static-l2vc backup
no mpls static-l2vc
exit

no interface tunnel 1/1/1
no interface tunnel 1/1/2
no interface loopback 2
no ip route 22.22.22.22 255.255.255.255

no mpls bidirectional ingress A-B


mpls disable
no mpls lsr-id

```



185 设备配置
```
mpls lsr-id 22.22.22.22
mpls enable

interface loopback 2
ip address 22.22.22.22 255.255.255.255
exit

interface tunnel 1/1/1
tunnel mode gre
source 10.128.1.185
destination 10.128.1.184
gre key plain 2000
exit

interface tunnel 1/1/2
tunnel mode mpls
destination 11.11.11.11
mpls tunnel-id 1
mpls te commit
exit

interface tengigabitethernet 1/3/1
ip address 10.128.1.185 255.255.255.0
exit

interface gigaethernet 1/2/3
ip address 10.10.1.185 255.255.255.0
exit

### ip route 11.11.11.11 255.255.255.255 10.128.1.184 tengigabitethernet 1/3/1
### ip route 11.11.11.11 255.255.255.255 10.10.1.184 gigaethernet 1/2/3 distance 100

interface gigaethernet 1/2/1
mode l2
mpls static-l2vc destination 11.11.11.11 raw vc-id 3 in-label 2001 out-label 1001 tunnel 1/1/2
mpls static-l2vc destination 11.11.11.11 raw vc-id 2 in-label 2000 out-label 1000 tunnel 1/1/1 backup
mpls l2vpn redundancy master switch-mode revertive wtr-time 60
exit

mpls bidirectional static-lsp ingress A-B lsr-id 11.11.11.11 tunnel-id 1
forward 11.11.11.11 255.255.255.255 nexthop 10.10.1.184 out-label 10011
backward in-label 10021
exit


```





### 5G 模块

#### 模块上电

上电后进入 uboot，配置5G模块上电，给 gpio 104 配置为高电平，目的是给 5G 模块上电
```
gpio_test 104 1
```
执行完后敲`r`启动 system，注意要断电进入 uboot 执行上面命令。

> 这个后续需要写进 boot，启动时自动上电


#### 模块复位

复位管脚高电平有效：
```
btest gpio w 107 1
btest gpio w 107 0
```

先写高电平，然后写低电平
```
# btest gpio w 107 1
bsp_gpio_set_output_value unit(107) value(1)
# 
# usb 2-1: USB disconnect, device number 3
cdc_ncm 2-1:1.0 cell: unregister 'cdc_ncm' usb-f0200000.usb3-xhci-1, CDC NCM
option1 ttyUSB0: GSM modem (1-port) converter now disconnected from ttyUSB0
option 2-1:1.2: device disconnected
option1 ttyUSB1: GSM modem (1-port) converter now disconnected from ttyUSB1
option 2-1:1.3: device disconnected
option1 ttyUSB2: GSM modem (1-port) converter now disconnected from ttyUSB2
option 2-1:1.4: device disconnected
option1 ttyUSB3: GSM modem (1-port) converter now disconnected from ttyUSB3
option 2-1:1.5: device disconnected

# 
# lsusb
Bus 001 Device 001: ID 1d6b:0002
Bus 002 Device 001: ID 1d6b:0003
# 
# 
# btest gpio w 107 0
bsp_gpio_set_output_value unit(107) value(0)

usb 2-1: new SuperSpeed Gen 1 USB device number 4 using xhci-hcd
cdc_ncm 2-1:1.0: MAC-Address: 62:5f:cd:d9:28:eb
cdc_ncm 2-1:1.0 cell: register 'cdc_ncm' at usb-f0200000.usb3-xhci-1, CDC NCM, 62:5f:cd:d9:28:eb
register 'cdc_ncm' at usb-f0200000.usb3-xhci-1, CDC NCM, 62:5f:cd:d9:28:eb
option 2-1:1.2: GSM modem (1-port) converter detected
usb 2-1: GSM modem (1-port) converter now attached to ttyUSB0
option 2-1:1.3: GSM modem (1-port) converter detected
usb 2-1: GSM modem (1-port) converter now attached to ttyUSB1
option 2-1:1.4: GSM modem (1-port) converter detected
usb 2-1: GSM modem (1-port) converter now attached to ttyUSB2
option 2-1:1.5: GSM modem (1-port) converter detected
usb 2-1: GSM modem (1-port) converter now attached to ttyUSB3

# lsusb
Bus 002 Device 004: ID 2cb7:0a05
Bus 001 Device 001: ID 1d6b:0002
Bus 002 Device 001: ID 1d6b:0003
```


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
```
通信测试： AT -> 返回OK表示串口连通。
信号强度： AT+CSQ -> 返回 +CSQ:<rssi>,<ber> (数值越大越好)
SIM卡状态： AT+CPIN? -> 返回 READY 表示识别成功。
```

（2）网络控制指令
模式切换：
```
AT+CFUN=0 # 进入飞行模式
AT+CFUN=1 # 恢复全功能模式：ml-citation{ref="9,13" data="citationList"}

5G频段锁定： AT+QNWPREFCFG="nr5g_band",1:78
APN设置： AT+CGDCONT=1,"IPV4V6","cmnet" （移动默认APN）
```

（3）高级功能指令
IP过滤关闭：
```

AT^WHITELISTCTL=0
AT^IPFILTERSWITCH=0 #允许非终端IP数据包转发：ml-citation{ref="6,10" data="citationList"}

自动拨号： AT+GTAUTOCONNECT=1(广和通模组专用)
```



##### 常用命令

###### 查询模块生产厂家信息
```
cat /dev/ttyUSB0 &
echo -e "AT+CGMI?\r\n" > /dev/ttyUSB0

#应答
AT+CGMI?

+CGMI: "Fibocom Wireless Inc."

OK

# 
# 
```

###### 查询当前模式
```
cat /dev/ttyUSB0 &
echo -e "AT+GTUSBMODE?\r\n" > /dev/ttyUSB0

#应答
AT+GTUSBMODE?

+GTUSBMODE: 36

OK
```


###### 查询IMEI，每个模块唯一
```
cat /dev/ttyUSB0 &
echo -e "AT+CGSN?\r\n" > /dev/ttyUSB0

#应答
AT+CGSN?

+CGSN: "862138051858640"

OK
```


###### 查询批次号
```
cat /dev/ttyUSB0 &
echo -e "AT+CFSN?\r\n" > /dev/ttyUSB0

# 应答
AT+CFSN?

+CFSN: "BDUJQJ0028"

OK
```


###### 查询 PIN 码
```
cat /dev/ttyUSB0 &
echo -e "AT+CPIN?\r\n" > /dev/ttyUSB0

# 应答
AT+CPIN?

+CPIN: READY

OK
```

拔出 SIM 卡串口会打印提示
```
+SIM: Removed
+SIM DROP
```

插入时会提示
```
+SIM: Inserted
+SIM READY
```


###### 查询当前 SIM 卡信息
```
cat /dev/ttyUSB0 &
echo -e "AT+GTDUALSIM?\r\n" > /dev/ttyUSB0

```

###### 查询 APN 配置
```
cat /dev/ttyUSB0 &
echo -e "AT+CGDCONT?\r\n" > /dev/ttyUSB0

# 应答
AT+CGDCONT?

+CGDCONT: 0,"IPV4V6","CMIOT","10.2.60.180,36.9.141.76.0.30.4.217.0.0.0.0.0.0.0.1",0,0,0,0
+CGDCONT: 1,"IPV4V6","CMIOT","10.2.60.180,36.9.141.76.0.30.4.217.0.0.0.0.0.0.0.1",0,0,0,0
+CGDCONT: 2,"IPV4V6","","0.0.0.0,0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0",0,0,0,0
+CGDCONT: 3,"IPV4V6","","0.0.0.0,0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0",0,0,0,0
+CGDCONT: 4,"IPV4V6","","0.0.0.0,0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0",0,0,0,0
+CGDCONT: 5,"IPV4V6","","0.0.0.0,0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0",0,0,0,0
+CGDCONT: 6,"IPV4V6","","0.0.0.0,0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0",0,0,0,0
+CGDCONT: 7,"IPV4V6","","0.0.0.0,0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0",0,0,0,0
+CGDCONT: 8,"IPV4V6","","0.0.0.0,0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0",0,0,0,0
+CGDCONT: 9,"IPV4V6","","0.0.0.0,0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0",0,0,0,0
+CGDCONT: 10,"IPV4V6","","0.0.0.0,0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0",0,0,0,0
+CGDCONT: 11,"IPV4V6","ims","0.0.0.0,0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0",0,0,0,0

OK
```

###### 设置 APN

移动卡默认 APN 名称是 CMIOT
```
cat /dev/ttyUSB0 &
echo -e "AT+CGDCONT=1,\"IPV4V6\",\"CMIOT\"\r\n" > /dev/ttyUSB0

# 或者
echo -e "AT+CGDCONT=1,\"IP\",\"CMIOT\"\r\n" > /dev/ttyUSB0

```

###### 查看热插拔是否关闭

若返回0，则热插拔关闭。可以试着开启热插拔（AT+MSMPD=1，需要重启生效），看能否正常识卡（AT+CPIN?是否返回ready）。
```
cat /dev/ttyUSB0 &
echo -e "AT+MSMPD?\r\n" > /dev/ttyUSB0
```

###### 查询信号强度
```
cat /dev/ttyUSB0 &
echo -e "AT+CSQ?\r\n" > /dev/ttyUSB0

```


###### 查询信号指标

返回值：
`+CESQ: <rxlev>,<ber>,<rscp>,<ecno>,<rsrq>,<rsrp>,<ss_rsrq>,<ss_rsrp>,<ss_sinr>`

其中：
`<rxlev>,<ber>`： 表示2G，参数大于0并且不是99。
`<rscp>,<ecno>`：表示3G，参数大于0并且不是255。
`<rsrq>,<rsrp>`：表示4G，参数大于0并且不是255。
`<ss_rsrq>,<ss_rsrp>,<ss_sinr>`：表示5G，参数大于0并且不是255。

假如连续查90秒返回值不正确，则复位模块。
```

cat /dev/ttyUSB0 &
echo -e "AT+CESQ\r\n" > /dev/ttyUSB0

# 应答
AT+CESQ

+CESQ: 99,99,255,255,255,255,71,74,74

OK
```

###### 设置驻网

```
cat /dev/ttyUSB0 &
echo -e "AT+COPS?\r\n" > /dev/ttyUSB0

# 应答
AT+COPS?

+COPS: 0,0,"CHINA MOBILE",11

OK
```

###### 网络注册状态
```
cat /dev/ttyUSB0 &
echo -e "AT+CREG?\r\n" > /dev/ttyUSB0

# 应答

```

```
cat /dev/ttyUSB0 &
echo -e "AT+CGREG?\r\n" > /dev/ttyUSB0
```


###### 自动/手动拨号配置

查询
```
cat /dev/ttyUSB0 &
echo -e "AT+GTRNDIS?\r\n" > /dev/ttyUSB0

# 应答
+SIM READY

AT+GTRNDIS?

+GTRNDIS: 1,1,"10.2.60.180,2409:8d4c:001e:04d9:0000:0000:0000:0001","111.48.8.188,2409:804c:0000:2000:0000:0000:0000:0001","111.48.10.188,2409:804c:0000:200a:0000:0000:0000:0001"

OK
```

配置激活手动拨号
```
cat /dev/ttyUSB0 &
echo -e "AT+GTRNDIS=1,1\r\n" > /dev/ttyUSB0
```

释放上次拨号
```
echo -e "AT+GTRNDIS=0,1\r\n" > /dev/ttyUSB0
```

通过GTRNDIS命令拨号，它同样可以支持自动拨号，模组上电入网自动拨号给上位机分配IP地址
```
cat /dev/ttyUSB0 &
echo -e "AT+GTAUTOCONNECT=0\r\n" > /dev/ttyUSB0

# 读取当前配置
echo -e "AT+GTAUTOCONNECT?\r\n" > /dev/ttyUSB0
```


######  GTPING 检查数据业务连接状态
```
echo -e "AT+GTPING=0,\"8.8.8.8\"\r\n" > /dev/ttyUSB0

# ping 对端模组获取的IP
echo -e "AT+GTPING=0,\"10.13.234.76\"\r\n" > /dev/ttyUSB0

# ping 对端模组获取的IP
echo -e "AT+GTPING=0,\"10.6.55.59\"\r\n" > /dev/ttyUSB0

# ping 百度的地址
echo -e "AT+GTPING=0,\"220.181.111.232\"\r\n" > /dev/ttyUSB0


# 应答
AT+GTPING=0,"8.8.8.8"

+GTPING: 1

OK
```

###### 设置模块功能

```
cat /dev/ttyUSB0 &
echo -e "AT+CFUN?\r\n" > /dev/ttyUSB0

# 应答
AT+CFUN?

+CFUN: 1,0

OK
```


###### 设置模块低电平识别 SIM 卡

模块默认是高电平识别，设置低电平后重启模块，该命令可以掉电保存
```
cat /dev/ttyUSB0 &
echo -e "AT+GTSET="SIMPHASE",0,0\r\n" > /dev/ttyUSB0
```


###### 查询配置项
```
cat /dev/ttyUSB0 &
echo -e "AT+GTSET?\r\n" > /dev/ttyUSB0
```


###### 设置日志
```
echo -e "AT+GTSET=\"LOGLEVEL\",7\r\n" > /dev/ttyUSB0

echo -e "AT+GTLOGEN=1\r\n" > /dev/ttyUSB0

# 查询当前使能状态
echo -e "AT+GTLOGEN?\r\n" > /dev/ttyUSB0
```

然后把logtool 拷贝到 /tmp 路径（因为其他路径不允许创建文件），执行二进制文件后台运行
```
./logtool &
```

> 注意：logtool 二进制文件在哪个路径下时，执行后会在相同路径下创建一个 log_files 目录，用来存放生成的 .logel 日志文件。


###### 开启后路由功能
```
echo -e "AT+GTSET=\"POSTROUTE\",1\r\n" > /dev/ttyUSB0

```


###### 配置 IP 透传模式

配置 GTIPPASS=1，将模块从 NAT 模式切换为 IP 透传模式，此时运营商分配的公网 IP（如10.192.141.192）会直接分配给主机网卡（如 cell 接口），而非模块私网 IP（如192.168.225.\*）。
两者的区别：
- NAT 模式（IPPASS=0）：模块作为网关，主机通过私网 IP 访问外网（如 192.168.225.2 -> 模块转发流量）。
- 透传模式（IPPASS=1）：主机直接使用公网 IP（如10.192.141.192），模块仅作为通道，主机需要自行处理路由与防火墙规则。

```
cat /dev/ttyUSB0 &

# 查询当前配置
echo -e "AT+GTIPPASS?\r\n" > /dev/ttyUSB0

# 查询配置参数选项
echo -e "AT+GTIPPASS=?\r\n" > /dev/ttyUSB0

# 配置透传模式
echo -e "AT+GTIPPASS=1\r\n" > /dev/ttyUSB0
```


###### ECM 拨号配置自动 DHCP
```
cat /dev/ttyUSB0 &
echo -e "AT+GTAUTODHCP=1\r\n" > /dev/ttyUSB0

echo -e "AT+GTAUTODHCP?\r\n" > /dev/ttyUSB0

```


###### 配置模组锁定 5G 模式
```
echo -e "AT+GTACT=14\r\n" > /dev/ttyUSB0
```


###### 配置模组代理 arp 使能后本地IP掩码长度

默认的长度应该是8，可能会出现 wan 口 IP 和模组的这个本地代理 IP 不在同一网段，无法正常学习 arp，比如 wan 口 IP：10.128.1.185，收到的 arp 请求的源 IP 是默认的模组代理 IP：10.0.0.1，配置长度为 24 后，wan 口收到的 arp 请求的源 IP 就变成了 10.128.1.1，可以正常走 arp 学习流程。
```
# 配置掩码长度 24
echo -e "AT+GTSETMASK=3\r\n" > /dev/ttyUSB0
```



##### 拨号流程

查询当前模式，Fx650 模块默认是 NCM 拨号，需要设置 USB 模式为 36 或者 37
```
ip link set cell up
cat /dev/ttyUSB0 &
echo -e "AT+GTUSBMODE?\r\n" > /dev/ttyUSB0

```

确保 SIM 卡识别正常
```
echo -e "AT+CPIN?\r\n" > /dev/ttyUSB0
```

模块注册上运营商网络
```
echo -e "AT+COPS?\r\n" > /dev/ttyUSB0
```

模块注册上 CS 域
```
echo -e "AT+CREG?\r\n" > /dev/ttyUSB0
```

模块注册上 PS 域
```
echo -e "AT+CGREG?\r\n" > /dev/ttyUSB0

echo -e "AT+CEREG?\r\n" > /dev/ttyUSB0
```

设置 APN 接入点信息（）
```
echo -e "AT+CGDCONT=1,\"IPV4V6\",\"CMIOT\"\r\n" > /dev/ttyUSB0

echo -e "AT+CGDCONT=1,\"IPV4V6\",\"5gto5gproject-a.5gzx.hb\"\r\n" > /dev/ttyUSB0
```

查询 APN 设置是否成功
```
echo -e "AT+CGDCONT?\r\n" > /dev/ttyUSB0
```

释放上次拨号
```
echo -e "AT+GTRNDIS=0,1\r\n" > /dev/ttyUSB0
```

配置激活 RNDIS 拨号
```
cat /dev/ttyUSB0 &
echo -e "AT+GTRNDIS=1,1\r\n" > /dev/ttyUSB0
```

查询模块状态，`AT+GTRNDIS=1,1`返回 OK 后，下发 `AT+GTRNDIS?`，查询拨号是否成功，如果成功则会返回获取到的 IP 地址
```
echo -e "AT+GTRNDIS?\r\n" > /dev/ttyUSB0
```


```
ip link set cell up
cat /dev/ttyUSB0 &
echo -e "AT+GTUSBMODE?\r\n" > /dev/ttyUSB0

echo -e "AT+GTSET?\r\n" > /dev/ttyUSB0
echo -e "AT+GTIPPASS?\r\n" > /dev/ttyUSB0
echo -e "AT+GTACT?\r\n" > /dev/ttyUSB0

echo -e "AT+GTIPPASS=1\r\n" > /dev/ttyUSB0
echo -e "AT+GTSET=\"POSTROUTE\",1\r\n" > /dev/ttyUSB0
echo -e "AT+GTACT=14\r\n" > /dev/ttyUSB0
echo -e "AT+GTSETMASK=3\r\n" > /dev/ttyUSB0

echo 1 > /proc/sys/net/ipv4/conf/cell/proxy_arp

echo -e "AT+GTRNDIS=1,1\r\n" > /dev/ttyUSB0



```

##### 网卡IP 和路由配置

应用层软件需要配置的内容，dhcp 从模组申请 IP，cell 网卡配置 IP，网关和 DNS 配置。

**单台设备配置**
```
ip link set cell up

# 设置udhcp唯一id
echo "client_id=00:0E:5E:11:00:02" > /etc/udhcpc.conf

udhcpc -i cell

# 这个IP目前两台设备申请的都一样
ip link set cell down
ip addr add 192.168.225.2/24 dev cell
ip link set cell up

ip route add default via 192.168.225.1 dev cell

echo "nameserver 8.8.8.8" > /etc/resolv.conf
echo "nameserver 114.114.114.114" >> /etc/resolv.conf


ping 8.8.8.8

# ping baidu.com
ping 220.181.111.232 -I cell

# ping sina.com
ping 49.7.37.60 -I cell
```


**设备A**（公网IP：10.13.30.10）
```
# ip link set cell up
# udhcpc -i cell -r 192.168.225.100

ip link set cell down
ip addr add 192.168.225.100/24 dev cell
ip link set cell up

# 配置默认网关
ip route add default via 192.168.225.1 dev cell

# 配置更精确的静态路由，目的 IP 为这个指定地址的报文都通过大网发给对端的公网 IP
ip route add 192.168.225.200/32 via 10.6.226.220 dev cell

# ping 操作发起端配置 SNAT
iptables -t nat -A POSTROUTING -s 192.168.225.100 -d 192.168.225.200 -j SNAT --to-source 10.13.30.10



echo "nameserver 8.8.8.8" > /etc/resolv.conf
echo "nameserver 114.114.114.114" >> /etc/resolv.conf

```

**设备B**（公网IP：10.6.226.220）
```
# ip link set cell up
# udhcpc -i cell -r 192.168.225.200

ip link set cell down
ip addr add 192.168.225.200/24 dev cell
ip link set cell up

# 配置默认网关
ip route add default via 192.168.225.1 dev cell

iptables -A FORWARD -s 10.13.30.10 -d 192.168.225.200 -j ACCEPT

iptables -t nat -A POSTROUTING -o cell -j MASQUERADE



echo "nameserver 8.8.8.8" > /etc/resolv.conf
echo "nameserver 114.114.114.114" >> /etc/resolv.conf
```

上面 NAT 模式需要申请本地 IP，透传模式使用模组从基站分配的公网 IP
```
10.13.131.214
10.6.12.247


udhcpc -i cell

ifconfig cell 10.13.131.214 netmask 255.255.255.0
ip route add default via 10.13.131.1 dev cell

# 或者
ifconfig cell 10.6.12.247 netmask 255.255.255.0
ip route add default via 10.6.12.1 dev cell

echo "nameserver 8.8.8.8" > /etc/resolv.conf
echo "nameserver 114.114.114.114" >> /etc/resolv.conf
```

###### DHCP 获取 IP 地址

```
# udhcpc -i cell
udhcpc (v1.22.1) started
Sending discover...
Sending select for 192.168.225.2...
Lease of 192.168.225.2 obtained, lease time 86400
```

如果 cell 没有自动配置上 ip，手动配置
```
ifconfig cell 192.168.225.2 netmask 255.255.255.0
```

###### 网关设置

正常情况你应该看到类似这样的输出：​​
```
default via 192.168.225.1 dev cell
192.168.225.0/24 dev cell proto kernel scope link src 192.168.225.2
```

`default via 192.168.225.1`这一行至关重要！它表示发往所有非本地网络（即外网）的数据包，都会通过网关 `192.168.225.1`发送。如果没有这一条，你的设备就不知道把外网数据包发给谁。

​如果缺少默认路由：​​你可能需要手动添加：

```
ip route add default via 192.168.1.1 dev cell
```

再查看路由配置信息
```
# ip route show
default via 192.168.225.1 dev cell 
10.0.0.0/24 dev ibc0 proto kernel scope link src 10.0.0.3 
127.0.0.0/24 dev lo scope link 
192.168.1.0/24 dev br0 proto kernel scope link src 192.168.1.254 
192.168.225.0/24 dev cell proto kernel scope link src 192.168.225.2
```

```
# route -n
Kernel IP routing table
Destination     Gateway         Genmask         Flags Metric Ref    Use Iface
0.0.0.0         192.168.225.1   0.0.0.0         UG    0      0        0 cell
10.0.0.0        0.0.0.0         255.255.255.0   U     0      0        0 ibc0
127.0.0.0       0.0.0.0         255.255.255.0   U     0      0        0 lo
192.168.1.0     0.0.0.0         255.255.255.0   U     0      0        0 br0
192.168.225.0   0.0.0.0         255.255.255.0   U     0      0        0 cell
```


###### 检查并配置 DNS（最常见的问题根源！）

`udhcpc`会从 DHCP 服务器获取 DNS 服务器地址，但它​不会自动更新系统的 DNS 配置文件​（如 `/etc/resolv.conf`）。你需要确保这个文件里有可用的 DNS 服务器。

如果文件是空的，或者里面的 DNS 服务器不可用（如 `127.0.0.1`但本地没运行 DNS 服务），就会导致能 Ping 通 IP 但打不开网站（域名无法解析）。

如果没有这个配置文件，手动创建并编辑：
```
echo "nameserver 8.8.8.8" > /etc/resolv.conf
echo "nameserver 114.114.114.114" >> /etc/resolv.conf
```

- `8.8.8.8`是 Google 的公共 DNS。
- `114.114.114.114`是国内移动/电信的公共 DNS。
- `>`表示覆盖原文件，`>>`表示追加到文件末尾。


 **一劳永逸的解决方案**：使用 `udhcpc`的脚本​`udhcpc`在获取到 IP 后，会调用一个脚本去设置网络参数。这个脚本通常叫 `/usr/share/udhcpc/default.script`。它的核心工作就是配置 IP、路由和 DNS​。

检查这个脚本是否存在且有执行权限。
确保脚本中有类似这样配置 DNS 的行：
```
[ -n "$dns" ] && echo "nameserver $dns" > /etc/resolv.conf
```

如果你的系统没有这个脚本，或者脚本有问题，你就需要手动完成上述配置。


###### 测试网关连通性

```
ping 192.168.225.1
```

**如果不通**​​：问题在局域网层面，检查网线、物理连接、模组和路由器的配置。


###### 测试外网 IP（测试网关的 NAT 和出口）

```
ping 8.8.8.8

#或者ping baidu.com
ping 220.181.111.232 -I cell
```

**如果通​**​：恭喜，你的 IP 和路由配置是正确的！问题 100% 出在 ​**​DNS​**​。请重点检查 DNS 配置。

**如果不通​**​：但网关设置步骤中的网关是通的，那问题可能出在：

- 防火墙​：路由器或你的设备本身的防火墙丢弃了数据包。
- ​运营商的限制​：特别是对于 ​**​4G/5G** **蜂窝模组​**​，你可能获取的是一个运营商内网的 IP（如 `10.x.x.x`），运营商没有为你做 NAT 或者有额外的激活步骤。



###### 针对蜂窝模组（4G/5G Cat.1/NB-IoT 等）的特殊情况

对于通过蜂窝网络上网的模组，情况可能更复杂一些：

1. ​**​APN** **配置是否正确？​**​ 在拨号前（例如使用 `pppd`或 `quectel-CM`等工具时），必须设置正确的 APN（接入点名称）。错误的 APN 可能导致你能获取 IP（运营商内网 IP），但无法访问外网。

2. ​**​PDP** **上下文是否完全激活？​**​ 有些模组在获取 IP 后，还需要一个额外的“激活”步骤才能真正建立数据通道。

3. **​获取的 IP 类型​**​：你获取到的可能是私有 IP（如 `10.x.x.x`），此时需要运营商网关为你做 NAT。如果运营商侧策略限制，即使有 IP 也无法上网。可以尝试在模组上执行 ​**​PDP 去激活再重新激活​**​。

总结与行动清单

| **步骤**    | **检查/操作**           | **命令/方法**                                                  | **预期结果**                       |
| --------- | ------------------- | ---------------------------------------------------------- | ------------------------------ |
| ​**​1​**​ | ​**​确认 IP 和默认路由​**​ | `ip route`或 `route -n`                                     | 看到 `default via [``网关IP]`      |
| ​**​2​**​ | ​**​检查并配置 DNS​**​   | `cat /etc/resolv.conf`                                     | 文件中有有效的 `nameserver [DNS IP]`  |
| ​**​3​**​ | ​**​分层 Ping 测试​**​  | 1. `ping [``网关IP]`2. `ping 8.8.8.8`3. `ping www.baidu.com` | 1. 通2. 通 -> DNS 问题3. 通 -> 全部正常 |

​**​大概率问题出现在 DNS 配置。​**​请首先确保 `/etc/resolv.conf`被正确写入。如果上述步骤都无法解决，请提供以下信息以便更深入的分析：

- 你使用的模组型号（如 EC20, BC26, M6315 等）。
- 你获取到的 IP 地址、网关是什么（`ip addr show [``网卡名]`和 `ip route`的输出）。
- `cat /etc/resolv.conf`的输出。
- `ping 8.8.8.8`的结果。







