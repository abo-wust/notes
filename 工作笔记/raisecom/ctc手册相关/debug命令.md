
### packet trace

接口配置：
```
interface gigaethernet 1/5/7
portswitch
switchport trunk allowed vlan 3,100,200,300,513,515
switchport mode trunk
switchport vlan-mapping both cvlan 100  add-outer 200  translate 300

interface gigaethernet 1/5/8
portswitch
switchport trunk allowed vlan 3,100,200,300,513,515
switchport mode trunk
```

打开`sdk`的`packet trace`：
```
diag trigger pkt-trace mode network-packet svlan 100 watch-point ipe-scl
```

往`1/5/7`口打`vlan 100`的报文，业务正常，规则生效，查看`packet trace`的详细信息：
```
CTC_CLI(ctc-sdk)#
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# show diag pkt-trace
Show Diag Packet Trace:
--------------------------------
Dest Type           :Network
Dest Port           :0x0018
Mcast Group         :200
Current Position    :EPE
--------------------------------
CTC_CLI(ctc-sdk)#
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)#
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# show diag pkt-trace detail 
Show Diag Packet Trace:
--------------------------------
Dest Type           :Network
Dest Port           :0x0018
Mcast Group         :200
Current Position    :EPE
--------------------------------
Detail Path Info:
------------------------------------------------

1st Parsing
    mac-da                   :0000.0100.0001
    mac-sa                   :0010.9400.0002
    svlan-id                 :100
    stag-cos                 :5
    ether-type               :0x00000800
    layer3-type              :L3TYPE_IPV4
    ip-protocol              :0x000000fd
    ip-da                    :192.0.0.1
    ip-sa                    :192.85.1.2
    layer4-type              :L4TYPE_NONE
    udf-hit-index            :1

SCL
    ad-type-1                :USERID
    hash-0-lookup-result     :HIT-DEFAULT-ENTRY
    key-index                :1088
    key-name                 :DsUserIdDoubleVlanPortHashKey
    ad-index                 :15922
    ad-name                  :DsUserId
    hash-1-lookup-result     :HIT
    key-index                :1960
    key-name                 :DsUserId1SvlanPortHashKey
    ad-index                 :80
    ad-name                  :DsUserId1
    tcam-0-lookup-result     :HIT-NONE
    tcam-1-lookup-result     :HIT-NONE
    tcam-2-lookup-result     :HIT-NONE
    tcam-3-lookup-result     :HIT-NONE

Interface Mapper
    source-port              :0x00000019
    vlan-ptr                 :200
    ingress-svlan-action     :USER
    new-svlan-id             :200
    ingress-cvlan-action     :USER
    new-cvlan-id             :300
    bridge-port              :YES
    route-port               :NO

Decapsulation

Lookup Manager
    inner-macsa-lookup       :TRUE
    macda-lookup             :ENABLE
    key-macda                :0000.0100.0001
    key-fid                  :200
    macsa-lookup             :ENABLE
    key-macsa                :0010.9400.0002
    key-fid                  :200

Layer2 Bridging
    macda-lookup-result      :HIT-DEFAULT-ENTRY
    bridge-fid               :200
    ad-name                  :DsMac
    ad-index                 :200
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
    key-type                 :TCAMCOPPKEY
    TCAM6-lookup-result      :HIT-NONE
    key-type                 :TCAML2KEY
    TCAM7-lookup-result      :HIT-NONE
    key-type                 :TCAMFWDEXT

Storm Control
    stormctl-color           :Green
    stormctl-type            :Unknow-Ucast
    stormctl-length          :128

Learning
    macsa-lookup-result      :HIT-DEFAULT-ENTRY
    learning-mode            :HW
    learning-fid             :200
    learning-global-port     :25
    aging-ptr                :0
    do-learning              :Y

Destination Processing
    dest-type                :Mcast Group
    mcast-group              :200

BufferStore
    met-fifo-priority        :MCAST-L

MetFifo
    dest-map                 :0x000400c8
    forward-type             :MCAST
    mcast-group              :200
    met-entry-0              :HIT
    table-name               :DsMetEntry3W
    table-index              :200
    met-mode                 :bitmap
    bitmap-type              :normal-gport
    port-bitmap-base         :0
    dest-port-bitmap63~0     :0x3000000
    nexthop-ptr              :0x0000fffc

EPE Header Adjust
    channel-id               :24
    local-phy-port           :24
    nexthop-ptr              :0x0000fffc
    nexthop-table-name       :EpeNextHopInternal
    nexthop-table-index      :6
    mux-length-type          :0
    packet-length-adjust     :4
    packet-length-adjust-type:0
    operation-type           :NORMAL
    svlan-action             :Replace
    cvlan-action             :Add

Egress Parsing
    mac-da                   :0000.0100.0001
    mac-sa                   :0010.9400.0002
    svlan-id                 :200
    stag-cos                 :0
    cvlan-id                 :300
    ctag-cos                 :5
    ether-type               :0x00000800
    layer3-type              :L3TYPE_IPV4
    ip-protocol              :0x000000fd
    ip-da                    :192.0.0.1
    ip-sa                    :192.85.1.2
    layer4-type              :L4TYPE_NONE
    udf-hit-index            :1

NextHop Mapper
    dest-gport               :0x00000018
    vlan-ptr                 :200
    logic-port               :48
    payload-opration         :BRIDGE

SCL Process
    hash-0-lookup-result     :HIT-DEFAULT-ENTRY
    key-name                 :DsEgressXcOamDoubleVlanPortHashKey
    key-index                :1976
    hash-1-lookup-result     :HIT-DEFAULT-ENTRY
    key-name                 :DsEgressXcOamSvlanPortHashKey
    key-index                :480

Layer3 Packet Editing

Layer2 Packet Editing

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

