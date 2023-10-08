
## 环境：

- 一块mx02在17槽
- 一块tx02在8槽
- 一块ES08SE在21槽

---

## 现象：

- 步骤 1：上述环境掉电重启，mx02无法ping通es08se板卡

- 步骤 2：两种操作分开
    - （1）如果保持es08se不动，mx02换到16槽，ping不通，然后插拔es08se，可以ping通
    - （2）如果mx02换到16槽，整机掉电重启，可以ping通

- 步骤 3：mx02再换到17槽，es08se保持不动，可以ping通

- 步骤 4：mx02在17槽保持不动，插拔es08se板卡，ping不通

- 步骤 5：重启写背板phy接口的0xa000地址，eth0会变成1000M，然后再配置eth0强制100M，可以ping通
    ```
    ./tools/nm_phy_rw w 0x1e  0xa000
    ./tools/nm_phy_rw w 0x1f  0x0002
    ethtool -s eth0 autoneg off speed 100 duplex full
    ```

- 步骤 6：整机掉电重启、插拔mx02、插拔es08se，都ping不通

---

## 定位：

在es08se上读背板phy地址0xa000、0xa006、0xa001：
```
/mnt/home # ./tools/nm_phy_rw w 0x1e  0xa000
write phyid[5] reg[30] data[0xa000]

/mnt/home # ./tools/nm_phy_rw r 0x1f
read  phyid[5] reg[31]: 0x0000

/mnt/home # 
/mnt/home # ./tools/nm_phy_rw w 0x1e  0xa006
write phyid[5] reg[30] data[0xa006]

/mnt/home # ./tools/nm_phy_rw r 0x1f
read  phyid[5] reg[31]: 0x000c

/mnt/home # 
/mnt/home # ./tools/nm_phy_rw w 0x1e  0xa001
write phyid[5] reg[30] data[0xa001]

/mnt/home # ./tools/nm_phy_rw r 0x1f
read  phyid[5] reg[31]: 0x8152

/mnt/home # 
```

期望的0xa000地址值应该是0x0002，在start.sh脚本里已经用工具配置过。并且这个时候eth0的状态是正确的100M
```
/mnt/home # ethtool eth0
Settings for eth0:
        Supported ports: [ TP MII ]
        Supported link modes:   10baseT/Half 10baseT/Full 
                                100baseT/Half 100baseT/Full 
                                1000baseT/Half 1000baseT/Full 
        Supported pause frame use: Symmetric Receive-only
        Supports auto-negotiation: Yes
        Supported FEC modes: Not reported
        Advertised link modes:  100baseT/Full 
        Advertised pause frame use: Symmetric Receive-only
        Advertised auto-negotiation: No
        Advertised FEC modes: Not reported
        Speed: 100Mb/s
        Duplex: Full
        Port: MII
        PHYAD: 5
        Transceiver: external
        Auto-negotiation: off
        Supports Wake-on: umag
        Wake-on: umag
        Link detected: yes
/mnt/home # 
```



> **原因不明**

---

## 解决方法：

重新配置phy的0xa000地址，配置完后eth0会变成1000M，在配置eth0强制百兆，就可以ping通了




---

ping通板卡eth0的phy接口初始化：
```
#phy初始化
./tools/nm_phy_rw w 0x1e  0xa000
./tools/nm_phy_rw w 0x1f  0x0002

./tools/nm_phy_rw w 0x1e  0xa006
./tools/nm_phy_rw w 0x1f  0x000C

./tools/nm_phy_rw w 0x1e  0xa001
./tools/nm_phy_rw w 0x1f  0x0152

#板卡根据槽位号添加虚拟网卡，配置强制100M
vconfig add eth0 21
ifconfig eth0.21 up
ifconfig eth0.21 1.0.1.21 netmask 255.255.255.0
ethtool -s eth0 autoneg off speed 100 duplex full

```
