

### switchport port-security 

```
1217@bcmIfmMaccfgStructSet:ulPort=272670747 iCmd=33286
1253@bcmIfmMaccfgStructSet:ulPort=47 ulValue=0
2271@ctc_tms_mac_learn_set:ulSnmpPort=47,ulEnable=0
2323@ctc_tms_mac_learn_set:Set local port learn info
2331@ctc_tms_mac_learn_set:ulSnmpPort=47 iUnit=0 gPort=25
2356@ctc_tms_mac_learn_set:exit OK rc=0
60@bcmMacSecStructSet:
MacSecHw:60@bcmMacSecStructSet:ENTRY:cmd = 47616.

2994@ctc_security_mac_cml_set:Entry:ulSnmpPort=47,Cml=3.
2912@ctc_port_mac_limit_action_set:Entry:ulSnmpPort=47,action=0x2.
2927@ctc_port_mac_limit_action_set:Port:bcmGPort=25.
2976@ctc_port_mac_limit_action_set:Exit:rc=ROS_OK.
1367@bcm_tms_port_mac_del:entry, ulSnmpPort=47, ulFlags = 10
862@ssp_mac_port_del_period_limit:ENTRY:clear dynamic base port,snmpport=47.
954@ssp_mac_port_del_period_limit:exit:ROS_OK.
1454@bcm_tms_port_mac_del:exit OK
3057@ctc_security_mac_cml_set:Exit:rc=ROS_OK.
82@bcmMacSecStructSet:MacSecHw:82@bcmMacSecStructSet:EXIT:rc = 0.

60@bcmMacSecStructSet:
MacSecHw:60@bcmMacSecStructSet:ENTRY:cmd = 47616.

2994@ctc_security_mac_cml_set:Entry:ulSnmpPort=47,Cml=2.
2912@ctc_port_mac_limit_action_set:Entry:ulSnmpPort=47,action=0x3.
2927@ctc_port_mac_limit_action_set:Port:bcmGPort=25.
2976@ctc_port_mac_limit_action_set:Exit:rc=ROS_OK.
1367@bcm_tms_port_mac_del:entry, ulSnmpPort=47, ulFlags = 10
862@ssp_mac_port_del_period_limit:ENTRY:clear dynamic base port,snmpport=47.
954@ssp_mac_port_del_period_limit:exit:ROS_OK.
1454@bcm_tms_port_mac_del:exit OK
3057@ctc_security_mac_cml_set:Exit:rc=ROS_OK.
82@bcmMacSecStructSet:MacSecHw:82@bcmMacSecStructSet:EXIT:rc = 0.
```


### switchport port-security mac-address 0010.9400.0102 vlan 100

```
1217@bcmIfmMaccfgStructSet:ulPort=272670747 iCmd=33289
1273@bcmIfmMaccfgStructSet:entry ssp ROS_HW_SS_L2_ADDR_SET 
1276@bcmIfmMaccfgStructSet:case SSP_ifm_ifIndex2Port_extend_get 
120@bcmMaclibMacAdd:pArlDefs->flags=1, ulgChip=0, vlanId=100 port=47 tgid=0
1174@bcm_tms_mac_add:bcm_tms_mac_add: 0:10:94:0:1:2 ulgChip=0, ulPort=47 vlan=100,flag is 32
1287@bcm_tms_mac_add:STATIC add begin
1293@bcm_tms_mac_add:STATIC add
1306@bcm_tms_mac_add:case snmp port
1340@bcm_tms_mac_add:exit 0
```


### clear port-security configured

```
60@bcmMacSecStructSet:
MacSecHw:60@bcmMacSecStructSet:ENTRY:cmd = 47616.

2994@ctc_security_mac_cml_set:Entry:ulSnmpPort=47,Cml=2.
2912@ctc_port_mac_limit_action_set:Entry:ulSnmpPort=47,action=0x3.
2927@ctc_port_mac_limit_action_set:Port:bcmGPort=25.
2976@ctc_port_mac_limit_action_set:Exit:rc=ROS_OK.
1367@bcm_tms_port_mac_del:entry, ulSnmpPort=47, ulFlags = 10
862@ssp_mac_port_del_period_limit:ENTRY:clear dynamic base port,snmpport=47.
954@ssp_mac_port_del_period_limit:exit:ROS_OK.
1454@bcm_tms_port_mac_del:exit OK
3057@ctc_security_mac_cml_set:Exit:rc=ROS_OK.
82@bcmMacSecStructSet:MacSecHw:82@bcmMacSecStructSet:EXIT:rc = 0.

1391@bcmIfmMaccfgStructDel:bcmIfmMaccfgStructDel iCmd=33290
1393@bcmIfmMaccfgStructDel:flags=1 vlanId=100 port=0 tgid=0
1546@bcmIfmMaccfgStructDel:<0x0 0x10 0x94 0x0 0x1 0x2 > 100
1106@ctc_tms_mac_delete:entry, MAC:  010.94 0. 1 2, vlan = 100.
1137@ctc_tms_mac_delete:sdk, MAC:  010.94 0. 1 2, fid = 100.
1144@ctc_tms_mac_delete:exit 0
```


### clear port-security dynamic

```
60@bcmMacSecStructSet:
MacSecHw:60@bcmMacSecStructSet:ENTRY:cmd = 47616.

2994@ctc_security_mac_cml_set:Entry:ulSnmpPort=47,Cml=2.
2912@ctc_port_mac_limit_action_set:Entry:ulSnmpPort=47,action=0x3.
2927@ctc_port_mac_limit_action_set:Port:bcmGPort=25.
2976@ctc_port_mac_limit_action_set:Exit:rc=ROS_OK.
1367@bcm_tms_port_mac_del:entry, ulSnmpPort=47, ulFlags = 10
862@ssp_mac_port_del_period_limit:ENTRY:clear dynamic base port,snmpport=47.
954@ssp_mac_port_del_period_limit:exit:ROS_OK.
1454@bcm_tms_port_mac_del:exit OK
3057@ctc_security_mac_cml_set:Exit:rc=ROS_OK.
82@bcmMacSecStructSet:MacSecHw:82@bcmMacSecStructSet:EXIT:rc = 0.

1391@bcmIfmMaccfgStructDel:bcmIfmMaccfgStructDel iCmd=33290
1393@bcmIfmMaccfgStructDel:flags=1 vlanId=100 port=0 tgid=0
1546@bcmIfmMaccfgStructDel:<0x0 0x10 0x94 0x0 0x1 0x4 > 100
1106@ctc_tms_mac_delete:entry, MAC:  010.94 0. 1 4, vlan = 100.
1137@ctc_tms_mac_delete:sdk, MAC:  010.94 0. 1 4, fid = 100.
1144@ctc_tms_mac_delete:exit 0
```
























