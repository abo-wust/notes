


hw-rw 0 command phy/ge2/DIG_CTL1000X1r/FIBER_MODE_1000X=0
hw-rw 0 command phy/ge2/COMBO_MIICTLr/RESTART_AUTONEG=1


加载fpga
cat system_vc12.bin  > /dev/xdevcfg

初始化锁相环：
i2cset -y 0 0x70 1 7
./idt_8A3400 init 0x58 es08s.tcs

使能网卡
ifconfig eth1 up

关闭广播
./rpc8211fs_mmd write 0x00 0xa005 0x80


killall es08s

./fpga_slot read 9 0x000 32

./fpga_slot w 21 0x13 0x10
./fpga_slot w 21 0x400 0xf000
./fpga_slot read 21 0x440 16
./fpga_slot read 21 0x0 16

./fpga_slot w 21 0x13 0x10
./fpga_slot read 21 0x414 2


//查看时钟状态
    i2cset -y 0 0x70 1 7
    i2cset -y 0 0x58 0xfd 0xc0
    i2cdump -y 0 0x58


/mnt # i2cdump -y 0 0x58
```
     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f    0123456789abcdef
00: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
10: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
20: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
30: 00 00 00 00 3a 00 00 00 00 00 00 00 00 08 01 01    ....:........???
40: 58 02 01 5b 11 00 00 00 00 00 00 00 00 00 00 00    X??[?...........
50: 00 00 00 00 34 34 34 34 00 00 00 00 00 10 1f 1f    ....4444.....???
60: 1f 1f 1f 1f 1f 1f 00 00 00 00 00 00 00 00 00 00    ??????..........
70: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
80: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
90: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
a0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
b0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
c0: 00 00 00 00 00 00 f8 ff 00 e0 00 00 00 00 00 00    ......?..?......
d0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
e0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
f0: 00 00 00 00 00 00 00 00 00 00 00 00 00 c0 10 20    .............?? 
/mnt # 
/mnt # i2cdump -y 0 0x58
     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f    0123456789abcdef
00: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
10: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
20: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
30: 00 00 00 00 3a 00 00 00 00 00 00 00 00 08 01 01    ....:........???
40: 58 02 01 5b 10 00 00 00 00 00 00 00 00 00 00 00    X??[?...........
50: 00 00 00 00 33 33 33 33 00 00 00 00 00 10 00 00    ....3333.....?..
60: 00 00 1f 1f 1f 1f 00 00 00 00 00 00 00 00 00 00    ..????..........
70: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
80: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
90: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
a0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
b0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
c0: 00 00 00 00 00 00 f8 ff 00 e0 00 00 00 00 00 00    ......?..?......
d0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
e0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
f0: 00 00 00 00 00 00 00 00 00 00 00 00 00 c0 10 20    .............?? 
/mnt # 

```


echo "fpga_imagetype=system_vc12.bin" > image_type.txt


在主控上可以加上
vconfig add eth2 18
vconfig add eth2 19
vconfig add eth2 20
vconfig add eth2 21
ifconfig eth2.18 up
ifconfig eth2.19 up
ifconfig eth2.20 up
ifconfig eth2.21 up
brctl addbr bridge    名字如果重复了，就换个别的
brctl stp bridge 0
brctl addif eth2.18
brctl addif eth2.19
brctl addif eth2.20
brctl addif eth2.21
ifconfig bridge 1.0.1.1 up      主备都加上，注意备的时候 需要 down这个桥，防止主备同时发包影响支路

支路上根据槽位 eth0 增加特定vlan， ip用 1.0.1.slot


写eth0 的phy
./tools/nm_phy_rw w 0x1e  0xa000
./tools/nm_phy_rw w 0x1f  0x0002

./tools/nm_phy_rw w 0x1e  0xa006
./tools/nm_phy_rw w 0x1f  0x000C

./tools/nm_phy_rw w 0x1e  0xa001
./tools/nm_phy_rw w 0x1f  0x0152


vconfig add eth0 21
ifconfig eth0.21 up
ifconfig eth0.21 1.0.1.21 netmask 255.255.255.0
ethtool -s eth0 autoneg off speed 100 duplex full


读eth0 的phy状态
./tools/nm_phy_rw w 0x1e  0xa000
./tools/nm_phy_rw r 0x1f

./tools/nm_phy_rw w 0x1e  0xa006
./tools/nm_phy_rw r 0x1f

./tools/nm_phy_rw w 0x1e  0xa001
./tools/nm_phy_rw r 0x1f



./fpga_mx w 0x20  0x12
./fpga_mx w 0x21  0x13
./fpga_mx w 0x22  0x14
./fpga_mx w 0x23  0x15
./fpga_mx w 0x24  0x8
./fpga_mx w 0x25  0x9




./fpga_slot w 21 0x01a 0x0f52
./fpga_slot w 21 0x21a 0x0ec2
./fpga_slot w 21 0x81a 0x0e32
./fpga_slot w 21 0xa1a 0x0da2


在支路上执行
echo 38 > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio38/direction


mx02 在主
echo 1 > /sys/class/gpio/gpio38/value

mx02 在备
echo 0 > /sys/class/gpio/gpio38/value




i2cset -y 0 0x70 1 7
./idt_8A3400 init 0x58 es08s.tcs

cat system_vc3.bin  > /dev/xdevcfg


./ip link set can0 up type can bitrate 125000
./ip -details link show can0
./ip -details -statistics link show can0



./nm_phy_rw w 0x1e  0xa000
./nm_phy_rw w 0x1f  0x0002


vconfig add eth0 21
ifconfig eth0.21 hw ether 00:1D:80:00:00:15
ifconfig eth0.21 up 1.0.1.21 netmask 255.255.255.0
ethtool -s eth0 autoneg off speed 100 duplex full



cat system_vc12.bin > /dev/xdevcfg
ifconfig eth1 1.0.21.1 netmask 255.255.255.0



cat system_vc12.bin > /dev/xdevcfg
ifconfig eth1 1.0.21.2 netmask 255.255.255.0


### 初始化
./rpc8211fs_mmd w 1 0xa006 0xb
./rpc8211fs_mmd w 1 0xa001 0x0152
./rpc8211fs_mmd w 1 0xa000 0x2
./rpc8211fs_mmd w 1 0xa005 0x80
./rpc8211fs_mmd w 1 0xa00e 0x4670


### phy外环
./rpc8211fs_mmd w 0 0xa006 0x4b
./rpc8211fs_mmd w 1 0xa006 0x4b


### 探测eeprom，切7探测pll时钟
i2cset -y 0 0x70 1 4
i2cdetect -ry 0



第一片有ABC，AB分别对应8/9槽位的TX02，只需要检测主TX02的主备状态。
第二片有AC，没有B
第三片有AC，没有B
第四片有A，没有BC


### 延时寄存器，在mx02上读写
./fpga_slot r 21 0x01a 4
./fpga_slot r 21 0x21a 4
./fpga_slot r 21 0x81a 4
./fpga_slot r 21 0xa1a 4


9544片选4，数字标签地址0x51
    片选5，PLL的eeprom地址0x51、0x55
    片选7，PLL的软件读写地址0x58



启动步骤：
1、烧写nor程序，焊上
2、第一片主，copy sd卡的程序到emmc；跳塞到12烧写pll（tools下的工具 ./plltoeeprom bin8A34012.bin），跳塞到23
3、第2-4片，copy sd卡程序到emmc














