
### 查看子接口vlan

查看配置文件：
```
cat /proc/net/vlan/config
```

知道网卡是子接口，但是不知道对应vlan：
```
ip -d link show ibc0

或者
cat /proc/net/vlan/ibc0
```


### 网卡状态

```
# ifconfig eth0.2
eth0.2    Link encap:Ethernet  HWaddr 00:0E:5E:62:EE:EE  
          UP BROADCAST MULTICAST  MTU:1500  Metric:1
          RX packets:1007 errors:0 dropped:0 overruns:0 frame:0
          TX packets:0 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:1000 
          RX bytes:82574 (80.6 KiB)  TX bytes:0 (0.0 B)
          Interrupt:12 

# ifconfig ibc0
ibc0      Link encap:Ethernet  HWaddr 20:20:02:25:15:03  
          inet addr:10.0.0.3  Bcast:10.0.0.255  Mask:255.255.255.0
          UP BROADCAST MULTICAST  MTU:1500  Metric:1
          RX packets:0 errors:0 dropped:0 overruns:0 frame:0
          TX packets:0 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:1000 
          RX bytes:0 (0.0 B)  TX bytes:0 (0.0 B)
```

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

可以看到子接口 ibc0 和父接口 eth0.2 的状态都有问题，正常 link up 的网卡信息应该是这样的：
```
# 
# ip -d link show eth0.6
11: eth0.6: <BROADCAST,MULTICAST,PROMISC,UP,LOWER_UP> mtu 1500 qdisc pfifo_fast state UP mode DEFAULT group default qlen 1000
    link/ether 00:0e:5e:62:ee:ee brd ff:ff:ff:ff:ff:ff permaddr 00:00:01:00:02:00 promiscuity 1 minmtu 60 maxmtu 9022 addrgenmode eui64 numtxqueues 1 numrxqueues 1 gso_max_size 65536 gso_max_segs 65535 
```

其中，
- `<BROADCAST,MULTICAST,PROMISC,UP,LOWER_UP>`：接口标志位（flags），这是状态信息最集中的地方
	- **UP 是逻辑/管理状态**，表示已被管理员启用，例如 ip link set ibc0 up。这时网卡处于就绪状态，操作系统允许数据通过网卡进行收发，但是不保证物理链路是通的；
	- **LOWER_UP 是物理链路状态**，表示物理层或下层链路是通的。对于物理网卡，这通常表示网线已连接，且对端设备也正常工作。对于子接口（如ibc0），这意味着父接口（eth0.2）的链路状态是 UP 的。
	- **只有 UP，没有 LOWER_UP**，表示管理状态已打开，但是物理状态有问题，后面的state 很可能是 LOWERLAYERDOWN，需要检查父接口。通常会有 NO-CARRIER。

- `state UP`：逻辑状态
	- **UP**：这是最理想的状态，表示 UP（逻辑状态）和 LOWER_UP （物理链路状态）两个条件同时满足，接口完全可操作；
	- **UNKNOWN**：通常出现在接口刚被创建或驱动初始化时，状态尚未确定；
	- **DOWN**：表示接口被管理员禁用，或者物理链路不通；
	- **LOWERLAYERDOWN**：故障状态，表示逻辑上被启用了，但是物理链路是断开的，对vlan子接口（ibc0）来说，ibc0 本身是 up 的，但是父接口 eth0.2 要么是 down 的，要么 eth0.2 的物理链路不通。

- `<NO-CARRIER>`标志
	- 物理层故障的标志，表示接口试图检测物理链路上的载波信号，即对端设备，但是没有检测到。常见原因：网线问题，对端设备的网口问题或者网卡端口物理损坏。
	- 当出现这个标志时，LOWER_UP 标志肯定不存在，并且 state 可能是 LOWERLAYERDOWN 或者 DOWN。


