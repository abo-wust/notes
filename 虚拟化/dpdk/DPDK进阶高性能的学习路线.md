
## DPDK网络专栏
### dpdk基础知识

多队列网卡，vmxnet/e1000
igb_ uio与vfio模块
kni模块
hugepage的理解
零拷贝
dpdk与netmap 区别
dpdk的工作环境

### 网络协议栈

dpdk-arp
netsh静态arp表设置
dpdk-icmp
udp协议格式分析
udp协议sendto, recvfrom实现
dpdk-ip
dpdk-tcp
tcp协议栈bind,listen, accept实现
tcp协议栈recv, send, close的实现
tcp三次握手实现
tcp四次挥手 实现
tcp acknum与seqnum的确认机制实现
tcp的并发连接设计
epoll并发的实现
tcp协议栈 与epoll之间的回调实现

### dpdk组件项目

dpdk-ac
dpdk-kni
/dev/ kni的原理分析
kni_ dev的流程
kni的t缓冲区，rx缓冲区
kni的用户空间与内核空间 映射
mbuf 如何转化为kernel的sk_ buff
dpdk- timer
bpftrace 的使用
dpdk- bpf源码流程

### dpdk经典项目

dpdk- dns
dpdk- gateway
dpdk-ddos熵计算源码
ddos attach检测精确度调试
ddos attach测试T具hping3
布谷鸟 hash原理与使用

## 储存技术专栏

### 高效磁盘io读写spdk(C)

存储框架spdk ,为技术栈 打开扇存储的大门
spdk运行环境与vhost
NVMe与PCl的关系
手把手实现spdk_ server
nvme与pcie以及手写nvme读写操作
bdev与blob之间的关系
实现blob异步读写
blobstore的读写操作实现与rpc的关系
fio性能测试 性能对比libaio,io_ uring,psync
fio plugin工作流程
fio plugin开发

### spdk文件系统 的实现

文件系统功能拆解
spdk_ env_ init与spdk_ app init的差别
spdk_ _thread_ poll实现rpc回调
fs_ operations结构体定义
file_ operations结构体定义
dir_ operations结构体定义
syscall的hook实现
io内存管理
基数树 对文件系统内存管理
spdk_ blob的open,read,write,close
测试用例 与调试入口函数

### spdk kv存储的实现

KV存储拆解Set, Get, Mod, Del
app/.a库/so库对于kv存储的选择
bdev与blob对于kv存储的选择
kv service启动blob资源操作
kv service关闭blob资源回收
kv service接口set,get,modify,delete
kv遍历 与查找实现
page存储chunk的管理
pagechunk的get与put
page单查找与多页查找
btree, artree, hashmap,radixtree, rbtree之间的选择
slab的实现
slab分配slot与释放slot
为kv加上conf文件
测试用例与性能测试

## 安全与网关开发专栏

### 可扩展的矢量数据包处理框架vpp (c/c++)

vpp命令详解
mac/ip转发plugin
load_ balance plugin
flowtable plugin
vpp源码之间的差异
多网卡数据接收与转发
解决plugin编译加载
vpp启动load so的流程
vpp的结构体vlib_main实现分析
vpp的结构体vnet_ main
vector 的操作实现
vpp vcl库与LD_ PRELOAD实现分析
vcl原理讲解
vcl tcpserver实现原理
vcl tcpclient实现原理
vcl与iperf3的客户端 与服务器
vcl与nginx的wrk性能测试
vcl与haproxy的性能测试
vpp 1801版本与vpp 2206版本
vpp httpserver的实现源码
vpp plugin quic源码分析
vpp plugin hs_ app的源码
vpp plugin rdma的实现分析
vpp plugin loadbalance
vpp plugin nat的源码分析
vpp host-stack tcp协议实现
vpp plugin的测试用例实现

### golang的网络开发框架nff-go (golang)

nff-go实现的技术原理
nff-go/low.h实现分析
nff- go数据接收的实现
nff-go数据发送的实现
ipsec协议解析与strongswan的ipsec
nff go的缺陷与不足

## 虚拟化与云原生 专栏

### DPDK的虚拟交换机 框架OvS

ovs编译安装，ovs核心组件内容
ovs-vswitchd的工作原理
ovs-vswitchd与dpdk的关系
ovs-vsctl的网桥，网口操作
qemu-system-x86_ 64构建多子网
ovs与qemu数据流分发
ovs搭建docker跨主机通信
ovsdb server与ovsdb协议
json-rpc为控制面提供开发
ovs-tcpdump/ovs-l3ping
OvS 4种数据路径
VXL AN数据协议
ovs流量统计

### 高性能4层负载均衡器 DPVS

dpvs 的技术组件与功能边界
lvs+keepalived配置高可用server
dpvs与|lvs+ keepalived的关系
dpvs.conf的配置文件
dpvs的FNat/NAT/SNAT模式
dpvs的DR模式
dpvs的tun模式
通过quagga配置ospf
dpvs的tc流控操作与源码实现
dpvs代码架构分析
dpvs测试用例ipset, tc,mempool

## 测试工具专栏

### perf3

vpp vcl 的perf3接口hook
perf3测网络带宽
tcp吞吐量测试
udp丢包与延迟测试
json测试结果输出

### TRex

TRex的运行原理
TRex与dpdk
构建TRex测试系统
t-rex -64- debug gdb调试
bg-sim- 64模拟单元测试
YAML文件编写
流编排与自动化框架
报文变量设置

### dpdk-pktgen

pktgen命令讲解
default.cfg配置文件分析
120M bits/s的转发速率
### fio

ioengine 的实现
ioengine_ ops的分析
iodepth的分析
spdk_ nvme的fio分析
spdk_ bdev的fio分析
spdk_ blob的ioengine实现
psync，io_ uring，libaio性能对比

## 性能测试专栏

### 性能指标

吞吐量bps
拆链/建链pps
并发
最大时延
最小时延
平均时延
负载
包速fps
丢包率
### 测试方法

测试用例
vpp sandbox
perf3灌包
rfc2544


### 岗位

高级网络开发工程师
DPDK开发工程师
云基础开发工程师
NFV开发工程师
高性能优化 工程师
云产品研发工程师
SDN开发工程师


## DPDk应用场景

dpdk 作为优秀的用户空间高性能数据包加速套件，现在已经作为一个“胶水”模块被用在多个网络数据处理方案中，用来提高性能。如下是众多的应用。

### 网络虚拟化

网络虚拟化是一种将物理网络资源虚拟化为多个逻辑网络资源的技术，它可以帮助企业节省网络设备成本，提高网络资源利用率。DPDK可以帮助网络虚拟化平台 实现高性能、低延迟的数据包处理，从而提高虚拟网络 的性能和可靠性

### 云计算

云计算是一-种将计算资源虚拟化为多个逻辑计算资源的技术，它可以帮助企业节省计算资源成本，提高计算资源利用率。DPDK可以帮助云计算平台 实现高性能、低延迟的数据包处理，从而提高云计算的性能和可靠性。

### 网络安全

网络安全是一种保护网络资源免受恶意攻击的技术，它可以帮助企业保护网络资源的安全性和可靠性。DPDK可以帮助网络安全平台实现高性能、低延迟的数据包处理，从而提高网络安全的性能和可靠性。

### 5G通信

5G通信是一-种新一代的移动通信技术 ，它可以帮助企业实现更快、更可靠的移动通信服务。DPDK 可以帮助5G通信平台实现高性能、低延迟的数据包处理，从而提高5G通信的性能和可靠性。

### 数据面（虚拟交换机）

- OVS：Open vSwitch 是一个多核虚拟交换机平台，支持标准的管理接口和开放可扩展的可编程接口，支持第三方的控制接入。
- VPP：是 cisco 开源的一个高性能的包处理框架，提供了交换/路由功能，在虚拟化环境中，使它可以当做一个虚拟交换机来使用。在一个类 SDN 的处理框架中，它往往充当数据面的角色。经研究表明，VPP 性能要好于 ovs+dpdk 的组合，但它更适用于NFV，适合做特定功能的网络模块。
- Lagopus：是另一个多核虚拟交换的实现，功能和 OVS 差不多，支持多种网络协议，如Ethernet，VLAN，QinQ，MAC-in-MAC，MPLS 和 PBB，以及隧道协议 ，如 GRE，VxLan和 GTP。
- Snabb： 是一个简单且快速的数据包处理工具箱 。

### 数据面（虚拟路由器 ）

- OPENCONTRAIL：一个集成了 SDN 控制器的虚拟路由器，现在多用在 OpenStack 中，结合Neutron 为 OpenStack 提供一站式的网络支持。
- CloudRouter：一个分布式的路由器。

### 用户空间协议栈

- mTCP：是一个针对多核系统的高可扩展性的用户空间 TCP/IP 协议栈。
- IwIP：针对 RAM 平台的精简版的 TCP/IP 协议栈实现。
- Seastar：是一个开源的，基于 C++ 11/14 feature，支持高并发和低延迟的异步编程 高性能库。
- f-stack：腾讯开源的用户空间协议栈，移植于 FreeBSD协议栈，粘合了 POSIX API，上层应用（协程框架，Nginx,Redis），纯 C 编写，易上手。