

从ELOOP出去，然后接进ILOOP，然后就走业务封装；

解封装的时候，vxlan解封装后到ELOOP，再走ILOOP继续MPLS解封装
 
只能用个特定的vlan，mpls封装的时候走内部口，arp带特定的vlan。

> 上行和下行需要使用不同的loop口，避免配置冲突导致无法解封装

## 拓扑

106 设备1/3/2口接仪表打流
```
                     -----                                    -----
                    |     |                                  |     |
           gi 1/3/2 |     |  g 1/1/1                g 1/1/1  |     | g 1/2/2
        ----------->| 106 | -------------------------------> | 107 | ----------->
                    |     |                                  |     |
                    |     |                                  |     |
                     -----                                    -----
```


## 配置

### 配置说明

```
###################################### 106 设备配置 ######################################
in g 1/1/1
ip address 1.0.0.2 255.255.255.0

in g 1/3/2
portswitch
switchport mode trunk
switchport trunk allowed vlan 10 confirm


#
#MPLS上行vxlan封装
#UNI->MPLS->VXLAN
#

#第一圈，封装PW label，另外封装一层tunnel label，这一层tunnel label只用于loop后匹配

### 需要创建两个loop口，上下行分开使用，这里只配置一个说明
port allocate internal-port gchip 0 type iloop 
nexthop add iloop 79 lpbk-lport 79 
port allocate internal-port gchip 0 type eloop nhid 79

l3if create ifid 99 type phy-if gport 79
l3if ifid 99 mpls-property mpls-en value 1
l3if ifid 99 router-mac 0000.0000.0002


#ARP封装临时VLAN 2用于LOOP前后关联
nexthop add arp-id 10 mac 0.1.2 vlan 2 gport 79

nexthop add mpls-tunnel 13 working-path arp-id 10 label1 16 ttl1 255 exp1 0 exp1-type 1
nexthop add mpls 23 push fwd payload-op op-l2vpn cvlan-edit-type 0 svlan-edit-type 0 output-cvid 0 output-svid 0 tunnel-id 13 label1 100 ttl1 255 exp1 0 exp1-type 1
port 1 scl-key-type scl-id 0 direction ingress type svid
vlan mapping add port 1 svid 10 mapping-to nexthop 23



#### 删除命令
vlan mapping remove port 1 svid 10
port 1 scl-key-type scl-id 0 direction ingress type disable
nexthop remove mpls 23
nexthop remove mpls-tunnel 13
nexthop remove arp-id 10
l3if destroy ifid 99 type phy-if gport 79


#########################################################################################

#第二圈，走ILOOP封装VXLAN


#vxlan vni及洪范域
overlay-tunnel vn-id 1000 fid 4096
l2 fdb add vlan-default-entry fid 4096 group 4096 use-logic-port 
 
 
#MPLS业务LOOP后通过VLAN 2接入业务，这里VLAN 2是否需要配置删除
port 79 scl-key-type scl-id 0 direction ingress type svid
vlan mapping add port 79 svid 2 mapping-to vn-id 1000 logic-port 500 scl-id 0


#vxlan出口走的arp
nexthop add arp-id 20 mac 000E.5E6B.0004 gport 27
nexthop add ipuc 30 fwd arp-id 20
ipuc add 0 10.10.10.10 32 30


#vxlan nexthop  
nexthop add ip-tunnel 10 dsnh-offset 10 fwd arp-id 20 type tunnel-v4 vxlan strip-vlan ipsa 1.0.0.2 ipda 1.0.0.1 dscp 0 dscp-select 0 ttl 10 map-ttl logic-dest-port 1000
l2 fdb add mac 0.0.2 fid 4096 nexthop 10 static



#vxlan解封装 
port 27 scl-key-type scl-id 1 direction ingress type vxlan
overlay-tunnel add vxlan src-vnid 1000 ipda 1.0.0.2 ipsa 1.0.0.1 dst-vnid 1000 tunnel-src-port 1000 scl-id 1
nexthop add egs-vlan-edit 20 port 79 cvlan-edit-type 1 svlan-edit-type 3 output-cvid 1 output-svid 10 svlan-valid logic-dest-port 500
#l2 fdb add mac 0000.0001.0002 fid 4096 nexthop 20 static

l2 fdb add mac 000e.5e6a.0014 fid 4096 nexthop 20 static


#
l2 fdb logic-nhid logic-port 1000 nexthop 10
l2 fdb logic-nhid logic-port 500 nexthop 20
l2 fdb vlan-default-entry fid 4096 add nexthop 10
l2 fdb vlan-default-entry fid 4096 add nexthop 20
 



### 删除命令
l2 fdb logic-nhid logic-port 1000 nexthop 0
l2 fdb logic-nhid logic-port 500 nexthop 0
l2 fdb remove vlan-default-entry fid 4096 

overlay-tunnel vn-id 1000 fid 0
port 79 scl-key-type scl-id 0 direction ingress type disable
port 27 scl-key-type scl-id 0 direction ingress type disable

nexthop remove ip-tunnel 10
nexthop remove egs-vlan-edit 20

ipuc remove 0 10.10.10.10 32 30
nexthop remove ipuc 30
nexthop remove arp-id 20

vlan mapping remove port 79 svid 2
overlay-tunnel del vxlan src-vnid 1000 ipda 1.0.0.2 ipsa 1.0.0.1 scl-id 1

l3if router-mac 0.0.0

l2 fdb flush by all dynamic
l2 fdb flush by all static 





Inject to mac 1
0x00000000   0x00020000   0x00000102   0x8100000a   
0x08004500   0x003e0000   0x00004037   0x63860b01   
0x01010a01   0x01010000   0x00000000   0x00000000   
0x00000000   0x00000000   0x00000000   0x00000000   
0x00000000   0x00000000   0x00000000   0x00000000   
0x00000000   0x00000000   0x00000000   0x00000000   
0x00000000   0x00000000   0x00000000   
CTC_CLI(ctc-cmodel)# 
Leave at mac 2
0x000e000e   0x000e001f   0xfeafadfb   0x08004500   
0x00aa0000   0x0000f511   0xbf3d0101   0x01010202   
0x0202c000   0x12b50096   0x00000800   0x00000003   
0xe8000000   0x00010002   0x001ffeaf   0xadfb8847   
0x000100ff   0x000641ff   0x00000000   0x00020000   
0x00000102   0x8100000a   0x08004500   0x003e0000   
0x00004037   0x63860b01   0x01010a01   0x01010000   
0x00000000   0x00000000   0x00000000   0x00000000   
0x00000000   0x00000000   0x00000000   0x00000000   
0x00000000   0x00000000   0x00000000   0x00000000   
0x00000000   0x00000000   0x00000000   0x00000000   
0x00000000













###################################### 107 设备配置 ######################################
in g 1/1/1
ip address 1.0.0.1 255.255.255.0
in g 1/2/2
portswitch 
switchport mode trunk 
switchport trunk allowed vlan 10 confirm

### 需要创建两个loop口，上下行分开使用，这里只配置一个说明
port allocate internal-port gchip 0 type iloop 
nexthop add iloop 79 lpbk-lport 79 
port allocate internal-port gchip 0 type eloop nhid 79

### loop口l3if配置
l3if create ifid 99 type phy-if gport 79
l3if ifid 99 router-mac 0000.0001.0002
l3if ifid 99 mpls-property mpls-en value 1


#ARP封装临时VLAN 2用于LOOP前后关联
nexthop add arp-id 10 mac 0.1.2 vlan 2 gport 79

nexthop add mpls-tunnel 13 working-path arp-id 10 label1 16 ttl1 255 exp1 0 exp1-type 1
nexthop add mpls 23 push fwd payload-op op-l2vpn cvlan-edit-type 0 svlan-edit-type 0 output-cvid 0 output-svid 0 tunnel-id 13 label1 100 ttl1 255 exp1 0 exp1-type 1
port 49 scl-key-type scl-id 0 direction ingress type svid
vlan mapping add port 49 svid 10 mapping-to nexthop 23


### mpls 解封装命令
mpls ilm add space 0 label 16 pop
nexthop add egs-vlan-edit 106 dsnh-offset 106 port 49 cvlan-edit-type 0 svlan-edit-type 3 output-cvid 0 output-svid 10 svlan-valid
mpls l2vpn-pw add 100 vpws 106 encapsulation-mode tagged


#### 删除命令
vlan mapping remove port 49 svid 10
port 49 scl-key-type scl-id 0 direction ingress type disable
nexthop remove mpls 23
nexthop remove mpls-tunnel 13
nexthop remove arp-id 10
l3if destroy ifid 99 type phy-if gport 79

nexthop remove egs-vlan-edit 106
mpls ilm remove space 0 label 16
mpls l2vpn-pw remove 100




#########################################################################################

#第二圈，走ILOOP封装VXLAN


#vxlan vni及洪范域
overlay-tunnel vn-id 1000 fid 4096
l2 fdb add vlan-default-entry fid 4096 group 4096 use-logic-port 
 
#MPLS业务LOOP后通过VLAN 2接入业务，这里VLAN 2是否需要配置删除
port 79 scl-key-type scl-id 0 direction ingress type svid


#这个配置上行封装有效，下行不能配，跟下面的vlan mapping不能使用同一个loop
vlan mapping add port 79 svid 2 mapping-to vn-id 1000 logic-port 500 scl-id 0

#user-vlanptr 0x1063表示0x1000+L3IF ID，下行解封装配置，需要与上面的vlan mapping使用不同的loop口
vlan mapping add port 79 default-entry mapping-to user-vlanptr 0x1063


#vxlan出口走的arp
nexthop add arp-id 20 mac 000E.5E6A.0014 gport 27
nexthop add ipuc 30 fwd arp-id 20
ipuc add 0 10.10.10.10 32 30


#vxlan nexthop  
nexthop add ip-tunnel 10 dsnh-offset 10 fwd arp-id 20 type tunnel-v4 vxlan strip-vlan ipsa 1.0.0.1 ipda 1.0.0.2 dscp 0 dscp-select 0 ttl 10 map-ttl logic-dest-port 1000
l2 fdb add mac 000E.5E6A.0014 fid 4096 nexthop 10 static



#vxlan解封装 
port 27 scl-key-type scl-id 1 direction ingress type vxlan
overlay-tunnel add vxlan src-vnid 1000 ipda 1.0.0.1 ipsa 1.0.0.2 dst-vnid 1000 tunnel-src-port 1000 scl-id 1
nexthop add egs-vlan-edit 20 port 79 cvlan-edit-type 1 svlan-edit-type 3 output-cvid 1 output-svid 2 svlan-valid logic-dest-port 500
l2 fdb add mac 0000.0001.0002 fid 4096 nexthop 20 static


#
l2 fdb logic-nhid logic-port 1000 nexthop 10
l2 fdb logic-nhid logic-port 500 nexthop 20
l2 fdb vlan-default-entry fid 4096 add nexthop 10
l2 fdb vlan-default-entry fid 4096 add nexthop 20
 



### 删除命令
l2 fdb logic-nhid logic-port 1000 nexthop 0
l2 fdb logic-nhid logic-port 500 nexthop 0
l2 fdb remove vlan-default-entry fid 4096 

overlay-tunnel vn-id 1000 fid 0
port 79 scl-key-type scl-id 0 direction ingress type disable
port 27 scl-key-type scl-id 0 direction ingress type disable

nexthop remove ip-tunnel 10
nexthop remove egs-vlan-edit 20

ipuc remove 0 10.10.10.10 32 30
nexthop remove ipuc 30
nexthop remove arp-id 20

vlan mapping remove port 79 default-entry
vlan mapping remove port 79 svid 2
overlay-tunnel del vxlan src-vnid 1000 ipda 1.0.0.1 ipsa 1.0.0.2 scl-id 1

l3if router-mac 0.0.0

l2 fdb flush by all dynamic
l2 fdb flush by all static
```

### 106 上行封装配置

```
port allocate internal-port gchip 0 type iloop 
nexthop add iloop 79 lpbk-lport 79 
port allocate internal-port gchip 0 type eloop nhid 79

l3if create ifid 99 type phy-if gport 79
l3if ifid 99 mpls-property mpls-en value 1
l3if ifid 99 router-mac 0000.0000.0002


nexthop add arp-id 10 mac 0.1.2 vlan 2 gport 79

nexthop add mpls-tunnel 13 working-path arp-id 10 label1 16 ttl1 255 exp1 0 exp1-type 1
nexthop add mpls 23 push fwd payload-op op-l2vpn cvlan-edit-type 0 svlan-edit-type 0 output-cvid 0 output-svid 0 tunnel-id 13 label1 100 ttl1 255 exp1 0 exp1-type 1
port 1 scl-key-type scl-id 0 direction ingress type svid
vlan mapping add port 1 svid 10 mapping-to nexthop 23


overlay-tunnel vn-id 1000 fid 4096
l2 fdb add vlan-default-entry fid 4096 group 4096 use-logic-port 

port 79 scl-key-type scl-id 0 direction ingress type svid
vlan mapping add port 79 svid 2 mapping-to vn-id 1000 logic-port 500 scl-id 0

nexthop add arp-id 20 mac 000E.5E6B.0004 gport 27
nexthop add ipuc 30 fwd arp-id 20
ipuc add 0 10.10.10.10 32 30

nexthop add ip-tunnel 10 dsnh-offset 10 fwd arp-id 20 type tunnel-v4 vxlan strip-vlan ipsa 1.0.0.2 ipda 1.0.0.1 dscp 0 dscp-select 0 ttl 10 map-ttl logic-dest-port 1000
l2 fdb add mac 0.0.2 fid 4096 nexthop 10 static


l2 fdb logic-nhid logic-port 1000 nexthop 10
l2 fdb vlan-default-entry fid 4096 add nexthop 10
```


### 107 下行解封装配置

```
port allocate internal-port gchip 0 type iloop 
nexthop add iloop 79 lpbk-lport 79 
port allocate internal-port gchip 0 type eloop nhid 79


l3if create ifid 99 type phy-if gport 79
l3if ifid 99 router-mac 0000.0001.0002
l3if ifid 99 mpls-property mpls-en value 1


mpls ilm add space 0 label 16 pop
nexthop add egs-vlan-edit 106 dsnh-offset 106 port 49 cvlan-edit-type 0 svlan-edit-type 3 output-cvid 0 output-svid 10 svlan-valid
mpls l2vpn-pw add 100 vpws 106 encapsulation-mode tagged



overlay-tunnel vn-id 1000 fid 4096
l2 fdb add vlan-default-entry fid 4096 group 4096 use-logic-port 

port 79 scl-key-type scl-id 0 direction ingress type svid
vlan mapping add port 79 default-entry mapping-to user-vlanptr 0x1063

port 27 scl-key-type scl-id 1 direction ingress type vxlan
overlay-tunnel add vxlan src-vnid 1000 ipda 1.0.0.1 ipsa 1.0.0.2 dst-vnid 1000 tunnel-src-port 1000 scl-id 1
nexthop add egs-vlan-edit 20 port 79 cvlan-edit-type 1 svlan-edit-type 3 output-cvid 1 output-svid 2 svlan-valid logic-dest-port 500
l2 fdb add mac 0000.0001.0002 fid 4096 nexthop 20 static

l2 fdb logic-nhid logic-port 500 nexthop 20
l2 fdb vlan-default-entry fid 4096 add nexthop 20
```


## 报文

### 仪表发送报文

```
0000   00 00 00 00 00 02 00 00 00 00 01 02 81 00 00 0a
0010   08 00 45 00 00 6a 9b 85 00 00 ff fd 08 0e 0b 01
0020   01 01 0a 01 01 01 00 00 00 00 00 00 00 00 00 00
0030   00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
0040   00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
0050   00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
0060   00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
0070   00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
```

### 106设备1/1/口封装报文

```
0000   00 0e 5e 6b 00 04 00 0e 5e 6a 00 14 08 00 45 00
0010   00 b6 72 c2 00 00 f5 11 50 72 01 00 00 02 01 00
0020   00 01 c0 75 12 b5 00 a2 00 00 08 00 00 00 00 03
0030   e8 00 00 00 00 01 00 02 00 00 00 00 00 02 88 47
0040   00 01 00 ff 00 06 41 ff 00 00 00 00 00 02 00 00
0050   00 00 01 02 81 00 00 0a 08 00 45 00 00 6a ab 71
0060   00 00 ff fd f8 21 0b 01 01 01 0a 01 01 01 00 00
0070   00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
0080   00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
0090   00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
00a0   00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
00b0   04 b6 a4 9f a1 7a 1d 5b ce 35 66 a6 1c d3 58 53
00c0   7e db 6e f6 42 b8 b3 23
```


### 106 设备loop口流水

```
CTC_CLI(ctc-sdk)# diag trigger pkt-trace mode network-packet watch-key src-lport 79
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# show diag pkt-trace detail 
Show Diag Packet Trace:
--------------------------------
Dest Type           :Network
Dest Port           :0x001b
Mcast Group         :4096
Interface Id        :79
Current Position    :EPE
--------------------------------
Detail Path Info:
------------------------------------------------

1st Parsing
    mac-da                   :0000.0001.0002
    mac-sa                   :0000.0000.0002
    svlan-id                 :2
    stag-cos                 :0
    ether-type               :0x00008847
    layer3-type              :L3TYPE_MPLS
    mpls-label-1             :100
    mpls-label-0             :16
    layer4-type              :L4TYPE_NONE
    udf-hit-index            :3

SCL
    ad-type-0                :USERID
    hash-0-lookup-result     :HIT
    key-index                :300
    key-name                 :DsUserIdSvlanPortHashKey
    ad-index                 :72
    ad-name                  :DsUserIdHalf
    hash-1-lookup-result     :HIT-DEFAULT-ENTRY
    key-index                :300
    key-name                 :DsUserId1SvlanPortHashKey
    ad-index                 :7838
    ad-name                  :DsUserId1
    tcam-1-lookup-result     :HIT-NONE
    tcam-2-lookup-result     :HIT-NONE
    tcam-3-lookup-result     :HIT-NONE

Interface Mapper
    source-port              :0x0000004f
    vlan-ptr                 :2
    bridge-port              :YES
    route-port               :NO

Decapsulation

Lookup Manager
    inner-macsa-lookup       :TRUE
    macda-lookup             :ENABLE
    key-macda                :0000.0001.0002
    key-fid                  :4096
    macsa-lookup             :ENABLE
    key-macsa                :0000.0000.0002
    key-fid                  :4096

Layer2 Bridging
    macda-lookup-result      :HIT-DEFAULT-ENTRY
    bridge-fid               :4096
    ad-name                  :DsMac
    ad-index                 :4096
    bridge-stromctl          :ENABLE

Per-hop Behavior
    phb-color                :Green
    phb-prio                 :0
    phb-classified-cfi       :0
    phb-classified-cos       :0
    phb-classified-dscp      :0
    phb-classified-tc        :0

IPE OAM

ACL/TCAM Processing
    TCAM0-lookup-result      :HIT-NONE
    key-type                 :TCAMFWDEXT
    TCAM1-lookup-result      :HIT-NONE
    key-type                 :TCAML2KEY
    TCAM2-lookup-result      :HIT-NONE
    key-type                 :TCAML3EXTKEY
    TCAM3-lookup-result      :HIT-NONE
    key-type                 :TCAML2L3KEY
    TCAM4-lookup-result      :HIT-NONE
    key-type                 :TCAML2L3KEY
    TCAM5-lookup-result      :HIT-NONE
    key-type                 :TCAMCOPPEXTKEY
    TCAM6-lookup-result      :HIT-NONE
    key-type                 :TCAML2KEY
    TCAM7-lookup-result      :HIT-NONE
    key-type                 :TCAMFWDEXT

Storm Control
    stormctl-color           :Green
    stormctl-type            :Unknow-Ucast
    stormctl-length          :154

Learning
    macsa-lookup-result      :HIT
    key-name                 :DsFibHost0MacHashKey
    key-index                :2200
    ad-name                  :DsMac
    ad-index                 :19966
    do-learning              :N

Destination Processing
    packet-header            :0x00000000000000020000000000000000000000000000000000000000000000000000000000000000
    dest-type                :Mcast Group
    mcast-group              :4096

BufferStore
    met-fifo-priority        :MCAST-L

MetFifo
    dest-map                 :0x00041000
    forward-type             :MCAST
    mcast-group              :4096
    met-entry-0              :HIT
    table-name               :DsMetEntry3W
    table-index              :4096
    met-mode                 :link-list
    dest-gport               :0x00000002
    replication-count        :0
    nexthop-ptr              :0x0000000a

EPE Header Adjust
    channel-id               :27
    local-phy-port           :27
    nexthop-ptr              :0x0000000a
    nexthop-table-name       :DsNextHop8W
    nexthop-table-index      :10
    mux-length-type          :0
    packet-length-adjust     :0
    packet-length-adjust-type:0
    operation-type           :NORMAL

Egress Parsing
    mac-da                   :0000.0001.0002
    mac-sa                   :0000.0000.0002
    svlan-id                 :2
    stag-cos                 :0
    ether-type               :0x00008847
    layer3-type              :L3TYPE_MPLS
    mpls-label-1             :100
    mpls-label-0             :16
    layer4-type              :L4TYPE_NONE
    udf-hit-index            :3

NextHop Mapper
    dest-gport               :0x0000001b
    vlan-ptr                 :4175
    interface-id             :79
    logic-port               :1000
    payload-opration         :BRIDGE-VPLS

SCL Process
    hash-0-lookup-result     :HIT-DEFAULT-ENTRY
    key-name                 :DsEgressXcOamDoubleVlanPortHashKey
    key-index                :2812
    hash-1-lookup-result     :HIT-DEFAULT-ENTRY
    key-name                 :DsEgressXcOamSvlanPortHashKey
    key-index                :332

Layer3 Packet Editing
    TUNNELV4-edit            :ENABLE
    edit-table-index         :12
    edit-table-name          :DsL3EditTunnelV4

Layer2 Packet Editing
    svlan-edit               :Delete
    mac-edit                 :ENABLE
    edit-table-index         :5
    edit-table-name          :DsL2Edit3WOuter
    new-macda                :000e.5e6b.0004

Egress ACL/TCAM Processing
    TCAM0-lookup-result      :HIT-NONE
    key-type                 :TCAML2L3KEYEXT
    TCAM1-lookup-result      :HIT-NONE
    key-type                 :TCAML2KEY
    TCAM2-lookup-result      :HIT-NONE
    key-type                 :TCAML3EXTKEY

EPE Header Editing
    new-l2-header-length     :14
    new-l3-header-length     :36

CTC_CLI(ctc-sdk)#
```


### 107 设备1/1/1口收包流水

```
CTC_CLI(ctc-sdk)# diag trigger pkt-trace mode network-packet watch-key src-lport 27
CTC_CLI(ctc-sdk)# show diag pkt-trace detail 
Show Diag Packet Trace:
--------------------------------
Dest Type           :Network
Dest Port           :0x004f
Dest Channel        :75
Interface Id        :79
Current Position    :EPE
--------------------------------
Detail Path Info:
------------------------------------------------

1st Parsing
    mac-da                   :000e.5e6b.0004
    mac-sa                   :000e.5e6a.0014
    ether-type               :0x00000800
    layer3-type              :L3TYPE_IPV4
    ip-protocol              :0x00000011
    ip-da                    :1.0.0.1
    ip-sa                    :1.0.0.2
    layer4-type              :L4TYPE_UDP
    l4-src-port              :49269
    l4-dst-port              :4789
    udf-hit-index            :1

SCL
    hash-0-lookup-result     :HIT-DEFAULT-ENTRY
    key-index                :7388
    key-name                 :DsUserIdDoubleVlanPortHashKey
    ad-index                 :7734
    ad-name                  :DsUserId
    hash-1-lookup-result     :HIT
    key-index                :5944
    key-name                 :DsUserId1TunnelIpv4UcVxlanMode1HashKey
    ad-index                 :86
    ad-name                  :DsTunnelId1
    tcam-0-lookup-result     :HIT-NONE
    tcam-2-lookup-result     :HIT-NONE
    tcam-3-lookup-result     :HIT-NONE

Interface Mapper
    source-port              :0x0000001b
    vlan-ptr                 :4175
    l3-intf-id               :79
    intf-vrfid               :0
    route-mac                :YES
    bridge-port              :YES
    route-port               :YES
    mpls-port                :YES

Decapsulation
    tunnel-process           :inner-packet-forward

2nd Parsing
    mac-da                   :0000.0001.0002
    mac-sa                   :0000.0000.0002
    ether-type               :0x00008847
    layer3-type              :L3TYPE_MPLS
    mpls-label-1             :100
    mpls-label-0             :16
    layer4-type              :L4TYPE_NONE
    udf-hit-index            :3

Lookup Manager
    inner-macsa-lookup       :TRUE
    macda-lookup             :ENABLE
    key-macda                :0000.0001.0002
    key-fid                  :4096
    macsa-lookup             :ENABLE
    key-macsa                :0000.0000.0002
    key-fid                  :4096

Layer2 Bridging
    macda-lookup-result      :HIT
    key-name                 :DsFibHost0MacHashKey
    key-index                :2944
    ad-name                  :DsMac
    ad-index                 :19967

Per-hop Behavior
    phb-color                :Green
    phb-prio                 :0
    phb-classified-cfi       :0
    phb-classified-cos       :0
    phb-classified-dscp      :0
    phb-classified-tc        :0

IPE OAM

ACL/TCAM Processing
    TCAM0-lookup-result      :HIT-NONE
    key-type                 :TCAMFWDEXT
    TCAM1-lookup-result      :HIT-NONE
    key-type                 :TCAML2KEY
    TCAM2-lookup-result      :HIT-NONE
    key-type                 :TCAML3EXTKEY
    TCAM3-lookup-result      :HIT-NONE
    key-type                 :TCAML2L3KEY
    TCAM4-lookup-result      :HIT-NONE
    key-type                 :TCAML2L3KEY
    TCAM5-lookup-result      :HIT-NONE
    key-type                 :TCAMCOPPEXTKEY
    TCAM6-lookup-result      :HIT-NONE
    key-type                 :TCAML2KEY
    TCAM7-lookup-result      :HIT-NONE
    key-type                 :TCAMFWDEXT

Learning
    macsa-lookup-result      :HIT
    key-name                 :DsFibHost0MacHashKey
    key-index                :2200
    ad-name                  :DsMac
    ad-index                 :21480
    do-learning              :N

Destination Processing
    dest-type                :Network Port
    network-port             :0x0000004f
    nexthop-lookup-result    :HIT
    nexthop-ptr              :0x00003e94

BufferStore
    met-fifo-priority        :UCAST-L
    dest-channel             :75

MetFifo
    dest-map                 :0x0000004f
    forward-type             :UCAST

Queue
    channel-id               :75
    queue-id                 :600
    dest-map                 :0x0000004f
    mapped-sc                :0
    mapped-tc                :0

BufRetrv
    queue-id                 :600
    packet-header            :0x320009E002A4003600187D280000413C3120000FA3FC0040000DE0D9E017000100EA000780000000

EPE Header Adjust
    channel-id               :75
    local-phy-port           :79
    nexthop-ptr              :0x00003e94
    nexthop-table-name       :DsNextHop4W
    nexthop-table-index      :16020
    mux-length-type          :0
    strip-packet(bytes)      :50
    packet-length-adjust     :206
    packet-length-adjust-type:1
    operation-type           :NORMAL

Egress Parsing
    mac-da                   :0000.0001.0002
    mac-sa                   :0000.0000.0002
    ether-type               :0x00008847
    layer3-type              :L3TYPE_MPLS
    mpls-label-1             :100
    mpls-label-0             :16
    layer4-type              :L4TYPE_NONE
    udf-hit-index            :3

NextHop Mapper
    dest-gport               :0x0000004f
    vlan-ptr                 :4175
    output-svlan             :2
    interface-id             :79
    logic-port               :500
    payload-opration         :BRIDGE

SCL Process
    hash-0-lookup-result     :HIT-DEFAULT-ENTRY
    key-name                 :DsEgressXcOamDoubleVlanPortHashKey
    key-index                :6420
    hash-1-lookup-result     :HIT-DEFAULT-ENTRY
    key-name                 :DsEgressXcOamSvlanPortHashKey
    key-index                :6508

Layer3 Packet Editing

Layer2 Packet Editing
    svlan-edit               :Add
    new-cos                  :0
    new-cfi                  :0
    new-vlan-id              :2

Egress ACL/TCAM Processing
    TCAM0-lookup-result      :HIT-NONE
    key-type                 :TCAML2L3KEYEXT
    TCAM1-lookup-result      :HIT-NONE
    key-type                 :TCAML2KEY
    TCAM2-lookup-result      :HIT-NONE
    key-type                 :TCAML3EXTKEY

EPE Header Editing
    loopback                 :ENABLE

CTC_CLI(ctc-sdk)# 
```

### 107 设备loop口流水

```
CTC_CLI(ctc-sdk)# diag trigger pkt-trace mode network-packet watch-key src-lport 79
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# show diag pkt-trace detail                                       
Show Diag Packet Trace:
--------------------------------
Dest Type           :Network
Dest Port           :0x0031
Dest Channel        :49
Interface Id        :99
Current Position    :EPE
--------------------------------
Detail Path Info:
------------------------------------------------

1st Parsing
    mac-da                   :0000.0001.0002
    mac-sa                   :0000.0000.0002
    svlan-id                 :2
    stag-cos                 :0
    ether-type               :0x00008847
    layer3-type              :L3TYPE_MPLS
    mpls-label-1             :100
    mpls-label-0             :16
    layer4-type              :L4TYPE_NONE
    udf-hit-index            :3

SCL
    ad-type-0                :USERID
    hash-0-lookup-result     :HIT-DEFAULT-ENTRY
    key-index                :300
    key-name                 :DsUserIdSvlanPortHashKey
    ad-index                 :7838
    ad-name                  :DsUserId
    hash-1-lookup-result     :HIT-DEFAULT-ENTRY
    key-index                :300
    key-name                 :DsUserId1SvlanPortHashKey
    ad-index                 :7838
    ad-name                  :DsUserId1
    tcam-1-lookup-result     :HIT-NONE
    tcam-2-lookup-result     :HIT-NONE
    tcam-3-lookup-result     :HIT-NONE

Interface Mapper
    source-port              :0x0000004f
    vlan-ptr                 :4195
    l3-intf-id               :99
    intf-vrfid               :0
    route-mac                :YES
    bridge-port              :YES
    route-port               :NO
    mpls-port                :YES

Decapsulation
    mpls-process             :mpls-label-forward
    1st-label-lookup-result  :HIT
    key-name                 :DsMplsLabelHashKey
    key-index                :1120
    ad-name                  :DsMpls
    ad-index                 :1056
    pop-lable                :YES
    mpls-process             :mpls-label-forward
    2nd-label-lookup-result  :HIT
    key-name                 :DsMplsLabelHashKey
    key-index                :1176
    ad-name                  :DsMpls
    ad-index                 :1112
    pop-lable                :YES
    mpls-process             :mpls-label-forward
    3th-label-lookup-result  :HIT
    key-name                 :DsMplsLabelHashKey
    key-index                :1120
    ad-name                  :DsMpls
    ad-index                 :1056
    pop-lable                :YES

2nd Parsing
    mac-da                   :0000.0000.0002
    mac-sa                   :0000.0000.0102
    svlan-id                 :10
    stag-cos                 :0
    ether-type               :0x00000800
    layer3-type              :L3TYPE_IPV4
    ip-protocol              :0x000000fd
    ip-da                    :10.1.1.1
    ip-sa                    :11.1.1.1
    layer4-type              :L4TYPE_NONE
    udf-hit-index            :1

Lookup Manager
    macsa-lookup             :ENABLE
    key-macsa                :0000.0000.0002
    key-fid                  :0

Per-hop Behavior
    phb-color                :Green
    phb-prio                 :0
    phb-classified-cfi       :0
    phb-classified-cos       :0
    phb-classified-dscp      :0
    phb-classified-tc        :0

IPE OAM

ACL/TCAM Processing
    TCAM0-lookup-result      :HIT-NONE
    key-type                 :TCAMFWDEXT
    TCAM1-lookup-result      :HIT-NONE
    key-type                 :TCAML2KEY
    TCAM2-lookup-result      :HIT-NONE
    key-type                 :TCAML3EXTKEY
    TCAM3-lookup-result      :HIT-NONE
    key-type                 :TCAML2L3KEY
    TCAM4-lookup-result      :HIT-NONE
    key-type                 :TCAML2L3KEY
    TCAM5-lookup-result      :HIT-NONE
    key-type                 :TCAMCOPPKEY
    TCAM6-lookup-result      :HIT-NONE
    key-type                 :TCAML2KEY
    TCAM7-lookup-result      :HIT-NONE
    key-type                 :TCAMFWDEXT

Learning
    macsa-lookup-result      :HIT-DEFAULT-ENTRY
    do-learning              :N

Destination Processing
    packet-header            :0x00000000000000360000000000000000000000000000000000000000000000000000000000000000
    dest-type                :Network Port
    network-port             :0x00000031
    nexthop-lookup-result    :HIT
    nexthop-ptr              :0x0000006a

BufferStore
    met-fifo-priority        :UCAST-L
    dest-channel             :49

MetFifo
    dest-map                 :0x00000031
    forward-type             :UCAST

Queue
    channel-id               :49
    queue-id                 :392
    dest-map                 :0x00000031
    mapped-sc                :0
    mapped-tc                :0

BufRetrv
    queue-id                 :392
    packet-header            :0x1A0006200240009E005800D40040418C11000007D3FC00400827A9BE86D180010120000780000000

EPE Header Adjust
    channel-id               :49
    local-phy-port           :49
    nexthop-ptr              :0x0000006a
    nexthop-table-name       :DsNextHop4W
    nexthop-table-index      :106
    mux-length-type          :0
    strip-packet(bytes)      :26
    packet-length-adjust     :230
    packet-length-adjust-type:1
    operation-type           :NORMAL

Egress Parsing
    mac-da                   :0000.0000.0002
    mac-sa                   :0000.0000.0102
    svlan-id                 :10
    stag-cos                 :0
    ether-type               :0x00000800
    layer3-type              :L3TYPE_IPV4
    ip-protocol              :0x000000fd
    ip-da                    :10.1.1.1
    ip-sa                    :11.1.1.1
    layer4-type              :L4TYPE_NONE
    udf-hit-index            :1

NextHop Mapper
    dest-gport               :0x00000031
    vlan-ptr                 :4195
    output-svlan             :10
    interface-id             :99
    logic-port               :4
    payload-opration         :BRIDGE

SCL Process
    hash-0-lookup-result     :HIT-DEFAULT-ENTRY
    key-name                 :DsEgressXcOamDoubleVlanPortHashKey
    key-index                :796
    hash-1-lookup-result     :HIT-DEFAULT-ENTRY
    key-name                 :DsEgressXcOamSvlanPortHashKey
    key-index                :920

Layer3 Packet Editing

Layer2 Packet Editing
    svlan-edit               :Replace
    new-cos                  :0
    new-cfi                  :0
    new-vlan-id              :10

Egress ACL/TCAM Processing
    TCAM0-lookup-result      :HIT-NONE
    key-type                 :TCAML2L3KEYEXT
    TCAM1-lookup-result      :HIT-NONE
    key-type                 :TCAML2KEY
    TCAM2-lookup-result      :HIT-NONE
    key-type                 :TCAML3EXTKEY

EPE Header Editing

CTC_CLI(ctc-sdk)# 
```

### 107 设备1/2/2口发出报文

```
0000   00 00 00 00 00 02 00 00 00 00 01 02 81 00 00 0a
0010   08 00 45 00 00 6a 9b 84 00 00 ff fd 08 0f 0b 01
0020   01 01 0a 01 01 01 00 00 00 00 00 00 00 00 00 00
0030   00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
0040   00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
0050   00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
0060   00 00 00 00 00 00 00 00 f1 66 c1 ce 0d f2 27 67
0070   b1 f1 a0 22 3d 2f cb d4 02 5d b9 fb 0a 82 eb a3
```