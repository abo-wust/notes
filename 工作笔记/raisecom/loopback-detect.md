
### loopback-detection

```
ctc_tms_lbd_create-1019:
LbdCreateInfo:port=45,detectmode=0,VlanList=
ctcLbdSwitchStructSet-99:Enter ROS_HW_SS_LBD_PKT_TO_CPU, IfIndex = 272670739, SnmpPort=45
aclm_ProfileFind-1898:input aclm profile id, 0
aclm_ProfileFind-2002:Mapping to: profile,CTC_ACL_KEY_MAC ,CTC_INGRESS
aclm_ProfileFind-2080:find available profile(profile_acl_mac_ingress)
aclm_IdAlloc-209: alloc type 10 id 44
aclm_ProfileInfoGet-1854:entry. profileType: 0 
FDI_aclm_RuleCreate-5637:service api profile id:0, ctc profile id:0, ctc key type:0

aclm_RuleCreate-5309:ENTER !!
aclm_RuleCreate-5339:iunit:0,gid=1,eid=44,key_type=0

aclm_RuleQualifySet-3437:Enter
aclm_RuleQualifySet-3472:port: 8e293908 mask 8e2938b0
aclm_RuleQualifySet-3678:ctc_acl_add_key_field : iUnit=0,ulEid=44,macAddr=0x ff ff ff ff ff ff,macMask=0x ff ff ff ff ff ff
aclm_RuleQualifySet-3710:ctc_acl_add_key_field : iUnit=0,ulEid=44,uiData=0x898,uiMask=0xffff
aclm_RuleQualifySet-5015:exit
aclm_RuleActionSet-2476:action: ctcs_acl_add_action_field iUnit=0 ulEid=44
aclm_RuleActionSet-2573:action: bcmFieldActionCopyToCpu iUnit=0 ulEid=44
aclm_RuleActionSet-3397:exit
aclm_RuleCreate-5398:EXIT !!
FDI_aclm_RuleCreate-5649: good !! rv(0)
aclm_RuleInfoCreate-2235:enter info set.rule:45
aclm_RuleInfoCreate-2261:leave info create 
FDI_aclm_RuleCreate-5739:create acl success rule id =44
ctc_tms_lbd_to_cpu_set-516:ctc_tms_route_Lbd_to_cpu function return correct .
ctc_tms_lbd_to_cpu_set-519:ctc_tms_lbd_to_cpu_set ¡£ulPort = 45
2476@bcm_tms_port_stp_set:ulSnmpPort=45 ulState=2 
2497@bcm_tms_port_stp_set:ulSnmpPort=45 ulIfindex=272670739
```


### no loopback-detection

```
ctcLbdSwitchStructDel-161:Enter ROS_HW_SS_LBD_PKT_TO_CPU_DEL
aclm_RuleInfoGetByRuleId-259: find rule info ulRuleId=45
aclm_RuleInfoGetByRuleId-259: find rule info ulRuleId=45
FDI_aclm_RuleStatDelete-283:can't find ruleId or stat not exit

aclm_RuleInfoGetByRuleId-259: find rule info ulRuleId=45
aclm_RulePolicerNodeGet-440:ERR:can't find ruleId or stat not exit

aclm_RulePolicerDelete-503: not find the record ulEid=45
aclm_RuleDelete-528:NORMAL:iEid = 44
aclm_IdFree-225: free type 10 id 44
aclm_ProfileInfoGet-1854:entry. profileType: 0 
FDI_aclm_RuleDelete-5799: 1 1

ctc_tms_Ap_Lbd_to_cpu-385:AP LBD TO CPU DELETE
ctc_tms_lbd_to_cpu_del-555:ctc_tms_route_Lbd_to_cpu function return correct .
2476@bcm_tms_port_stp_set:ulSnmpPort=45 ulState=1 
2497@bcm_tms_port_stp_set:ulSnmpPort=45 ulIfindex=272670739 
ctc_tms_lbd_delete-1187:LbddelInfo:port=45,detectmode=0

ctc_tms_lbd_delete-1214:plbdInfo lbdmode!
```



### loopback-detection detect-vlanlist 100,200

```
ctc_tms_lbd_create-1019:
LbdCreateInfo:port=45,detectmode=1,VlanList=100,200
ctc_tms_lbd_create-1086:ctc_tms_lbd_create:vlan=100

ctc_tms_lbd_create-1122:ctc_tms_lbd_create:vlan=100,logStgId=2

ctc_tms_lbd_create-1086:ctc_tms_lbd_create:vlan=200

ctc_tms_lbd_create-1122:ctc_tms_lbd_create:vlan=200,logStgId=3

ctcLbdSwitchStructSet-99:Enter ROS_HW_SS_LBD_PKT_TO_CPU, IfIndex = 272670739, SnmpPort=45
aclm_ProfileFind-1898:input aclm profile id, 0
aclm_ProfileFind-2002:Mapping to: profile,CTC_ACL_KEY_MAC ,CTC_INGRESS
aclm_ProfileFind-2080:find available profile(profile_acl_mac_ingress)
aclm_IdAlloc-209: alloc type 10 id 44
aclm_ProfileInfoGet-1854:entry. profileType: 0 
FDI_aclm_RuleCreate-5637:service api profile id:0, ctc profile id:0, ctc key type:0

aclm_RuleCreate-5309:ENTER !!
aclm_RuleCreate-5339:iunit:0,gid=1,eid=44,key_type=0

aclm_RuleQualifySet-3437:Enter
aclm_RuleQualifySet-3472:port: 8e293908 mask 8e2938b0
aclm_RuleQualifySet-3678:ctc_acl_add_key_field : iUnit=0,ulEid=44,macAddr=0x ff ff ff ff ff ff,macMask=0x ff ff ff ff ff ff
aclm_RuleQualifySet-3710:ctc_acl_add_key_field : iUnit=0,ulEid=44,uiData=0x898,uiMask=0xffff
aclm_RuleQualifySet-5015:exit
aclm_RuleActionSet-2476:action: ctcs_acl_add_action_field iUnit=0 ulEid=44
aclm_RuleActionSet-2573:action: bcmFieldActionCopyToCpu iUnit=0 ulEid=44
aclm_RuleActionSet-3397:exit
aclm_RuleCreate-5398:EXIT !!
FDI_aclm_RuleCreate-5649: good !! rv(0)
aclm_RuleInfoCreate-2235:enter info set.rule:45
aclm_RuleInfoCreate-2261:leave info create 
FDI_aclm_RuleCreate-5739:create acl success rule id =44
ctc_tms_lbd_to_cpu_set-516:ctc_tms_route_Lbd_to_cpu function return correct .
ctc_tms_lbd_to_cpu_set-519:ctc_tms_lbd_to_cpu_set ¡£ulPort = 45
ctc_tms_lbd_port_state_set-1302:LbdInfo:port=45,vlan=200,portstate=0

ctc_tms_lbd_port_state_set-1335:LbdInfo:port=45,stgId=3,vlan=200 time=1034085224

ctc_tms_lbd_port_state_set-1357:set port(45) stg state over time=1034085225

ctc_tms_lbd_to_drop_set-765:Entry:ulSnmpPort = 45, ulvlan = 200.

ctc_tms_Ap_Lbd_to_drop-589:Entry : ulSnmpPort = 45, ulVlanId = 200

aclm_ProfileFind-1898:input aclm profile id, 0
aclm_ProfileFind-2002:Mapping to: profile,CTC_ACL_KEY_MAC ,CTC_INGRESS
aclm_ProfileFind-2080:find available profile(profile_acl_mac_ingress)
aclm_IdAlloc-209: alloc type 10 id 49
aclm_ProfileInfoGet-1854:entry. profileType: 0 
FDI_aclm_RuleCreate-5637:service api profile id:0, ctc profile id:0, ctc key type:0

aclm_RuleCreate-5309:ENTER !!
aclm_RuleCreate-5339:iunit:0,gid=1,eid=49,key_type=0

aclm_RuleQualifySet-3437:Enter
aclm_RuleQualifySet-3472:port: 8e293908 mask 8e2938b0
aclm_RuleQualifySet-3678:ctc_acl_add_key_field : iUnit=0,ulEid=49,macAddr=0x ff ff ff ff ff ff,macMask=0x ff ff ff ff ff ff
aclm_RuleQualifySet-3710:ctc_acl_add_key_field : iUnit=0,ulEid=49,uiData=0x898,uiMask=0xffff
aclm_RuleQualifySet-4028:ctc_acl_add_key_field : iUnit=0,ulEid=49,uiData=200,uiMask=0xfff
aclm_RuleQualifySet-5015:exit
aclm_RuleActionSet-2476:action: ctcs_acl_add_action_field iUnit=0 ulEid=49
aclm_RuleActionSet-3397:exit
aclm_RuleCreate-5398:EXIT !!
FDI_aclm_RuleCreate-5649: good !! rv(0)
aclm_RuleInfoCreate-2235:enter info set.rule:50
aclm_RuleInfoCreate-2261:leave info create 
FDI_aclm_RuleCreate-5739:create acl success rule id =49
ctc_tms_Ap_Lbd_to_drop-669:LstADD:ulSnmpPort = 45, ulVlanId = 200, ulAclId = 50

ctc_tms_Ap_Lbd_to_drop-674:EXIT OK!

ctc_tms_lbd_to_drop_set-776:ctc_tms_Ap_Lbd_to_drop function return correct .
ctc_tms_lbd_to_drop_set-779:ctc_tms_lbd_to_drop_set ¡£ulSnmpPort = 45
ctc_tms_lbd_to_drop_set-798:EXIT OK!

ctc_tms_lbd_port_state_set-1302:LbdInfo:port=45,vlan=100,portstate=0

ctc_tms_lbd_port_state_set-1335:LbdInfo:port=45,stgId=2,vlan=100 time=1034085236

ctc_tms_lbd_port_state_set-1357:set port(45) stg state over time=1034085236

ctc_tms_lbd_to_drop_set-765:Entry:ulSnmpPort = 45, ulvlan = 100.

ctc_tms_Ap_Lbd_to_drop-589:Entry : ulSnmpPort = 45, ulVlanId = 100

aclm_ProfileFind-1898:input aclm profile id, 0
aclm_ProfileFind-2002:Mapping to: profile,CTC_ACL_KEY_MAC ,CTC_INGRESS
aclm_ProfileFind-2080:find available profile(profile_acl_mac_ingress)
aclm_IdAlloc-209: alloc type 10 id 50
aclm_ProfileInfoGet-1854:entry. profileType: 0 
FDI_aclm_RuleCreate-5637:service api profile id:0, ctc profile id:0, ctc key type:0

aclm_RuleCreate-5309:ENTER !!
aclm_RuleCreate-5339:iunit:0,gid=1,eid=50,key_type=0

aclm_RuleQualifySet-3437:Enter
aclm_RuleQualifySet-3472:port: 8e293908 mask 8e2938b0
aclm_RuleQualifySet-3678:ctc_acl_add_key_field : iUnit=0,ulEid=50,macAddr=0x ff ff ff ff ff ff,macMask=0x ff ff ff ff ff ff
aclm_RuleQualifySet-3710:ctc_acl_add_key_field : iUnit=0,ulEid=50,uiData=0x898,uiMask=0xffff
aclm_RuleQualifySet-4028:ctc_acl_add_key_field : iUnit=0,ulEid=50,uiData=100,uiMask=0xfff
aclm_RuleQualifySet-5015:exit
aclm_RuleActionSet-2476:action: ctcs_acl_add_action_field iUnit=0 ulEid=50
aclm_RuleActionSet-3397:exit
aclm_RuleCreate-5398:EXIT !!
FDI_aclm_RuleCreate-5649: good !! rv(0)
aclm_RuleInfoCreate-2235:enter info set.rule:51
aclm_RuleInfoCreate-2261:leave info create 
FDI_aclm_RuleCreate-5739:create acl success rule id =50
ctc_tms_Ap_Lbd_to_drop-669:LstADD:ulSnmpPort = 45, ulVlanId = 100, ulAclId = 51

ctc_tms_Ap_Lbd_to_drop-674:EXIT OK!

ctc_tms_lbd_to_drop_set-776:ctc_tms_Ap_Lbd_to_drop function return correct .
ctc_tms_lbd_to_drop_set-779:ctc_tms_lbd_to_drop_set ¡£ulSnmpPort = 45
ctc_tms_lbd_to_drop_set-798:EXIT OK!
```


###  no loopback-detection

```
ctcLbdSwitchStructDel-161:Enter ROS_HW_SS_LBD_PKT_TO_CPU_DEL
aclm_RuleInfoGetByRuleId-259: find rule info ulRuleId=45
aclm_RuleInfoGetByRuleId-259: find rule info ulRuleId=45
FDI_aclm_RuleStatDelete-283:can't find ruleId or stat not exit

aclm_RuleInfoGetByRuleId-259: find rule info ulRuleId=45
aclm_RulePolicerNodeGet-440:ERR:can't find ruleId or stat not exit

aclm_RulePolicerDelete-503: not find the record ulEid=45
aclm_RuleDelete-528:NORMAL:iEid = 44
aclm_IdFree-225: free type 10 id 44
aclm_ProfileInfoGet-1854:entry. profileType: 0 
FDI_aclm_RuleDelete-5799: 1 1

ctc_tms_Ap_Lbd_to_cpu-385:AP LBD TO CPU DELETE
ctc_tms_lbd_to_cpu_del-555:ctc_tms_route_Lbd_to_cpu function return correct .
ctc_tms_lbd_to_drop_del-830:Entry:ulSnmpPort = 45, ulvlan = 100.

aclm_RuleInfoGetByRuleId-259: find rule info ulRuleId=50
aclm_RuleInfoGetByRuleId-259: find rule info ulRuleId=50
FDI_aclm_RuleStatDelete-283:can't find ruleId or stat not exit

aclm_RuleInfoGetByRuleId-259: find rule info ulRuleId=50
aclm_RulePolicerNodeGet-440:ERR:can't find ruleId or stat not exit

aclm_RulePolicerDelete-503: not find the record ulEid=50
aclm_RuleDelete-528:NORMAL:iEid = 49
aclm_IdFree-225: free type 10 id 49
aclm_ProfileInfoGet-1854:entry. profileType: 0 
FDI_aclm_RuleDelete-5799: 1 1

ctc_tms_lbd_to_drop_del-858:ctc_tms_route_Lbd_to_cpu function return correct .
ctc_tms_lbd_to_drop_del-868:ctc_tms_lbd_to_drop_del ¡£ulSnmpPort = 45
ctc_tms_lbd_port_state_set-1302:LbdInfo:port=45,vlan=100,portstate=1

ctc_tms_lbd_port_state_set-1335:LbdInfo:port=45,stgId=2,vlan=100 time=1034173248

ctc_tms_lbd_port_state_set-1357:set port(45) stg state over time=1034173248

ctc_tms_lbd_to_drop_del-830:Entry:ulSnmpPort = 45, ulvlan = 200.

aclm_RuleInfoGetByRuleId-259: find rule info ulRuleId=51
aclm_RuleInfoGetByRuleId-259: find rule info ulRuleId=51
FDI_aclm_RuleStatDelete-283:can't find ruleId or stat not exit

aclm_RuleInfoGetByRuleId-259: find rule info ulRuleId=51
aclm_RulePolicerNodeGet-440:ERR:can't find ruleId or stat not exit

aclm_RulePolicerDelete-503: not find the record ulEid=51
aclm_RuleDelete-528:NORMAL:iEid = 50
aclm_IdFree-225: free type 10 id 50
aclm_ProfileInfoGet-1854:entry. profileType: 0 
FDI_aclm_RuleDelete-5799: 1 1

ctc_tms_lbd_to_drop_del-858:ctc_tms_route_Lbd_to_cpu function return correct .
ctc_tms_lbd_to_drop_del-868:ctc_tms_lbd_to_drop_del ¡£ulSnmpPort = 45
ctc_tms_lbd_port_state_set-1302:LbdInfo:port=45,vlan=200,portstate=1

ctc_tms_lbd_port_state_set-1335:LbdInfo:port=45,stgId=3,vlan=200 time=1034173250

ctc_tms_lbd_port_state_set-1357:set port(45) stg state over time=1034173251

ctc_tms_lbd_delete-1187:LbddelInfo:port=45,detectmode=1

ctc_tms_lbd_delete-1245:LbddelInfo:snmpport=45,plbdinfo->port=45,delvlan=100,plbdinfo->vlan=100,logstgId=2

ctc_tms_lbd_delete-1245:LbddelInfo:snmpport=45,plbdinfo->port=45,delvlan=200,plbdinfo->vlan=200,logstgId=3
```