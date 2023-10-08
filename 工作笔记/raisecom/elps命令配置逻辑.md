
## 基本原理

### 简介

ELPS（Ethernet Linear Protection Switching，以太网线性保护倒换）是基于 ITU-T G.8031 标准的 APS（Automatic Protection Switching，自动保护倒换）协议的一种端到端的保护技术，用于保护一条以太网连接线路，它可以用于不同的网络结构，如网状网，环网等。

APS 报文是 CFM（Connectivity Fault Management，连接故障管理）报文的一种，当 CFM 帧结构中的 OpCode 取值为 39 时，表示的是 APS 报文。APS 报文外部结构由ITU-T Y.1731 定义，G.8031 在此基础上利用 4 字节空间对 APS 具体信息进行了定义。

G.8031 定义了 1＋1 和 1:1 两种保护方式。ELPS 的保护倒换机制对工作资源都分配相应的保护资源，如路径和带宽等。ELPS 技术简单快速，以一种可预测的方式实现网络资源切换，更易于运营商有效地规划网络，实现电信级的保护。


### 保护方式

ELPS 支持 1＋1 和 1:1 两种保护方式：
- 1＋1 保护倒换：每个工作路径分配一条保护路径。在保护域内，正常情况下，源端在工作和保护路径都传输流量，而宿端选择其中一个路径接收流量，宿端对工作链路和保护链路的选择基于某种预定标准，如服务器缺陷指示。业务在故障情况下直接倒换到保护链路。

- 1:1 保护倒换：每个工作路径分配一条保护路径。与 1＋1 保护不同的是，业务只在其中的一个路径进行传输。正常情况下，业务在工作路径上传输，保护路径做备份，当工作路径出现故障时，需要通过 APS 协议进行协商，以便源端和宿端同时切换到保护路径。

> 1+1保护为了避免正常的业务信号出现第二次“glitch”，一般采用非返回式倒换。而1：1保护虽然可以配置为非返回式倒换，但是一般工作路径的传输实体在可靠性上更优，所以在故障排除后，回切到工作路径，这种情况可以接收正常业务信号的“glitch”。



### 倒换方式

按照链路发生故障时，两端是否同时切换，可以分为单向倒换和双向倒换。

- 单向倒换是指一条链路的一个方向发生故障时，一个方向能够接收业务流量，另一个方向不能接收，此时不能接收的方向的节点检测到故障时进行切换，能够接收的方向的链路正常则不进行切换，切换的结果是 ELPS 连接的两端可能选择不同的链路传输业务流量。

- 双向倒换是指链路发生故障时，即使仅有一个方向故障，两端也需要根据 APS 协议协商同时切换到保护链路，切换的结果是 ELPS 连接的两端需要选择同一条链路进行发送和接收。

> 设备只有配置为 1＋1 方式时才支持单向和双向倒换，当配置为 1:1 方式时仅支持双向倒换。


### 故障检测方式

ELPS 提供了三种方式来检测故障：
- 基于物理接口状态：能够快速获知链路故障达到快速倒换的目的，适用于相邻设备之间。
- 基于 CFM：适用于单向检测或跨越多个设备检测的情况。
- 基于物理接口状态和 CFM：物理链路或 CFM 任何一种检测方式检测到故障都上报。



## 命令行配置


### ethernet line-protection 1 working gigaethernet 1/5/5 1-10 protection gigaethernet 1/5/7 1-10 one-to-one protocol-vlan 100

```
ssp_apsHwSwitchStructSet@108:ulApsId=1, ulWorkIfIndex=272670739, ulProtectIfIndex=272670747
ssp_tms_aps_eth_create@1019:ApsId = 1, WorkIfidx = 0x1040a013, ProtIfidx = 0x1040a01b.
2238-ssp_logic_stg_create:entry. iUnit=0,eModId = 2
1179-_logic_stg_vlanlist_to_stg_set:entry:unit=0,iLogId=0,bDefaultStg=1,ulStgNeed=1
1188-_logic_stg_vlanlist_to_stg_set:entry:unit=0,usedstg=1,maxstg=129
232-bcm_tms_stg_create:Entry iUnit=0,stg= 0.

244-bcm_tms_stg_create:create stg = 2.

276-bcm_tms_stg_create:Exit rc = 0.
178-_stg_stg_hw_create:get hw stg=1, used hw stg num=2
1194-_logic_stg_vlanlist_to_stg_set:entry:unit=0,iStgId=1
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
2248-ssp_logic_stg_create:exit. rc  = 0.
ssp_tms_aps_eth_create@1029:ssp_logic_stg_create: stgid=0.
1791-ssp_stg_port_vlanlist_take:entry:iUnit=0,ulIfindex=272670739,eModId=2,eOperType=1.
1849-ssp_stg_port_vlanlist_take:mode 2 take port 272670739 by opertype=1 , create stpModPort
871-_stg_port_state_copy_by_port_new:entry.iUnit=0,Ifindex=272670739,iLogStgId=0.
902-_stg_port_state_copy_by_port_new:get gport=0 hwstg=0 stgstate=0, return rc=0x0

945-_stg_port_state_copy_by_port_new:Ifindex=272670739,ulStgIndex = 0,eStateRos = 3.
902-_stg_port_state_copy_by_port_new:get gport=0 hwstg=1 stgstate=0, return rc=0x0

945-_stg_port_state_copy_by_port_new:Ifindex=272670739,ulStgIndex = 1,eStateRos = 3.
963-_stg_port_state_copy_by_port_new:exit.rc = 0.
1886-ssp_stg_port_vlanlist_take:exit. rc  = 0.
1791-ssp_stg_port_vlanlist_take:entry:iUnit=0,ulIfindex=272670747,eModId=2,eOperType=1.
1849-ssp_stg_port_vlanlist_take:mode 2 take port 272670747 by opertype=1 , create stpModPort
871-_stg_port_state_copy_by_port_new:entry.iUnit=0,Ifindex=272670747,iLogStgId=0.
902-_stg_port_state_copy_by_port_new:get gport=0 hwstg=0 stgstate=0, return rc=0x0

945-_stg_port_state_copy_by_port_new:Ifindex=272670747,ulStgIndex = 0,eStateRos = 3.
902-_stg_port_state_copy_by_port_new:get gport=0 hwstg=1 stgstate=0, return rc=0x0

945-_stg_port_state_copy_by_port_new:Ifindex=272670747,ulStgIndex = 1,eStateRos = 3.
963-_stg_port_state_copy_by_port_new:exit.rc = 0.
1886-ssp_stg_port_vlanlist_take:exit. rc  = 0.
_ssp_aps_eth_switch_process_create@135:StgId=0, StgBlockPort=0x1040a01b, StgForwardPort=0x1040a013, Flag=1. start time=72957550
2302-ssp_logic_stg_port_state_set:entry:iUnit=0,STGId=0,Ifindex=272670747,eState=1,eModId=2. start time=72957550
390-_stg_port_state_set:entry:iUnit=0,STGId=1,Ifindex=272670747,eState=1,eType=1.
301-_stg_port_state_hw_set:entry:iUnit=0,STGId=1,Ifindex=272670747,eState=1.
338-bcm_tms_stg_stp_set:Entry ulStgId = 1 ulPort = 47 ulState = 1.
427-bcm_tms_stg_stp_set:Exit state = 1, rc = 0. 
315-_stg_port_state_hw_set:exit. rc  = 0.
500-_stg_port_state_set:exit. rc  = 0.
2611-ssp_stg_port_state_change_notify:ifIdx=0x1040a01b, state(1:BLOCK;3:FORWARD)=1, etype(1:STG;2:STP)=1.
2342-ssp_logic_stg_port_state_set:exit. rc  = 0. end time=72957550
2302-ssp_logic_stg_port_state_set:entry:iUnit=0,STGId=0,Ifindex=272670739,eState=3,eModId=2. start time=72957550
390-_stg_port_state_set:entry:iUnit=0,STGId=1,Ifindex=272670739,eState=3,eType=1.
301-_stg_port_state_hw_set:entry:iUnit=0,STGId=1,Ifindex=272670739,eState=3.
338-bcm_tms_stg_stp_set:Entry ulStgId = 1 ulPort = 45 ulState = 3.
427-bcm_tms_stg_stp_set:Exit state = 0, rc = 0. 
315-_stg_port_state_hw_set:exit. rc  = 0.
500-_stg_port_state_set:exit. rc  = 0.
2611-ssp_stg_port_state_change_notify:ifIdx=0x1040a013, state(1:BLOCK;3:FORWARD)=3, etype(1:STG;2:STP)=1.
2342-ssp_logic_stg_port_state_set:exit. rc  = 0. end time=72957550
_ssp_aps_eth_switch_process_create@167:Clear port mac:blockPort=47, forwrdPort=45. end time=72957550
ssp_tms_aps_eth_create@1094:exit ok
ssp_apsHwSwitchStructSet@230:exit ok
ssp_apsHwSwitchStructSet@65:ulLevel=0, ulVlanId=100, ulIfIndex=272670747, ulIdx=0
ssp_tms_aps_frame_to_cpu_set@809:ulLevel = 0, ulVlanId = 100
ssp_tms_aps_frame_to_cpu_set@894:create acl rule: rule id is 45.
ssp_apsHwSwitchStructSet@230:exit ok
ssp_apsHwSwitchStructSet@132:ulApsId=1, bQuickSwitch=1
ssp_tms_aps_eth_set_quickswitch@1298:ApsId = 1
_ssp_aps_eth_modify@306:ulApsId = 1
_ssp_aps_eth_modify@331:modify quichswitch flag to 1
ssp_tms_aps_eth_set_quickswitch@1308:exit ok
ssp_apsHwSwitchStructSet@230:exit ok
ssp_apsHwSwitchStructSet@145:ulApsId=1, ucWorkFdTypelist=0x80, ucProtectFdTypelist=0x80
ssp_tms_aps_eth_set_fdtype@1330:ulApsId=1
_ssp_aps_eth_modify@306:ulApsId = 1
_ssp_aps_eth_modify@337:modify fd type
ssp_tms_aps_eth_set_fdtype@1341:exit ok
ssp_apsHwSwitchStructSet@230:exit ok
ssp_apsHwSwitchStructSet@120:ulApsId=1, ulAction=1
ssp_tms_aps_eth_switch@1195:ApsId = 1.
ssp_tms_aps_eth_switch@1220:WorkIfidx = 0x1040a013, ProIfidx = 0x1040a01b, SwiAct=1, stgid=0
ssp_tms_aps_eth_switch@1229:eth aps has been switched already!
ssp_apsHwSwitchStructSet@230:exit ok
ssp_apsHwSwitchStructSet@132:ulApsId=1, bQuickSwitch=0
ssp_tms_aps_eth_set_quickswitch@1298:ApsId = 1
_ssp_aps_eth_modify@306:ulApsId = 1
_ssp_aps_eth_modify@331:modify quichswitch flag to 0
ssp_tms_aps_eth_set_quickswitch@1308:exit ok
ssp_apsHwSwitchStructSet@230:exit ok
ssp_apsHwSwitchStructSet@120:ulApsId=1, ulAction=1
ssp_tms_aps_eth_switch@1195:ApsId = 1.
ssp_tms_aps_eth_switch@1220:WorkIfidx = 0x1040a013, ProIfidx = 0x1040a01b, SwiAct=1, stgid=0
ssp_tms_aps_eth_switch@1229:eth aps has been switched already!
ssp_apsHwSwitchStructSet@230:exit ok
```


### no ethernet ring-protection 1

```
ssp_apsHwSwitchStructDel@305:ulApsId=1, ulWorkIfIndex=272670739, ulProtectIfIndex=272670747
ssp_tms_aps_eth_delete@1119:ApsId = 1
ssp_tms_aps_eth_delete@1143:WorkIfidx=0x1040a013, ProtIfidx=0x1040a01b, stgid=0
_ssp_aps_eth_switch_process_create@135:StgId=0, StgBlockPort=0x1040a01b, StgForwardPort=0x1040a013, Flag=0. start time=72983399
2302-ssp_logic_stg_port_state_set:entry:iUnit=0,STGId=0,Ifindex=272670747,eState=3,eModId=2. start time=72983399
390-_stg_port_state_set:entry:iUnit=0,STGId=1,Ifindex=272670747,eState=3,eType=1.
301-_stg_port_state_hw_set:entry:iUnit=0,STGId=1,Ifindex=272670747,eState=3.
338-bcm_tms_stg_stp_set:Entry ulStgId = 1 ulPort = 47 ulState = 3.
427-bcm_tms_stg_stp_set:Exit state = 0, rc = 0. 
315-_stg_port_state_hw_set:exit. rc  = 0.
500-_stg_port_state_set:exit. rc  = 0.
2611-ssp_stg_port_state_change_notify:ifIdx=0x1040a01b, state(1:BLOCK;3:FORWARD)=3, etype(1:STG;2:STP)=1.
2342-ssp_logic_stg_port_state_set:exit. rc  = 0. end time=72983399
2302-ssp_logic_stg_port_state_set:entry:iUnit=0,STGId=0,Ifindex=272670739,eState=3,eModId=2. start time=72983399
390-_stg_port_state_set:entry:iUnit=0,STGId=1,Ifindex=272670739,eState=3,eType=1.
301-_stg_port_state_hw_set:entry:iUnit=0,STGId=1,Ifindex=272670739,eState=3.
338-bcm_tms_stg_stp_set:Entry ulStgId = 1 ulPort = 45 ulState = 3.
427-bcm_tms_stg_stp_set:Exit state = 0, rc = 0. 
315-_stg_port_state_hw_set:exit. rc  = 0.
500-_stg_port_state_set:exit. rc  = 0.
2611-ssp_stg_port_state_change_notify:ifIdx=0x1040a013, state(1:BLOCK;3:FORWARD)=3, etype(1:STG;2:STP)=1.
2342-ssp_logic_stg_port_state_set:exit. rc  = 0. end time=72983400
_ssp_aps_eth_switch_process_create@167:Clear port mac:blockPort=47, forwrdPort=45. end time=72983400
2024-ssp_stg_port_vlanlist_give:entry:iUnit=0,ulIfindex=272670747,eModId=2,eOperType=1.
2038-ssp_stg_port_vlanlist_give:Found And Delete.
2089-ssp_stg_port_vlanlist_give:exit. rc  = 0.
2024-ssp_stg_port_vlanlist_give:entry:iUnit=0,ulIfindex=272670739,eModId=2,eOperType=1.
2038-ssp_stg_port_vlanlist_give:Found And Delete.
2089-ssp_stg_port_vlanlist_give:exit. rc  = 0.
2267-ssp_logic_stg_del:entry:iUnit=0,STGId=0,eModId=2.
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
2271-ssp_logic_stg_del:exit. rc  = 0.
ssp_tms_aps_eth_delete@1167:exit ROS_OK

ssp_apsHwSwitchStructDel@346:exit ok
ssp_apsHwSwitchStructDel@263:ulLevel=0, ulVlanId=100, ulIfIndex=272670747, ulIdx=0
ssp_tms_aps_frame_to_cpu_del@965:the counter is 1 ! ulLevel = 0, ulVlanId = 100
ssp_tms_aps_frame_to_cpu_del@977:delete acl rule: ruldID=45.
ssp_apsHwSwitchStructDel@346:exit ok
```



