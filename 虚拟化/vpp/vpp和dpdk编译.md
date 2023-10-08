### VPP编译

**1. 下载代码**

```shell
    git clone https://github.com/FDio/vpp.git
```

网络问题可能导致下载失败，重复几次就可以。

也有可能缓存不足导致下载失败：

```
    error: RPC failed; curl 56 GnuTLS recv error (-54): Error in the pull function.
    fatal: The remote end hung up unexpectedly
    fatal: early EOF
    fatal: index-pack failed
```

解决方法，设置较大的缓存：

```shell
    git config --global http.postBuffer 2000000000
```

**2. 指定VPP版本**

代码下载完成后指定需要的版本，命令`git branch -a`查看当前所有分支版本：

```shell
    $ git branch -a
    * master
    remotes/origin/HEAD -> origin/master
    remotes/origin/map-fib
    remotes/origin/master
    remotes/origin/stable/1606
    remotes/origin/stable/1609
    remotes/origin/stable/1701
    remotes/origin/stable/1704
    remotes/origin/stable/1707
    remotes/origin/stable/1710
    remotes/origin/stable/1801
    remotes/origin/stable/1804
    remotes/origin/stable/1807
    remotes/origin/stable/1810
    remotes/origin/stable/1901
    remotes/origin/stable/1904
    remotes/origin/stable/1908
    remotes/origin/stable/2001
    remotes/origin/stable/2005
    remotes/origin/stable/2009
    remotes/origin/stable/2101
    remotes/origin/stable/2106
    remotes/origin/stable/2110
    remotes/origin/stable/2202
    remotes/origin/stable/2206
    remotes/origin/stable/2210
    remotes/origin/stable/test
```

切换到2005版本：

```shell
    $ git checkout -b 2005 origin/stable/2005
    $ git branch -a
    * 2005
    master
    remotes/origin/HEAD -> origin/master
    remotes/origin/map-fib
    remotes/origin/master
    remotes/origin/stable/1606
    remotes/origin/stable/1609
    remotes/origin/stable/1701
    remotes/origin/stable/1704
    remotes/origin/stable/1707
    remotes/origin/stable/1710
    remotes/origin/stable/1801
    remotes/origin/stable/1804
    remotes/origin/stable/1807
    remotes/origin/stable/1810
    remotes/origin/stable/1901
    remotes/origin/stable/1904
    remotes/origin/stable/1908
    remotes/origin/stable/2001
    remotes/origin/stable/2005
    remotes/origin/stable/2009
    remotes/origin/stable/2101
    remotes/origin/stable/2106
    remotes/origin/stable/2110
    remotes/origin/stable/2202
    remotes/origin/stable/2206
    remotes/origin/stable/2210
    remotes/origin/stable/test
```

**3. 安装依赖**

开始安装依赖和编译之前需要确保系统没有安装过其他VPP和DPDK：

```shell
    dpkg -l | grep vpp
    dpkg -l | grep DPDK
```

输入命令行后应该没有任何显示信息，如果有，则删除对应安装包：

```shell
    dpkg --purge <显示的安装包名>
```

然后安装依赖：

```shell
    make install-dep
    make install-ext-deps
```

安装依赖的过程中可能会出现部分压缩包下载超时导致安装失败的问题，重复几次即可。

**4. 编译**

进入下载的VPP目录`cd vpp/`，使用`make build`命令开始编译`debug`版本的VPP：

```shell
    $ make build
    make[1]: Entering directory '/home/huahuan/work/vpp/build-root'
    @@@@ Arch for platform 'vpp' is native @@@@
    @@@@ Finding source for external @@@@
    @@@@ Makefile fragment found in /home/huahuan/work/vpp/build-data/packages/external.mk @@@@
    @@@@ Source found in /home/huahuan/work/vpp/build @@@@
    @@@@ Arch for platform 'vpp' is native @@@@
    @@@@ Finding source for vpp @@@@
    @@@@ Makefile fragment found in /home/huahuan/work/vpp/build-data/packages/vpp.mk @@@@
    @@@@ Source found in /home/huahuan/work/vpp/src @@@@
    @@@@ Configuring external: nothing to do @@@@
    @@@@ Building external in /home/huahuan/work/vpp/build-root/build-vpp_debug-native/external @@@@
    make[2]: Entering directory '/home/huahuan/work/vpp/build/external'
    ...
    ...
```

编译完成后的二进制文件以及动态链接库文件在`build-root/build-vpp_debug-native/`可以找到。

使用`make build-release`命令可以编译`release`版本的VPP，编译后的文件在`build-root/build-vpp-native/vpp`。

**5. 生成安装包**

编译成功无错误之后，生成用于安装VPP的软件包，在`Debian`系统中需要`deb`安装包，`Red-Hat`系统中是`rpm`安装包。生成的安装包在`build-root`目录。

```shell
    $ make pkg-deb
    $ cd build-root
    build-root$ ls *.deb -l
    -rw-r--r-- 1 root root   174176 11月 23 15:44 libvppinfra_20.05.1-6~gf53edbc3b_amd64.deb
    -rw-r--r-- 1 root root   127612 11月 23 15:44 libvppinfra-dev_20.05.1-6~gf53edbc3b_amd64.deb
    -rw-r--r-- 1 root root    24396 11月 23 15:44 python3-vpp-api_20.05.1-6~gf53edbc3b_amd64.deb
    -rw-r--r-- 1 root root  3251932 11月 23 15:44 vpp_20.05.1-6~gf53edbc3b_amd64.deb
    -rw-r--r-- 1 root root    24340 11月 23 15:44 vpp-api-python_20.05.1-6~gf53edbc3b_amd64.deb
    -rw-r--r-- 1 root root 61616456 11月 23 15:44 vpp-dbg_20.05.1-6~gf53edbc3b_amd64.deb
    -rw-r--r-- 1 root root   968888 11月 23 15:44 vpp-dev_20.05.1-6~gf53edbc3b_amd64.deb
    -rw-r--r-- 1 root root  2888996 11月 23 15:44 vpp-plugin-core_20.05.1-6~gf53edbc3b_amd64.deb
    -rw-r--r-- 1 root root  3806912 11月 23 15:44 vpp-plugin-dpdk_20.05.1-6~gf53edbc3b_amd64.deb
```

> 生成的`deb`安装包名带有VPP版本号，如20.05。如果之前有编译生成过其他版本的`deb`包，则需要删除其他版本的安装包。可以不删除，但是下一步安装比较麻烦。

**6. 安装VPP**

编译以及安装包生成完成后，需要将VPP安装到系统。使用命令`dpkg -l | grep vpp`再次查看时就会显示指定VPP版本的安装包信息。

```shell
    $ dpkg –i build-root/*.deb

    $ dpkg -l | grep vpp
    ii  libvppinfra              20.05.1-6~gf53edbc3b         amd64        Vector Packet Processing--runtime libraries
    ii  libvppinfra-dev          20.05.1-6~gf53edbc3b         amd64        Vector Packet Processing--runtime libraries
    ii  python3-vpp-api          20.05.1-6~gf53edbc3b         amd64        VPP Python3 API bindings
    ii  vpp                      20.05.1-6~gf53edbc3b         amd64        Vector Packet Processing--executables
    ii  vpp-api-python           20.05.1-6~gf53edbc3b         amd64        VPP Python API bindings
    ii  vpp-dbg                  20.05.1-6~gf53edbc3b         amd64        Vector Packet Processing--debug symbols
    ii  vpp-dev                  20.05.1-6~gf53edbc3b         amd64        Vector Packet Processing--development support
    ii  vpp-ext-deps             20.05-10                     amd64        VPP developer package containing dependencies
    ii  vpp-plugin-core          20.05.1-6~gf53edbc3b         amd64        Vector Packet Processing--runtime core plugins
    ii  vpp-plugin-dpdk          20.05.1-6~gf53edbc3b         amd64        Vector Packet Processing--runtime dpdk plugin

```

### DPDK编译

**1. 下载代码**

在VPP的编译完成后，`build/external/downloads/`目录下应该会有VPP编译过程中下载的代码压缩包。

```shell
    $ ls build/external/downloads/
    dpdk-20.02.tar.xz  nasm-2.14.02.tar.xz  quicly_0.1.0-vpp.tar.gz  rdma-core-28.0.tar.gz  v0.53.tar.gz
```

打开`build/external/packages/dpdk.mk`文件，找到`DPDK_VERSION`对应DPDK的版本，与`build/external/downloads/`目录下的压缩包版本一致：

```
    DPDK_VERSION                 ?= 20.02
    DPDK_BASE_URL                ?= http://fast.dpdk.org/rel
```

解压`build/external/downloads/`目录下的压缩包：

```shell
    $ xz -dk dpdk-20.02.tar.xz
    $ ls dpdk-20.02*
    dpdk-20.02.tar  dpdk-20.02.tar.xz
    $ tar -xvf dpdk-20.02.tar
    $ ls dpdk-20.02*
    dpdk-20.02  dpdk-20.02.tar  dpdk-20.02.tar.xz
```

**2. 安装依赖**

参考官方文档说明[http://doc.dpdk.org/guides-20.02/linux_gsg/sys_reqs.html](http://doc.dpdk.org/guides-20.02/linux_gsg/sys_reqs.html)。

**3. 编译**

参考 [DPDK编译及使用](https://turbock79.cn/?p=1755)

由于使用的是因特尔的DPDK网卡，所以需要编译生成`igb_uio.ko`，但是，20.02版本的DPDK默认关闭`igb_uio`模块，需要配置`config/common_base`文件的`CONFIG_RTE_EAL_IGB_UIO`选项使能，将默认的`n`改为`y`：

```shell
    $ grep "CONFIG_RTE_EAL_IGB_UIO" -rn config/common_base 
    106:CONFIG_RTE_EAL_IGB_UIO=n
```

使用`usertools/dpdk-setup.sh`脚本编译：

```shell
    $ ./usertools/dpdk-setup.sh 
    ------------------------------------------------------------------------------
    RTE_SDK exported as /home/huahuan/work/vpp/build/external/downloads/dpdk-20.02
    ------------------------------------------------------------------------------
    ----------------------------------------------------------
    Step 1: Select the DPDK environment to build
    ----------------------------------------------------------
    [1] arm64-armada-linuxapp-gcc
    [2] arm64-armada-linux-gcc
    [3] arm64-armv8a-linuxapp-clang
    [4] arm64-armv8a-linuxapp-gcc
    [5] arm64-armv8a-linux-clang
    [6] arm64-armv8a-linux-gcc
    [7] arm64-bluefield-linuxapp-gcc
    [8] arm64-bluefield-linux-gcc
    [9] arm64-dpaa-linuxapp-gcc
    [10] arm64-dpaa-linux-gcc
    [11] arm64-emag-linuxapp-gcc
    [12] arm64-emag-linux-gcc
    [13] arm64-n1sdp-linuxapp-gcc
    [14] arm64-n1sdp-linux-gcc
    [15] arm64-octeontx2-linuxapp-gcc
    [16] arm64-octeontx2-linux-gcc
    [17] arm64-stingray-linuxapp-gcc
    [18] arm64-stingray-linux-gcc
    [19] arm64-thunderx2-linuxapp-gcc
    [20] arm64-thunderx2-linux-gcc
    [21] arm64-thunderx-linuxapp-gcc
    [22] arm64-thunderx-linux-gcc
    [23] arm64-xgene1-linuxapp-gcc
    [24] arm64-xgene1-linux-gcc
    [25] arm-armv7a-linuxapp-gcc
    [26] arm-armv7a-linux-gcc
    [27] i686-native-linuxapp-gcc
    [28] i686-native-linuxapp-icc
    [29] i686-native-linux-gcc
    [30] i686-native-linux-icc
    [31] ppc_64-power8-linuxapp-gcc
    [32] ppc_64-power8-linux-gcc
    [33] x86_64-native-bsdapp-clang
    [34] x86_64-native-bsdapp-gcc
    [35] x86_64-native-freebsd-clang
    [36] x86_64-native-freebsd-gcc
    [37] x86_64-native-linuxapp-clang
    [38] x86_64-native-linuxapp-gcc
    [39] x86_64-native-linuxapp-icc
    [40] x86_64-native-linux-clang
    [41] x86_64-native-linux-gcc
    [42] x86_64-native-linux-icc
    [43] x86_x32-native-linuxapp-gcc
    [44] x86_x32-native-linux-gcc

    ----------------------------------------------------------
    Step 2: Setup linux environment
    ----------------------------------------------------------
    [45] Insert IGB UIO module
    [46] Insert VFIO module
    [47] Insert KNI module
    [48] Setup hugepage mappings for non-NUMA systems
    [49] Setup hugepage mappings for NUMA systems
    [50] Display current Ethernet/Baseband/Crypto device settings
    [51] Bind Ethernet/Baseband/Crypto device to IGB UIO module
    [52] Bind Ethernet/Baseband/Crypto device to VFIO module
    [53] Setup VFIO permissions

    ----------------------------------------------------------
    Step 3: Run test application for linux environment
    ----------------------------------------------------------
    [54] Run test application ($RTE_TARGET/app/test)
    [55] Run testpmd application in interactive mode ($RTE_TARGET/app/testpmd)

    ----------------------------------------------------------
    Step 4: Other tools
    ----------------------------------------------------------
    [56] List hugepage info from /proc/meminfo

    ----------------------------------------------------------
    Step 5: Uninstall and system cleanup
    ----------------------------------------------------------
    [57] Unbind devices from IGB UIO or VFIO driver
    [58] Remove IGB UIO module
    [59] Remove VFIO module
    [60] Remove KNI module
    [61] Remove hugepage mappings

    [62] Exit Script

    Option: 
```

执行脚本后出现`Option:`，选择需要进行的操作，第一步输入`41`，选择编译的工具`x86_64-native-linux-gcc`开始编译：

```
    Configuration done using x86_64-native-linux-gcc
    == Build lib
    == Build lib/librte_kvargs
    == Build lib/librte_eal
    ...
    ...
    Build complete [x86_64-native-linux-gcc]
    Installation cannot run with T defined and DESTDIR undefined
    ------------------------------------------------------------------------------
    RTE_TARGET exported as x86_64-native-linux-gcc
    ------------------------------------------------------------------------------

    Press enter to continue ...
```

> 最后`Installation cannot run with T defined and DESTDIR undefined`提示未指定安装路径，但是我们的目的是编译生成`igb_uio`模块，用来将内核管理下的网卡接管到DPDK，本来也不需要安装，所以不影响使用，忽略即可。

执行后出现上述提示，按回车键继续执行脚本，回到`Option:`选择，第二步输入`45`，加载`igb_uio`模块：

```
    Option: 45

    Unloading any existing DPDK UIO module
    Loading DPDK UIO module

    Press enter to continue ...
```

再按回车键继续执行脚本，回到`Option:`选择，第二步输入`51`，绑定网卡的接口：

```
    Option: 51

    Network devices using kernel driver
    ===================================
    0000:03:00.0 '82546EB Gigabit Ethernet Controller (Copper) 1010' if=enp3s0f0 drv=e1000 unused=igb_uio,vfio-pci,uio_pci_generic 
    0000:03:00.1 '82546EB Gigabit Ethernet Controller (Copper) 1010' if=enp3s0f1 drv=e1000 unused=igb_uio,vfio-pci,uio_pci_generic 
    0000:04:00.0 'RTL8111/8168/8411 PCI Express Gigabit Ethernet Controller 8168' if=enp4s0 drv=r8169 unused=igb_uio,vfio-pci,uio_pci_generic *Active*

    No 'Baseband' devices detected
    ==============================

    No 'Crypto' devices detected
    ============================

    No 'Eventdev' devices detected
    ==============================

    No 'Mempool' devices detected
    =============================

    No 'Compress' devices detected
    ==============================

    No 'Misc (rawdev)' devices detected
    ===================================

    Enter PCI address of device to bind to IGB UIO driver:
```

输入`51`后弹出当前所有网卡接口的信息，这里支持DPDK的两个接口分别是`0000:03:00.0`和`0000:03:00.1`，分别输入这两个网卡接口的PCI地址：

```
    Enter PCI address of device to bind to IGB UIO driver: 0000:03:00.0
    OK

    Press enter to continue ...
```

第一个接口绑定成功，回显提示`OK`，再按回车键继续执行脚本，回到`Option:`选择，再次输入`51`，绑定第二个网卡接口：

```
    Option: 51

    Network devices using DPDK-compatible driver
    ============================================
    0000:03:00.0 '82546EB Gigabit Ethernet Controller (Copper) 1010' drv=igb_uio unused=e1000,vfio-pci,uio_pci_generic

    Network devices using kernel driver
    ===================================
    0000:03:00.1 '82546EB Gigabit Ethernet Controller (Copper) 1010' if=enp3s0f1 drv=e1000 unused=igb_uio,vfio-pci,uio_pci_generic 
    0000:04:00.0 'RTL8111/8168/8411 PCI Express Gigabit Ethernet Controller 8168' if=enp4s0 drv=r8169 unused=igb_uio,vfio-pci,uio_pci_generic *Active*

    No 'Baseband' devices detected
    ==============================

    No 'Crypto' devices detected
    ============================

    No 'Eventdev' devices detected
    ==============================

    No 'Mempool' devices detected
    =============================

    No 'Compress' devices detected
    ==============================

    No 'Misc (rawdev)' devices detected
    ===================================

    Enter PCI address of device to bind to IGB UIO driver: 
```

这个时候的回显信息显示第一个接口已经由DPDK接管，输入第二个接口的PCI地址：

```
    Enter PCI address of device to bind to IGB UIO driver: 0000:03:00.1
    OK

    Press enter to continue ...
```

第二个接口绑定成功，回显提示`OK`，再按回车键继续执行脚本，回到`Option:`，输入`62`退出脚本执行。

**4. 查看接口绑定：**

```shell
    $ ./usertools/dpdk-devbind.py -s

    Network devices using DPDK-compatible driver
    ============================================
    0000:03:00.0 '82546EB Gigabit Ethernet Controller (Copper) 1010' drv=igb_uio unused=e1000,vfio-pci,uio_pci_generic
    0000:03:00.1 '82546EB Gigabit Ethernet Controller (Copper) 1010' drv=igb_uio unused=e1000,vfio-pci,uio_pci_generic

    Network devices using kernel driver
    ===================================
    0000:04:00.0 'RTL8111/8168/8411 PCI Express Gigabit Ethernet Controller 8168' if=enp4s0 drv=r8169 unused=igb_uio,vfio-pci,uio_pci_generic *Active*

    No 'Baseband' devices detected
    ==============================

    No 'Crypto' devices detected
    ============================

    No 'Eventdev' devices detected
    ==============================

    No 'Mempool' devices detected
    =============================

    No 'Compress' devices detected
    ==============================

    No 'Misc (rawdev)' devices detected
    ===================================
```

可以看到网卡的两个接口都被DPDK接管，在`Network devices using DPDK-compatible driver`下显示，两个网卡接口的收发包都将由DPDK进行处理，不经过系统内核。

### 开始使用VPP

**1. 修改启动配置**

VPP和DPDK编译，以及DPDK接口绑定完成之后，在电脑环境下开始使用VPP。

```shell
    # vppctl
        _______    _        _   _____  ___ 
    __/ __/ _ \  (_)__    | | / / _ \/ _ \
    _/ _// // / / / _ \   | |/ / ___/ ___/
    /_/ /____(_)_/\___/   |___/_/  /_/    

    vpp# 
    vpp# show interface 
Name Idx State MTU (L3/IP4/IP6/MPLS) Counter Count
local0 0 down 0/0/0/0
    vpp# 
```

进入VPP之后查看接口信息，发现只有一个`local0`接口，并没有显示DPDK绑定的两个网卡接口。需要修改VPP的启动配置`/etc/vpp/startup.conf`：

```
    ...
    dpdk {
            ## Change default settings for all interfaces
            # dev default {
                    ## Number of receive queues, enables RSS
                    ## Default is 1
                    # num-rx-queues 3

                    ## Number of transmit queues, Default is equal
                    ## to number of worker threads or 1 if no workers treads
                    # num-tx-queues 3

                    ## Number of descriptors in transmit and receive rings
                    ## increasing or reducing number can impact performance
                    ## Default is 1024 for both rx and tx
                    # num-rx-desc 512
                    # num-tx-desc 512

                    ## VLAN strip offload mode for interface
                    ## Default is off
                    # vlan-strip-offload on

                    ## TCP Segment Offload
                    ## Default is off
                    ## To enable TSO, 'enable-tcp-udp-checksum' must be set
                    # tso on

                    ## Devargs
                    ## device specific init args
                    ## Default is NULL
                    # devargs safe-mode-support=1,pipeline-mode-support=1
            # }

            ## Whitelist specific interface by specifying PCI address
            # dev 0000:02:00.0
            dev 0000:03:00.0
            dev 0000:03:00.1
    ...
```

找到启动配置中的`dpdk`部分，将`dpdk`前的注释`#`删除，并在括号内添加绑定的两个网卡接口地址。然后重启VPP服务，重新进入VPP SHELL：

```shell
    $ service vpp restart
    $ vppctl
        _______    _        _   _____  ___ 
    __/ __/ _ \  (_)__    | | / / _ \/ _ \
    _/ _// // / / / _ \   | |/ / ___/ ___/
    /_/ /____(_)_/\___/   |___/_/  /_/    

    vpp# show interface 
                Name               Idx    State  MTU (L3/IP4/IP6/MPLS)     Counter          Count     
    GigabitEthernet3/0/0              1     down         9000/0/0/0     
    GigabitEthernet3/0/1              2     down         9000/0/0/0     
    local0                            0     down          0/0/0/0       
    vpp# 
```

再次查看接口信息，能看到绑定的两个网卡接口。

**2. 开始使用**

配置接口IP：

```shell
    vpp# set interface state GigabitEthernet3/0/0 up
    vpp# set interface ip address GigabitEthernet3/0/0 192.168.1.222/24
    vpp# show interface address
    GigabitEthernet3/0/0 (up):
    L3 192.168.1.222/24
    GigabitEthernet3/0/1 (dn):
    local0 (dn):
    vpp# 
```

连接网线后，从另一主机`ping`刚才设置的接口`IP`，

```
    PS C:\Users\BJHUAHUAN> ping 192.168.1.222 -S 192.168.1.123 -t
    正在 Ping 192.168.1.222 从 192.168.1.123 具有 32 字节
    来自 192.168.1.123 的回复: 无法访问目标主机。
    来自 192.168.1.222 的回复: 字节=32 时间=998ms TTL=64
    来自 192.168.1.222 的回复: 字节=32 时间<1ms TTL=64
    来自 192.168.1.222 的回复: 字节=32 时间<1ms TTL=64
    来自 192.168.1.222 的回复: 字节=32 时间<1ms TTL=64
    来自 192.168.1.222 的回复: 字节=32 时间<1ms TTL=64
```

能`ping`通网卡接口，说明VPP功能正常，其他功能也可以正常使用。

使能报文跟踪功能，查看`ping`操作的报文处理流程：

```shell
    vpp# trace add dpdk-input 20
    vpp# show trace     
    ------------------- Start of thread 0 vpp_main -------------------
    No packets in trace buffer
    vpp# show trace 
    ------------------- Start of thread 0 vpp_main -------------------
    No packets in trace buffer
    vpp# show trace 
    ------------------- Start of thread 0 vpp_main -------------------
    Packet 1

    00:19:32:452955: dpdk-input
    GigabitEthernet3/0/0 rx queue 0
    buffer 0x9e3a5: current data 0, length 60, buffer-pool 0, ref-count 1, totlen-nifb 0, trace handle 0x0
                    ext-hdr-valid 
                    l4-cksum-computed l4-cksum-correct 
    PKT MBUF: port 0, nb_segs 1, pkt_len 60
        buf_len 2176, data_len 60, ol_flags 0x0, data_off 128, phys_addr 0x8e78e9c0
        packet_type 0x0 l2_len 0 l3_len 0 outer_l2_len 0 outer_l3_len 0
        rss 0x0 fdir.hi 0x0 fdir.lo 0x0
    ARP: 08:57:00:f1:f3:0a -> ff:ff:ff:ff:ff:ff
    request, type ethernet/IP4, address size 6/4
    08:57:00:f1:f3:0a/192.168.1.123 -> 00:00:00:00:00:00/192.168.1.222
    00:19:32:452962: ethernet-input
    frame: flags 0x3, hw-if-index 1, sw-if-index 1
    ARP: 08:57:00:f1:f3:0a -> ff:ff:ff:ff:ff:ff
    00:19:32:452967: arp-input
    request, type ethernet/IP4, address size 6/4
    08:57:00:f1:f3:0a/192.168.1.123 -> 00:00:00:00:00:00/192.168.1.222
    00:19:32:452969: arp-reply
    request, type ethernet/IP4, address size 6/4
    08:57:00:f1:f3:0a/192.168.1.123 -> 00:00:00:00:00:00/192.168.1.222
    00:19:32:453029: GigabitEthernet3/0/0-output
    GigabitEthernet3/0/0 
    ARP: 00:11:0e:16:21:f6 -> 08:57:00:f1:f3:0a
    reply, type ethernet/IP4, address size 6/4
    00:11:0e:16:21:f6/192.168.1.222 -> 08:57:00:f1:f3:0a/192.168.1.123
    00:19:32:453030: GigabitEthernet3/0/0-tx
    GigabitEthernet3/0/0 tx queue 0
    buffer 0x9e3a5: current data 0, length 60, buffer-pool 0, ref-count 1, totlen-nifb 0, trace handle 0x0
                    ext-hdr-valid 
                    l4-cksum-computed l4-cksum-correct l2-hdr-offset 0 l3-hdr-offset 14 
    PKT MBUF: port 0, nb_segs 1, pkt_len 60
        buf_len 2176, data_len 60, ol_flags 0x0, data_off 128, phys_addr 0x8e78e9c0
        packet_type 0x0 l2_len 0 l3_len 0 outer_l2_len 0 outer_l3_len 0
        rss 0x0 fdir.hi 0x0 fdir.lo 0x0
    ARP: 00:11:0e:16:21:f6 -> 08:57:00:f1:f3:0a
    reply, type ethernet/IP4, address size 6/4
    00:11:0e:16:21:f6/192.168.1.222 -> 08:57:00:f1:f3:0a/192.168.1.123

    Packet 2

    00:19:32:453178: dpdk-input
    GigabitEthernet3/0/0 rx queue 0
    buffer 0x9e37e: current data 0, length 74, buffer-pool 0, ref-count 1, totlen-nifb 0, trace handle 0x1
                    ext-hdr-valid 
                    l4-cksum-computed l4-cksum-correct 
    PKT MBUF: port 0, nb_segs 1, pkt_len 74
        buf_len 2176, data_len 74, ol_flags 0x0, data_off 128, phys_addr 0x8e78e000
        packet_type 0x0 l2_len 0 l3_len 0 outer_l2_len 0 outer_l3_len 0
        rss 0x0 fdir.hi 0x0 fdir.lo 0x0
    IP4: 08:57:00:f1:f3:0a -> 00:11:0e:16:21:f6
    ICMP: 192.168.1.123 -> 192.168.1.222
        tos 0x00, ttl 64, length 60, checksum 0xc195 dscp CS0 ecn NON_ECN
        fragment id 0x3482
    ICMP echo_request checksum 0x7175
    00:19:32:453180: ethernet-input
    frame: flags 0x3, hw-if-index 1, sw-if-index 1
    IP4: 08:57:00:f1:f3:0a -> 00:11:0e:16:21:f6
    00:19:32:453180: ip4-input-no-checksum
    ICMP: 192.168.1.123 -> 192.168.1.222
        tos 0x00, ttl 64, length 60, checksum 0xc195 dscp CS0 ecn NON_ECN
        fragment id 0x3482
    ICMP echo_request checksum 0x7175
    00:19:32:453181: ip4-lookup
    fib 0 dpo-idx 6 flow hash: 0x00000000
    ICMP: 192.168.1.123 -> 192.168.1.222
        tos 0x00, ttl 64, length 60, checksum 0xc195 dscp CS0 ecn NON_ECN
        fragment id 0x3482
    ICMP echo_request checksum 0x7175
    00:19:32:453184: ip4-local
        ICMP: 192.168.1.123 -> 192.168.1.222
        tos 0x00, ttl 64, length 60, checksum 0xc195 dscp CS0 ecn NON_ECN
        fragment id 0x3482
        ICMP echo_request checksum 0x7175
    00:19:32:453184: ip4-icmp-input
    ICMP: 192.168.1.123 -> 192.168.1.222
        tos 0x00, ttl 64, length 60, checksum 0xc195 dscp CS0 ecn NON_ECN
        fragment id 0x3482
    ICMP echo_request checksum 0x7175
    00:19:32:453185: ip4-icmp-echo-request
    ICMP: 192.168.1.123 -> 192.168.1.222
        tos 0x00, ttl 64, length 60, checksum 0xc195 dscp CS0 ecn NON_ECN
        fragment id 0x3482
    ICMP echo_request checksum 0x7175
    00:19:32:453189: ip4-load-balance
    fib 0 dpo-idx 1 flow hash: 0x00000000
    ICMP: 192.168.1.222 -> 192.168.1.123
        tos 0x00, ttl 64, length 60, checksum 0x370b dscp CS0 ecn NON_ECN
        fragment id 0xbf0c
    ICMP echo_reply checksum 0x7975
    00:19:32:453190: ip4-rewrite
    tx_sw_if_index 1 dpo-idx 1 : ipv4 via 192.168.1.123 GigabitEthernet3/0/0: mtu:9000 next:3 085700f1f30a00110e1621f60800 flow hash: 0x00000000
    00000000: 085700f1f30a00110e1621f608004500003cbf0c00004001370bc0a801dec0a8
    00000020: 017b000079750001dbe56162636465666768696a6b6c6d6e6f707172
    00:19:32:453191: GigabitEthernet3/0/0-output
    GigabitEthernet3/0/0 
    IP4: 00:11:0e:16:21:f6 -> 08:57:00:f1:f3:0a
    ICMP: 192.168.1.222 -> 192.168.1.123
        tos 0x00, ttl 64, length 60, checksum 0x370b dscp CS0 ecn NON_ECN
        fragment id 0xbf0c
    ICMP echo_reply checksum 0x7975
    00:19:32:453192: GigabitEthernet3/0/0-tx
    GigabitEthernet3/0/0 tx queue 0
    buffer 0x9e37e: current data 0, length 74, buffer-pool 0, ref-count 1, totlen-nifb 0, trace handle 0x1
                    ext-hdr-valid 
                    l4-cksum-computed l4-cksum-correct local l2-hdr-offset 0 l3-hdr-offset 14 
    PKT MBUF: port 0, nb_segs 1, pkt_len 74
        buf_len 2176, data_len 74, ol_flags 0x0, data_off 128, phys_addr 0x8e78e000
        packet_type 0x0 l2_len 0 l3_len 0 outer_l2_len 0 outer_l3_len 0
        rss 0x0 fdir.hi 0x0 fdir.lo 0x0
    IP4: 00:11:0e:16:21:f6 -> 08:57:00:f1:f3:0a
    ICMP: 192.168.1.222 -> 192.168.1.123
        tos 0x00, ttl 64, length 60, checksum 0x370b dscp CS0 ecn NON_ECN
        fragment id 0xbf0c
    ICMP echo_reply checksum 0x7975

```

