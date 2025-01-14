
### ethernet ring-protection 1 east gigaethernet 1/5/5 west gigaethernet 1/5/6 node-type rpl-owner rpl east

接口索引查询：
```
show rnet interface gigaethernet 1/5/5
gigaethernet1/5/5
    gigaethernet1/5/5 is UP RUNNING MULTICAST PROMISC 
    Dev type is AP
    MODE is SWITCH
	 MODE is L3
    MAC address is 4CB9.1131.C400
    MTU 1500 bytes
    TCP MSS 0 bytes
    Ifindex is 272670739(1040a013) ,refcount 624

88-E(debug)#
88-E(debug)#
88-E(debug)#show rnet interface gigaethernet 1/5/6
gigaethernet1/5/6
    gigaethernet1/5/6 is UP RUNNING MULTICAST PROMISC 
    Dev type is AP
    MODE is SWITCH
	 MODE is L3
    MAC address is 4CB9.1131.C400
    MTU 1500 bytes
    TCP MSS 0 bytes
    Ifindex is 272670743(1040a017) ,refcount 634
    
88-E(debug)#
88-E(debug)#
88-E(debug)#show rnet interface gigaethernet 1/5/7
gigaethernet1/5/7
    gigaethernet1/5/7 is UP RUNNING MULTICAST PROMISC 
    Dev type is AP
    MODE is SWITCH
	 MODE is L3
    MAC address is 4CB9.1131.C400
    MTU 1500 bytes
    TCP MSS 0 bytes
    Ifindex is 272670747(1040a01b) ,refcount 1

88-E(debug)#
88-E(debug)#
88-E(debug)#show rnet interface gigaethernet 1/5/8
gigaethernet1/5/8
    gigaethernet1/5/8 is DOWN RUNNING MULTICAST PROMISC 
    Dev type is AP
    MODE is SWITCH
	 MODE is L3
    MAC address is 4CB9.1131.C400
    MTU 1500 bytes
    TCP MSS 0 bytes
    Ifindex is 272670751(1040a01f) ,refcount 1

```


命令行配置打印：
```
bcmErpsSwitchStructSet@64:ROS_HW_ERPS_MCAST_GROUP ulRingId=1, ulVlanId=1, usPortCount=2
ssp_tms_erps_frame_to_cpu_set@960:pRoserpsStructure->pucPortList[0]=0x1040a013, snmp_port=45
ssp_tms_erps_frame_to_cpu_set@960:pRoserpsStructure->pucPortList[1]=0x1040a017, snmp_port=46
_ssp_erps_frame_forward_mcast_set@162:l2mc_addr.l2mc_grp_id = 5121, l2mc_addr.fid=1, pErpsMcGroup->ulRingId = 1.

_ssp_erps_frame_forward_mcast_set@164:l2mc_addr.mac=:0119.a700.0001

_ssp_erps_frame_forward_mcast_set@184:ulSnmpPort=45, ctc_port=27.

_ssp_erps_frame_forward_mcast_set@184:ulSnmpPort=46, ctc_port=26.

_ssp_erps_frame_copy2cpu_set@371:aclid=45.
bcmErpsSwitchStructSet@71:ROS_HW_ERPS_RING ulRingId=1, ulEastPort=272670739, ulWestPort=272670743
ssp_tms_erps_create@727:RingId = 1, EastPort = 0x1040a013 WestPort = 0x1040a017.
ssp_tms_erps_create@730:stBlockVlanList[0] = 0xff
ssp_tms_erps_create@730:stBlockVlanList[1] = 0xff
ssp_tms_erps_create@730:stBlockVlanList[2] = 0xff
ssp_tms_erps_create@730:stBlockVlanList[3] = 0xff
ssp_tms_erps_create@730:stBlockVlanList[4] = 0xff
ssp_tms_erps_create@730:stBlockVlanList[5] = 0xff
ssp_tms_erps_create@730:stBlockVlanList[6] = 0xff
ssp_tms_erps_create@730:stBlockVlanList[7] = 0xff
ssp_tms_erps_create@730:stBlockVlanList[8] = 0xff
ssp_tms_erps_create@730:stBlockVlanList[9] = 0xff
ssp_tms_erps_create@730:stBlockVlanList[10] = 0xff
ssp_tms_erps_create@730:stBlockVlanList[11] = 0xff
ssp_tms_erps_create@730:stBlockVlanList[12] = 0xff
ssp_tms_erps_create@730:stBlockVlanList[13] = 0xff
ssp_tms_erps_create@730:stBlockVlanList[14] = 0xff
ssp_tms_erps_create@730:stBlockVlanList[15] = 0xff
ssp_tms_erps_create@730:stBlockVlanList[16] = 0xff
ssp_tms_erps_create@730:stBlockVlanList[17] = 0xff
ssp_tms_erps_create@730:stBlockVlanList[18] = 0xff
ssp_tms_erps_create@730:stBlockVlanList[19] = 0xff
2237-ssp_logic_stg_create:entry. iUnit=0,eModId = 3
1179-_logic_stg_vlanlist_to_stg_set:entry:unit=0,iLogId=0,bDefaultStg=1,ulStgNeed=1
1188-_logic_stg_vlanlist_to_stg_set:entry:unit=0,usedstg=1,maxstg=129
232-bcm_tms_stg_create:Entry iUnit=0,stg= 0.

244-bcm_tms_stg_create:create stg = 2.

276-bcm_tms_stg_create:Exit rc = 0.
178-_stg_stg_hw_create:get hw stg=1, used hw stg num=2
1194-_logic_stg_vlanlist_to_stg_set:entry:unit=0,iStgId=1
736-_stg_port_state_copy_by_stg:entry.iUnit=0,StgSrcId=0,StgDstId=1.
775-_stg_port_state_copy_by_stg:rc = 0.
255-_stg_stg_vlan_hw_set:entry:iUnit=0,stg=1,vlan=1.
512-bcm_tms_vlan_stg_set:Entry iUnit=0,ulStgId = 1 ulVlanId = 1.
541-bcm_tms_vlan_stg_set:Exit rc = 0.
264-_stg_stg_vlan_hw_set:exit. rc  = 0.
255-_stg_stg_vlan_hw_set:entry:iUnit=0,stg=1,vlan=2.
512-bcm_tms_vlan_stg_set:Entry iUnit=0,ulStgId = 1 ulVlanId = 2.
541-bcm_tms_vlan_stg_set:Exit rc = 0.
264-_stg_stg_vlan_hw_set:exit. rc  = 0.

......

255-_stg_stg_vlan_hw_set:entry:iUnit=0,stg=1,vlan=4093.
512-bcm_tms_vlan_stg_set:Entry iUnit=0,ulStgId = 1 ulVlanId = 4093.
541-bcm_tms_vlan_stg_set:Exit rc = 0.
264-_stg_stg_vlan_hw_set:exit. rc  = 0.
255-_stg_stg_vlan_hw_set:entry:iUnit=0,stg=1,vlan=4094.
512-bcm_tms_vlan_stg_set:Entry iUnit=0,ulStgId = 1 ulVlanId = 4094.
541-bcm_tms_vlan_stg_set:Exit rc = 0.
264-_stg_stg_vlan_hw_set:exit. rc  = 0.
2247-ssp_logic_stg_create:exit. rc  = 0.
ssp_tms_erps_create@776:ssp_logi_stg_create:stgid=0.
ssp_tms_erps_create@782:ssp_stg_port_vlanlist_take:ulEastPort=0x1040a013.
1790-ssp_stg_port_vlanlist_take:entry:iUnit=0,ulIfindex=272670739,eModId=3,eOperType=1.
1848-ssp_stg_port_vlanlist_take:mode 3 take port 272670739 by opertype=1 , create stpModPort
871-_stg_port_state_copy_by_port_new:entry.iUnit=0,Ifindex=272670739,iLogStgId=0.
902-_stg_port_state_copy_by_port_new:get gport=0 hwstg=0 stgstate=0, return rc=0x0

945-_stg_port_state_copy_by_port_new:Ifindex=272670739,ulStgIndex = 0,eStateRos = 3.
902-_stg_port_state_copy_by_port_new:get gport=0 hwstg=1 stgstate=0, return rc=0x0

945-_stg_port_state_copy_by_port_new:Ifindex=272670739,ulStgIndex = 1,eStateRos = 3.
963-_stg_port_state_copy_by_port_new:exit.rc = 0.
1885-ssp_stg_port_vlanlist_take:exit. rc  = 0.
ssp_tms_erps_create@798:ssp_stg_port_vlanlist_take:ulWestPort=0x1040a017.
1790-ssp_stg_port_vlanlist_take:entry:iUnit=0,ulIfindex=272670743,eModId=3,eOperType=1.
1848-ssp_stg_port_vlanlist_take:mode 3 take port 272670743 by opertype=1 , create stpModPort
871-_stg_port_state_copy_by_port_new:entry.iUnit=0,Ifindex=272670743,iLogStgId=0.
902-_stg_port_state_copy_by_port_new:get gport=0 hwstg=0 stgstate=0, return rc=0x0

945-_stg_port_state_copy_by_port_new:Ifindex=272670743,ulStgIndex = 0,eStateRos = 3.
902-_stg_port_state_copy_by_port_new:get gport=0 hwstg=1 stgstate=0, return rc=0x0

945-_stg_port_state_copy_by_port_new:Ifindex=272670743,ulStgIndex = 1,eStateRos = 3.
963-_stg_port_state_copy_by_port_new:exit.rc = 0.
1885-ssp_stg_port_vlanlist_take:exit. rc  = 0.
ssp_tms_erps_create@820:exit:OK
bcmErpsSwitchStructSet@85:ROS_HW_ERPS_ETH_FD_TYPE ulRingId=1, EastFdType=128, WestFdType=128.
ssp_tms_erps_fdtype_set@1240:ulApsId=1
ssp_tms_erps_fdtype_set@1250:Erps 1 is found
ssp_tms_erps_fdtype_set@1263:modify fd type
ssp_tms_erps_fdtype_set@1267:east port:fdtypeList=0x80, mdLevel=0, mdName=, maName=, Lmep=0, Rmep=0.
ssp_tms_erps_fdtype_set@1281:west port:fdtypeList=0x80, mdLevel=0, mdName=, maName=, Lmep=0, Rmep=0.
ssp_tms_erps_fdtype_set@1292:exit ok
bcmErpsSwitchStructSet@78:ROS_HW_ERPS_PORT_STATE ulRingId=1, ulPort=272670739, bState=0
ssp_tms_erps_port_state_set@1149:Erps 1 is found
ssp_tms_erps_port_state_set@1163:ulEastPort = 0x1040a013, ulWestPort = 0x1040a017, stgid=0 time=21783225
_ssp_erps_port_state_set@389:IfIndex = 0x1040a013, bState=0, iStgId=0 start time=21783225
2301-ssp_logic_stg_port_state_set:entry:iUnit=0,STGId=0,Ifindex=272670739,eState=1,eModId=3. start time=21783225
390-_stg_port_state_set:entry:iUnit=0,STGId=1,Ifindex=272670739,eState=1,eType=1.
301-_stg_port_state_hw_set:entry:iUnit=0,STGId=1,Ifindex=272670739,eState=1.
338-bcm_tms_stg_stp_set:Entry ulStgId = 1 ulPort = 45 ulState = 1.
427-bcm_tms_stg_stp_set:Exit state = 1, rc = 0. 
315-_stg_port_state_hw_set:exit. rc  = 0.
ssp_cfm_port_stg_change_add@2196:ifIndex=0x1040a013.
500-_stg_port_state_set:exit. rc  = 0.
2610-ssp_stg_port_state_change_notify:ifIdx=0x1040a013, state(1:BLOCK;3:FORWARD)=1, etype(1:STG;2:STP)=1.
2341-ssp_logic_stg_port_state_set:exit. rc  = 0. end time=21783232
_ssp_erps_port_state_set@408:set over time=21783232
ssp_tms_erps_port_state_set@1200:set state over time=21783232
ssp_tms_erps_port_state_set@1212:set flush port 272670739 over time=21783232
ssp_tms_erps_port_state_set@1217:exit ok
_ssp_cfm_port_status_change@1185:entry:ifIndex=0x1040a013.
_ssp_cfm_port_status_change@1208:ifIndex=0x1040a013. ulIfMode = 1
_ssp_cfm_port_status_change@1253:snmpport = 45, unit = 0, bcmport = 27
```



### no ethernet ring-protection 1

```
bcmErpsSwitchStructDelete@134:ROS_HW_ERPS_MCAST_GROUP ulRingId=1, ulVlanId=1, usPortCount=2
_ssp_erps_frame_forward_mcast_del@261:l2mc_addr.l2mc_grp_id = 5121, l2mc_addr.fid=1, pErpsMcGroup->ulRingId = 1.

_ssp_erps_frame_forward_mcast_del@263:l2mc_addr.mac=:0119.a700.0001

bcmErpsSwitchStructSet@78:ROS_HW_ERPS_PORT_STATE ulRingId=1, ulPort=272670739, bState=1
ssp_tms_erps_port_state_set@1149:Erps 1 is found
ssp_tms_erps_port_state_set@1163:ulEastPort = 0x1040a013, ulWestPort = 0x1040a017, stgid=0 time=21794221
_ssp_erps_port_state_set@389:IfIndex = 0x1040a013, bState=1, iStgId=0 start time=21794221
2301-ssp_logic_stg_port_state_set:entry:iUnit=0,STGId=0,Ifindex=272670739,eState=3,eModId=3. start time=21794221
390-_stg_port_state_set:entry:iUnit=0,STGId=1,Ifindex=272670739,eState=3,eType=1.
301-_stg_port_state_hw_set:entry:iUnit=0,STGId=1,Ifindex=272670739,eState=3.
338-bcm_tms_stg_stp_set:Entry ulStgId = 1 ulPort = 45 ulState = 3.
427-bcm_tms_stg_stp_set:Exit state = 0, rc = 0. 
315-_stg_port_state_hw_set:exit. rc  = 0.
ssp_cfm_port_stg_change_add@2196:ifIndex=0x1040a013.
500-_stg_port_state_set:exit. rc  = 0.
2610-ssp_stg_port_state_change_notify:ifIdx=0x1040a013, state(1:BLOCK;3:FORWARD)=3, etype(1:STG;2:STP)=1.
2341-ssp_logic_stg_port_state_set:exit. rc  = 0. end time=21794222
_ssp_erps_port_state_set@408:set over time=21794222
ssp_tms_erps_port_state_set@1200:set state over time=21794222
ssp_tms_erps_port_state_set@1217:exit ok
bcmErpsSwitchStructDelete@141:ROS_HW_ERPS_RING ulRingId=1, ulEastPort=272670739, ulWestPort=272670743
ssp_tms_erps_delete@853:Erps 1 is found
ssp_tms_erps_delete@868:EastPort = 0x1040a013, WestPort = 0x1040a017, stgid=0, bEastTaken=1, bWestTaken=1
2023-ssp_stg_port_vlanlist_give:entry:iUnit=0,ulIfindex=272670739,eModId=3,eOperType=1.
2037-ssp_stg_port_vlanlist_give:Found And Delete.
2088-ssp_stg_port_vlanlist_give:exit. rc  = 0.
ssp_tms_erps_delete@875:ssp_stg_port_vlanlist_give:ulEastPort=0x1040a013,rc=0.
2023-ssp_stg_port_vlanlist_give:entry:iUnit=0,ulIfindex=272670743,eModId=3,eOperType=1.
2037-ssp_stg_port_vlanlist_give:Found And Delete.
2088-ssp_stg_port_vlanlist_give:exit. rc  = 0.
ssp_tms_erps_delete@882:ssp_stg_port_vlanlist_give:ulWestPort=0x1040a017,rc=0.
2266-ssp_logic_stg_del:entry:iUnit=0,STGId=0,eModId=3.
255-_stg_stg_vlan_hw_set:entry:iUnit=0,stg=0,vlan=1.
512-bcm_tms_vlan_stg_set:Entry iUnit=0,ulStgId = 0 ulVlanId = 1.
541-bcm_tms_vlan_stg_set:Exit rc = 0.
264-_stg_stg_vlan_hw_set:exit. rc  = 0.
255-_stg_stg_vlan_hw_set:entry:iUnit=0,stg=0,vlan=2.
512-bcm_tms_vlan_stg_set:Entry iUnit=0,ulStgId = 0 ulVlanId = 2.
541-bcm_tms_vlan_stg_set:Exit rc = 0.
264-_stg_stg_vlan_hw_set:exit. rc  = 0.

......

255-_stg_stg_vlan_hw_set:entry:iUnit=0,stg=0,vlan=4093.
512-bcm_tms_vlan_stg_set:Entry iUnit=0,ulStgId = 0 ulVlanId = 4093.
541-bcm_tms_vlan_stg_set:Exit rc = 0.
264-_stg_stg_vlan_hw_set:exit. rc  = 0.
255-_stg_stg_vlan_hw_set:entry:iUnit=0,stg=0,vlan=4094.
512-bcm_tms_vlan_stg_set:Entry iUnit=0,ulStgId = 0 ulVlanId = 4094.
541-bcm_tms_vlan_stg_set:Exit rc = 0.
264-_stg_stg_vlan_hw_set:exit. rc  = 0.
621-_stg_port_state_del_by_stg:entry.iUnit=0,StgId=1.
210-_stg_stg_hw_del:entry:iUnit=0,stg = 1.
293-bcm_tms_stg_destroy:Entry stg= 1.
306-bcm_tms_stg_destroy:Exit rc = 0.
236-_stg_stg_hw_del:exit. rc  = 0.
647-_stg_port_state_del_by_stg:exit.rc = 0.
657-_stg_port_state_del_by_logstgid:entry.iUnit=0,iLogStgId=0.
677-_stg_port_state_del_by_logstgid:exit.
2270-ssp_logic_stg_del:exit. rc  = 0.
ssp_tms_erps_delete@890:ssp_logic_stg_del:iStgId=0x0,rc=0.
ssp_tms_erps_delete@898:exit ok
```






```

CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# bcmErpsSwitchStructSet@64:ROS_HW_ERPS_MCAST_GROUP ulRingId=1, ulVlanId=1, usPortCount=2
ssp_tms_erps_frame_to_cpu_set@985:pRoserpsStructure->pucPortList[0]=0x1040a013, snmp_port=45
ssp_tms_erps_frame_to_cpu_set@985:pRoserpsStructure->pucPortList[1]=0x1040a01b, snmp_port=47
_ssp_erps_frame_forward_mcast_set@169:l2mc_addr.l2mc_grp_id = 5120, l2mc_addr.fid=1, pErpsMcGroup->ulRingId = 1.

_ssp_erps_frame_forward_mcast_set@171:l2mc_addr.mac=:0119.a700.0001

_ssp_erps_frame_forward_mcast_set@195:ulSnmpPort=45, ctc_port=27.

_ssp_erps_frame_forward_mcast_set@195:ulSnmpPort=47, ctc_port=25.

_ssp_erps_frame_copy2cpu_set@396:aclid=45.
bcmErpsSwitchStructSet@71:ROS_HW_ERPS_RING ulRingId=1, ulEastPort=272670739, ulWestPort=272670747
ssp_tms_erps_create@752:RingId = 1, EastPort = 0x1040a013 WestPort = 0x1040a01b.
ssp_tms_erps_create@755:stBlockVlanList[0] = 0xff
ssp_tms_erps_create@755:stBlockVlanList[1] = 0xc0
ssp_tms_erps_create@755:stBlockVlanList[2] = 0x0
ssp_tms_erps_create@755:stBlockVlanList[3] = 0x0
ssp_tms_erps_create@755:stBlockVlanList[4] = 0x0
ssp_tms_erps_create@755:stBlockVlanList[5] = 0x0
ssp_tms_erps_create@755:stBlockVlanList[6] = 0x0
ssp_tms_erps_create@755:stBlockVlanList[7] = 0x0
ssp_tms_erps_create@755:stBlockVlanList[8] = 0x0
ssp_tms_erps_create@755:stBlockVlanList[9] = 0x0
ssp_tms_erps_create@755:stBlockVlanList[10] = 0x0
ssp_tms_erps_create@755:stBlockVlanList[11] = 0x0
ssp_tms_erps_create@755:stBlockVlanList[12] = 0x0
ssp_tms_erps_create@755:stBlockVlanList[13] = 0x0
ssp_tms_erps_create@755:stBlockVlanList[14] = 0x0
ssp_tms_erps_create@755:stBlockVlanList[15] = 0x0
ssp_tms_erps_create@755:stBlockVlanList[16] = 0x0
ssp_tms_erps_create@755:stBlockVlanList[17] = 0x0
ssp_tms_erps_create@755:stBlockVlanList[18] = 0x0
ssp_tms_erps_create@755:stBlockVlanList[19] = 0x0
2254-ssp_logic_stg_create:entry. iUnit=0,eModId = 3
1060-_stg_need_set:stgin:
1063-_stg_need_set: 0x0
1063-_stg_need_set: 0x0
1063-_stg_need_set: 0x0
1063-_stg_need_set: 0x0
1063-_stg_need_set: 0x0
1063-_stg_need_set: 0x0
1063-_stg_need_set: 0x0
1063-_stg_need_set: 0x0
1063-_stg_need_set: 0x0
1063-_stg_need_set: 0x0
1063-_stg_need_set: 0x0
1063-_stg_need_set: 0x0
1063-_stg_need_set: 0x0
1063-_stg_need_set: 0x0
1063-_stg_need_set: 0x0
1063-_stg_need_set: 0x0
1066-_stg_need_set:stgout:
1069-_stg_need_set: 0x0
1069-_stg_need_set: 0x0
1069-_stg_need_set: 0x0
1069-_stg_need_set: 0x0
1069-_stg_need_set: 0x0
1069-_stg_need_set: 0x0
1069-_stg_need_set: 0x0
1069-_stg_need_set: 0x0
1069-_stg_need_set: 0x0
1069-_stg_need_set: 0x0
1069-_stg_need_set: 0x0
1069-_stg_need_set: 0x0
1069-_stg_need_set: 0x0
1069-_stg_need_set: 0x0
1069-_stg_need_set: 0x0
1069-_stg_need_set: 0x0
1195-_logic_stg_vlanlist_to_stg_set:entry:unit=0,iLogId=0,bDefaultStg=1,ulStgNeed=1
1204-_logic_stg_vlanlist_to_stg_set:entry:unit=0,usedstg=1,maxstg=129
232-bcm_tms_stg_create:Entry iUnit=0,stg= 0.

244-bcm_tms_stg_create:create stg = 2.

276-bcm_tms_stg_create:Exit rc = 0.
178-_stg_stg_hw_create:get hw stg=1, used hw stg num=2
1210-_logic_stg_vlanlist_to_stg_set:entry:unit=0,iStgId=1
736-_stg_port_state_copy_by_stg:entry.iUnit=0,StgSrcId=0,StgDstId=1.
301-_stg_port_state_hw_set:entry:iUnit=0,STGId=1,Ifindex=272670723,eState=1.
338-bcm_tms_stg_stp_set:Entry ulStgId = 1 ulPort = 41 ulState = 1.
427-bcm_tms_stg_stp_set:Exit state = 1, rc = 0. 
315-_stg_port_state_hw_set:exit. rc  = 0.
301-_stg_port_state_hw_set:entry:iUnit=0,STGId=1,Ifindex=272670735,eState=1.
338-bcm_tms_stg_stp_set:Entry ulStgId = 1 ulPort = 44 ulState = 1.
427-bcm_tms_stg_stp_set:Exit state = 1, rc = 0. 
315-_stg_port_state_hw_set:exit. rc  = 0.
775-_stg_port_state_copy_by_stg:rc = 0.
255-_stg_stg_vlan_hw_set:entry:iUnit=0,stg=1,vlan=1.
512-bcm_tms_vlan_stg_set:Entry iUnit=0,ulStgId = 1 ulVlanId = 1.
541-bcm_tms_vlan_stg_set:Exit rc = 0.
264-_stg_stg_vlan_hw_set:exit. rc  = 0.
255-_stg_stg_vlan_hw_set:entry:iUnit=0,stg=1,vlan=2.
512-bcm_tms_vlan_stg_set:Entry iUnit=0,ulStgId = 1 ulVlanId = 2.
541-bcm_tms_vlan_stg_set:Exit rc = 0.
264-_stg_stg_vlan_hw_set:exit. rc  = 0.
255-_stg_stg_vlan_hw_set:entry:iUnit=0,stg=1,vlan=3.
512-bcm_tms_vlan_stg_set:Entry iUnit=0,ulStgId = 1 ulVlanId = 3.
541-bcm_tms_vlan_stg_set:Exit rc = 0.
264-_stg_stg_vlan_hw_set:exit. rc  = 0.
255-_stg_stg_vlan_hw_set:entry:iUnit=0,stg=1,vlan=4.
512-bcm_tms_vlan_stg_set:Entry iUnit=0,ulStgId = 1 ulVlanId = 4.
541-bcm_tms_vlan_stg_set:Exit rc = 0.
264-_stg_stg_vlan_hw_set:exit. rc  = 0.
255-_stg_stg_vlan_hw_set:entry:iUnit=0,stg=1,vlan=5.
512-bcm_tms_vlan_stg_set:Entry iUnit=0,ulStgId = 1 ulVlanId = 5.
541-bcm_tms_vlan_stg_set:Exit rc = 0.
264-_stg_stg_vlan_hw_set:exit. rc  = 0.
255-_stg_stg_vlan_hw_set:entry:iUnit=0,stg=1,vlan=6.
512-bcm_tms_vlan_stg_set:Entry iUnit=0,ulStgId = 1 ulVlanId = 6.
541-bcm_tms_vlan_stg_set:Exit rc = 0.
264-_stg_stg_vlan_hw_set:exit. rc  = 0.
255-_stg_stg_vlan_hw_set:entry:iUnit=0,stg=1,vlan=7.
512-bcm_tms_vlan_stg_set:Entry iUnit=0,ulStgId = 1 ulVlanId = 7.
541-bcm_tms_vlan_stg_set:Exit rc = 0.
264-_stg_stg_vlan_hw_set:exit. rc  = 0.
255-_stg_stg_vlan_hw_set:entry:iUnit=0,stg=1,vlan=8.
512-bcm_tms_vlan_stg_set:Entry iUnit=0,ulStgId = 1 ulVlanId = 8.
541-bcm_tms_vlan_stg_set:Exit rc = 0.
264-_stg_stg_vlan_hw_set:exit. rc  = 0.
255-_stg_stg_vlan_hw_set:entry:iUnit=0,stg=1,vlan=9.
512-bcm_tms_vlan_stg_set:Entry iUnit=0,ulStgId = 1 ulVlanId = 9.
541-bcm_tms_vlan_stg_set:Exit rc = 0.
264-_stg_stg_vlan_hw_set:exit. rc  = 0.
255-_stg_stg_vlan_hw_set:entry:iUnit=0,stg=1,vlan=10.
512-bcm_tms_vlan_stg_set:Entry iUnit=0,ulStgId = 1 ulVlanId = 10.
541-bcm_tms_vlan_stg_set:Exit rc = 0.
264-_stg_stg_vlan_hw_set:exit. rc  = 0.
2264-ssp_logic_stg_create:exit. rc  = 0.
ssp_tms_erps_create@801:ssp_logi_stg_create:stgid=0.
ssp_tms_erps_create@807:ssp_stg_port_vlanlist_take:ulEastPort=0x1040a013.
1807-ssp_stg_port_vlanlist_take:entry:iUnit=0,ulIfindex=272670739,eModId=3,eOperType=1.
1865-ssp_stg_port_vlanlist_take:mode 3 take port 272670739 by opertype=1 , create stpModPort
871-_stg_port_state_copy_by_port_new:entry.iUnit=0,Ifindex=272670739,iLogStgId=0.
902-_stg_port_state_copy_by_port_new:get gport=0 hwstg=0 stgstate=0, return rc=0x0

945-_stg_port_state_copy_by_port_new:Ifindex=272670739,ulStgIndex = 0,eStateRos = 3.
902-_stg_port_state_copy_by_port_new:get gport=0 hwstg=1 stgstate=0, return rc=0x0

945-_stg_port_state_copy_by_port_new:Ifindex=272670739,ulStgIndex = 1,eStateRos = 3.
963-_stg_port_state_copy_by_port_new:exit.rc = 0.
1902-ssp_stg_port_vlanlist_take:exit. rc  = 0.
ssp_tms_erps_create@823:ssp_stg_port_vlanlist_take:ulWestPort=0x1040a01b.
1807-ssp_stg_port_vlanlist_take:entry:iUnit=0,ulIfindex=272670747,eModId=3,eOperType=1.
1865-ssp_stg_port_vlanlist_take:mode 3 take port 272670747 by opertype=1 , create stpModPort
871-_stg_port_state_copy_by_port_new:entry.iUnit=0,Ifindex=272670747,iLogStgId=0.
902-_stg_port_state_copy_by_port_new:get gport=0 hwstg=0 stgstate=0, return rc=0x0

945-_stg_port_state_copy_by_port_new:Ifindex=272670747,ulStgIndex = 0,eStateRos = 3.
902-_stg_port_state_copy_by_port_new:get gport=0 hwstg=1 stgstate=0, return rc=0x0

945-_stg_port_state_copy_by_port_new:Ifindex=272670747,ulStgIndex = 1,eStateRos = 3.
963-_stg_port_state_copy_by_port_new:exit.rc = 0.
1902-ssp_stg_port_vlanlist_take:exit. rc  = 0.
ssp_tms_erps_create@845:exit:OK
bcmErpsSwitchStructSet@85:ROS_HW_ERPS_ETH_FD_TYPE ulRingId=1, EastFdType=128, WestFdType=128.
ssp_tms_erps_fdtype_set@1265:ulApsId=1
ssp_tms_erps_fdtype_set@1275:Erps 1 is found
ssp_tms_erps_fdtype_set@1288:modify fd type
ssp_tms_erps_fdtype_set@1292:east port:fdtypeList=0x80, mdLevel=0, mdName=, maName=, Lmep=0, Rmep=0.
ssp_tms_erps_fdtype_set@1306:west port:fdtypeList=0x80, mdLevel=0, mdName=, maName=, Lmep=0, Rmep=0.
ssp_tms_erps_fdtype_set@1317:exit ok
bcmErpsSwitchStructSet@78:ROS_HW_ERPS_PORT_STATE ulRingId=1, ulPort=272670739, bState=0
ssp_tms_erps_port_state_set@1174:Erps 1 is found
ssp_tms_erps_port_state_set@1188:ulEastPort = 0x1040a013, ulWestPort = 0x1040a01b, stgid=0 time=1492805
_ssp_erps_port_state_set@414:IfIndex = 0x1040a013, bState=0, iStgId=0 start time=1492805
2318-ssp_logic_stg_port_state_set:entry:iUnit=0,STGId=0,Ifindex=272670739,eState=1,eModId=3. start time=1492805
390-_stg_port_state_set:entry:iUnit=0,STGId=1,Ifindex=272670739,eState=1,eType=1.
301-_stg_port_state_hw_set:entry:iUnit=0,STGId=1,Ifindex=272670739,eState=1.
338-bcm_tms_stg_stp_set:Entry ulStgId = 1 ulPort = 45 ulState = 1.
427-bcm_tms_stg_stp_set:Exit state = 1, rc = 0. 
315-_stg_port_state_hw_set:exit. rc  = 0.
500-_stg_port_state_set:exit. rc  = 0.
2627-ssp_stg_port_state_change_notify:ifIdx=0x1040a013, state(1:BLOCK;3:FORWARD)=1, etype(1:STG;2:STP)=1.
2358-ssp_logic_stg_port_state_set:exit. rc  = 0. end time=1492807
_ssp_erps_port_state_set@433:set over time=1492807
ssp_tms_erps_port_state_set@1225:set state over time=1492807
ssp_tms_erps_port_state_set@1237:set flush port 272670739 over time=1492807
ssp_tms_erps_port_state_set@1242:exit ok
bcmErpsSwitchStructSet@78:ROS_HW_ERPS_PORT_STATE ulRingId=1, ulPort=272670747, bState=0
ssp_tms_erps_port_state_set@1174:Erps 1 is found
ssp_tms_erps_port_state_set@1188:ulEastPort = 0x1040a013, ulWestPort = 0x1040a01b, stgid=0 time=1492808
_ssp_erps_port_state_set@414:IfIndex = 0x1040a01b, bState=0, iStgId=0 start time=1492808
2318-ssp_logic_stg_port_state_set:entry:iUnit=0,STGId=0,Ifindex=272670747,eState=1,eModId=3. start time=1492808
390-_stg_port_state_set:entry:iUnit=0,STGId=1,Ifindex=272670747,eState=1,eType=1.
301-_stg_port_state_hw_set:entry:iUnit=0,STGId=1,Ifindex=272670747,eState=1.
338-bcm_tms_stg_stp_set:Entry ulStgId = 1 ulPort = 47 ulState = 1.
427-bcm_tms_stg_stp_set:Exit state = 1, rc = 0. 
315-_stg_port_state_hw_set:exit. rc  = 0.
500-_stg_port_state_set:exit. rc  = 0.
2627-ssp_stg_port_state_change_notify:ifIdx=0x1040a01b, state(1:BLOCK;3:FORWARD)=1, etype(1:STG;2:STP)=1.
2358-ssp_logic_stg_port_state_set:exit. rc  = 0. end time=1492812
_ssp_erps_port_state_set@433:set over time=1492812
ssp_tms_erps_port_state_set@1225:set state over time=1492813
ssp_tms_erps_port_state_set@1237:set flush port 272670747 over time=1492813
ssp_tms_erps_port_state_set@1242:exit ok

CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# bcmErpsSwitchStructSet@64:ROS_HW_ERPS_MCAST_GROUP ulRingId=2, ulVlanId=1, usPortCount=2
ssp_tms_erps_frame_to_cpu_set@985:pRoserpsStructure->pucPortList[0]=0x1040a017, snmp_port=46
ssp_tms_erps_frame_to_cpu_set@985:pRoserpsStructure->pucPortList[1]=0x1040a01f, snmp_port=48
_ssp_erps_frame_forward_mcast_set@169:l2mc_addr.l2mc_grp_id = 5121, l2mc_addr.fid=1, pErpsMcGroup->ulRingId = 2.

_ssp_erps_frame_forward_mcast_set@171:l2mc_addr.mac=:0119.a700.0002

_ssp_erps_frame_forward_mcast_set@195:ulSnmpPort=46, ctc_port=26.

_ssp_erps_frame_forward_mcast_set@195:ulSnmpPort=48, ctc_port=24.

_ssp_erps_frame_copy2cpu_set@396:aclid=48.
bcmErpsSwitchStructSet@71:ROS_HW_ERPS_RING ulRingId=2, ulEastPort=272670743, ulWestPort=272670751
ssp_tms_erps_create@752:RingId = 2, EastPort = 0x1040a017 WestPort = 0x1040a01f.
ssp_tms_erps_create@755:stBlockVlanList[0] = 0xf
ssp_tms_erps_create@755:stBlockVlanList[1] = 0xfe
ssp_tms_erps_create@755:stBlockVlanList[2] = 0x0
ssp_tms_erps_create@755:stBlockVlanList[3] = 0x0
ssp_tms_erps_create@755:stBlockVlanList[4] = 0x0
ssp_tms_erps_create@755:stBlockVlanList[5] = 0x0
ssp_tms_erps_create@755:stBlockVlanList[6] = 0x0
ssp_tms_erps_create@755:stBlockVlanList[7] = 0x0
ssp_tms_erps_create@755:stBlockVlanList[8] = 0x0
ssp_tms_erps_create@755:stBlockVlanList[9] = 0x0
ssp_tms_erps_create@755:stBlockVlanList[10] = 0x0
ssp_tms_erps_create@755:stBlockVlanList[11] = 0x0
ssp_tms_erps_create@755:stBlockVlanList[12] = 0x0
ssp_tms_erps_create@755:stBlockVlanList[13] = 0x0
ssp_tms_erps_create@755:stBlockVlanList[14] = 0x0
ssp_tms_erps_create@755:stBlockVlanList[15] = 0x0
ssp_tms_erps_create@755:stBlockVlanList[16] = 0x0
ssp_tms_erps_create@755:stBlockVlanList[17] = 0x0
ssp_tms_erps_create@755:stBlockVlanList[18] = 0x0
ssp_tms_erps_create@755:stBlockVlanList[19] = 0x0
2254-ssp_logic_stg_create:entry. iUnit=0,eModId = 3
1060-_stg_need_set:stgin:
1063-_stg_need_set: 0x40
1063-_stg_need_set: 0x0
1063-_stg_need_set: 0x0
1063-_stg_need_set: 0x0
1063-_stg_need_set: 0x0
1063-_stg_need_set: 0x0
1063-_stg_need_set: 0x0
1063-_stg_need_set: 0x0
1063-_stg_need_set: 0x0
1063-_stg_need_set: 0x0
1063-_stg_need_set: 0x0
1063-_stg_need_set: 0x0
1063-_stg_need_set: 0x0
1063-_stg_need_set: 0x0
1063-_stg_need_set: 0x0
1063-_stg_need_set: 0x0
1066-_stg_need_set:stgout:
1069-_stg_need_set: 0x40
1069-_stg_need_set: 0x0
1069-_stg_need_set: 0x0
1069-_stg_need_set: 0x0
1069-_stg_need_set: 0x0
1069-_stg_need_set: 0x0
1069-_stg_need_set: 0x0
1069-_stg_need_set: 0x0
1069-_stg_need_set: 0x0
1069-_stg_need_set: 0x0
1069-_stg_need_set: 0x0
1069-_stg_need_set: 0x0
1069-_stg_need_set: 0x0
1069-_stg_need_set: 0x0
1069-_stg_need_set: 0x0
1069-_stg_need_set: 0x0
1081-_stg_need_set:ulStgId=1
1195-_logic_stg_vlanlist_to_stg_set:entry:unit=0,iLogId=1,bDefaultStg=1,ulStgNeed=2
1204-_logic_stg_vlanlist_to_stg_set:entry:unit=0,usedstg=2,maxstg=129
232-bcm_tms_stg_create:Entry iUnit=0,stg= 0.

244-bcm_tms_stg_create:create stg = 3.

276-bcm_tms_stg_create:Exit rc = 0.
178-_stg_stg_hw_create:get hw stg=2, used hw stg num=3
1210-_logic_stg_vlanlist_to_stg_set:entry:unit=0,iStgId=2
232-bcm_tms_stg_create:Entry iUnit=0,stg= 0.

244-bcm_tms_stg_create:create stg = 4.

276-bcm_tms_stg_create:Exit rc = 0.
178-_stg_stg_hw_create:get hw stg=3, used hw stg num=4
1210-_logic_stg_vlanlist_to_stg_set:entry:unit=0,iStgId=3
736-_stg_port_state_copy_by_stg:entry.iUnit=0,StgSrcId=1,StgDstId=2.
301-_stg_port_state_hw_set:entry:iUnit=0,STGId=2,Ifindex=272670723,eState=1.
338-bcm_tms_stg_stp_set:Entry ulStgId = 2 ulPort = 41 ulState = 1.
427-bcm_tms_stg_stp_set:Exit state = 1, rc = 0. 
315-_stg_port_state_hw_set:exit. rc  = 0.
301-_stg_port_state_hw_set:entry:iUnit=0,STGId=2,Ifindex=272670735,eState=1.
338-bcm_tms_stg_stp_set:Entry ulStgId = 2 ulPort = 44 ulState = 1.
427-bcm_tms_stg_stp_set:Exit state = 1, rc = 0. 
315-_stg_port_state_hw_set:exit. rc  = 0.
301-_stg_port_state_hw_set:entry:iUnit=0,STGId=2,Ifindex=272670739,eState=1.
338-bcm_tms_stg_stp_set:Entry ulStgId = 2 ulPort = 45 ulState = 1.
427-bcm_tms_stg_stp_set:Exit state = 1, rc = 0. 
315-_stg_port_state_hw_set:exit. rc  = 0.
301-_stg_port_state_hw_set:entry:iUnit=0,STGId=2,Ifindex=272670747,eState=1.
338-bcm_tms_stg_stp_set:Entry ulStgId = 2 ulPort = 47 ulState = 1.
427-bcm_tms_stg_stp_set:Exit state = 1, rc = 0. 
315-_stg_port_state_hw_set:exit. rc  = 0.
775-_stg_port_state_copy_by_stg:rc = 0.
736-_stg_port_state_copy_by_stg:entry.iUnit=0,StgSrcId=0,StgDstId=3.
301-_stg_port_state_hw_set:entry:iUnit=0,STGId=3,Ifindex=272670723,eState=1.
338-bcm_tms_stg_stp_set:Entry ulStgId = 3 ulPort = 41 ulState = 1.
427-bcm_tms_stg_stp_set:Exit state = 1, rc = 0. 
315-_stg_port_state_hw_set:exit. rc  = 0.
301-_stg_port_state_hw_set:entry:iUnit=0,STGId=3,Ifindex=272670735,eState=1.
338-bcm_tms_stg_stp_set:Entry ulStgId = 3 ulPort = 44 ulState = 1.
427-bcm_tms_stg_stp_set:Exit state = 1, rc = 0. 
315-_stg_port_state_hw_set:exit. rc  = 0.
775-_stg_port_state_copy_by_stg:rc = 0.
255-_stg_stg_vlan_hw_set:entry:iUnit=0,stg=2,vlan=5.
512-bcm_tms_vlan_stg_set:Entry iUnit=0,ulStgId = 2 ulVlanId = 5.
541-bcm_tms_vlan_stg_set:Exit rc = 0.
264-_stg_stg_vlan_hw_set:exit. rc  = 0.
255-_stg_stg_vlan_hw_set:entry:iUnit=0,stg=2,vlan=6.
512-bcm_tms_vlan_stg_set:Entry iUnit=0,ulStgId = 2 ulVlanId = 6.
541-bcm_tms_vlan_stg_set:Exit rc = 0.
264-_stg_stg_vlan_hw_set:exit. rc  = 0.
255-_stg_stg_vlan_hw_set:entry:iUnit=0,stg=2,vlan=7.
512-bcm_tms_vlan_stg_set:Entry iUnit=0,ulStgId = 2 ulVlanId = 7.
541-bcm_tms_vlan_stg_set:Exit rc = 0.
264-_stg_stg_vlan_hw_set:exit. rc  = 0.
255-_stg_stg_vlan_hw_set:entry:iUnit=0,stg=2,vlan=8.
512-bcm_tms_vlan_stg_set:Entry iUnit=0,ulStgId = 2 ulVlanId = 8.
541-bcm_tms_vlan_stg_set:Exit rc = 0.
264-_stg_stg_vlan_hw_set:exit. rc  = 0.
255-_stg_stg_vlan_hw_set:entry:iUnit=0,stg=2,vlan=9.
512-bcm_tms_vlan_stg_set:Entry iUnit=0,ulStgId = 2 ulVlanId = 9.
541-bcm_tms_vlan_stg_set:Exit rc = 0.
264-_stg_stg_vlan_hw_set:exit. rc  = 0.
255-_stg_stg_vlan_hw_set:entry:iUnit=0,stg=2,vlan=10.
512-bcm_tms_vlan_stg_set:Entry iUnit=0,ulStgId = 2 ulVlanId = 10.
541-bcm_tms_vlan_stg_set:Exit rc = 0.
264-_stg_stg_vlan_hw_set:exit. rc  = 0.
255-_stg_stg_vlan_hw_set:entry:iUnit=0,stg=3,vlan=11.
512-bcm_tms_vlan_stg_set:Entry iUnit=0,ulStgId = 3 ulVlanId = 11.
541-bcm_tms_vlan_stg_set:Exit rc = 0.
264-_stg_stg_vlan_hw_set:exit. rc  = 0.
255-_stg_stg_vlan_hw_set:entry:iUnit=0,stg=3,vlan=12.
512-bcm_tms_vlan_stg_set:Entry iUnit=0,ulStgId = 3 ulVlanId = 12.
541-bcm_tms_vlan_stg_set:Exit rc = 0.
264-_stg_stg_vlan_hw_set:exit. rc  = 0.
255-_stg_stg_vlan_hw_set:entry:iUnit=0,stg=3,vlan=13.
512-bcm_tms_vlan_stg_set:Entry iUnit=0,ulStgId = 3 ulVlanId = 13.
541-bcm_tms_vlan_stg_set:Exit rc = 0.
264-_stg_stg_vlan_hw_set:exit. rc  = 0.
255-_stg_stg_vlan_hw_set:entry:iUnit=0,stg=3,vlan=14.
512-bcm_tms_vlan_stg_set:Entry iUnit=0,ulStgId = 3 ulVlanId = 14.
541-bcm_tms_vlan_stg_set:Exit rc = 0.
264-_stg_stg_vlan_hw_set:exit. rc  = 0.
255-_stg_stg_vlan_hw_set:entry:iUnit=0,stg=3,vlan=15.
512-bcm_tms_vlan_stg_set:Entry iUnit=0,ulStgId = 3 ulVlanId = 15.
541-bcm_tms_vlan_stg_set:Exit rc = 0.
264-_stg_stg_vlan_hw_set:exit. rc  = 0.
2264-ssp_logic_stg_create:exit. rc  = 0.
ssp_tms_erps_create@801:ssp_logi_stg_create:stgid=1.
ssp_tms_erps_create@807:ssp_stg_port_vlanlist_take:ulEastPort=0x1040a017.
1807-ssp_stg_port_vlanlist_take:entry:iUnit=0,ulIfindex=272670743,eModId=3,eOperType=1.
1865-ssp_stg_port_vlanlist_take:mode 3 take port 272670743 by opertype=1 , create stpModPort
871-_stg_port_state_copy_by_port_new:entry.iUnit=0,Ifindex=272670743,iLogStgId=1.
902-_stg_port_state_copy_by_port_new:get gport=0 hwstg=0 stgstate=0, return rc=0x0

945-_stg_port_state_copy_by_port_new:Ifindex=272670743,ulStgIndex = 0,eStateRos = 3.
902-_stg_port_state_copy_by_port_new:get gport=0 hwstg=1 stgstate=0, return rc=0x0

945-_stg_port_state_copy_by_port_new:Ifindex=272670743,ulStgIndex = 1,eStateRos = 3.
902-_stg_port_state_copy_by_port_new:get gport=0 hwstg=2 stgstate=0, return rc=0x0

945-_stg_port_state_copy_by_port_new:Ifindex=272670743,ulStgIndex = 2,eStateRos = 3.
902-_stg_port_state_copy_by_port_new:get gport=0 hwstg=3 stgstate=0, return rc=0x0

945-_stg_port_state_copy_by_port_new:Ifindex=272670743,ulStgIndex = 3,eStateRos = 3.
963-_stg_port_state_copy_by_port_new:exit.rc = 0.
1902-ssp_stg_port_vlanlist_take:exit. rc  = 0.
ssp_tms_erps_create@823:ssp_stg_port_vlanlist_take:ulWestPort=0x1040a01f.
1807-ssp_stg_port_vlanlist_take:entry:iUnit=0,ulIfindex=272670751,eModId=3,eOperType=1.
1865-ssp_stg_port_vlanlist_take:mode 3 take port 272670751 by opertype=1 , create stpModPort
871-_stg_port_state_copy_by_port_new:entry.iUnit=0,Ifindex=272670751,iLogStgId=1.
902-_stg_port_state_copy_by_port_new:get gport=0 hwstg=0 stgstate=0, return rc=0x0

945-_stg_port_state_copy_by_port_new:Ifindex=272670751,ulStgIndex = 0,eStateRos = 3.
902-_stg_port_state_copy_by_port_new:get gport=0 hwstg=1 stgstate=0, return rc=0x0

945-_stg_port_state_copy_by_port_new:Ifindex=272670751,ulStgIndex = 1,eStateRos = 3.
902-_stg_port_state_copy_by_port_new:get gport=0 hwstg=2 stgstate=0, return rc=0x0

945-_stg_port_state_copy_by_port_new:Ifindex=272670751,ulStgIndex = 2,eStateRos = 3.
902-_stg_port_state_copy_by_port_new:get gport=0 hwstg=3 stgstate=0, return rc=0x0

945-_stg_port_state_copy_by_port_new:Ifindex=272670751,ulStgIndex = 3,eStateRos = 3.
963-_stg_port_state_copy_by_port_new:exit.rc = 0.
1902-ssp_stg_port_vlanlist_take:exit. rc  = 0.
ssp_tms_erps_create@845:exit:OK
bcmErpsSwitchStructSet@85:ROS_HW_ERPS_ETH_FD_TYPE ulRingId=2, EastFdType=128, WestFdType=128.
ssp_tms_erps_fdtype_set@1265:ulApsId=2
ssp_tms_erps_fdtype_set@1275:Erps 2 is found
ssp_tms_erps_fdtype_set@1288:modify fd type
ssp_tms_erps_fdtype_set@1292:east port:fdtypeList=0x80, mdLevel=0, mdName=, maName=, Lmep=0, Rmep=0.
ssp_tms_erps_fdtype_set@1306:west port:fdtypeList=0x80, mdLevel=0, mdName=, maName=, Lmep=0, Rmep=0.
ssp_tms_erps_fdtype_set@1317:exit ok
bcmErpsSwitchStructSet@78:ROS_HW_ERPS_PORT_STATE ulRingId=2, ulPort=272670743, bState=0
ssp_tms_erps_port_state_set@1174:Erps 2 is found
ssp_tms_erps_port_state_set@1188:ulEastPort = 0x1040a017, ulWestPort = 0x1040a01f, stgid=1 time=1499731
_ssp_erps_port_state_set@414:IfIndex = 0x1040a017, bState=0, iStgId=1 start time=1499731
2318-ssp_logic_stg_port_state_set:entry:iUnit=0,STGId=1,Ifindex=272670743,eState=1,eModId=3. start time=1499731
390-_stg_port_state_set:entry:iUnit=0,STGId=2,Ifindex=272670743,eState=1,eType=1.
301-_stg_port_state_hw_set:entry:iUnit=0,STGId=2,Ifindex=272670743,eState=1.
338-bcm_tms_stg_stp_set:Entry ulStgId = 2 ulPort = 46 ulState = 1.
427-bcm_tms_stg_stp_set:Exit state = 1, rc = 0. 
315-_stg_port_state_hw_set:exit. rc  = 0.
500-_stg_port_state_set:exit. rc  = 0.
390-_stg_port_state_set:entry:iUnit=0,STGId=3,Ifindex=272670743,eState=1,eType=1.
301-_stg_port_state_hw_set:entry:iUnit=0,STGId=3,Ifindex=272670743,eState=1.
338-bcm_tms_stg_stp_set:Entry ulStgId = 3 ulPort = 46 ulState = 1.
427-bcm_tms_stg_stp_set:Exit state = 1, rc = 0. 
315-_stg_port_state_hw_set:exit. rc  = 0.
500-_stg_port_state_set:exit. rc  = 0.
2627-ssp_stg_port_state_change_notify:ifIdx=0x1040a017, state(1:BLOCK;3:FORWARD)=1, etype(1:STG;2:STP)=1.
2358-ssp_logic_stg_port_state_set:exit. rc  = 0. end time=1499733
_ssp_erps_port_state_set@433:set over time=1499733
ssp_tms_erps_port_state_set@1225:set state over time=1499733
ssp_tms_erps_port_state_set@1237:set flush port 272670743 over time=1499733
ssp_tms_erps_port_state_set@1242:exit ok
bcmErpsSwitchStructSet@78:ROS_HW_ERPS_PORT_STATE ulRingId=2, ulPort=272670751, bState=0
ssp_tms_erps_port_state_set@1174:Erps 2 is found
ssp_tms_erps_port_state_set@1188:ulEastPort = 0x1040a017, ulWestPort = 0x1040a01f, stgid=1 time=1499733
_ssp_erps_port_state_set@414:IfIndex = 0x1040a01f, bState=0, iStgId=1 start time=1499734
2318-ssp_logic_stg_port_state_set:entry:iUnit=0,STGId=1,Ifindex=272670751,eState=1,eModId=3. start time=1499734
390-_stg_port_state_set:entry:iUnit=0,STGId=2,Ifindex=272670751,eState=1,eType=1.
301-_stg_port_state_hw_set:entry:iUnit=0,STGId=2,Ifindex=272670751,eState=1.
338-bcm_tms_stg_stp_set:Entry ulStgId = 2 ulPort = 48 ulState = 1.
427-bcm_tms_stg_stp_set:Exit state = 1, rc = 0. 
315-_stg_port_state_hw_set:exit. rc  = 0.
500-_stg_port_state_set:exit. rc  = 0.
390-_stg_port_state_set:entry:iUnit=0,STGId=3,Ifindex=272670751,eState=1,eType=1.
301-_stg_port_state_hw_set:entry:iUnit=0,STGId=3,Ifindex=272670751,eState=1.
338-bcm_tms_stg_stp_set:Entry ulStgId = 3 ulPort = 48 ulState = 1.
427-bcm_tms_stg_stp_set:Exit state = 1, rc = 0. 
315-_stg_port_state_hw_set:exit. rc  = 0.
500-_stg_port_state_set:exit. rc  = 0.
2627-ssp_stg_port_state_change_notify:ifIdx=0x1040a01f, state(1:BLOCK;3:FORWARD)=1, etype(1:STG;2:STP)=1.
2358-ssp_logic_stg_port_state_set:exit. rc  = 0. end time=1499735
_ssp_erps_port_state_set@433:set over time=1499735
ssp_tms_erps_port_state_set@1225:set state over time=1499735
ssp_tms_erps_port_state_set@1237:set flush port 272670751 over time=1499735
ssp_tms_erps_port_state_set@1242:exit ok

CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# bcmErpsSwitchStructDelete@134:ROS_HW_ERPS_MCAST_GROUP ulRingId=2, ulVlanId=1, usPortCount=2
_ssp_erps_frame_forward_mcast_del@281:l2mc_addr.l2mc_grp_id = 5121, l2mc_addr.fid=1, pErpsMcGroup->ulRingId = 2.

_ssp_erps_frame_forward_mcast_del@283:l2mc_addr.mac=:0119.a700.0002

bcmErpsSwitchStructSet@78:ROS_HW_ERPS_PORT_STATE ulRingId=2, ulPort=272670743, bState=1
ssp_tms_erps_port_state_set@1174:Erps 2 is found
ssp_tms_erps_port_state_set@1188:ulEastPort = 0x1040a017, ulWestPort = 0x1040a01f, stgid=1 time=1512000
_ssp_erps_port_state_set@414:IfIndex = 0x1040a017, bState=1, iStgId=1 start time=1512000
2318-ssp_logic_stg_port_state_set:entry:iUnit=0,STGId=1,Ifindex=272670743,eState=3,eModId=3. start time=1512000
390-_stg_port_state_set:entry:iUnit=0,STGId=2,Ifindex=272670743,eState=3,eType=1.
301-_stg_port_state_hw_set:entry:iUnit=0,STGId=2,Ifindex=272670743,eState=3.
338-bcm_tms_stg_stp_set:Entry ulStgId = 2 ulPort = 46 ulState = 3.
427-bcm_tms_stg_stp_set:Exit state = 0, rc = 0. 
315-_stg_port_state_hw_set:exit. rc  = 0.
500-_stg_port_state_set:exit. rc  = 0.
390-_stg_port_state_set:entry:iUnit=0,STGId=3,Ifindex=272670743,eState=3,eType=1.
301-_stg_port_state_hw_set:entry:iUnit=0,STGId=3,Ifindex=272670743,eState=3.
338-bcm_tms_stg_stp_set:Entry ulStgId = 3 ulPort = 46 ulState = 3.
427-bcm_tms_stg_stp_set:Exit state = 0, rc = 0. 
315-_stg_port_state_hw_set:exit. rc  = 0.
500-_stg_port_state_set:exit. rc  = 0.
2627-ssp_stg_port_state_change_notify:ifIdx=0x1040a017, state(1:BLOCK;3:FORWARD)=3, etype(1:STG;2:STP)=1.
2358-ssp_logic_stg_port_state_set:exit. rc  = 0. end time=1512003
_ssp_erps_port_state_set@433:set over time=1512004
ssp_tms_erps_port_state_set@1225:set state over time=1512004
ssp_tms_erps_port_state_set@1242:exit ok
bcmErpsSwitchStructSet@78:ROS_HW_ERPS_PORT_STATE ulRingId=2, ulPort=272670751, bState=1
ssp_tms_erps_port_state_set@1174:Erps 2 is found
ssp_tms_erps_port_state_set@1188:ulEastPort = 0x1040a017, ulWestPort = 0x1040a01f, stgid=1 time=1512005
_ssp_erps_port_state_set@414:IfIndex = 0x1040a01f, bState=1, iStgId=1 start time=1512005
2318-ssp_logic_stg_port_state_set:entry:iUnit=0,STGId=1,Ifindex=272670751,eState=3,eModId=3. start time=1512005
390-_stg_port_state_set:entry:iUnit=0,STGId=2,Ifindex=272670751,eState=3,eType=1.
301-_stg_port_state_hw_set:entry:iUnit=0,STGId=2,Ifindex=272670751,eState=3.
338-bcm_tms_stg_stp_set:Entry ulStgId = 2 ulPort = 48 ulState = 3.
427-bcm_tms_stg_stp_set:Exit state = 0, rc = 0. 
315-_stg_port_state_hw_set:exit. rc  = 0.
500-_stg_port_state_set:exit. rc  = 0.
390-_stg_port_state_set:entry:iUnit=0,STGId=3,Ifindex=272670751,eState=3,eType=1.
301-_stg_port_state_hw_set:entry:iUnit=0,STGId=3,Ifindex=272670751,eState=3.
338-bcm_tms_stg_stp_set:Entry ulStgId = 3 ulPort = 48 ulState = 3.
427-bcm_tms_stg_stp_set:Exit state = 0, rc = 0. 
315-_stg_port_state_hw_set:exit. rc  = 0.
500-_stg_port_state_set:exit. rc  = 0.
2627-ssp_stg_port_state_change_notify:ifIdx=0x1040a01f, state(1:BLOCK;3:FORWARD)=3, etype(1:STG;2:STP)=1.
2358-ssp_logic_stg_port_state_set:exit. rc  = 0. end time=1512006
_ssp_erps_port_state_set@433:set over time=1512006
ssp_tms_erps_port_state_set@1225:set state over time=1512007
ssp_tms_erps_port_state_set@1242:exit ok
bcmErpsSwitchStructDelete@141:ROS_HW_ERPS_RING ulRingId=2, ulEastPort=272670743, ulWestPort=272670751
ssp_tms_erps_delete@878:Erps 2 is found
ssp_tms_erps_delete@893:EastPort = 0x1040a017, WestPort = 0x1040a01f, stgid=1, bEastTaken=1, bWestTaken=1
2040-ssp_stg_port_vlanlist_give:entry:iUnit=0,ulIfindex=272670743,eModId=3,eOperType=1.
2054-ssp_stg_port_vlanlist_give:Found And Delete.
2105-ssp_stg_port_vlanlist_give:exit. rc  = 0.
ssp_tms_erps_delete@900:ssp_stg_port_vlanlist_give:ulEastPort=0x1040a017,rc=0.
2040-ssp_stg_port_vlanlist_give:entry:iUnit=0,ulIfindex=272670751,eModId=3,eOperType=1.
2054-ssp_stg_port_vlanlist_give:Found And Delete.
2105-ssp_stg_port_vlanlist_give:exit. rc  = 0.
ssp_tms_erps_delete@907:ssp_stg_port_vlanlist_give:ulWestPort=0x1040a01f,rc=0.
2283-ssp_logic_stg_del:entry:iUnit=0,STGId=1,eModId=3.
255-_stg_stg_vlan_hw_set:entry:iUnit=0,stg=1,vlan=5.
512-bcm_tms_vlan_stg_set:Entry iUnit=0,ulStgId = 1 ulVlanId = 5.
541-bcm_tms_vlan_stg_set:Exit rc = 0.
264-_stg_stg_vlan_hw_set:exit. rc  = 0.
255-_stg_stg_vlan_hw_set:entry:iUnit=0,stg=1,vlan=6.
512-bcm_tms_vlan_stg_set:Entry iUnit=0,ulStgId = 1 ulVlanId = 6.
541-bcm_tms_vlan_stg_set:Exit rc = 0.
264-_stg_stg_vlan_hw_set:exit. rc  = 0.
255-_stg_stg_vlan_hw_set:entry:iUnit=0,stg=1,vlan=7.
512-bcm_tms_vlan_stg_set:Entry iUnit=0,ulStgId = 1 ulVlanId = 7.
541-bcm_tms_vlan_stg_set:Exit rc = 0.
264-_stg_stg_vlan_hw_set:exit. rc  = 0.
255-_stg_stg_vlan_hw_set:entry:iUnit=0,stg=1,vlan=8.
512-bcm_tms_vlan_stg_set:Entry iUnit=0,ulStgId = 1 ulVlanId = 8.
541-bcm_tms_vlan_stg_set:Exit rc = 0.
264-_stg_stg_vlan_hw_set:exit. rc  = 0.
255-_stg_stg_vlan_hw_set:entry:iUnit=0,stg=1,vlan=9.
512-bcm_tms_vlan_stg_set:Entry iUnit=0,ulStgId = 1 ulVlanId = 9.
541-bcm_tms_vlan_stg_set:Exit rc = 0.
264-_stg_stg_vlan_hw_set:exit. rc  = 0.
255-_stg_stg_vlan_hw_set:entry:iUnit=0,stg=1,vlan=10.
512-bcm_tms_vlan_stg_set:Entry iUnit=0,ulStgId = 1 ulVlanId = 10.
541-bcm_tms_vlan_stg_set:Exit rc = 0.
264-_stg_stg_vlan_hw_set:exit. rc  = 0.
255-_stg_stg_vlan_hw_set:entry:iUnit=0,stg=0,vlan=11.
512-bcm_tms_vlan_stg_set:Entry iUnit=0,ulStgId = 0 ulVlanId = 11.
541-bcm_tms_vlan_stg_set:Exit rc = 0.
264-_stg_stg_vlan_hw_set:exit. rc  = 0.
255-_stg_stg_vlan_hw_set:entry:iUnit=0,stg=0,vlan=12.
512-bcm_tms_vlan_stg_set:Entry iUnit=0,ulStgId = 0 ulVlanId = 12.
541-bcm_tms_vlan_stg_set:Exit rc = 0.
264-_stg_stg_vlan_hw_set:exit. rc  = 0.
255-_stg_stg_vlan_hw_set:entry:iUnit=0,stg=0,vlan=13.
512-bcm_tms_vlan_stg_set:Entry iUnit=0,ulStgId = 0 ulVlanId = 13.
541-bcm_tms_vlan_stg_set:Exit rc = 0.
264-_stg_stg_vlan_hw_set:exit. rc  = 0.
255-_stg_stg_vlan_hw_set:entry:iUnit=0,stg=0,vlan=14.
512-bcm_tms_vlan_stg_set:Entry iUnit=0,ulStgId = 0 ulVlanId = 14.
541-bcm_tms_vlan_stg_set:Exit rc = 0.
264-_stg_stg_vlan_hw_set:exit. rc  = 0.
255-_stg_stg_vlan_hw_set:entry:iUnit=0,stg=0,vlan=15.
512-bcm_tms_vlan_stg_set:Entry iUnit=0,ulStgId = 0 ulVlanId = 15.
541-bcm_tms_vlan_stg_set:Exit rc = 0.
264-_stg_stg_vlan_hw_set:exit. rc  = 0.
621-_stg_port_state_del_by_stg:entry.iUnit=0,StgId=2.
210-_stg_stg_hw_del:entry:iUnit=0,stg = 2.
293-bcm_tms_stg_destroy:Entry stg= 2.
306-bcm_tms_stg_destroy:Exit rc = 0.
236-_stg_stg_hw_del:exit. rc  = 0.
647-_stg_port_state_del_by_stg:exit.rc = 0.
621-_stg_port_state_del_by_stg:entry.iUnit=0,StgId=3.
210-_stg_stg_hw_del:entry:iUnit=0,stg = 3.
293-bcm_tms_stg_destroy:Entry stg= 3.
306-bcm_tms_stg_destroy:Exit rc = 0.
236-_stg_stg_hw_del:exit. rc  = 0.
647-_stg_port_state_del_by_stg:exit.rc = 0.
657-_stg_port_state_del_by_logstgid:entry.iUnit=0,iLogStgId=1.
677-_stg_port_state_del_by_logstgid:exit.
2287-ssp_logic_stg_del:exit. rc  = 0.
ssp_tms_erps_delete@915:ssp_logic_stg_del:iStgId=0x1,rc=0.
ssp_tms_erps_delete@923:exit ok

CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# bcmErpsSwitchStructDelete@134:ROS_HW_ERPS_MCAST_GROUP ulRingId=1, ulVlanId=1, usPortCount=2
_ssp_erps_frame_forward_mcast_del@281:l2mc_addr.l2mc_grp_id = 5120, l2mc_addr.fid=1, pErpsMcGroup->ulRingId = 1.

_ssp_erps_frame_forward_mcast_del@283:l2mc_addr.mac=:0119.a700.0001

bcmErpsSwitchStructSet@78:ROS_HW_ERPS_PORT_STATE ulRingId=1, ulPort=272670739, bState=1
ssp_tms_erps_port_state_set@1174:Erps 1 is found
ssp_tms_erps_port_state_set@1188:ulEastPort = 0x1040a013, ulWestPort = 0x1040a01b, stgid=0 time=1517100
_ssp_erps_port_state_set@414:IfIndex = 0x1040a013, bState=1, iStgId=0 start time=1517100
2318-ssp_logic_stg_port_state_set:entry:iUnit=0,STGId=0,Ifindex=272670739,eState=3,eModId=3. start time=1517101
390-_stg_port_state_set:entry:iUnit=0,STGId=1,Ifindex=272670739,eState=3,eType=1.
301-_stg_port_state_hw_set:entry:iUnit=0,STGId=1,Ifindex=272670739,eState=3.
338-bcm_tms_stg_stp_set:Entry ulStgId = 1 ulPort = 45 ulState = 3.
427-bcm_tms_stg_stp_set:Exit state = 0, rc = 0. 
315-_stg_port_state_hw_set:exit. rc  = 0.
500-_stg_port_state_set:exit. rc  = 0.
2627-ssp_stg_port_state_change_notify:ifIdx=0x1040a013, state(1:BLOCK;3:FORWARD)=3, etype(1:STG;2:STP)=1.
2358-ssp_logic_stg_port_state_set:exit. rc  = 0. end time=1517101
_ssp_erps_port_state_set@433:set over time=1517101
ssp_tms_erps_port_state_set@1225:set state over time=1517101
ssp_tms_erps_port_state_set@1242:exit ok
bcmErpsSwitchStructSet@78:ROS_HW_ERPS_PORT_STATE ulRingId=1, ulPort=272670747, bState=1
ssp_tms_erps_port_state_set@1174:Erps 1 is found
ssp_tms_erps_port_state_set@1188:ulEastPort = 0x1040a013, ulWestPort = 0x1040a01b, stgid=0 time=1517104
_ssp_erps_port_state_set@414:IfIndex = 0x1040a01b, bState=1, iStgId=0 start time=1517104
2318-ssp_logic_stg_port_state_set:entry:iUnit=0,STGId=0,Ifindex=272670747,eState=3,eModId=3. start time=1517104
390-_stg_port_state_set:entry:iUnit=0,STGId=1,Ifindex=272670747,eState=3,eType=1.
301-_stg_port_state_hw_set:entry:iUnit=0,STGId=1,Ifindex=272670747,eState=3.
338-bcm_tms_stg_stp_set:Entry ulStgId = 1 ulPort = 47 ulState = 3.
427-bcm_tms_stg_stp_set:Exit state = 0, rc = 0. 
315-_stg_port_state_hw_set:exit. rc  = 0.
500-_stg_port_state_set:exit. rc  = 0.
2627-ssp_stg_port_state_change_notify:ifIdx=0x1040a01b, state(1:BLOCK;3:FORWARD)=3, etype(1:STG;2:STP)=1.
2358-ssp_logic_stg_port_state_set:exit. rc  = 0. end time=1517106
_ssp_erps_port_state_set@433:set over time=1517106
ssp_tms_erps_port_state_set@1225:set state over time=1517106
ssp_tms_erps_port_state_set@1242:exit ok
bcmErpsSwitchStructDelete@141:ROS_HW_ERPS_RING ulRingId=1, ulEastPort=272670739, ulWestPort=272670747
ssp_tms_erps_delete@878:Erps 1 is found
ssp_tms_erps_delete@893:EastPort = 0x1040a013, WestPort = 0x1040a01b, stgid=0, bEastTaken=1, bWestTaken=1
2040-ssp_stg_port_vlanlist_give:entry:iUnit=0,ulIfindex=272670739,eModId=3,eOperType=1.
2054-ssp_stg_port_vlanlist_give:Found And Delete.
2105-ssp_stg_port_vlanlist_give:exit. rc  = 0.
ssp_tms_erps_delete@900:ssp_stg_port_vlanlist_give:ulEastPort=0x1040a013,rc=0.
2040-ssp_stg_port_vlanlist_give:entry:iUnit=0,ulIfindex=272670747,eModId=3,eOperType=1.
2054-ssp_stg_port_vlanlist_give:Found And Delete.
2105-ssp_stg_port_vlanlist_give:exit. rc  = 0.
ssp_tms_erps_delete@907:ssp_stg_port_vlanlist_give:ulWestPort=0x1040a01b,rc=0.
2283-ssp_logic_stg_del:entry:iUnit=0,STGId=0,eModId=3.
255-_stg_stg_vlan_hw_set:entry:iUnit=0,stg=0,vlan=1.
512-bcm_tms_vlan_stg_set:Entry iUnit=0,ulStgId = 0 ulVlanId = 1.
541-bcm_tms_vlan_stg_set:Exit rc = 0.
264-_stg_stg_vlan_hw_set:exit. rc  = 0.
255-_stg_stg_vlan_hw_set:entry:iUnit=0,stg=0,vlan=2.
512-bcm_tms_vlan_stg_set:Entry iUnit=0,ulStgId = 0 ulVlanId = 2.
541-bcm_tms_vlan_stg_set:Exit rc = 0.
264-_stg_stg_vlan_hw_set:exit. rc  = 0.
255-_stg_stg_vlan_hw_set:entry:iUnit=0,stg=0,vlan=3.
512-bcm_tms_vlan_stg_set:Entry iUnit=0,ulStgId = 0 ulVlanId = 3.
541-bcm_tms_vlan_stg_set:Exit rc = 0.
264-_stg_stg_vlan_hw_set:exit. rc  = 0.
255-_stg_stg_vlan_hw_set:entry:iUnit=0,stg=0,vlan=4.
512-bcm_tms_vlan_stg_set:Entry iUnit=0,ulStgId = 0 ulVlanId = 4.
541-bcm_tms_vlan_stg_set:Exit rc = 0.
264-_stg_stg_vlan_hw_set:exit. rc  = 0.
255-_stg_stg_vlan_hw_set:entry:iUnit=0,stg=0,vlan=5.
512-bcm_tms_vlan_stg_set:Entry iUnit=0,ulStgId = 0 ulVlanId = 5.
541-bcm_tms_vlan_stg_set:Exit rc = 0.
264-_stg_stg_vlan_hw_set:exit. rc  = 0.
255-_stg_stg_vlan_hw_set:entry:iUnit=0,stg=0,vlan=6.
512-bcm_tms_vlan_stg_set:Entry iUnit=0,ulStgId = 0 ulVlanId = 6.
541-bcm_tms_vlan_stg_set:Exit rc = 0.
264-_stg_stg_vlan_hw_set:exit. rc  = 0.
255-_stg_stg_vlan_hw_set:entry:iUnit=0,stg=0,vlan=7.
512-bcm_tms_vlan_stg_set:Entry iUnit=0,ulStgId = 0 ulVlanId = 7.
541-bcm_tms_vlan_stg_set:Exit rc = 0.
264-_stg_stg_vlan_hw_set:exit. rc  = 0.
255-_stg_stg_vlan_hw_set:entry:iUnit=0,stg=0,vlan=8.
512-bcm_tms_vlan_stg_set:Entry iUnit=0,ulStgId = 0 ulVlanId = 8.
541-bcm_tms_vlan_stg_set:Exit rc = 0.
264-_stg_stg_vlan_hw_set:exit. rc  = 0.
255-_stg_stg_vlan_hw_set:entry:iUnit=0,stg=0,vlan=9.
512-bcm_tms_vlan_stg_set:Entry iUnit=0,ulStgId = 0 ulVlanId = 9.
541-bcm_tms_vlan_stg_set:Exit rc = 0.
264-_stg_stg_vlan_hw_set:exit. rc  = 0.
255-_stg_stg_vlan_hw_set:entry:iUnit=0,stg=0,vlan=10.
512-bcm_tms_vlan_stg_set:Entry iUnit=0,ulStgId = 0 ulVlanId = 10.
541-bcm_tms_vlan_stg_set:Exit rc = 0.
264-_stg_stg_vlan_hw_set:exit. rc  = 0.
621-_stg_port_state_del_by_stg:entry.iUnit=0,StgId=1.
210-_stg_stg_hw_del:entry:iUnit=0,stg = 1.
293-bcm_tms_stg_destroy:Entry stg= 1.
306-bcm_tms_stg_destroy:Exit rc = 0.
236-_stg_stg_hw_del:exit. rc  = 0.
647-_stg_port_state_del_by_stg:exit.rc = 0.
657-_stg_port_state_del_by_logstgid:entry.iUnit=0,iLogStgId=0.
677-_stg_port_state_del_by_logstgid:exit.
2287-ssp_logic_stg_del:exit. rc  = 0.
ssp_tms_erps_delete@915:ssp_logic_stg_del:iStgId=0x0,rc=0.
ssp_tms_erps_delete@923:exit ok

CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# exit
CTC_CLI# sdk
CTC_CLI(ctc-sdk)# show st
stacking           stats              stp                
CTC_CLI(ctc-sdk)# show stp state port 0x1b 
<cr>               stpid              
CTC_CLI(ctc-sdk)# show stp state port 0x1b stpid 2
 Gport    StpId   State
 ---------------------------------
 0x001b   2       Blocking
CTC_CLI(ctc-sdk)# show stp state port 0x1b stpid 1
 Gport    StpId   State
 ---------------------------------
 0x001b   1       Forwarding
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# exit
CTC_CLI# exit
CTC_CLI# 
<ritp>BcmShellClient_IPCTask[main.c-232] DEBUG:   send reply ok!
<ritp>BcmShellClient_IPCTask[main.c-237] DEBUG:   recv request, exit bcmshell client!
ret = 0


bcmshell client exit!
/usr/local/ros/bin # 
/usr/local/ros/bin # 
/usr/local/ros/bin # exit
88-E(debug)#
88-E(debug)# 

```

