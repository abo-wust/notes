
## port-channel 10

**ROS_HW_SS_LACP_MEMBER_CHANGE_NOTIFY**

接口1/5/8加入trunk 10：

```
63@bcmLacpStructSet:iCmd = 33553
1842@ssp_tms_trunk_port_change:Group=10,Port=48,bAddIn=1
2811@ctc_vlan_trunk_change_call:enter, trunkId=10, portId=48, isAdd=1
390@ctc_vlan_port_qtag_set:entry, ulSnmpPort=48, ulVlanId=0
326@_vlan_port_qtag_set:entry, ulSnmpPort=48, ulVlanId=0
186@ctc_tms_port_set_property:lport = 24,type=91 enable 1
244@ctc_tms_port_set_property:exit:0 !
361@_vlan_port_qtag_set:exit OK
203@ssp_vlan_qinq_event_callback:ulSnmpPort=48,arg1=0,arg2=0,eventType=0

[1161, ctc_tms_qinq_pvid_change_callback]  entry! snmpPort=48,pPort->dot1qEnable= 0

[1165, ctc_tms_qinq_pvid_change_callback]  no need to set
221@ssp_vlan_qinq_event_callback:Module 197 eventType 0hook executed.
464@ctc_vlan_port_qtag_set:exit OK
526@ctc_vlan_port_frame_reject_type_set:entry, ulSnmpPort=48, ulMode=1
600@ctc_vlan_port_frame_reject_type_set:exit OK
233@ctc_tms_scl_port_vlan_class_enable:gport=24 direction=0 hash_type=0 tcam_type=2 class_id=0 action_type=2

250@ctc_tms_scl_port_vlan_class_enable:gport=24 direction=0 hash_type=0 tcam_type=2 class_id=0 action_type=2

259@ctc_tms_scl_port_vlan_class_enable:EXIT OK!
233@ctc_tms_scl_port_vlan_class_enable:gport=24 direction=0 hash_type=0 tcam_type=2 class_id=0 action_type=2

250@ctc_tms_scl_port_vlan_class_enable:gport=24 direction=0 hash_type=0 tcam_type=2 class_id=0 action_type=2

259@ctc_tms_scl_port_vlan_class_enable:EXIT OK!
895@_vlan_pvlan_port_set:Entry:ulSnmpPort =48
917@_vlan_pvlan_port_set:exit
2954@ctc_vlan_trunk_change_call:exit OK
477@ctc_vlan_port_qtag_get:entry, ulSnmpPort = 110
493@ctc_vlan_port_qtag_get:*pVlanId = 0
494@ctc_vlan_port_qtag_get:exit OK

[280, _ctc_port_dot1q_enable]  entry! snmp_port=48,bDot1qEna=0,bCosOvrd=0,bMissEna=0,pvid=0

[320, _ctc_port_dot1q_enable]  exit succeed ,[rv: 0]

[1131, ctc_tms_qinq_trunk_member_callback]  Exit
141@bcmLacpStructSet:EXIT:rc = 0.

63@bcmLacpStructSet:iCmd = 33551
93@bcmLacpStructSet: ulGroup=10, ulMemberCount = 1.
109@bcmLacpStructSet: Ifindex = 0x1040a01f, ulSnmpPort = 48.
1506@ssp_tms_trunk_group_set:Tid=10 portlen=16.
496@_ssp_trunk_port_add:mem_idx=1:bcmPort=24
1208@ssp_trunk_active_port_change:trunkId=10, isAdd=1
1221@ssp_trunk_active_port_change:trunkId=10, portId=24,snmpport=48.
141@bcmLacpStructSet:EXIT:rc = 0.
```

## no port-channel

**ROS_HW_SS_LACP_MEMBER_CHANGE_NOTIFY**

trunk 10删掉1/5/8口：

```
63@bcmLacpStructSet:iCmd = 33553
1842@ssp_tms_trunk_port_change:Group=10,Port=48,bAddIn=0
2811@ctc_vlan_trunk_change_call:enter, trunkId=10, portId=48, isAdd=0
390@ctc_vlan_port_qtag_set:entry, ulSnmpPort=48, ulVlanId=0
326@_vlan_port_qtag_set:entry, ulSnmpPort=48, ulVlanId=0
186@ctc_tms_port_set_property:lport = 24,type=91 enable 1
244@ctc_tms_port_set_property:exit:0 !
361@_vlan_port_qtag_set:exit OK
203@ssp_vlan_qinq_event_callback:ulSnmpPort=48,arg1=0,arg2=0,eventType=0

[1161, ctc_tms_qinq_pvid_change_callback]  entry! snmpPort=48,pPort->dot1qEnable= 0

[1165, ctc_tms_qinq_pvid_change_callback]  no need to set
221@ssp_vlan_qinq_event_callback:Module 197 eventType 0hook executed.
464@ctc_vlan_port_qtag_set:exit OK
526@ctc_vlan_port_frame_reject_type_set:entry, ulSnmpPort=48, ulMode=1
600@ctc_vlan_port_frame_reject_type_set:exit OK
233@ctc_tms_scl_port_vlan_class_enable:gport=24 direction=0 hash_type=0 tcam_type=2 class_id=0 action_type=2

250@ctc_tms_scl_port_vlan_class_enable:gport=24 direction=0 hash_type=0 tcam_type=2 class_id=0 action_type=2

259@ctc_tms_scl_port_vlan_class_enable:EXIT OK!
233@ctc_tms_scl_port_vlan_class_enable:gport=24 direction=0 hash_type=0 tcam_type=2 class_id=0 action_type=2

250@ctc_tms_scl_port_vlan_class_enable:gport=24 direction=0 hash_type=0 tcam_type=2 class_id=0 action_type=2

259@ctc_tms_scl_port_vlan_class_enable:EXIT OK!
895@_vlan_pvlan_port_set:Entry:ulSnmpPort =48
917@_vlan_pvlan_port_set:exit
2954@ctc_vlan_trunk_change_call:exit OK
477@ctc_vlan_port_qtag_get:entry, ulSnmpPort = 48
493@ctc_vlan_port_qtag_get:*pVlanId = 0
494@ctc_vlan_port_qtag_get:exit OK

[280, _ctc_port_dot1q_enable]  entry! snmp_port=48,bDot1qEna=0,bCosOvrd=0,bMissEna=0,pvid=0

[320, _ctc_port_dot1q_enable]  exit succeed ,[rv: 0]

[1131, ctc_tms_qinq_trunk_member_callback]  Exit
141@bcmLacpStructSet:EXIT:rc = 0.

311@ctc_trunk_port_get:ulSnmpPort :48  *gport:24
63@bcmLacpStructSet:iCmd = 33551
93@bcmLacpStructSet: ulGroup=10, ulMemberCount = 0.
1506@ssp_tms_trunk_group_set:Tid=10 portlen=16.
1208@ssp_trunk_active_port_change:trunkId=10, isAdd=0
1221@ssp_trunk_active_port_change:trunkId=10, portId=24,snmpport=48.
141@bcmLacpStructSet:EXIT:rc = 0.
```

## link-aggregation load-sharing mode （all | smac | dmac | sip | dip)

**ROS_HW_SS_LACP_GLOBAL_LOAD_SHARING_MODE**

config视图下配置全局负载分担模式，默认all







