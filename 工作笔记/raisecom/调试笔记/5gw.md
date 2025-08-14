
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

- 子卡插在 3 槽（左边槽位）上时，7118 对应 serdes 8、9、10、11；
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
- 產測時候用 MIB 命令配置，系统已经启动并且跑了软件
mib set PON_MODE x
mib commit hs

- preconfig 內的 config_default_hs.xml 裡面修改。這樣改指針對乾淨的 flash 有效。

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








