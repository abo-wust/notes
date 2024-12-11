
## 拓扑

```

                     -----                                    -----
                    |     |                                  |     |
           gi 1/3/2 |     | gi 1/3/4               gi 1/3/2  |     | gi 1/2/2
        ----------->| 102 | -------------------------------> | 105 | ----------->
                    |     |                                  |     |
                    |     |                                  |     |
                     -----                                    -----

```


## SDK 配置命令
### 102 配置

```
#1/3/2口配置加入vlan 10，20，1/3/4口配置IP 1.1.1.1

port 0x0003 phy-if enable
l3if create ifid 2 type vlan-if vlan 10
l3if router-mac 000E.5E66.0001

overlay-tunnel vn-id 1000 fid 4096


#port 1/3/2 -> 1/3/4，fwd mac配置成105设备的网络口1/3/2的接口mac

port 0x0001 scl-key-type scl-id 0 direction ingress type ipsg-port-mac
vlan mapping add port 0x0001 macsa 0000.aaaa.1111 mapping-to vn-id 1000 logic-port 500

nexthop add ip-tunnel 10 dsnh-offset 10 fwd mac 000E.5E69.000D routed-port 0x0003 type tunnel-v4 vxlan strip-vlan ipsa 1.1.1.1 ipda 1.1.1.2 dscp 0 dscp-select 0 ttl 10 map-ttl logic-dest-port 1000
l2 fdb add mac 0000.aaaa.0000 fid 4096 nexthop 10 static 


#port 1/3/4 -> 1/3/2

port 0x0003 scl-key-type scl-id 0 direction ingress type vxlan
overlay-tunnel add vxlan src-vnid 1000 ipda 1.1.1.1 ipsa 1.1.1.2 dst-vnid 1000 tunnel-src-port 1000
nexthop add egs-vlan-edit 20 port 0x0001 cvlan-edit-type 1 svlan-edit-type 3 output-cvid 1 output-svid 10 svlan-valid logic-dest-port 500
l2 fdb add mac 0000.aaaa.1111 fid 4096 nexthop 20 static



l2 fdb add vlan-default-entry fid 4096 group 4096 use-logic-port 
l2 fdb logic-nhid logic-port 1000 nexthop 10
l2 fdb logic-nhid logic-port 500 nexthop 20
l2 fdb vlan-default-entry fid 4096 add nexthop 10
l2 fdb vlan-default-entry fid 4096 add nexthop 20

```

### 102 配置删除

```

l2 fdb logic-nhid logic-port 1000 nexthop 0
l2 fdb logic-nhid logic-port 500 nexthop 0
l2 fdb remove vlan-default-entry fid 4096 

overlay-tunnel vn-id 1000 fid 0
port 0x0001 scl-key-type scl-id 0 direction ingress type disable
port 0x0003 scl-key-type scl-id 0 direction ingress type disable

nexthop remove ip-tunnel 10
nexthop remove egs-vlan-edit 20

vlan mapping remove port 0x0001 macsa 0000.aaaa.1111
overlay-tunnel del vxlan src-vnid 1000 ipda 1.1.1.1 ipsa 1.1.1.2
port 0x0003 phy-if disable

l3if destroy ifid 2 type vlan-if vlan 10
l3if router-mac 0.0.0

l2 fdb flush by all dynamic
l2 fdb flush by all static 

```


### 105 配置

```
#1/2/2口配置加入vlan 10，20，1/3/2口配置IP 1.1.1.2

port 0x0001 phy-if enable
l3if create ifid 2 type vlan-if vlan 10
l3if router-mac 000E.5E69.0001

overlay-tunnel vn-id 1000 fid 4096



#port 1/2/2 -> 1/3/2，fwd mac配置成102设备的网络口1/3/4的接口mac

port 49 scl-key-type scl-id 0 direction ingress type ipsg-port-mac
vlan mapping add port 49 macsa 0000.aaaa.0000 mapping-to vn-id 1000 logic-port 500

nexthop add ip-tunnel 10 dsnh-offset 10 fwd mac 000E.5E66.000F routed-port 0x0001 type tunnel-v4 vxlan strip-vlan ipsa 1.1.1.2 ipda 1.1.1.1 dscp 0 dscp-select 0 ttl 10 map-ttl logic-dest-port 1000
l2 fdb add mac 0000.aaaa.1111 fid 4096 nexthop 10 static 


#port 1/3/2 -> 1/2/2

port 0x0001 scl-key-type scl-id 0 direction ingress type vxlan
overlay-tunnel add vxlan src-vnid 1000 ipda 1.1.1.2 ipsa 1.1.1.1 dst-vnid 1000 tunnel-src-port 1000
nexthop add egs-vlan-edit 20 port 49 cvlan-edit-type 1 svlan-edit-type 3 output-cvid 1 output-svid 10 svlan-valid logic-dest-port 500
l2 fdb add mac 0000.aaaa.0000 fid 4096 nexthop 20 static



l2 fdb add vlan-default-entry fid 4096 group 4096 use-logic-port 
l2 fdb logic-nhid logic-port 1000 nexthop 10
l2 fdb logic-nhid logic-port 500 nexthop 20
l2 fdb vlan-default-entry fid 4096 add nexthop 10
l2 fdb vlan-default-entry fid 4096 add nexthop 20

```


### 105 配置删除

```
l2 fdb logic-nhid logic-port 1000 nexthop 0
l2 fdb logic-nhid logic-port 500 nexthop 0
l2 fdb remove vlan-default-entry fid 4096 

overlay-tunnel vn-id 1000 fid 0
port 1 scl-key-type scl-id 0 direction ingress type disable
port 49 scl-key-type scl-id 0 direction ingress type disable

nexthop remove ip-tunnel 10
nexthop remove egs-vlan-edit 20

vlan mapping remove port 49 macsa 0000.aaaa.0000
overlay-tunnel del vxlan src-vnid 1000 ipda 1.1.1.2 ipsa 1.1.1.1
port 0x0003 phy-if disable

l3if destroy ifid 2 type vlan-if vlan 10
l3if router-mac 0.0.0

l2 fdb flush by all dynamic
l2 fdb flush by all static 

```


## SDK 报文流水

在 105 设备上抓包分析，从1/3/2口（sdk端口0x01）收包后的流水如下：

```
CTC_CLI(ctc-sdk)# show diag pkt-trace detail 
Show Diag Packet Trace:
--------------------------------
Dest Type           :Network
Dest Port           :0x0031
Dest Channel        :49
Interface Id        :86
Current Position    :EPE
--------------------------------
Detail Path Info:
------------------------------------------------

1st Parsing
    mac-da                   :000e.5e69.000d
    mac-sa                   :000e.5e66.000f
    ether-type               :0x00000800
    layer3-type              :L3TYPE_IPV4
    ip-protocol              :0x00000011
    ip-da                    :1.1.1.2
    ip-sa                    :1.1.1.1
    layer4-type              :L4TYPE_UDP
    l4-src-port              :49197
    l4-dst-port              :4789
    udf-hit-index            :1

SCL
    hash-0-lookup-result     :HIT
    key-index                :3936
    key-name                 :DsUserIdTunnelIpv4UcVxlanMode1HashKey
    ad-index                 :72
    ad-name                  :DsTunnelId
    hash-1-lookup-result     :HIT-DEFAULT-ENTRY
    key-index                :5720
    key-name                 :DsUserId1SvlanPortHashKey
    ad-index                 :7682
    ad-name                  :DsUserId1
    tcam-1-lookup-result     :HIT-NONE
    tcam-2-lookup-result     :HIT-NONE
    tcam-3-lookup-result     :HIT-NONE

Interface Mapper
    source-port              :0x00000001
    vlan-ptr                 :4182
    l3-intf-id               :86
    intf-vrfid               :0
    route-mac                :YES
    bridge-port              :NO
    route-port               :YES
    mpls-port                :YES

Decapsulation
    tunnel-process           :inner-packet-forward

2nd Parsing
    mac-da                   :0000.aaaa.0000
    mac-sa                   :0000.aaaa.1111
    ether-type               :0x00000800
    layer3-type              :L3TYPE_IPV4
    ip-protocol              :0x000000fd
    ip-da                    :10.0.0.1
    ip-sa                    :10.0.0.2
    layer4-type              :L4TYPE_NONE
    udf-hit-index            :1

Lookup Manager
    inner-macsa-lookup       :TRUE
    macda-lookup             :ENABLE
    key-macda                :0000.aaaa.0000
    key-fid                  :4096
    macsa-lookup             :ENABLE
    key-macsa                :0000.aaaa.1111
    key-fid                  :4096

Layer2 Bridging
    macda-lookup-result      :HIT
    key-name                 :DsFibHost0MacHashKey
    key-index                :9648
    ad-name                  :DsMac
    ad-index                 :19964

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
    macsa-lookup-result      :HIT
    key-name                 :DsFibHost0MacHashKey
    key-index                :11308
    ad-name                  :DsMac
    ad-index                 :19963
    do-learning              :N

Destination Processing
    dest-type                :Network Port
    network-port             :0x00000031
    nexthop-lookup-result    :HIT
    nexthop-ptr              :0x00003efc

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
    packet-header            :0x3200062000B4000200187DF8000041583120000FA3FC0040000084BA29390001005A000180000000

EPE Header Adjust
    channel-id               :49
    local-phy-port           :49
    nexthop-ptr              :0x00003efc
    nexthop-table-name       :DsNextHop4W
    nexthop-table-index      :16124
    mux-length-type          :0
    strip-packet(bytes)      :50
    packet-length-adjust     :206
    packet-length-adjust-type:1
    operation-type           :NORMAL

Egress Parsing
    mac-da                   :0000.aaaa.0000
    mac-sa                   :0000.aaaa.1111
    ether-type               :0x00000800
    layer3-type              :L3TYPE_IPV4
    ip-protocol              :0x000000fd
    ip-da                    :10.0.0.1
    ip-sa                    :10.0.0.2
    layer4-type              :L4TYPE_NONE
    udf-hit-index            :1

NextHop Mapper
    dest-gport               :0x00000031
    vlan-ptr                 :4182
    output-svlan             :10
    interface-id             :86
    logic-port               :500
    payload-opration         :BRIDGE

SCL Process
    hash-0-lookup-result     :HIT-DEFAULT-ENTRY
    key-name                 :DsEgressXcOamDoubleVlanPortHashKey
    key-index                :980
    hash-1-lookup-result     :HIT-DEFAULT-ENTRY
    key-name                 :DsEgressXcOamSvlanPortHashKey
    key-index                :940

Layer3 Packet Editing

Layer2 Packet Editing
    svlan-edit               :Add
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