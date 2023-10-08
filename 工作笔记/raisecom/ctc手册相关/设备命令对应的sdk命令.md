
不同QINQ的命令使用的查表方案不同，有的是hash，有的是tcam，而tcam还分mac和IP，所以用sdk命令配置QINQ规则时，可以先用设备命令配置，然后查看对应表项的详细信息（`show scl entry-info entry ENTRY_ID detail`），再用sdk命令配置。

以下示例都是在设备1/5/7口配置。

### switchport vlan-mapping both inner 1 add-outer 513 cos override

ingress方向：
```
scl add group 0x82305019 entry 0x3 port-svlan-hash-entry gport 0x19 direction ingress svlan 1 ingress-action vlan-edit stag-op 4 svid-sl 2 new-svid 513 scos-sl 0 ctag-op 2 cvid-sl 2 new-cvid 1 ccos-sl 1
```

egress方向：
```
scl add group 0x82309019 entry 0x4 port-double-vlan-hash-entry gport 0x19 direction egress svlan 513 cvlan 1 egress-action vlan-edit stag-op 1 svid-sl 2 new-svid 1 scos-sl 1 ctag-op 3
```

然后下发表项使生效
```
scl install entry 3
scl install entry 4
```

查看表项详细信息：
```
CTC_CLI(ctc-sdk)# show scl group-info all
No.    GROUP_ID         TYPE                          ENTRY_COUNT
----------------------------------------------------------------------------
0      0x80E15000       Internal(security)                 3
1      0x80E11000       Internal(security)                 3
2      0x80154000       Internal(vlan class)               0
3      0x80150000       Internal(vlan class)               0
4      0x8014C000       Internal(vlan class)               0
5      0x80148000       Internal(vlan class)               0
6      0x80700001       Internal(l3if)                     8
7      0xFFFF0001       Reserved(hash_port)                0
8      0xFFFF0002       Reserved(hash_port_cvlan)          0
9      0xFFFF0003       Reserved(hash_port_svlan)          0
10     0xFFFF0004       Reserved(hash_port_double_vlan)    0
11     0xFFFF0005       Reserved(hash_port_cvlan_cos)      0
12     0xFFFF0006       Reserved(hash_port_svlan_cos)      0
13     0xFFFF0007       Reserved(hash_mac)                 0
14     0xFFFF0008       Reserved(hash_port_mac)            0
15     0xFFFF0009       Reserved(hash_ipv4)                0
16     0xFFFF000A       Reserved(hash_port_ipv4)           0
17     0xFFFF000B       Reserved(hash_ipv6)                0
18     0xFFFF000C       Reserved(hash_l2)                  0
19     0xFFFF000D       Reserved(hash_port_ipv6)           0
20     0xFFFF000E       Reserved(tcam0)                    0
21     0xFFFF000F       Reserved(tcam1)                    0
22     0xFFFF0010       Reserved(tcam2)                    0
23     0xFFFF0011       Reserved(tcam3)                    2
24     0x82305019       Internal(vlan mapping)             1
25     0x82309019       Internal(vlan mapping)             2
26     0x82305031       Internal(vlan mapping)             1
Total Group Count:27
Total Entry Count:20
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# show scl entry-info entry 3 detail 
No.   ENTRY_ID     GROUP_ID     HW     PRI    KEY_INDEX                           KEY_TYPE
-------------------------------------------------------------------------------------------------
1     0x3          0x82305019   yes    1      0x1B50                              hash_port_svlan

#ENTRY_ID: 0x3 
Table:
  --DsUserId1SvlanPortHashKey        :0x1B50                              
  --DsUserId1                        :0x50                                
Primary Key(Value/Mask):
  Gport           : 0x19
  Svlan ID        : 1
SCL ingress action
-----------------------------------------------
  VLAN_EDIT       : 
                  Stag_op - Replace
                    svid_sl - New           New_svid - 513
                    scos_sl - As parser     New_scos - 0
                    scfi_sl - As parser     New_scfi - 0
                   tpid_idx - 255         
                  Ctag_op - Add
                    cvid_sl - New           New_cvid - 1
                    ccos_sl - Alternative   New_ccos - 0
                    ccfi_sl - As parser     New_ccfi - 0
                  Vlan domain - Svlan domain
                  Vlan action profile id - 65

CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# show scl entry-info entry 4 detail 
No.   ENTRY_ID     GROUP_ID     HW     PRI    KEY_INDEX                           KEY_TYPE
-------------------------------------------------------------------------------------------------
1     0x4          0x82309019   yes    1      0xFD8                               hash_port_2vlan

#ENTRY_ID: 0x4 
Table:
  --DsEgressXcOamDoubleVlanPortHashKey :0xFD8                               
  --DsEgressXcOamDoubleVlanPortHashKey :0xFD8                               
Primary Key(Value/Mask):
  DST Gport       : 0x19
  Svlan ID        : 513
  Cvlan ID        : 1
SCL Egress action
-----------------------------------------------
  VLAN_EDIT   : 
              Stag_op - Replace or Add
                svid_sl - New           New_svid - 1
                scos_sl - Alternative   New_scos - 0
                scfi_sl - As parser     New_scfi - 0
               tpid_idx - 255         
              Ctag_op - Delete

CTC_CLI(ctc-sdk)# 
```

设备命令行配置时，ingress和egress用的都是hash查表类型，使用vlan-mapping类型的group，也就是24、25行的group。所以，sdk命令配置时指定的group id注意不能选错了。







