
ros：平台进程，ros/main/main.c
    ros/config/cli 目录：设备所有命令行文件，比如：mac_cli.c，vlan_cli.c等
    

ritp：适配层进程，product_esw_ritp/106_iTN8800-A-NXU/dev/src/main.c
rnet：协议栈进程，ros/rnet/main/rnet_main.c
fnet：进程，ros/rnet/fnet/src/main.c


## vlan

### switchport access vlan 100（第一次配置access vlan）

```
56@bcmVlanSwitchStructSet:enter ulPortIndex=0
1636@ctc_ssp_vlan_port_add:enter
1516@ctc_ssp_vlan_port_add_inner:enter vlanId 100 portNum 1 btag 1

1530@ctc_ssp_vlan_port_add_inner:snmp_port 47

1582@ctc_ssp_vlan_port_add_inner:ADD gport 25,vlan = 100

1613@ctc_ssp_vlan_port_add_inner:exit
1516@ctc_ssp_vlan_port_add_inner:enter vlanId 100 portNum 1 btag 0

1530@ctc_ssp_vlan_port_add_inner:snmp_port 47

1582@ctc_ssp_vlan_port_add_inner:ADD gport 25,vlan = 100

1613@ctc_ssp_vlan_port_add_inner:exit
203@ssp_vlan_qinq_event_callback:ulSnmpPort=47,arg1=100,arg2=0,eventType=1
221@ssp_vlan_qinq_event_callback:Module 197 eventType 1hook executed.
1715@ctc_ssp_vlan_port_add:Exit
72@bcmVlanSwitchStructSet:INFO:bcm_ssp_vlan_port_add OK.

56@bcmVlanSwitchStructSet:enter ulPortIndex=47
390@ctc_vlan_port_qtag_set:entry, ulSnmpPort=47, ulVlanId=100
326@_vlan_port_qtag_set:entry, ulSnmpPort=47, ulVlanId=100
186@ctc_tms_port_set_property:lport = 25,type=91 enable 0
244@ctc_tms_port_set_property:exit:0 !
361@_vlan_port_qtag_set:exit OK
203@ssp_vlan_qinq_event_callback:ulSnmpPort=47,arg1=100,arg2=0,eventType=0
221@ssp_vlan_qinq_event_callback:Module 197 eventType 0hook executed.
464@ctc_vlan_port_qtag_set:exit OK
```


### switchport access vlan 200（修改access vlan）

```
56@bcmVlanSwitchStructSet:enter ulPortIndex=0
1636@ctc_ssp_vlan_port_add:enter
1516@ctc_ssp_vlan_port_add_inner:enter vlanId 200 portNum 1 btag 1

1530@ctc_ssp_vlan_port_add_inner:snmp_port 47

1582@ctc_ssp_vlan_port_add_inner:ADD gport 25,vlan = 200

1613@ctc_ssp_vlan_port_add_inner:exit
1516@ctc_ssp_vlan_port_add_inner:enter vlanId 200 portNum 1 btag 0

1530@ctc_ssp_vlan_port_add_inner:snmp_port 47

1582@ctc_ssp_vlan_port_add_inner:ADD gport 25,vlan = 200

1613@ctc_ssp_vlan_port_add_inner:exit
203@ssp_vlan_qinq_event_callback:ulSnmpPort=47,arg1=200,arg2=0,eventType=1
221@ssp_vlan_qinq_event_callback:Module 197 eventType 1hook executed.
1715@ctc_ssp_vlan_port_add:Exit
72@bcmVlanSwitchStructSet:INFO:bcm_ssp_vlan_port_add OK.

314@bcmIfmVlanSwitchStructDelete:
bcmIfmVlanSwitchStructDelete-314 uiIfIndex = 0 ulSnmpPort=0

236@bcmVlanSwitchStructDelete:enter
1867@ctc_ssp_vlan_port_remove:enter
1742@ctc_ssp_vlan_port_remove_inner:enter vlanId 100 portNum 1 btag 0

1756@ctc_ssp_vlan_port_remove_inner:snmp_port 47

1849@ctc_ssp_vlan_port_remove_inner:exit
1742@ctc_ssp_vlan_port_remove_inner:enter vlanId 100 portNum 1 btag 1

1756@ctc_ssp_vlan_port_remove_inner:snmp_port 47

1849@ctc_ssp_vlan_port_remove_inner:exit
1903@ctc_ssp_vlan_port_remove:Exit
56@bcmVlanSwitchStructSet:enter ulPortIndex=47
390@ctc_vlan_port_qtag_set:entry, ulSnmpPort=47, ulVlanId=200
326@_vlan_port_qtag_set:entry, ulSnmpPort=47, ulVlanId=200
186@ctc_tms_port_set_property:lport = 25,type=91 enable 0
244@ctc_tms_port_set_property:exit:0 !
361@_vlan_port_qtag_set:exit OK
203@ssp_vlan_qinq_event_callback:ulSnmpPort=47,arg1=200,arg2=0,eventType=0
221@ssp_vlan_qinq_event_callback:Module 197 eventType 0hook executed.
464@ctc_vlan_port_qtag_set:exit OK
```


### no switchport access vlan

```
314@bcmIfmVlanSwitchStructDelete:
bcmIfmVlanSwitchStructDelete-314 uiIfIndex = 0 ulSnmpPort=0

236@bcmVlanSwitchStructDelete:enter
1867@ctc_ssp_vlan_port_remove:enter
1742@ctc_ssp_vlan_port_remove_inner:enter vlanId 100 portNum 1 btag 0

1756@ctc_ssp_vlan_port_remove_inner:snmp_port 47

1849@ctc_ssp_vlan_port_remove_inner:exit
1742@ctc_ssp_vlan_port_remove_inner:enter vlanId 100 portNum 1 btag 1

1756@ctc_ssp_vlan_port_remove_inner:snmp_port 47

1849@ctc_ssp_vlan_port_remove_inner:exit
1903@ctc_ssp_vlan_port_remove:Exit
56@bcmVlanSwitchStructSet:enter ulPortIndex=47
390@ctc_vlan_port_qtag_set:entry, ulSnmpPort=47, ulVlanId=0
326@_vlan_port_qtag_set:entry, ulSnmpPort=47, ulVlanId=0
186@ctc_tms_port_set_property:lport = 25,type=91 enable 1
244@ctc_tms_port_set_property:exit:0 !
361@_vlan_port_qtag_set:exit OK
203@ssp_vlan_qinq_event_callback:ulSnmpPort=47,arg1=0,arg2=0,eventType=0
221@ssp_vlan_qinq_event_callback:Module 197 eventType 0hook executed.
464@ctc_vlan_port_qtag_set:exit OK
```



### switchport access egress-allowed vlan 200 confirm

入方向报文带vlan 200，正常转发，出方向如果vlan 为200，则剥掉vlan，untag转发出去。但是这里都是通过`ctc_ssp_vlan_port_add_inner`配置，只是btag参数值不同，怎么区分ingress和egress？？？

```
56@bcmVlanSwitchStructSet:enter ulPortIndex=0
1636@ctc_ssp_vlan_port_add:enter
1516@ctc_ssp_vlan_port_add_inner:enter vlanId 200 portNum 1 btag 1

1530@ctc_ssp_vlan_port_add_inner:snmp_port 47

1582@ctc_ssp_vlan_port_add_inner:ADD gport 25,vlan = 200

1613@ctc_ssp_vlan_port_add_inner:exit
1516@ctc_ssp_vlan_port_add_inner:enter vlanId 200 portNum 0 btag 0

1613@ctc_ssp_vlan_port_add_inner:exit
203@ssp_vlan_qinq_event_callback:ulSnmpPort=47,arg1=200,arg2=0,eventType=1
221@ssp_vlan_qinq_event_callback:Module 197 eventType 1hook executed.
1715@ctc_ssp_vlan_port_add:Exit
72@bcmVlanSwitchStructSet:INFO:bcm_ssp_vlan_port_add OK.

56@bcmVlanSwitchStructSet:enter ulPortIndex=0
1636@ctc_ssp_vlan_port_add:enter
1516@ctc_ssp_vlan_port_add_inner:enter vlanId 200 portNum 1 btag 1

1530@ctc_ssp_vlan_port_add_inner:snmp_port 47

1582@ctc_ssp_vlan_port_add_inner:ADD gport 25,vlan = 200

1613@ctc_ssp_vlan_port_add_inner:exit
1516@ctc_ssp_vlan_port_add_inner:enter vlanId 200 portNum 1 btag 0

1530@ctc_ssp_vlan_port_add_inner:snmp_port 47

1582@ctc_ssp_vlan_port_add_inner:ADD gport 25,vlan = 200

1613@ctc_ssp_vlan_port_add_inner:exit
203@ssp_vlan_qinq_event_callback:ulSnmpPort=47,arg1=200,arg2=0,eventType=1
221@ssp_vlan_qinq_event_callback:Module 197 eventType 1hook executed.
1715@ctc_ssp_vlan_port_add:Exit
72@bcmVlanSwitchStructSet:INFO:bcm_ssp_vlan_port_add OK.
```


### no switchport access egress-allowed vlan

```
314@bcmIfmVlanSwitchStructDelete:
bcmIfmVlanSwitchStructDelete-314 uiIfIndex = 0 ulSnmpPort=0

236@bcmVlanSwitchStructDelete:enter
1867@ctc_ssp_vlan_port_remove:enter
1742@ctc_ssp_vlan_port_remove_inner:enter vlanId 200 portNum 1 btag 0

1756@ctc_ssp_vlan_port_remove_inner:snmp_port 47

1849@ctc_ssp_vlan_port_remove_inner:exit
1742@ctc_ssp_vlan_port_remove_inner:enter vlanId 200 portNum 1 btag 1

1756@ctc_ssp_vlan_port_remove_inner:snmp_port 47

1849@ctc_ssp_vlan_port_remove_inner:exit
1903@ctc_ssp_vlan_port_remove:Exit
314@bcmIfmVlanSwitchStructDelete:
bcmIfmVlanSwitchStructDelete-314 uiIfIndex = 0 ulSnmpPort=0

236@bcmVlanSwitchStructDelete:enter
1867@ctc_ssp_vlan_port_remove:enter
1742@ctc_ssp_vlan_port_remove_inner:enter vlanId 200 portNum 0 btag 0

1849@ctc_ssp_vlan_port_remove_inner:exit
1742@ctc_ssp_vlan_port_remove_inner:enter vlanId 200 portNum 0 btag 1

1849@ctc_ssp_vlan_port_remove_inner:exit
1903@ctc_ssp_vlan_port_remove:Exit
```


### switchport trunk allowed vlan 200 confirm

```
56@bcmVlanSwitchStructSet:enter ulPortIndex=0
1636@ctc_ssp_vlan_port_add:enter
1516@ctc_ssp_vlan_port_add_inner:enter vlanId 200 portNum 1 btag 1

1530@ctc_ssp_vlan_port_add_inner:snmp_port 47

1582@ctc_ssp_vlan_port_add_inner:ADD gport 25,vlan = 200

1613@ctc_ssp_vlan_port_add_inner:exit
1516@ctc_ssp_vlan_port_add_inner:enter vlanId 200 portNum 0 btag 0

1613@ctc_ssp_vlan_port_add_inner:exit
203@ssp_vlan_qinq_event_callback:ulSnmpPort=47,arg1=200,arg2=0,eventType=1
221@ssp_vlan_qinq_event_callback:Module 197 eventType 1hook executed.
1715@ctc_ssp_vlan_port_add:Exit
72@bcmVlanSwitchStructSet:INFO:bcm_ssp_vlan_port_add OK.
```



### no switchport trunk allowed vlan 200
或者 `switchport trunk allowed vlan remove 200`

```
314@bcmIfmVlanSwitchStructDelete:
bcmIfmVlanSwitchStructDelete-314 uiIfIndex = 0 ulSnmpPort=0

236@bcmVlanSwitchStructDelete:enter
1867@ctc_ssp_vlan_port_remove:enter
1742@ctc_ssp_vlan_port_remove_inner:enter vlanId 200 portNum 0 btag 0

1849@ctc_ssp_vlan_port_remove_inner:exit
1742@ctc_ssp_vlan_port_remove_inner:enter vlanId 200 portNum 1 btag 1

1756@ctc_ssp_vlan_port_remove_inner:snmp_port 47

1849@ctc_ssp_vlan_port_remove_inner:exit
1903@ctc_ssp_vlan_port_remove:Exit
```


### switchport trunk native vlan 100（没有配置过native vlan）

```
56@bcmVlanSwitchStructSet:enter ulPortIndex=0
1636@ctc_ssp_vlan_port_add:enter
1516@ctc_ssp_vlan_port_add_inner:enter vlanId 100 portNum 1 btag 1

1530@ctc_ssp_vlan_port_add_inner:snmp_port 47

1582@ctc_ssp_vlan_port_add_inner:ADD gport 25,vlan = 100

1613@ctc_ssp_vlan_port_add_inner:exit
1516@ctc_ssp_vlan_port_add_inner:enter vlanId 100 portNum 1 btag 0

1530@ctc_ssp_vlan_port_add_inner:snmp_port 47

1582@ctc_ssp_vlan_port_add_inner:ADD gport 25,vlan = 100

1613@ctc_ssp_vlan_port_add_inner:exit
203@ssp_vlan_qinq_event_callback:ulSnmpPort=47,arg1=100,arg2=0,eventType=1
221@ssp_vlan_qinq_event_callback:Module 197 eventType 1hook executed.
1715@ctc_ssp_vlan_port_add:Exit
72@bcmVlanSwitchStructSet:INFO:bcm_ssp_vlan_port_add OK.

56@bcmVlanSwitchStructSet:enter ulPortIndex=0
1636@ctc_ssp_vlan_port_add:enter
1516@ctc_ssp_vlan_port_add_inner:enter vlanId 100 portNum 1 btag 1

1530@ctc_ssp_vlan_port_add_inner:snmp_port 47

1582@ctc_ssp_vlan_port_add_inner:ADD gport 25,vlan = 100

1613@ctc_ssp_vlan_port_add_inner:exit
1516@ctc_ssp_vlan_port_add_inner:enter vlanId 100 portNum 0 btag 0

1613@ctc_ssp_vlan_port_add_inner:exit
203@ssp_vlan_qinq_event_callback:ulSnmpPort=47,arg1=100,arg2=0,eventType=1
221@ssp_vlan_qinq_event_callback:Module 197 eventType 1hook executed.
1715@ctc_ssp_vlan_port_add:Exit
72@bcmVlanSwitchStructSet:INFO:bcm_ssp_vlan_port_add OK.

56@bcmVlanSwitchStructSet:enter ulPortIndex=0
1636@ctc_ssp_vlan_port_add:enter
1516@ctc_ssp_vlan_port_add_inner:enter vlanId 100 portNum 1 btag 1

1530@ctc_ssp_vlan_port_add_inner:snmp_port 47

1582@ctc_ssp_vlan_port_add_inner:ADD gport 25,vlan = 100

1613@ctc_ssp_vlan_port_add_inner:exit
1516@ctc_ssp_vlan_port_add_inner:enter vlanId 100 portNum 1 btag 0

1530@ctc_ssp_vlan_port_add_inner:snmp_port 47

1582@ctc_ssp_vlan_port_add_inner:ADD gport 25,vlan = 100

1613@ctc_ssp_vlan_port_add_inner:exit
203@ssp_vlan_qinq_event_callback:ulSnmpPort=47,arg1=100,arg2=0,eventType=1
221@ssp_vlan_qinq_event_callback:Module 197 eventType 1hook executed.
1715@ctc_ssp_vlan_port_add:Exit
72@bcmVlanSwitchStructSet:INFO:bcm_ssp_vlan_port_add OK.

56@bcmVlanSwitchStructSet:enter ulPortIndex=47
390@ctc_vlan_port_qtag_set:entry, ulSnmpPort=47, ulVlanId=100
326@_vlan_port_qtag_set:entry, ulSnmpPort=47, ulVlanId=100
186@ctc_tms_port_set_property:lport = 25,type=91 enable 0
244@ctc_tms_port_set_property:exit:0 !
361@_vlan_port_qtag_set:exit OK
203@ssp_vlan_qinq_event_callback:ulSnmpPort=47,arg1=100,arg2=0,eventType=0
221@ssp_vlan_qinq_event_callback:Module 197 eventType 0hook executed.
464@ctc_vlan_port_qtag_set:exit OK
```



### switchport trunk native vlan 100（已经配置过native vlan 200）

```
56@bcmVlanSwitchStructSet:enter ulPortIndex=0
1636@ctc_ssp_vlan_port_add:enter
1516@ctc_ssp_vlan_port_add_inner:enter vlanId 100 portNum 1 btag 1

1530@ctc_ssp_vlan_port_add_inner:snmp_port 47

1582@ctc_ssp_vlan_port_add_inner:ADD gport 25,vlan = 100

1613@ctc_ssp_vlan_port_add_inner:exit
1516@ctc_ssp_vlan_port_add_inner:enter vlanId 100 portNum 1 btag 0

1530@ctc_ssp_vlan_port_add_inner:snmp_port 47

1582@ctc_ssp_vlan_port_add_inner:ADD gport 25,vlan = 100

1613@ctc_ssp_vlan_port_add_inner:exit
203@ssp_vlan_qinq_event_callback:ulSnmpPort=47,arg1=100,arg2=0,eventType=1
221@ssp_vlan_qinq_event_callback:Module 197 eventType 1hook executed.
1715@ctc_ssp_vlan_port_add:Exit
72@bcmVlanSwitchStructSet:INFO:bcm_ssp_vlan_port_add OK.

314@bcmIfmVlanSwitchStructDelete:
bcmIfmVlanSwitchStructDelete-314 uiIfIndex = 0 ulSnmpPort=0

236@bcmVlanSwitchStructDelete:enter
1867@ctc_ssp_vlan_port_remove:enter
1742@ctc_ssp_vlan_port_remove_inner:enter vlanId 200 portNum 1 btag 0

1756@ctc_ssp_vlan_port_remove_inner:snmp_port 47

1849@ctc_ssp_vlan_port_remove_inner:exit
1742@ctc_ssp_vlan_port_remove_inner:enter vlanId 200 portNum 1 btag 1

1756@ctc_ssp_vlan_port_remove_inner:snmp_port 47

1849@ctc_ssp_vlan_port_remove_inner:exit
1903@ctc_ssp_vlan_port_remove:Exit
314@bcmIfmVlanSwitchStructDelete:
bcmIfmVlanSwitchStructDelete-314 uiIfIndex = 0 ulSnmpPort=0

236@bcmVlanSwitchStructDelete:enter
1867@ctc_ssp_vlan_port_remove:enter
1742@ctc_ssp_vlan_port_remove_inner:enter vlanId 200 portNum 0 btag 0

1849@ctc_ssp_vlan_port_remove_inner:exit
1742@ctc_ssp_vlan_port_remove_inner:enter vlanId 200 portNum 0 btag 1

1849@ctc_ssp_vlan_port_remove_inner:exit
1903@ctc_ssp_vlan_port_remove:Exit
56@bcmVlanSwitchStructSet:enter ulPortIndex=0
1636@ctc_ssp_vlan_port_add:enter
1516@ctc_ssp_vlan_port_add_inner:enter vlanId 100 portNum 1 btag 1

1530@ctc_ssp_vlan_port_add_inner:snmp_port 47

1582@ctc_ssp_vlan_port_add_inner:ADD gport 25,vlan = 100

1613@ctc_ssp_vlan_port_add_inner:exit
1516@ctc_ssp_vlan_port_add_inner:enter vlanId 100 portNum 0 btag 0

1613@ctc_ssp_vlan_port_add_inner:exit
203@ssp_vlan_qinq_event_callback:ulSnmpPort=47,arg1=100,arg2=0,eventType=1
221@ssp_vlan_qinq_event_callback:Module 197 eventType 1hook executed.
1715@ctc_ssp_vlan_port_add:Exit
72@bcmVlanSwitchStructSet:INFO:bcm_ssp_vlan_port_add OK.

56@bcmVlanSwitchStructSet:enter ulPortIndex=0
1636@ctc_ssp_vlan_port_add:enter
1516@ctc_ssp_vlan_port_add_inner:enter vlanId 100 portNum 1 btag 1

1530@ctc_ssp_vlan_port_add_inner:snmp_port 47

1582@ctc_ssp_vlan_port_add_inner:ADD gport 25,vlan = 100

1613@ctc_ssp_vlan_port_add_inner:exit
1516@ctc_ssp_vlan_port_add_inner:enter vlanId 100 portNum 1 btag 0

1530@ctc_ssp_vlan_port_add_inner:snmp_port 47

1582@ctc_ssp_vlan_port_add_inner:ADD gport 25,vlan = 100

1613@ctc_ssp_vlan_port_add_inner:exit
203@ssp_vlan_qinq_event_callback:ulSnmpPort=47,arg1=100,arg2=0,eventType=1
221@ssp_vlan_qinq_event_callback:Module 197 eventType 1hook executed.
1715@ctc_ssp_vlan_port_add:Exit
72@bcmVlanSwitchStructSet:INFO:bcm_ssp_vlan_port_add OK.

56@bcmVlanSwitchStructSet:enter ulPortIndex=47
390@ctc_vlan_port_qtag_set:entry, ulSnmpPort=47, ulVlanId=100
326@_vlan_port_qtag_set:entry, ulSnmpPort=47, ulVlanId=100
186@ctc_tms_port_set_property:lport = 25,type=91 enable 0
244@ctc_tms_port_set_property:exit:0 !
361@_vlan_port_qtag_set:exit OK
203@ssp_vlan_qinq_event_callback:ulSnmpPort=47,arg1=100,arg2=0,eventType=0
221@ssp_vlan_qinq_event_callback:Module 197 eventType 0hook executed.
464@ctc_vlan_port_qtag_set:exit OK
```


### no switchport trunk native vlan

```
56@bcmVlanSwitchStructSet:enter ulPortIndex=0
1636@ctc_ssp_vlan_port_add:enter
1516@ctc_ssp_vlan_port_add_inner:enter vlanId 100 portNum 1 btag 1

1530@ctc_ssp_vlan_port_add_inner:snmp_port 47

1582@ctc_ssp_vlan_port_add_inner:ADD gport 25,vlan = 100

1613@ctc_ssp_vlan_port_add_inner:exit
1516@ctc_ssp_vlan_port_add_inner:enter vlanId 100 portNum 0 btag 0

1613@ctc_ssp_vlan_port_add_inner:exit
203@ssp_vlan_qinq_event_callback:ulSnmpPort=47,arg1=100,arg2=0,eventType=1
221@ssp_vlan_qinq_event_callback:Module 197 eventType 1hook executed.
1715@ctc_ssp_vlan_port_add:Exit
72@bcmVlanSwitchStructSet:INFO:bcm_ssp_vlan_port_add OK.

314@bcmIfmVlanSwitchStructDelete:
bcmIfmVlanSwitchStructDelete-314 uiIfIndex = 0 ulSnmpPort=0

236@bcmVlanSwitchStructDelete:enter
1867@ctc_ssp_vlan_port_remove:enter
1742@ctc_ssp_vlan_port_remove_inner:enter vlanId 100 portNum 1 btag 0

1756@ctc_ssp_vlan_port_remove_inner:snmp_port 47

1849@ctc_ssp_vlan_port_remove_inner:exit
1742@ctc_ssp_vlan_port_remove_inner:enter vlanId 100 portNum 0 btag 1

1849@ctc_ssp_vlan_port_remove_inner:exit
1903@ctc_ssp_vlan_port_remove:Exit
56@bcmVlanSwitchStructSet:enter ulPortIndex=47
390@ctc_vlan_port_qtag_set:entry, ulSnmpPort=47, ulVlanId=0
326@_vlan_port_qtag_set:entry, ulSnmpPort=47, ulVlanId=0
186@ctc_tms_port_set_property:lport = 25,type=91 enable 1
244@ctc_tms_port_set_property:exit:0 !
361@_vlan_port_qtag_set:exit OK
203@ssp_vlan_qinq_event_callback:ulSnmpPort=47,arg1=0,arg2=0,eventType=0
221@ssp_vlan_qinq_event_callback:Module 197 eventType 0hook executed.
464@ctc_vlan_port_qtag_set:exit OK
```



### switchport trunk untagged vlan 200 confirm

```
56@bcmVlanSwitchStructSet:enter ulPortIndex=0
1636@ctc_ssp_vlan_port_add:enter
1516@ctc_ssp_vlan_port_add_inner:enter vlanId 200 portNum 1 btag 1

1530@ctc_ssp_vlan_port_add_inner:snmp_port 47

1582@ctc_ssp_vlan_port_add_inner:ADD gport 25,vlan = 200

1613@ctc_ssp_vlan_port_add_inner:exit
1516@ctc_ssp_vlan_port_add_inner:enter vlanId 200 portNum 1 btag 0

1530@ctc_ssp_vlan_port_add_inner:snmp_port 47

1582@ctc_ssp_vlan_port_add_inner:ADD gport 25,vlan = 200

1613@ctc_ssp_vlan_port_add_inner:exit
203@ssp_vlan_qinq_event_callback:ulSnmpPort=47,arg1=200,arg2=0,eventType=1
221@ssp_vlan_qinq_event_callback:Module 197 eventType 1hook executed.
1715@ctc_ssp_vlan_port_add:Exit
72@bcmVlanSwitchStructSet:INFO:bcm_ssp_vlan_port_add OK.
```



### no switchport trunk untagged vlan
或者`switchport trunk untagged vlan remove 200`

```
314@bcmIfmVlanSwitchStructDelete:
bcmIfmVlanSwitchStructDelete-314 uiIfIndex = 0 ulSnmpPort=0

236@bcmVlanSwitchStructDelete:enter
1867@ctc_ssp_vlan_port_remove:enter
1742@ctc_ssp_vlan_port_remove_inner:enter vlanId 200 portNum 1 btag 0

1756@ctc_ssp_vlan_port_remove_inner:snmp_port 47

1849@ctc_ssp_vlan_port_remove_inner:exit
1742@ctc_ssp_vlan_port_remove_inner:enter vlanId 200 portNum 0 btag 1

1849@ctc_ssp_vlan_port_remove_inner:exit
1903@ctc_ssp_vlan_port_remove:Exit
```



























## qinq


平台命令行下发的命令字：
![](images/Pasted%20image%2020230524152347.png)

其中，`RC_HW_QINQ_START`定义如下，起始值（0xC500）：
```
#define RC_HW_QINQ_START (ROS_MID_QINQ * ROS_SSP_MODULE_CAPACITY)
```


### mls double-tagging tpid

**ROS_HW_SV_TPID**

```
[406, ctcQinQSwitchStructSet]  ifIndex 0,  SnmpPort 0,cmd  50434

[871, ctc_tms_set_dtag_sptpid]  entry. tpid=34984

[902, ctc_tms_set_dtag_sptpid]  exit succeed ,[rv: 0]

[477, ctcQinQSwitchStructSet]  exit succeed ,[rv: 0]
```



### no mls double-tagging tpid

**ROS_HW_SV_TPID**

```
[406, ctcQinQSwitchStructSet]  ifIndex 0,  SnmpPort 0,cmd  50434

[871, ctc_tms_set_dtag_sptpid]  entry. tpid=33024

[902, ctc_tms_set_dtag_sptpid]  exit succeed ,[rv: 0]

[477, ctcQinQSwitchStructSet]  exit succeed ,[rv: 0]

```



### mls double-tagging inner-tpid

**ROS_HW_SV_DTAG_CETPID**

```
[406, ctcQinQSwitchStructSet]  ifIndex 0,  SnmpPort 0,cmd  50436

[720, ctc_tms_set_dtag_cetpid]  entry. tpid=37120

[752, ctc_tms_set_dtag_cetpid]  exit succeed ,[rv: 0]

[477, ctcQinQSwitchStructSet]  exit succeed ,[rv: 0]
```



### no mls double-tagging inner-tpid

**ROS_HW_SV_DTAG_CETPID**

```
[406, ctcQinQSwitchStructSet]  ifIndex 0,  SnmpPort 0,cmd  50436

[720, ctc_tms_set_dtag_cetpid]  entry. tpid=33024

[752, ctc_tms_set_dtag_cetpid]  exit succeed ,[rv: 0]

[477, ctcQinQSwitchStructSet]  exit succeed ,[rv: 0]
```



### dot1q-tunnel

**ROS_HW_PORT_QINQ_ENABLE**

```
[406, ctcQinQSwitchStructSet]  ifIndex 272670747,  SnmpPort 47,cmd  50432

[411, ctcQinQSwitchStructSet]  uiIfIndex=272670747,snmp_port=47

[351, ctc_tms_set_port_qinq_enable]  entry! snmpPort=47,Enable=1,pPort->dot1qEnable= 0

[280, _ctc_port_dot1q_enable]  entry! snmp_port=47,bDot1qEna=1,bCosOvrd=0,bMissEna=0,pvid=100

[320, _ctc_port_dot1q_enable]  exit succeed ,[rv: 0]

[436, ctc_tms_set_port_qinq_enable]  exit succeed ,[rv: 0]

[477, ctcQinQSwitchStructSet]  exit succeed ,[rv: 0]
```



### no dot1q-tunnel

**ROS_HW_PORT_QINQ_ENABLE**

```
[406, ctcQinQSwitchStructSet]  ifIndex 272670747,  SnmpPort 47,cmd  50432

[411, ctcQinQSwitchStructSet]  uiIfIndex=272670747,snmp_port=47

[351, ctc_tms_set_port_qinq_enable]  entry! snmpPort=47,Enable=2,pPort->dot1qEnable= 1

[280, _ctc_port_dot1q_enable]  entry! snmp_port=47,bDot1qEna=0,bCosOvrd=0,bMissEna=0,pvid=100

[320, _ctc_port_dot1q_enable]  exit succeed ,[rv: 0]

[436, ctc_tms_set_port_qinq_enable]  exit succeed ,[rv: 0]

[477, ctcQinQSwitchStructSet]  exit succeed ,[rv: 0]
```



### dot1q-tunnel cos override

**ROS_HW_PORT_QINQ_COS_OVERRIDE**

```
[406, ctcQinQSwitchStructSet]  ifIndex 272670747,  SnmpPort 47,cmd  50438

[500, ctc_tms_set_port_cos_override_enable]  entry! snmpPort=47,Enable=1,pPort->dot1qEnable= 0

[523, ctc_tms_set_port_cos_override_enable]  exit succeed ,[rv: 0]

[477, ctcQinQSwitchStructSet]  exit succeed ,[rv: 0]
```



### no dot1q-tunnel cos  override

**ROS_HW_PORT_QINQ_COS_OVERRIDE**

```
[406, ctcQinQSwitchStructSet]  ifIndex 272670747,  SnmpPort 47,cmd  50438

[500, ctc_tms_set_port_cos_override_enable]  entry! snmpPort=47,Enable=2,pPort->dot1qEnable= 0

[523, ctc_tms_set_port_cos_override_enable]  exit succeed ,[rv: 0]

[477, ctcQinQSwitchStructSet]  exit succeed ,[rv: 0]
```



### switchport vlan-mapping-miss discard

**ROS_HW_PORT_QINQ_VLAN_MAPPING_MISS_DROP**

从debug打印上看cmd_type值是50453（0xC515），平台类型是从0xC500开始枚举的，这个对不上。

```
[406, ctcQinQSwitchStructSet]  ifIndex 272670747,  SnmpPort 47,cmd  50453

[616, ctc_tms_set_port_miss_drop_enable]  entry! snmpPort=47,Enable=1,pPort->dot1qEnable= 0
477@ctc_vlan_port_qtag_get:entry, ulSnmpPort = 47
493@ctc_vlan_port_qtag_get:*pVlanId = 0
494@ctc_vlan_port_qtag_get:exit OK

[280, _ctc_port_dot1q_enable]  entry! snmp_port=47,bDot1qEna=0,bCosOvrd=0,bMissEna=1,pvid=0

[320, _ctc_port_dot1q_enable]  exit succeed ,[rv: 0]

[701, ctc_tms_set_port_miss_drop_enable]  exit succeed ,[rv: 0]

[477, ctcQinQSwitchStructSet]  exit succeed ,[rv: 0]
```

sdk查看配置前后该接口scl的默认动作，入方向和出方向都变成丢弃：
```
CTC_CLI(ctc-sdk)# show scl default-action port 0x19 
SCL default action
-----------------------------------------------
GPort 0x0019 default Ingress action :  FWD
--DsUserId :15922
GPort 0x0019 default Ingress action 1 :  FWD
--DsUserId1 :15922
GPort 0x0019 default Egress action  :  FWD
--DsVlanXlateDefault :25
 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# show scl default-action port 0x19 
SCL default action
-----------------------------------------------
GPort 0x0019 default Ingress action :  DISCARD
--DsUserId :15922
GPort 0x0019 default Ingress action 1 :  FWD
--DsUserId1 :15922
GPort 0x0019 default Egress action  :  DISCARD
--DsVlanXlateDefault :25
 
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# 
```

或者用命令：
```
CTC_CLI(ctc-sdk)# 
CTC_CLI(ctc-sdk)# show vlan mapping default info port 0x19 
SCL default action
-----------------------------------------------
GPort 0x0019 default Ingress action :  DISCARD
--DsUserId :15922
GPort 0x0019 default Ingress action 1 :  FWD
--DsUserId1 :15922
GPort 0x0019 default Egress action  :  DISCARD
--DsVlanXlateDefault :25
 
CTC_CLI(ctc-sdk)#
```


### no switchport vlan-mapping-miss discard

**ROS_HW_PORT_QINQ_VLAN_MAPPING_MISS_DROP**

```
[406, ctcQinQSwitchStructSet]  ifIndex 272670747,  SnmpPort 47,cmd  50453

[616, ctc_tms_set_port_miss_drop_enable]  entry! snmpPort=47,Enable=2,pPort->dot1qEnable= 0
477@ctc_vlan_port_qtag_get:entry, ulSnmpPort = 47
493@ctc_vlan_port_qtag_get:*pVlanId = 0
494@ctc_vlan_port_qtag_get:exit OK

[280, _ctc_port_dot1q_enable]  entry! snmp_port=47,bDot1qEna=0,bCosOvrd=0,bMissEna=0,pvid=0

[320, _ctc_port_dot1q_enable]  exit succeed ,[rv: 0]

[701, ctc_tms_set_port_miss_drop_enable]  exit succeed ,[rv: 0]

[477, ctcQinQSwitchStructSet]  exit succeed ,[rv: 0]
```



### switchport vlan-mapping both outer 100 translate 200

**ROS_HW_SS_QINQ_VLAN_MAPPING_TRANS_INGRESS**
**ROS_HW_SS_QINQ_VLAN_MAPPING_TRANS_EGRESS**

```
[406, ctcQinQSwitchStructSet]  ifIndex 272670747,  SnmpPort 47,cmd  50447

Enter Ingress VLAN Trans:
qinq->ulIfindex,	272670747
qinq->ulDefaultCvlan,	0
qinq->ulLocalPriority,	0
first_vlan_in_celist,	-1
qinq->ulCeVlanCos,	0
first_vlan_in_pelist,	100
qinq->ulPeVlanCos,	0
qinq->ulOuterVlanId,	200
qinq->ulOuterVlanCos,	9
qinq->ulInnerVlanId,	0
qinq->ulInnerVlanCos,	0
qinq->ulInnerMode,	1
qinq->ulOuterMode,	2
qinq->ulTransFlag,	0x28
qinq->ulRuleId,	1

[2928, ctc_tms_vlan_mapping_ingress_set]  gport=25

[2816, _outer_translate_outer_ingress]  1:1

[2834, _outer_translate_outer_ingress]  100 -> 200

Para write to hardware:
old_svid 100
new_svid 200

[2838, _outer_translate_outer_ingress]  [Succeed]

[1663, _qinq_save_ingress_data]  Set succeed ,save data, Exit

[2840, _outer_translate_outer_ingress]  [Succeed]

[2841, _outer_translate_outer_ingress]  Set succeed ! ,Exit !

[2967, ctc_tms_vlan_mapping_ingress_set]  [Succeed]

[3010, ctc_tms_vlan_mapping_ingress_set]  end

[477, ctcQinQSwitchStructSet]  exit succeed ,[rv: 0]

[406, ctcQinQSwitchStructSet]  ifIndex 272670747,  SnmpPort 47,cmd  50448

Enter Egress VLAN Trans:
qinq->ulIfindex,	272670747
qinq->ulDefaultCvlan,	0
qinq->ulLocalPriority,	0
first_vlan_in_celist,	-1
qinq->ulCeVlanCos,	0
first_vlan_in_pelist,	200
qinq->ulPeVlanCos,	0
qinq->ulOuterVlanId,	100
qinq->ulOuterVlanCos,	0
qinq->ulInnerVlanId,	0
qinq->ulInnerVlanCos,	0
qinq->ulInnerMode,	1
qinq->ulOuterMode,	2
qinq->ulTransFlag,	0x28
qinq->ulRuleId,	1

[4576, ctc_tms_vlan_mapping_egress_set]  gport=25

[4578, ctc_tms_vlan_mapping_egress_set]  ulTransFlag=0x28

[4285, _outer_translate_outer_egress]  egress outer trans outer 

[4321, _outer_translate_outer_egress]  egress : 200 -> 100

Para write to hardware:
old_svid 200
new_svid 100

[4323, _outer_translate_outer_egress]  [Succeed]

[4189, _qinq_save_egress_data]  Set succeed ,save data, Exit

[4201, _qinq_save_egress_data]  snmp_port=47

[4324, _outer_translate_outer_egress]  [Succeed]

[4325, _outer_translate_outer_egress]  Set succeed ! ,Exit !

[4635, ctc_tms_vlan_mapping_egress_set]  end

[477, ctcQinQSwitchStructSet]  exit succeed ,[rv: 0]
```



### no switchport vlan-mapping both outer 100

**ROS_HW_SS_QINQ_VLAN_MAPPING_TRANS_INGRESS**
**ROS_HW_SS_QINQ_VLAN_MAPPING_TRANS_EGRESS**

```
[491, ctcQinQSwitchStructDel]  ifIndex 272670747,  cmd  50447

[3043, ctc_tms_vlan_mapping_ingress_del]  snmp_port: 47,  ulIfindex: 272670747, rule_id 1

[3047, ctc_tms_vlan_mapping_ingress_del]  gport=25

[3125, ctc_tms_vlan_mapping_ingress_del]  Delete: Match Outer-Vid ( 100 )!

[3174, ctc_tms_vlan_mapping_ingress_del]  qinq del succeed

[3191, ctc_tms_vlan_mapping_ingress_del]  exit succeed ,[rv: 0]

[535, ctcQinQSwitchStructDel]  exit succeed ,[rv: 0]

[491, ctcQinQSwitchStructDel]  ifIndex 272670747,  cmd  50448

[4668, ctc_tms_vlan_mapping_egress_del]  snmp_port: 47,  ulIfindex: 272670747, rule_id 1

[4684, ctc_tms_vlan_mapping_egress_del]  gport=25

[4712, ctc_tms_vlan_mapping_egress_del]  Delete: Match Outer-Vid ( 200 )!

[4757, ctc_tms_vlan_mapping_egress_del]  qinq del succeed

[4774, ctc_tms_vlan_mapping_egress_del]  exit succeed ,[rv: 0]

[535, ctcQinQSwitchStructDel]  exit succeed ,[rv: 0]
```



### switchport vlan-mapping both inner 100 add-outer 200 cos 3

**ROS_HW_SS_QINQ_VLAN_MAPPING_TRANS_INGRESS**
**ROS_HW_SS_QINQ_VLAN_MAPPING_COS_TRANS_EGRESS**

```
[406, ctcQinQSwitchStructSet]  ifIndex 272670747,  SnmpPort 47,cmd  50447

Enter Ingress VLAN Trans:
qinq->ulIfindex,	272670747
qinq->ulDefaultCvlan,	0
qinq->ulLocalPriority,	0
first_vlan_in_celist,	100
qinq->ulCeVlanCos,	0
first_vlan_in_pelist,	-1
qinq->ulPeVlanCos,	0
qinq->ulOuterVlanId,	200
qinq->ulOuterVlanCos,	3
qinq->ulInnerVlanId,	0
qinq->ulInnerVlanCos,	0
qinq->ulInnerMode,	1
qinq->ulOuterMode,	6
qinq->ulTransFlag,	0x62
qinq->ulRuleId,	1

[2928, ctc_tms_vlan_mapping_ingress_set]  gport=25

[2718, _inner_add_outer_ingress]  inner add outer cos is 3, transflag is 0x62

[2728, _inner_add_outer_ingress]  100 -> 200(3),100

Para write to hardware:
old_svid 100
new_cvid 100
new_svid 200
new_scos 3

[2740, _inner_add_outer_ingress]  [Succeed]

[1663, _qinq_save_ingress_data]  Set succeed ,save data, Exit

[2749, _inner_add_outer_ingress]  [Succeed]

[2751, _inner_add_outer_ingress]  Set succeed ! ,Exit !

[2988, ctc_tms_vlan_mapping_ingress_set]  [Succeed]

[3010, ctc_tms_vlan_mapping_ingress_set]  end

[477, ctcQinQSwitchStructSet]  exit succeed ,[rv: 0]

[406, ctcQinQSwitchStructSet]  ifIndex 272670747,  SnmpPort 47,cmd  50450

Enter Egress VLAN Trans:
qinq->ulIfindex,	272670747
qinq->ulDefaultCvlan,	0
qinq->ulLocalPriority,	0
first_vlan_in_celist,	100
qinq->ulCeVlanCos,	0
first_vlan_in_pelist,	200
qinq->ulPeVlanCos,	3
qinq->ulOuterVlanId,	0
qinq->ulOuterVlanCos,	0
qinq->ulInnerVlanId,	0
qinq->ulInnerVlanCos,	0
qinq->ulInnerMode,	1
qinq->ulOuterMode,	3
qinq->ulTransFlag,	0x1a
qinq->ulRuleId,	1

[4576, ctc_tms_vlan_mapping_egress_set]  gport=25

[4578, ctc_tms_vlan_mapping_egress_set]  ulTransFlag=0x1a

[4533, _not_support_check_egress]  not support match{vlan + vlan + cos}!

[4383, _inner_add_outer_egress]  egress ,inner add outer ,2:1 or 2:0 or 1:0

[4448, _inner_add_outer_egress]  vlan_mapping.old_svid 200

[4454, _inner_add_outer_egress]  egress : 200 , 100 -> 100(0)

[4455, _inner_add_outer_egress]  entry num 0:


Para write to hardware:
old_cvid 100
old_svid 200
new_svid 100

[4457, _inner_add_outer_egress]  [Succeed]

[4189, _qinq_save_egress_data]  Set succeed ,save data, Exit

[4201, _qinq_save_egress_data]  snmp_port=47

[4458, _inner_add_outer_egress]  [Succeed]

[4461, _inner_add_outer_egress]  Set ok ,Exit

[4635, ctc_tms_vlan_mapping_egress_set]  end

[477, ctcQinQSwitchStructSet]  exit succeed ,[rv: 0]
```



### no switchport vlan-mapping both add-outer 200 cos 3

**ROS_HW_SS_QINQ_VLAN_MAPPING_TRANS_INGRESS**
**ROS_HW_SS_QINQ_VLAN_MAPPING_COS_TRANS_EGRESS**

```
[491, ctcQinQSwitchStructDel]  ifIndex 272670747,  cmd  50447

[3043, ctc_tms_vlan_mapping_ingress_del]  snmp_port: 47,  ulIfindex: 272670747, rule_id 1

[3047, ctc_tms_vlan_mapping_ingress_del]  gport=25

[3125, ctc_tms_vlan_mapping_ingress_del]  Delete: Match Outer-Vid ( 100 )!

[3174, ctc_tms_vlan_mapping_ingress_del]  qinq del succeed

[3191, ctc_tms_vlan_mapping_ingress_del]  exit succeed ,[rv: 0]

[535, ctcQinQSwitchStructDel]  exit succeed ,[rv: 0]

[491, ctcQinQSwitchStructDel]  ifIndex 272670747,  cmd  50450

[4668, ctc_tms_vlan_mapping_egress_del]  snmp_port: 47,  ulIfindex: 272670747, rule_id 1

[4684, ctc_tms_vlan_mapping_egress_del]  gport=25

[4724, ctc_tms_vlan_mapping_egress_del]  Delete: Match Outer-Vid( 200 ) + Inner-Vid( 100 ) !

[4757, ctc_tms_vlan_mapping_egress_del]  qinq del succeed

[4774, ctc_tms_vlan_mapping_egress_del]  exit succeed ,[rv: 0]

[535, ctcQinQSwitchStructDel]  exit succeed ,[rv: 0]
```



### switchport vlan-mapping both inner 100 add-outer 200

**ROS_HW_SS_QINQ_VLAN_MAPPING_TRANS_INGRESS**
**ROS_HW_SS_QINQ_VLAN_MAPPING_TRANS_EGRESS**

```
[406, ctcQinQSwitchStructSet]  ifIndex 272670747,  SnmpPort 47,cmd  50447

Enter Ingress VLAN Trans:
qinq->ulIfindex,	272670747
qinq->ulDefaultCvlan,	0
qinq->ulLocalPriority,	0
first_vlan_in_celist,	100
qinq->ulCeVlanCos,	0
first_vlan_in_pelist,	-1
qinq->ulPeVlanCos,	0
qinq->ulOuterVlanId,	200
qinq->ulOuterVlanCos,	9
qinq->ulInnerVlanId,	0
qinq->ulInnerVlanCos,	0
qinq->ulInnerMode,	1
qinq->ulOuterMode,	6
qinq->ulTransFlag,	0x22
qinq->ulRuleId,	1

[2928, ctc_tms_vlan_mapping_ingress_set]  gport=25

[2718, _inner_add_outer_ingress]  inner add outer cos is 9, transflag is 0x22

[2735, _inner_add_outer_ingress]  100 -> 200,100

Para write to hardware:
old_svid 100
new_cvid 100
new_svid 200

[2740, _inner_add_outer_ingress]  [Succeed]

[1663, _qinq_save_ingress_data]  Set succeed ,save data, Exit

[2749, _inner_add_outer_ingress]  [Succeed]

[2751, _inner_add_outer_ingress]  Set succeed ! ,Exit !

[2988, ctc_tms_vlan_mapping_ingress_set]  [Succeed]

[3010, ctc_tms_vlan_mapping_ingress_set]  end

[477, ctcQinQSwitchStructSet]  exit succeed ,[rv: 0]

[406, ctcQinQSwitchStructSet]  ifIndex 272670747,  SnmpPort 47,cmd  50448

Enter Egress VLAN Trans:
qinq->ulIfindex,	272670747
qinq->ulDefaultCvlan,	0
qinq->ulLocalPriority,	0
first_vlan_in_celist,	100
qinq->ulCeVlanCos,	0
first_vlan_in_pelist,	200
qinq->ulPeVlanCos,	0
qinq->ulOuterVlanId,	0
qinq->ulOuterVlanCos,	0
qinq->ulInnerVlanId,	0
qinq->ulInnerVlanCos,	0
qinq->ulInnerMode,	1
qinq->ulOuterMode,	3
qinq->ulTransFlag,	0xa
qinq->ulRuleId,	1

[4576, ctc_tms_vlan_mapping_egress_set]  gport=25

[4578, ctc_tms_vlan_mapping_egress_set]  ulTransFlag=0xa

[4383, _inner_add_outer_egress]  egress ,inner add outer ,2:1 or 2:0 or 1:0

[4448, _inner_add_outer_egress]  vlan_mapping.old_svid 200

[4454, _inner_add_outer_egress]  egress : 200 , 100 -> 100(0)

[4455, _inner_add_outer_egress]  entry num 0:


Para write to hardware:
old_cvid 100
old_svid 200
new_svid 100

[4457, _inner_add_outer_egress]  [Succeed]

[4189, _qinq_save_egress_data]  Set succeed ,save data, Exit

[4201, _qinq_save_egress_data]  snmp_port=47

[4458, _inner_add_outer_egress]  [Succeed]

[4461, _inner_add_outer_egress]  Set ok ,Exit

[4635, ctc_tms_vlan_mapping_egress_set]  end

[477, ctcQinQSwitchStructSet]  exit succeed ,[rv: 0]
```



### no switchport vlan-mapping both add-outer 200

**ROS_HW_SS_QINQ_VLAN_MAPPING_TRANS_INGRESS**
**ROS_HW_SS_QINQ_VLAN_MAPPING_TRANS_EGRESS**

```
[491, ctcQinQSwitchStructDel]  ifIndex 272670747,  cmd  50447

[3043, ctc_tms_vlan_mapping_ingress_del]  snmp_port: 47,  ulIfindex: 272670747, rule_id 1

[3047, ctc_tms_vlan_mapping_ingress_del]  gport=25

[3125, ctc_tms_vlan_mapping_ingress_del]  Delete: Match Outer-Vid ( 100 )!

[3174, ctc_tms_vlan_mapping_ingress_del]  qinq del succeed

[3191, ctc_tms_vlan_mapping_ingress_del]  exit succeed ,[rv: 0]

[535, ctcQinQSwitchStructDel]  exit succeed ,[rv: 0]

[491, ctcQinQSwitchStructDel]  ifIndex 272670747,  cmd  50448

[4668, ctc_tms_vlan_mapping_egress_del]  snmp_port: 47,  ulIfindex: 272670747, rule_id 1

[4684, ctc_tms_vlan_mapping_egress_del]  gport=25

[4724, ctc_tms_vlan_mapping_egress_del]  Delete: Match Outer-Vid( 200 ) + Inner-Vid( 100 ) !

[4757, ctc_tms_vlan_mapping_egress_del]  qinq del succeed

[4774, ctc_tms_vlan_mapping_egress_del]  exit succeed ,[rv: 0]

[535, ctcQinQSwitchStructDel]  exit succeed ,[rv: 0]
```


### switchport qinq default-cvlan 100

与命令：`switchport vlan-mapping both inner 100 add-outer 200`配合使用。实现的效果是：
如果入接口如果是tag 100的，那就直接加200，如果入接口报文是untag的，也能转换成（200，100）的双层报文。

配合灵活qinq使用时，如果先配置了灵活qinq，那么先删除然后再把`ulDefaultCvlan`值指定为配置的`default-cvlan`再下发灵活qinq。

```
[1253, ctc_tms_qinq_port_vlan_change_callback]  Exit

[1160, ctc_tms_qinq_pvid_change_callback]  entry! snmpPort=47,pPort->dot1qEnable= 0

[1164, ctc_tms_qinq_pvid_change_callback]  no need to set

[491, ctcQinQSwitchStructDel]  ifIndex 272670747,  cmd  50447

[3044, ctc_tms_vlan_mapping_ingress_del]  snmp_port: 47,  ulIfindex: 272670747, rule_id 1

[3048, ctc_tms_vlan_mapping_ingress_del]  gport=25

[3126, ctc_tms_vlan_mapping_ingress_del]  Delete: Match Outer-Vid ( 100 )!

[3175, ctc_tms_vlan_mapping_ingress_del]  qinq del succeed

[3192, ctc_tms_vlan_mapping_ingress_del]  exit succeed ,[rv: 0]

[535, ctcQinQSwitchStructDel]  exit succeed ,[rv: 0]

[491, ctcQinQSwitchStructDel]  ifIndex 272670747,  cmd  50448

[4669, ctc_tms_vlan_mapping_egress_del]  snmp_port: 47,  ulIfindex: 272670747, rule_id 1

[4685, ctc_tms_vlan_mapping_egress_del]  gport=25

[4725, ctc_tms_vlan_mapping_egress_del]  Delete: Match Outer-Vid( 200 ) + Inner-Vid( 100 ) !

[4758, ctc_tms_vlan_mapping_egress_del]  qinq del succeed

[4775, ctc_tms_vlan_mapping_egress_del]  exit succeed ,[rv: 0]

[535, ctcQinQSwitchStructDel]  exit succeed ,[rv: 0]

[406, ctcQinQSwitchStructSet]  ifIndex 272670747,  SnmpPort 47,cmd  50447

Enter Ingress VLAN Trans:
qinq->ulIfindex,	272670747
qinq->ulDefaultCvlan,	100
qinq->ulLocalPriority,	0
first_vlan_in_celist,	100
qinq->ulCeVlanCos,	0
first_vlan_in_pelist,	-1
qinq->ulPeVlanCos,	0
qinq->ulOuterVlanId,	200
qinq->ulOuterVlanCos,	9
qinq->ulInnerVlanId,	0
qinq->ulInnerVlanCos,	0
qinq->ulInnerMode,	1
qinq->ulOuterMode,	6
qinq->ulTransFlag,	0x23
qinq->ulRuleId,	1

[2929, ctc_tms_vlan_mapping_ingress_set]  gport=25

[2719, _inner_add_outer_ingress]  inner add outer cos is 9, transflag is 0x23

[2736, _inner_add_outer_ingress]  100 -> 200,100

Para write to hardware:
old_svid 100
new_cvid 100
new_svid 200

[2741, _inner_add_outer_ingress]  [Succeed]

[2211, _untag_add_both_ingress]  gport=25,ulSAddVid=200,ulScos=9,ulCAddVid=0,ulCcos=0

[2271, _untag_add_both_ingress]  exit succeed ,[rv: 0]

[2747, _inner_add_outer_ingress]  [Succeed]

[1663, _qinq_save_ingress_data]  Set succeed ,save data, Exit

[2750, _inner_add_outer_ingress]  [Succeed]

[2752, _inner_add_outer_ingress]  Set succeed ! ,Exit !

[2989, ctc_tms_vlan_mapping_ingress_set]  [Succeed]

[3011, ctc_tms_vlan_mapping_ingress_set]  end

[477, ctcQinQSwitchStructSet]  exit succeed ,[rv: 0]

[406, ctcQinQSwitchStructSet]  ifIndex 272670747,  SnmpPort 47,cmd  50448

Enter Egress VLAN Trans:
qinq->ulIfindex,	272670747
qinq->ulDefaultCvlan,	0
qinq->ulLocalPriority,	0
first_vlan_in_celist,	100
qinq->ulCeVlanCos,	0
first_vlan_in_pelist,	200
qinq->ulPeVlanCos,	0
qinq->ulOuterVlanId,	0
qinq->ulOuterVlanCos,	0
qinq->ulInnerVlanId,	0
qinq->ulInnerVlanCos,	0
qinq->ulInnerMode,	3
qinq->ulOuterMode,	3
qinq->ulTransFlag,	0xa
qinq->ulRuleId,	32769

[4577, ctc_tms_vlan_mapping_egress_set]  gport=25

[4579, ctc_tms_vlan_mapping_egress_set]  ulTransFlag=0xa

[4384, _inner_add_outer_egress]  egress ,inner add outer ,2:1 or 2:0 or 1:0

[4449, _inner_add_outer_egress]  vlan_mapping.old_svid 200

[4455, _inner_add_outer_egress]  egress : 200 , 100 -> 0(0)

[4456, _inner_add_outer_egress]  entry num 0:


Para write to hardware:
old_cvid 100
old_svid 200

[4458, _inner_add_outer_egress]  [Succeed]

[4190, _qinq_save_egress_data]  Set succeed ,save data, Exit

[4202, _qinq_save_egress_data]  snmp_port=47

[4459, _inner_add_outer_egress]  [Succeed]

[4462, _inner_add_outer_egress]  Set ok ,Exit

[4636, ctc_tms_vlan_mapping_egress_set]  end

[477, ctcQinQSwitchStructSet]  exit succeed ,[rv: 0]
```

### switchport vlan-mapping both priority-tagged cos 1 add-outer 200

**ROS_HW_SS_QINQ_VLAN_MAPPING_COS_TRANS_INGRESS**
**ROS_HW_SS_QINQ_VLAN_MAPPING_TRANS_EGRESS**

```
[406, ctcQinQSwitchStructSet]  ifIndex 272670747,  SnmpPort 47,cmd  50449

Enter Ingress VLAN Trans:
qinq->ulIfindex,	272670747
qinq->ulDefaultCvlan,	0
qinq->ulLocalPriority,	0
qinq->ulCeVlanCos,	1
first_vlan_in_pelist,	-1
qinq->ulPeVlanCos,	0
qinq->ulOuterVlanId,	200
qinq->ulOuterVlanCos,	9
qinq->ulInnerVlanId,	0
qinq->ulInnerVlanCos,	0
qinq->ulInnerMode,	1
qinq->ulOuterMode,	6
qinq->ulTransFlag,	0x26
qinq->ulRuleId,	1

[2928, ctc_tms_vlan_mapping_ingress_set]  gport=25

[1883, _pritag_add_ingress]  gport=25

[1932, _pritag_add_ingress]  ptag add remove [0 -> 200,0]

[1941, _pritag_add_ingress]  ptag add outer cos is 9, transflag is 0x26

[1997, _pritag_add_ingress]  succeed: create scl succeed! eid 0

[2009, _pritag_add_ingress]  succeed: create scl succeed! ipv6_eid 1

[1730, _qinq_save_tcam_ingress_data]  Set succeed ,save data [eid 0] [ipv6_eid 1] [id 1], Exit

[2013, _pritag_add_ingress]  [Succeed]

[2016, _pritag_add_ingress]  exit succeed ,[rv: 0]

[2947, ctc_tms_vlan_mapping_ingress_set]  [Succeed]

[3010, ctc_tms_vlan_mapping_ingress_set]  end

[477, ctcQinQSwitchStructSet]  exit succeed ,[rv: 0]

[406, ctcQinQSwitchStructSet]  ifIndex 272670747,  SnmpPort 47,cmd  50448

Enter Egress VLAN Trans:
qinq->ulIfindex,	272670747
qinq->ulDefaultCvlan,	0
qinq->ulLocalPriority,	0
qinq->ulCeVlanCos,	0
first_vlan_in_pelist,	200
qinq->ulPeVlanCos,	0
qinq->ulOuterVlanId,	0
qinq->ulOuterVlanCos,	0
qinq->ulInnerVlanId,	0
qinq->ulInnerVlanCos,	0
qinq->ulInnerMode,	1
qinq->ulOuterMode,	3
qinq->ulTransFlag,	0xa
qinq->ulRuleId,	1

[4576, ctc_tms_vlan_mapping_egress_set]  gport=25

[4578, ctc_tms_vlan_mapping_egress_set]  ulTransFlag=0xa

[4383, _inner_add_outer_egress]  egress ,inner add outer ,2:1 or 2:0 or 1:0

[4448, _inner_add_outer_egress]  vlan_mapping.old_svid 200

[4454, _inner_add_outer_egress]  egress : 200 , 0 -> 0(0)

[4455, _inner_add_outer_egress]  entry num 0:


Para write to hardware:
old_svid 200

[4457, _inner_add_outer_egress]  [Succeed]

[4189, _qinq_save_egress_data]  Set succeed ,save data, Exit

[4201, _qinq_save_egress_data]  snmp_port=47

[4458, _inner_add_outer_egress]  [Succeed]

[4461, _inner_add_outer_egress]  Set ok ,Exit

[4635, ctc_tms_vlan_mapping_egress_set]  end

[477, ctcQinQSwitchStructSet]  exit succeed ,[rv: 0]
```



### no switchport vlan-mapping both  priority-tagged cos 1

**ROS_HW_SS_QINQ_VLAN_MAPPING_COS_TRANS_INGRESS**
**ROS_HW_SS_QINQ_VLAN_MAPPING_TRANS_EGRESS**

```
[491, ctcQinQSwitchStructDel]  ifIndex 272670747,  cmd  50449

[3043, ctc_tms_vlan_mapping_ingress_del]  snmp_port: 47,  ulIfindex: 272670747, rule_id 1

[3047, ctc_tms_vlan_mapping_ingress_del]  gport=25

[3174, ctc_tms_vlan_mapping_ingress_del]  qinq del succeed

[3191, ctc_tms_vlan_mapping_ingress_del]  exit succeed ,[rv: 0]

[535, ctcQinQSwitchStructDel]  exit succeed ,[rv: 0]

[491, ctcQinQSwitchStructDel]  ifIndex 272670747,  cmd  50448

[4668, ctc_tms_vlan_mapping_egress_del]  snmp_port: 47,  ulIfindex: 272670747, rule_id 1

[4684, ctc_tms_vlan_mapping_egress_del]  gport=25

[4724, ctc_tms_vlan_mapping_egress_del]  Delete: Match Outer-Vid( 200 ) + Inner-Vid( 0 ) !

[4757, ctc_tms_vlan_mapping_egress_del]  qinq del succeed

[4774, ctc_tms_vlan_mapping_egress_del]  exit succeed ,[rv: 0]

[535, ctcQinQSwitchStructDel]  exit succeed ,[rv: 0]
```



### switchport vlan-mapping both priority-tagged  add-outer 200

**ROS_HW_SS_QINQ_VLAN_MAPPING_TRANS_INGRESS**
**ROS_HW_SS_QINQ_VLAN_MAPPING_TRANS_EGRESS**

```
[406, ctcQinQSwitchStructSet]  ifIndex 272670747,  SnmpPort 47,cmd  50447

Enter Ingress VLAN Trans:
qinq->ulIfindex,	272670747
qinq->ulDefaultCvlan,	0
qinq->ulLocalPriority,	0
qinq->ulCeVlanCos,	0
first_vlan_in_pelist,	-1
qinq->ulPeVlanCos,	0
qinq->ulOuterVlanId,	200
qinq->ulOuterVlanCos,	9
qinq->ulInnerVlanId,	0
qinq->ulInnerVlanCos,	0
qinq->ulInnerMode,	1
qinq->ulOuterMode,	6
qinq->ulTransFlag,	0x22
qinq->ulRuleId,	1

[2928, ctc_tms_vlan_mapping_ingress_set]  gport=25

[1883, _pritag_add_ingress]  gport=25

[1932, _pritag_add_ingress]  ptag add remove [0 -> 200,0]

[1941, _pritag_add_ingress]  ptag add outer cos is 9, transflag is 0x22

[1997, _pritag_add_ingress]  succeed: create scl succeed! eid 0

[2009, _pritag_add_ingress]  succeed: create scl succeed! ipv6_eid 1

[1730, _qinq_save_tcam_ingress_data]  Set succeed ,save data [eid 0] [ipv6_eid 1] [id 1], Exit

[2013, _pritag_add_ingress]  [Succeed]

[2016, _pritag_add_ingress]  exit succeed ,[rv: 0]

[2947, ctc_tms_vlan_mapping_ingress_set]  [Succeed]

[3010, ctc_tms_vlan_mapping_ingress_set]  end

[477, ctcQinQSwitchStructSet]  exit succeed ,[rv: 0]

[406, ctcQinQSwitchStructSet]  ifIndex 272670747,  SnmpPort 47,cmd  50448

Enter Egress VLAN Trans:
qinq->ulIfindex,	272670747
qinq->ulDefaultCvlan,	0
qinq->ulLocalPriority,	0
qinq->ulCeVlanCos,	0
first_vlan_in_pelist,	200
qinq->ulPeVlanCos,	0
qinq->ulOuterVlanId,	0
qinq->ulOuterVlanCos,	0
qinq->ulInnerVlanId,	0
qinq->ulInnerVlanCos,	0
qinq->ulInnerMode,	1
qinq->ulOuterMode,	3
qinq->ulTransFlag,	0xa
qinq->ulRuleId,	1

[4576, ctc_tms_vlan_mapping_egress_set]  gport=25

[4578, ctc_tms_vlan_mapping_egress_set]  ulTransFlag=0xa

[4383, _inner_add_outer_egress]  egress ,inner add outer ,2:1 or 2:0 or 1:0

[4448, _inner_add_outer_egress]  vlan_mapping.old_svid 200

[4454, _inner_add_outer_egress]  egress : 200 , 0 -> 0(0)

[4455, _inner_add_outer_egress]  entry num 0:


Para write to hardware:
old_svid 200

[4457, _inner_add_outer_egress]  [Succeed]

[4189, _qinq_save_egress_data]  Set succeed ,save data, Exit

[4201, _qinq_save_egress_data]  snmp_port=47

[4458, _inner_add_outer_egress]  [Succeed]

[4461, _inner_add_outer_egress]  Set ok ,Exit

[4635, ctc_tms_vlan_mapping_egress_set]  end

[477, ctcQinQSwitchStructSet]  exit succeed ,[rv: 0]

```


### no switchport vlan-mapping both  priority-tagged

**ROS_HW_SS_QINQ_VLAN_MAPPING_TRANS_INGRESS**
**ROS_HW_SS_QINQ_VLAN_MAPPING_TRANS_EGRESS**

```
[491, ctcQinQSwitchStructDel]  ifIndex 272670747,  cmd  50447

[3043, ctc_tms_vlan_mapping_ingress_del]  snmp_port: 47,  ulIfindex: 272670747, rule_id 1

[3047, ctc_tms_vlan_mapping_ingress_del]  gport=25

[3174, ctc_tms_vlan_mapping_ingress_del]  qinq del succeed

[3191, ctc_tms_vlan_mapping_ingress_del]  exit succeed ,[rv: 0]

[535, ctcQinQSwitchStructDel]  exit succeed ,[rv: 0]

[491, ctcQinQSwitchStructDel]  ifIndex 272670747,  cmd  50448

[4668, ctc_tms_vlan_mapping_egress_del]  snmp_port: 47,  ulIfindex: 272670747, rule_id 1

[4684, ctc_tms_vlan_mapping_egress_del]  gport=25

[4724, ctc_tms_vlan_mapping_egress_del]  Delete: Match Outer-Vid( 200 ) + Inner-Vid( 0 ) !

[4757, ctc_tms_vlan_mapping_egress_del]  qinq del succeed

[4774, ctc_tms_vlan_mapping_egress_del]  exit succeed ,[rv: 0]

[535, ctcQinQSwitchStructDel]  exit succeed ,[rv: 0]
```



### switchport vlan-mapping both priority-tagged cos 1 add-outer 100  translate 200

**ROS_HW_SS_QINQ_VLAN_MAPPING_COS_TRANS_INGRESS**
**ROS_HW_SS_QINQ_VLAN_MAPPING_TRANS_EGRESS**

```
[406, ctcQinQSwitchStructSet]  ifIndex 272670747,  SnmpPort 47,cmd  50449

Enter Ingress VLAN Trans:
qinq->ulIfindex,	272670747
qinq->ulDefaultCvlan,	0
qinq->ulLocalPriority,	0
qinq->ulCeVlanCos,	1
first_vlan_in_pelist,	-1
qinq->ulPeVlanCos,	0
qinq->ulOuterVlanId,	100
qinq->ulOuterVlanCos,	9
qinq->ulInnerVlanId,	200
qinq->ulInnerVlanCos,	0
qinq->ulInnerMode,	2
qinq->ulOuterMode,	6
qinq->ulTransFlag,	0xa6
qinq->ulRuleId,	1

[2928, ctc_tms_vlan_mapping_ingress_set]  gport=25

[1883, _pritag_add_ingress]  gport=25

[1914, _pritag_add_ingress]  ptag add trans [0 -> 100,200]

[1941, _pritag_add_ingress]  ptag add outer cos is 9, transflag is 0xa6

[1997, _pritag_add_ingress]  succeed: create scl succeed! eid 0

[2009, _pritag_add_ingress]  succeed: create scl succeed! ipv6_eid 1

[1730, _qinq_save_tcam_ingress_data]  Set succeed ,save data [eid 0] [ipv6_eid 1] [id 1], Exit

[2013, _pritag_add_ingress]  [Succeed]

[2016, _pritag_add_ingress]  exit succeed ,[rv: 0]

[2947, ctc_tms_vlan_mapping_ingress_set]  [Succeed]

[3010, ctc_tms_vlan_mapping_ingress_set]  end

[477, ctcQinQSwitchStructSet]  exit succeed ,[rv: 0]

[406, ctcQinQSwitchStructSet]  ifIndex 272670747,  SnmpPort 47,cmd  50448

Enter Egress VLAN Trans:
qinq->ulIfindex,	272670747
qinq->ulDefaultCvlan,	0
qinq->ulLocalPriority,	0
first_vlan_in_celist,	200
qinq->ulCeVlanCos,	0
first_vlan_in_pelist,	100
qinq->ulPeVlanCos,	0
qinq->ulOuterVlanId,	0
qinq->ulOuterVlanCos,	0
qinq->ulInnerVlanId,	0
qinq->ulInnerVlanCos,	1
qinq->ulInnerMode,	2
qinq->ulOuterMode,	3
qinq->ulTransFlag,	0x18a
qinq->ulRuleId,	1

[4576, ctc_tms_vlan_mapping_egress_set]  gport=25

[4578, ctc_tms_vlan_mapping_egress_set]  ulTransFlag=0x18a

[4335, _cvlan_add_trans_egress]  Enter ! [1:2], c add trans

[4366, _cvlan_add_trans_egress]  egress : 100 , 200 -> 0(0)

Para write to hardware:
old_cvid 200
old_svid 100

[4368, _cvlan_add_trans_egress]  [Succeed]

[4189, _qinq_save_egress_data]  Set succeed ,save data, Exit

[4201, _qinq_save_egress_data]  snmp_port=47

[4369, _cvlan_add_trans_egress]  [Succeed]

[4370, _cvlan_add_trans_egress]  Set succeed ! ,Exit !

[4635, ctc_tms_vlan_mapping_egress_set]  end

[477, ctcQinQSwitchStructSet]  exit succeed ,[rv: 0]
```



### no switchport vlan-mapping both priority-tagged cos 1

**ROS_HW_SS_QINQ_VLAN_MAPPING_COS_TRANS_INGRESS**
**ROS_HW_SS_QINQ_VLAN_MAPPING_TRANS_EGRESS**

```
[491, ctcQinQSwitchStructDel]  ifIndex 272670747,  cmd  50449

[3043, ctc_tms_vlan_mapping_ingress_del]  snmp_port: 47,  ulIfindex: 272670747, rule_id 1

[3047, ctc_tms_vlan_mapping_ingress_del]  gport=25

[3174, ctc_tms_vlan_mapping_ingress_del]  qinq del succeed

[3191, ctc_tms_vlan_mapping_ingress_del]  exit succeed ,[rv: 0]

[535, ctcQinQSwitchStructDel]  exit succeed ,[rv: 0]

[491, ctcQinQSwitchStructDel]  ifIndex 272670747,  cmd  50448

[4668, ctc_tms_vlan_mapping_egress_del]  snmp_port: 47,  ulIfindex: 272670747, rule_id 1

[4684, ctc_tms_vlan_mapping_egress_del]  gport=25

[4724, ctc_tms_vlan_mapping_egress_del]  Delete: Match Outer-Vid( 100 ) + Inner-Vid( 200 ) !

[4757, ctc_tms_vlan_mapping_egress_del]  qinq del succeed

[4774, ctc_tms_vlan_mapping_egress_del]  exit succeed ,[rv: 0]

[535, ctcQinQSwitchStructDel]  exit succeed ,[rv: 0]
```


### switchport vlan-mapping both priority-tagged cos 1 add-outer 100  remove

**ROS_HW_SS_QINQ_VLAN_MAPPING_COS_TRANS_INGRESS**
**ROS_HW_SS_QINQ_VLAN_MAPPING_TRANS_EGRESS**

```
[406, ctcQinQSwitchStructSet]  ifIndex 272670747,  SnmpPort 47,cmd  50449

Enter Ingress VLAN Trans:
qinq->ulIfindex,	272670747
qinq->ulDefaultCvlan,	0
qinq->ulLocalPriority,	0
qinq->ulCeVlanCos,	1
first_vlan_in_pelist,	-1
qinq->ulPeVlanCos,	0
qinq->ulOuterVlanId,	100
qinq->ulOuterVlanCos,	9
qinq->ulInnerVlanId,	0
qinq->ulInnerVlanCos,	0
qinq->ulInnerMode,	3
qinq->ulOuterMode,	6
qinq->ulTransFlag,	0x26
qinq->ulRuleId,	1

[2928, ctc_tms_vlan_mapping_ingress_set]  gport=25

[1883, _pritag_add_ingress]  gport=25

[1896, _pritag_add_ingress]  ptag add remove [0 -> 100]

[1941, _pritag_add_ingress]  ptag add outer cos is 9, transflag is 0x26

[1997, _pritag_add_ingress]  succeed: create scl succeed! eid 0

[2009, _pritag_add_ingress]  succeed: create scl succeed! ipv6_eid 1

[1730, _qinq_save_tcam_ingress_data]  Set succeed ,save data [eid 0] [ipv6_eid 1] [id 1], Exit

[2013, _pritag_add_ingress]  [Succeed]

[2016, _pritag_add_ingress]  exit succeed ,[rv: 0]

[2947, ctc_tms_vlan_mapping_ingress_set]  [Succeed]

[3010, ctc_tms_vlan_mapping_ingress_set]  end

[477, ctcQinQSwitchStructSet]  exit succeed ,[rv: 0]

[406, ctcQinQSwitchStructSet]  ifIndex 272670747,  SnmpPort 47,cmd  50448

Enter Egress VLAN Trans:
qinq->ulIfindex,	272670747
qinq->ulDefaultCvlan,	0
qinq->ulLocalPriority,	0
first_vlan_in_celist,	-1
qinq->ulCeVlanCos,	0
first_vlan_in_pelist,	100
qinq->ulPeVlanCos,	0
qinq->ulOuterVlanId,	0
qinq->ulOuterVlanCos,	0
qinq->ulInnerVlanId,	0
qinq->ulInnerVlanCos,	1
qinq->ulInnerMode,	6
qinq->ulOuterMode,	3
qinq->ulTransFlag,	0x188
qinq->ulRuleId,	1

[4576, ctc_tms_vlan_mapping_egress_set]  gport=25

[4578, ctc_tms_vlan_mapping_egress_set]  ulTransFlag=0x188

[4243, _cvlan_add_remove_egress]  [1:1] cvlan add remove egress 

Para write to hardware:
old_svid 100

[4273, _cvlan_add_remove_egress]  [Succeed]

[4189, _qinq_save_egress_data]  Set succeed ,save data, Exit

[4201, _qinq_save_egress_data]  snmp_port=47

[4274, _cvlan_add_remove_egress]  [Succeed]

[4275, _cvlan_add_remove_egress]  Set succeed ! ,Exit !

[4635, ctc_tms_vlan_mapping_egress_set]  end

[477, ctcQinQSwitchStructSet]  exit succeed ,[rv: 0]
```


### no switchport vlan-mapping both priority-tagged cos 1

**ROS_HW_SS_QINQ_VLAN_MAPPING_COS_TRANS_INGRESS**
**ROS_HW_SS_QINQ_VLAN_MAPPING_TRANS_EGRESS**

```
[491, ctcQinQSwitchStructDel]  ifIndex 272670747,  cmd  50449

[3043, ctc_tms_vlan_mapping_ingress_del]  snmp_port: 47,  ulIfindex: 272670747, rule_id 1

[3047, ctc_tms_vlan_mapping_ingress_del]  gport=25

[3174, ctc_tms_vlan_mapping_ingress_del]  qinq del succeed

[3191, ctc_tms_vlan_mapping_ingress_del]  exit succeed ,[rv: 0]

[535, ctcQinQSwitchStructDel]  exit succeed ,[rv: 0]

[491, ctcQinQSwitchStructDel]  ifIndex 272670747,  cmd  50448

[4668, ctc_tms_vlan_mapping_egress_del]  snmp_port: 47,  ulIfindex: 272670747, rule_id 1

[4684, ctc_tms_vlan_mapping_egress_del]  gport=25

[4712, ctc_tms_vlan_mapping_egress_del]  Delete: Match Outer-Vid ( 100 )!

[4757, ctc_tms_vlan_mapping_egress_del]  qinq del succeed

[4774, ctc_tms_vlan_mapping_egress_del]  exit succeed ,[rv: 0]

[535, ctcQinQSwitchStructDel]  exit succeed ,[rv: 0]
```



### switchport vlan-mapping both cvlan 100  add-outer 200 cos 3 translate 300

**ROS_HW_SS_QINQ_VLAN_MAPPING_TRANS_INGRESS**
**ROS_HW_SS_QINQ_VLAN_MAPPING_COS_TRANS_EGRESS**

```
[406, ctcQinQSwitchStructSet]  ifIndex 272670747,  SnmpPort 47,cmd  50447

Enter Ingress VLAN Trans:
qinq->ulIfindex,	272670747
qinq->ulDefaultCvlan,	0
qinq->ulLocalPriority,	0
first_vlan_in_celist,	100
qinq->ulCeVlanCos,	0
first_vlan_in_pelist,	-1
qinq->ulPeVlanCos,	0
qinq->ulOuterVlanId,	200
qinq->ulOuterVlanCos,	3
qinq->ulInnerVlanId,	300
qinq->ulInnerVlanCos,	0
qinq->ulInnerMode,	2
qinq->ulOuterMode,	6
qinq->ulTransFlag,	0xe2
qinq->ulRuleId,	1

[2928, ctc_tms_vlan_mapping_ingress_set]  gport=25

[2142, _cvlan_add_ingress]  cvlan cos add outer cos is 3, transflag is 0xe2

[2152, _cvlan_add_ingress]  100 -> 200(3)

Para write to hardware:
old_svid 100
new_cvid 300
new_svid 200
new_scos 3

[2187, _cvlan_add_ingress]  [Succeed]

[1663, _qinq_save_ingress_data]  Set succeed ,save data, Exit

[2189, _cvlan_add_ingress]  [Succeed]

[2190, _cvlan_add_ingress]  Set succeed ! ,Exit !

[2980, ctc_tms_vlan_mapping_ingress_set]  [Succeed]

[3010, ctc_tms_vlan_mapping_ingress_set]  end

[477, ctcQinQSwitchStructSet]  exit succeed ,[rv: 0]

[406, ctcQinQSwitchStructSet]  ifIndex 272670747,  SnmpPort 47,cmd  50450

Enter Egress VLAN Trans:
qinq->ulIfindex,	272670747
qinq->ulDefaultCvlan,	0
qinq->ulLocalPriority,	0
first_vlan_in_celist,	300
qinq->ulCeVlanCos,	0
first_vlan_in_pelist,	200
qinq->ulPeVlanCos,	3
qinq->ulOuterVlanId,	0
qinq->ulOuterVlanCos,	0
qinq->ulInnerVlanId,	100
qinq->ulInnerVlanCos,	0
qinq->ulInnerMode,	2
qinq->ulOuterMode,	3
qinq->ulTransFlag,	0x9a
qinq->ulRuleId,	1

[4576, ctc_tms_vlan_mapping_egress_set]  gport=25

[4578, ctc_tms_vlan_mapping_egress_set]  ulTransFlag=0x9a

[4521, _not_support_check_egress]  not support match{vlan + vlan + cos}!

[4335, _cvlan_add_trans_egress]  Enter ! [1:2], c add trans

[4366, _cvlan_add_trans_egress]  egress : 200 , 300 -> 100(0)

Para write to hardware:
old_cvid 300
old_svid 200
new_svid 100

[4368, _cvlan_add_trans_egress]  [Succeed]

[4189, _qinq_save_egress_data]  Set succeed ,save data, Exit

[4201, _qinq_save_egress_data]  snmp_port=47

[4369, _cvlan_add_trans_egress]  [Succeed]

[4370, _cvlan_add_trans_egress]  Set succeed ! ,Exit !

[4635, ctc_tms_vlan_mapping_egress_set]  end

[477, ctcQinQSwitchStructSet]  exit succeed ,[rv: 0]
```



### no switchport vlan-mapping both cvlan 100

**ROS_HW_SS_QINQ_VLAN_MAPPING_TRANS_INGRESS**
**ROS_HW_SS_QINQ_VLAN_MAPPING_COS_TRANS_EGRESS**

```
[491, ctcQinQSwitchStructDel]  ifIndex 272670747,  cmd  50447

[3043, ctc_tms_vlan_mapping_ingress_del]  snmp_port: 47,  ulIfindex: 272670747, rule_id 1

[3047, ctc_tms_vlan_mapping_ingress_del]  gport=25

[3125, ctc_tms_vlan_mapping_ingress_del]  Delete: Match Outer-Vid ( 100 )!

[3174, ctc_tms_vlan_mapping_ingress_del]  qinq del succeed

[3191, ctc_tms_vlan_mapping_ingress_del]  exit succeed ,[rv: 0]

[535, ctcQinQSwitchStructDel]  exit succeed ,[rv: 0]

[491, ctcQinQSwitchStructDel]  ifIndex 272670747,  cmd  50450

[4668, ctc_tms_vlan_mapping_egress_del]  snmp_port: 47,  ulIfindex: 272670747, rule_id 1

[4684, ctc_tms_vlan_mapping_egress_del]  gport=25

[4724, ctc_tms_vlan_mapping_egress_del]  Delete: Match Outer-Vid( 200 ) + Inner-Vid( 300 ) !

[4757, ctc_tms_vlan_mapping_egress_del]  qinq del succeed

[4774, ctc_tms_vlan_mapping_egress_del]  exit succeed ,[rv: 0]

[535, ctcQinQSwitchStructDel]  exit succeed ,[rv: 0]
```



### switchport vlan-mapping both cvlan 100 cos 2 add-outer 200 cos 3 translate 300

**ROS_HW_SS_QINQ_VLAN_MAPPING_COS_TRANS_INGRESS**
**ROS_HW_SS_QINQ_VLAN_MAPPING_COS_TRANS_EGRESS**

```
[406, ctcQinQSwitchStructSet]  ifIndex 272670747,  SnmpPort 47,cmd  50449

Enter Ingress VLAN Trans:
qinq->ulIfindex,	272670747
qinq->ulDefaultCvlan,	0
qinq->ulLocalPriority,	0
first_vlan_in_celist,	100
qinq->ulCeVlanCos,	2
first_vlan_in_pelist,	-1
qinq->ulPeVlanCos,	0
qinq->ulOuterVlanId,	200
qinq->ulOuterVlanCos,	3
qinq->ulInnerVlanId,	300
qinq->ulInnerVlanCos,	0
qinq->ulInnerMode,	2
qinq->ulOuterMode,	6
qinq->ulTransFlag,	0xe6
qinq->ulRuleId,	1

[2928, ctc_tms_vlan_mapping_ingress_set]  gport=25

[2031, _cvlan_cos_add_ingress]  gport=25

[2067, _cvlan_cos_add_ingress]  ctag_cos add trans [100 -> 200,300]

[2099, _cvlan_cos_add_ingress]  succeed: create scl succeed! eid 0

[2111, _cvlan_cos_add_ingress]  succeed: create scl ipv6 succeed! ipv6_eid 1

[1730, _qinq_save_tcam_ingress_data]  Set succeed ,save data [eid 0] [ipv6_eid 1] [id 1], Exit

[2114, _cvlan_cos_add_ingress]  [Succeed]

[2117, _cvlan_cos_add_ingress]  exit succeed ,[rv: 0]

[2976, ctc_tms_vlan_mapping_ingress_set]  [Succeed]

[3010, ctc_tms_vlan_mapping_ingress_set]  end

[477, ctcQinQSwitchStructSet]  exit succeed ,[rv: 0]

[406, ctcQinQSwitchStructSet]  ifIndex 272670747,  SnmpPort 47,cmd  50450

Enter Egress VLAN Trans:
qinq->ulIfindex,	272670747
qinq->ulDefaultCvlan,	0
qinq->ulLocalPriority,	0
first_vlan_in_celist,	300
qinq->ulCeVlanCos,	0
first_vlan_in_pelist,	200
qinq->ulPeVlanCos,	3
qinq->ulOuterVlanId,	0
qinq->ulOuterVlanCos,	0
qinq->ulInnerVlanId,	100
qinq->ulInnerVlanCos,	2
qinq->ulInnerMode,	2
qinq->ulOuterMode,	3
qinq->ulTransFlag,	0x19a
qinq->ulRuleId,	1

[4576, ctc_tms_vlan_mapping_egress_set]  gport=25

[4578, ctc_tms_vlan_mapping_egress_set]  ulTransFlag=0x19a

[4521, _not_support_check_egress]  not support match{vlan + vlan + cos}!

[4335, _cvlan_add_trans_egress]  Enter ! [1:2], c add trans

[4366, _cvlan_add_trans_egress]  egress : 200 , 300 -> 100(2)

Para write to hardware:
old_cvid 300
old_svid 200
new_svid 100
new_scos 2

[4368, _cvlan_add_trans_egress]  [Succeed]

[4189, _qinq_save_egress_data]  Set succeed ,save data, Exit

[4201, _qinq_save_egress_data]  snmp_port=47

[4369, _cvlan_add_trans_egress]  [Succeed]

[4370, _cvlan_add_trans_egress]  Set succeed ! ,Exit !

[4635, ctc_tms_vlan_mapping_egress_set]  end

[477, ctcQinQSwitchStructSet]  exit succeed ,[rv: 0]
```


### no switchport vlan-mapping both cvlan 100 cos 2

**ROS_HW_SS_QINQ_VLAN_MAPPING_COS_TRANS_INGRESS**
**ROS_HW_SS_QINQ_VLAN_MAPPING_COS_TRANS_EGRESS**

```
[491, ctcQinQSwitchStructDel]  ifIndex 272670747,  cmd  50449

[3043, ctc_tms_vlan_mapping_ingress_del]  snmp_port: 47,  ulIfindex: 272670747, rule_id 1

[3047, ctc_tms_vlan_mapping_ingress_del]  gport=25

[3174, ctc_tms_vlan_mapping_ingress_del]  qinq del succeed

[3191, ctc_tms_vlan_mapping_ingress_del]  exit succeed ,[rv: 0]

[535, ctcQinQSwitchStructDel]  exit succeed ,[rv: 0]

[491, ctcQinQSwitchStructDel]  ifIndex 272670747,  cmd  50450

[4668, ctc_tms_vlan_mapping_egress_del]  snmp_port: 47,  ulIfindex: 272670747, rule_id 1

[4684, ctc_tms_vlan_mapping_egress_del]  gport=25

[4724, ctc_tms_vlan_mapping_egress_del]  Delete: Match Outer-Vid( 200 ) + Inner-Vid( 300 ) !

[4757, ctc_tms_vlan_mapping_egress_del]  qinq del succeed

[4774, ctc_tms_vlan_mapping_egress_del]  exit succeed ,[rv: 0]

[535, ctcQinQSwitchStructDel]  exit succeed ,[rv: 0]
```



### switchport vlan-mapping both cvlan 100  add-outer 200  remove

**ROS_HW_SS_QINQ_VLAN_MAPPING_TRANS_INGRESS**
**ROS_HW_SS_QINQ_VLAN_MAPPING_TRANS_EGRESS**

```
[406, ctcQinQSwitchStructSet]  ifIndex 272670747,  SnmpPort 47,cmd  50447

Enter Ingress VLAN Trans:
qinq->ulIfindex,	272670747
qinq->ulDefaultCvlan,	0
qinq->ulLocalPriority,	0
first_vlan_in_celist,	100
qinq->ulCeVlanCos,	0
first_vlan_in_pelist,	-1
qinq->ulPeVlanCos,	0
qinq->ulOuterVlanId,	200
qinq->ulOuterVlanCos,	9
qinq->ulInnerVlanId,	0
qinq->ulInnerVlanCos,	0
qinq->ulInnerMode,	3
qinq->ulOuterMode,	6
qinq->ulTransFlag,	0x22
qinq->ulRuleId,	1

[2928, ctc_tms_vlan_mapping_ingress_set]  gport=25

[2142, _cvlan_add_ingress]  cvlan cos add outer cos is 9, transflag is 0x22

[2158, _cvlan_add_ingress]  100 -> 200

Para write to hardware:
old_svid 100
new_svid 200

[2187, _cvlan_add_ingress]  [Succeed]

[1663, _qinq_save_ingress_data]  Set succeed ,save data, Exit

[2189, _cvlan_add_ingress]  [Succeed]

[2190, _cvlan_add_ingress]  Set succeed ! ,Exit !

[2960, ctc_tms_vlan_mapping_ingress_set]  [Succeed]

[3010, ctc_tms_vlan_mapping_ingress_set]  end

[477, ctcQinQSwitchStructSet]  exit succeed ,[rv: 0]

[406, ctcQinQSwitchStructSet]  ifIndex 272670747,  SnmpPort 47,cmd  50448

Enter Egress VLAN Trans:
qinq->ulIfindex,	272670747
qinq->ulDefaultCvlan,	0
qinq->ulLocalPriority,	0
first_vlan_in_celist,	-1
qinq->ulCeVlanCos,	0
first_vlan_in_pelist,	200
qinq->ulPeVlanCos,	0
qinq->ulOuterVlanId,	0
qinq->ulOuterVlanCos,	0
qinq->ulInnerVlanId,	100
qinq->ulInnerVlanCos,	0
qinq->ulInnerMode,	6
qinq->ulOuterMode,	3
qinq->ulTransFlag,	0x88
qinq->ulRuleId,	1

[4576, ctc_tms_vlan_mapping_egress_set]  gport=25

[4578, ctc_tms_vlan_mapping_egress_set]  ulTransFlag=0x88

[4243, _cvlan_add_remove_egress]  [1:1] cvlan add remove egress 

[4269, _cvlan_add_remove_egress]  egress set cos 0

Para write to hardware:
old_svid 200
new_svid 100

[4273, _cvlan_add_remove_egress]  [Succeed]

[4189, _qinq_save_egress_data]  Set succeed ,save data, Exit

[4201, _qinq_save_egress_data]  snmp_port=47

[4274, _cvlan_add_remove_egress]  [Succeed]

[4275, _cvlan_add_remove_egress]  Set succeed ! ,Exit !

[4635, ctc_tms_vlan_mapping_egress_set]  end

[477, ctcQinQSwitchStructSet]  exit succeed ,[rv: 0]
```



### no switchport vlan-mapping both cvlan 100

**ROS_HW_SS_QINQ_VLAN_MAPPING_TRANS_INGRESS**
**ROS_HW_SS_QINQ_VLAN_MAPPING_TRANS_EGRESS**

```
[491, ctcQinQSwitchStructDel]  ifIndex 272670747,  cmd  50447

[3043, ctc_tms_vlan_mapping_ingress_del]  snmp_port: 47,  ulIfindex: 272670747, rule_id 1

[3047, ctc_tms_vlan_mapping_ingress_del]  gport=25

[3125, ctc_tms_vlan_mapping_ingress_del]  Delete: Match Outer-Vid ( 100 )!

[3174, ctc_tms_vlan_mapping_ingress_del]  qinq del succeed

[3191, ctc_tms_vlan_mapping_ingress_del]  exit succeed ,[rv: 0]

[535, ctcQinQSwitchStructDel]  exit succeed ,[rv: 0]

[491, ctcQinQSwitchStructDel]  ifIndex 272670747,  cmd  50448

[4668, ctc_tms_vlan_mapping_egress_del]  snmp_port: 47,  ulIfindex: 272670747, rule_id 1

[4684, ctc_tms_vlan_mapping_egress_del]  gport=25

[4712, ctc_tms_vlan_mapping_egress_del]  Delete: Match Outer-Vid ( 200 )!

[4757, ctc_tms_vlan_mapping_egress_del]  qinq del succeed

[4774, ctc_tms_vlan_mapping_egress_del]  exit succeed ,[rv: 0]

[535, ctcQinQSwitchStructDel]  exit succeed ,[rv: 0]
```



### switchport vlan-mapping both cvlan 100 cos 2 add-outer 200 cos 3 remove

**ROS_HW_SS_QINQ_VLAN_MAPPING_COS_TRANS_INGRESS**
**ROS_HW_SS_QINQ_VLAN_MAPPING_COS_TRANS_EGRESS**

```
[406, ctcQinQSwitchStructSet]  ifIndex 272670747,  SnmpPort 47,cmd  50449

Enter Ingress VLAN Trans:
qinq->ulIfindex,	272670747
qinq->ulDefaultCvlan,	0
qinq->ulLocalPriority,	0
first_vlan_in_celist,	100
qinq->ulCeVlanCos,	2
first_vlan_in_pelist,	-1
qinq->ulPeVlanCos,	0
qinq->ulOuterVlanId,	200
qinq->ulOuterVlanCos,	3
qinq->ulInnerVlanId,	0
qinq->ulInnerVlanCos,	0
qinq->ulInnerMode,	3
qinq->ulOuterMode,	6
qinq->ulTransFlag,	0x66
qinq->ulRuleId,	1

[2928, ctc_tms_vlan_mapping_ingress_set]  gport=25

[2031, _cvlan_cos_add_ingress]  gport=25

[2055, _cvlan_cos_add_ingress]  ctag_cos add remove [100 -> 200]

[2099, _cvlan_cos_add_ingress]  succeed: create scl succeed! eid 0

[2111, _cvlan_cos_add_ingress]  succeed: create scl ipv6 succeed! ipv6_eid 1

[1730, _qinq_save_tcam_ingress_data]  Set succeed ,save data [eid 0] [ipv6_eid 1] [id 1], Exit

[2114, _cvlan_cos_add_ingress]  [Succeed]

[2117, _cvlan_cos_add_ingress]  exit succeed ,[rv: 0]

[2956, ctc_tms_vlan_mapping_ingress_set]  [Succeed]

[3010, ctc_tms_vlan_mapping_ingress_set]  end

[477, ctcQinQSwitchStructSet]  exit succeed ,[rv: 0]

[406, ctcQinQSwitchStructSet]  ifIndex 272670747,  SnmpPort 47,cmd  50450

Enter Egress VLAN Trans:
qinq->ulIfindex,	272670747
qinq->ulDefaultCvlan,	0
qinq->ulLocalPriority,	0
first_vlan_in_celist,	-1
qinq->ulCeVlanCos,	0
first_vlan_in_pelist,	200
qinq->ulPeVlanCos,	3
qinq->ulOuterVlanId,	0
qinq->ulOuterVlanCos,	0
qinq->ulInnerVlanId,	100
qinq->ulInnerVlanCos,	2
qinq->ulInnerMode,	6
qinq->ulOuterMode,	3
qinq->ulTransFlag,	0x198
qinq->ulRuleId,	1

[4576, ctc_tms_vlan_mapping_egress_set]  gport=25

[4578, ctc_tms_vlan_mapping_egress_set]  ulTransFlag=0x198

[4243, _cvlan_add_remove_egress]  [1:1] cvlan add remove egress 

[4269, _cvlan_add_remove_egress]  egress set cos 2

Para write to hardware:
old_svid 200
new_svid 100
new_scos 2

[4273, _cvlan_add_remove_egress]  [Succeed]

[4189, _qinq_save_egress_data]  Set succeed ,save data, Exit

[4201, _qinq_save_egress_data]  snmp_port=47

[4274, _cvlan_add_remove_egress]  [Succeed]

[4275, _cvlan_add_remove_egress]  Set succeed ! ,Exit !

[4635, ctc_tms_vlan_mapping_egress_set]  end

[477, ctcQinQSwitchStructSet]  exit succeed ,[rv: 0]
```



### no switchport vlan-mapping both cvlan 100 cos 2

**ROS_HW_SS_QINQ_VLAN_MAPPING_COS_TRANS_INGRESS**
**ROS_HW_SS_QINQ_VLAN_MAPPING_COS_TRANS_EGRESS**

```
[491, ctcQinQSwitchStructDel]  ifIndex 272670747,  cmd  50449

[3043, ctc_tms_vlan_mapping_ingress_del]  snmp_port: 47,  ulIfindex: 272670747, rule_id 1

[3047, ctc_tms_vlan_mapping_ingress_del]  gport=25

[3174, ctc_tms_vlan_mapping_ingress_del]  qinq del succeed

[3191, ctc_tms_vlan_mapping_ingress_del]  exit succeed ,[rv: 0]

[535, ctcQinQSwitchStructDel]  exit succeed ,[rv: 0]

[491, ctcQinQSwitchStructDel]  ifIndex 272670747,  cmd  50450

[4668, ctc_tms_vlan_mapping_egress_del]  snmp_port: 47,  ulIfindex: 272670747, rule_id 1

[4684, ctc_tms_vlan_mapping_egress_del]  gport=25

[4712, ctc_tms_vlan_mapping_egress_del]  Delete: Match Outer-Vid ( 200 )!

[4757, ctc_tms_vlan_mapping_egress_del]  qinq del succeed

[4774, ctc_tms_vlan_mapping_egress_del]  exit succeed ,[rv: 0]

[535, ctcQinQSwitchStructDel]  exit succeed ,[rv: 0]
```



### switchport vlan-mapping both outer 200 inner 100 translate outer 300 inner 200 cos 2

**ROS_HW_SS_QINQ_VLAN_MAPPING_TRANS_INGRESS**
**ROS_HW_SS_QINQ_VLAN_MAPPING_COS_TRANS_EGRESS**

```
[406, ctcQinQSwitchStructSet]  ifIndex 272670747,  SnmpPort 47,cmd  50447

Enter Ingress VLAN Trans:
qinq->ulIfindex,	272670747
qinq->ulDefaultCvlan,	0
qinq->ulLocalPriority,	0
first_vlan_in_celist,	100
qinq->ulCeVlanCos,	0
first_vlan_in_pelist,	200
qinq->ulPeVlanCos,	0
qinq->ulOuterVlanId,	300
qinq->ulOuterVlanCos,	2
qinq->ulInnerVlanId,	200
qinq->ulInnerVlanCos,	0
qinq->ulInnerMode,	2
qinq->ulOuterMode,	2
qinq->ulTransFlag,	0xea
qinq->ulRuleId,	1

[2928, ctc_tms_vlan_mapping_ingress_set]  gport=25

[2854, _double2double_ingress]  2:2

[2885, _double2double_ingress]  ingress 2:2: 200 , 100 -> 300, 200(2)

Para write to hardware:
old_cvid 100
old_svid 200
new_cvid 200
new_svid 300
new_scos 2

[2897, _double2double_ingress]  [Succeed]

[1663, _qinq_save_ingress_data]  Set succeed ,save data, Exit

[2899, _double2double_ingress]  [Succeed]

[2900, _double2double_ingress]  Set succeed ! ,Exit !

[2995, ctc_tms_vlan_mapping_ingress_set]  [Succeed]

[3010, ctc_tms_vlan_mapping_ingress_set]  end

[477, ctcQinQSwitchStructSet]  exit succeed ,[rv: 0]

[406, ctcQinQSwitchStructSet]  ifIndex 272670747,  SnmpPort 47,cmd  50450

Enter Egress VLAN Trans:
qinq->ulIfindex,	272670747
qinq->ulDefaultCvlan,	0
qinq->ulLocalPriority,	0
first_vlan_in_celist,	200
qinq->ulCeVlanCos,	0
first_vlan_in_pelist,	300
qinq->ulPeVlanCos,	2
qinq->ulOuterVlanId,	200
qinq->ulOuterVlanCos,	0
qinq->ulInnerVlanId,	100
qinq->ulInnerVlanCos,	0
qinq->ulInnerMode,	2
qinq->ulOuterMode,	2
qinq->ulTransFlag,	0xaa
qinq->ulRuleId,	1

[4576, ctc_tms_vlan_mapping_egress_set]  gport=25

[4578, ctc_tms_vlan_mapping_egress_set]  ulTransFlag=0xaa

[4495, _double2double_egress]  egress 2:2 : 300 , 200 -> 200, 100

Para write to hardware:
old_cvid 200
old_svid 300
new_cvid 100
new_svid 200

[4500, _double2double_egress]  [Succeed]

[4189, _qinq_save_egress_data]  Set succeed ,save data, Exit

[4201, _qinq_save_egress_data]  snmp_port=47

[4501, _double2double_egress]  [Succeed]

[4502, _double2double_egress]  Set succeed ! ,Exit !

[4635, ctc_tms_vlan_mapping_egress_set]  end

[477, ctcQinQSwitchStructSet]  exit succeed ,[rv: 0]
```


### no switchport vlan-mapping both outer 200 inner 100

**ROS_HW_SS_QINQ_VLAN_MAPPING_TRANS_INGRESS**
**ROS_HW_SS_QINQ_VLAN_MAPPING_COS_TRANS_EGRESS**

```
[491, ctcQinQSwitchStructDel]  ifIndex 272670747,  cmd  50447

[3043, ctc_tms_vlan_mapping_ingress_del]  snmp_port: 47,  ulIfindex: 272670747, rule_id 1

[3047, ctc_tms_vlan_mapping_ingress_del]  gport=25

[3134, ctc_tms_vlan_mapping_ingress_del]  Delete: Match Outer-Vid( 200 ) + Inner-Vid( 100 ) !

[3174, ctc_tms_vlan_mapping_ingress_del]  qinq del succeed

[3191, ctc_tms_vlan_mapping_ingress_del]  exit succeed ,[rv: 0]

[535, ctcQinQSwitchStructDel]  exit succeed ,[rv: 0]

[491, ctcQinQSwitchStructDel]  ifIndex 272670747,  cmd  50450

[4668, ctc_tms_vlan_mapping_egress_del]  snmp_port: 47,  ulIfindex: 272670747, rule_id 1

[4684, ctc_tms_vlan_mapping_egress_del]  gport=25

[4724, ctc_tms_vlan_mapping_egress_del]  Delete: Match Outer-Vid( 300 ) + Inner-Vid( 200 ) !

[4757, ctc_tms_vlan_mapping_egress_del]  qinq del succeed

[4774, ctc_tms_vlan_mapping_egress_del]  exit succeed ,[rv: 0]

[535, ctcQinQSwitchStructDel]  exit succeed ,[rv: 0]
```



### switchport vlan-mapping both untag  add-outer 200 cos 2

**ROS_HW_SS_QINQ_VLAN_MAPPING_UNTAG_OUTER_VLAN_ADD**
**ROS_HW_SS_QINQ_VLAN_MAPPING_COS_TRANS_EGRESS**

```
[406, ctcQinQSwitchStructSet]  ifIndex 272670747,  SnmpPort 47,cmd  50446

Enter Ingress VLAN Trans:
qinq->ulIfindex,	272670747
qinq->ulDefaultCvlan,	0
qinq->ulLocalPriority,	0
first_vlan_in_celist,	-1
qinq->ulCeVlanCos,	0
first_vlan_in_pelist,	-1
qinq->ulPeVlanCos,	0
qinq->ulOuterVlanId,	200
qinq->ulOuterVlanCos,	2
qinq->ulInnerVlanId,	0
qinq->ulInnerVlanCos,	0
qinq->ulInnerMode,	1
qinq->ulOuterMode,	6
qinq->ulTransFlag,	0x60
qinq->ulRuleId,	1

[4066, ctc_tms_vlan_mapping_untag_sadd_ingress_set]  gport=25

[3989, _untag_add_svlan_ingress]  gport=25,ulSAddVid=200,ulScos=2,ulCAddVid=0,ulCcos=0

[4039, _untag_add_svlan_ingress]  exit succeed ,[rv: 0]

[4084, ctc_tms_vlan_mapping_untag_sadd_ingress_set]  [Succeed]

[1730, _qinq_save_tcam_ingress_data]  Set succeed ,save data [eid 0] [ipv6_eid 1] [id 1], Exit

[4085, ctc_tms_vlan_mapping_untag_sadd_ingress_set]  [Succeed]

[4087, ctc_tms_vlan_mapping_untag_sadd_ingress_set]  end

[477, ctcQinQSwitchStructSet]  exit succeed ,[rv: 0]

[406, ctcQinQSwitchStructSet]  ifIndex 272670747,  SnmpPort 47,cmd  50450

Enter Egress VLAN Trans:
qinq->ulIfindex,	272670747
qinq->ulDefaultCvlan,	0
qinq->ulLocalPriority,	0
first_vlan_in_celist,	-1
qinq->ulCeVlanCos,	0
first_vlan_in_pelist,	200
qinq->ulPeVlanCos,	2
qinq->ulOuterVlanId,	0
qinq->ulOuterVlanCos,	0
qinq->ulInnerVlanId,	0
qinq->ulInnerVlanCos,	0
qinq->ulInnerMode,	1
qinq->ulOuterMode,	3
qinq->ulTransFlag,	0x18
qinq->ulRuleId,	1

[4576, ctc_tms_vlan_mapping_egress_set]  gport=25

[4578, ctc_tms_vlan_mapping_egress_set]  ulTransFlag=0x18

[4383, _inner_add_outer_egress]  egress ,inner add outer ,2:1 or 2:0 or 1:0

Para write to hardware:
old_svid 200

[4420, _inner_add_outer_egress]  [Succeed]

[4189, _qinq_save_egress_data]  Set succeed ,save data, Exit

[4201, _qinq_save_egress_data]  snmp_port=47

[4421, _inner_add_outer_egress]  [Succeed]

[4422, _inner_add_outer_egress]  Set ok ,Exit

[4635, ctc_tms_vlan_mapping_egress_set]  end

[477, ctcQinQSwitchStructSet]  exit succeed ,[rv: 0]
```



### no switchport vlan-mapping both untag

**ROS_HW_SS_QINQ_VLAN_MAPPING_UNTAG_OUTER_VLAN_ADD**
**ROS_HW_SS_QINQ_VLAN_MAPPING_COS_TRANS_EGRESS**

```
[491, ctcQinQSwitchStructDel]  ifIndex 272670747,  cmd  50446

[4114, ctc_tms_vlan_mapping_untag_sadd_ingress_del]  snmp_port: 47,  ulIfindex: 272670747, rule_id 1

[4118, ctc_tms_vlan_mapping_untag_sadd_ingress_del]  gport=25

[4159, ctc_tms_vlan_mapping_untag_sadd_ingress_del]  qinq del succeed

[4172, ctc_tms_vlan_mapping_untag_sadd_ingress_del]  exit succeed ,[rv: 0]

[535, ctcQinQSwitchStructDel]  exit succeed ,[rv: 0]

[491, ctcQinQSwitchStructDel]  ifIndex 272670747,  cmd  50450

[4668, ctc_tms_vlan_mapping_egress_del]  snmp_port: 47,  ulIfindex: 272670747, rule_id 1

[4684, ctc_tms_vlan_mapping_egress_del]  gport=25

[4712, ctc_tms_vlan_mapping_egress_del]  Delete: Match Outer-Vid ( 200 )!

[4757, ctc_tms_vlan_mapping_egress_del]  qinq del succeed

[4774, ctc_tms_vlan_mapping_egress_del]  exit succeed ,[rv: 0]

[535, ctcQinQSwitchStructDel]  exit succeed ,[rv: 0]
```



### switchport vlan-mapping both 100,200 translate 300

**ROS_HW_SS_QINQ_VLAN_MAPPING_TRANS_BOTH**

```
[406, ctcQinQSwitchStructSet]  ifIndex 272670747,  SnmpPort 47,cmd  50445

[3566, ctc_tms_vlan_mapping_both_set]  entry.ulIfindex = 272670747

[3581, ctc_tms_vlan_mapping_both_set]  ulSnmpPort=47,ulVlanId=300,ulTransFlag=0x28

[3624, ctc_tms_vlan_mapping_both_set]  gport=25,cvlan=100,ulVlanId=300

[3639, ctc_tms_vlan_mapping_both_set]  logic port = 229

[3693, ctc_tms_vlan_mapping_both_set]  nexthop id = 21505

[1663, _qinq_save_ingress_data]  Set succeed ,save data, Exit

[3758, ctc_tms_vlan_mapping_both_set]  [Succeed]

[3624, ctc_tms_vlan_mapping_both_set]  gport=25,cvlan=200,ulVlanId=300

[3639, ctc_tms_vlan_mapping_both_set]  logic port = 230

[3693, ctc_tms_vlan_mapping_both_set]  nexthop id = 21506

[1663, _qinq_save_ingress_data]  Set succeed ,save data, Exit

[3758, ctc_tms_vlan_mapping_both_set]  [Succeed]

[1007, ctc_tms_qinq_port_vlan_default_entry_set]  exit succeed ,[rv: 0]

[3782, ctc_tms_vlan_mapping_both_set]  exit succeed ,[rv: 0]

[477, ctcQinQSwitchStructSet]  exit succeed ,[rv: 0]
```



### no switchport vlan-mapping both translate 300

**ROS_HW_SS_QINQ_VLAN_MAPPING_TRANS_BOTH**

```
[491, ctcQinQSwitchStructDel]  ifIndex 272670747,  cmd  50445

[3816, ctc_tms_vlan_mapping_both_del]  snmp_port: 47,  ulIfindex: 272670747, rule_id 1

[3820, ctc_tms_vlan_mapping_both_del]  gport=25

[3917, ctc_tms_vlan_mapping_both_del]  qinq del succeed

[3917, ctc_tms_vlan_mapping_both_del]  qinq del succeed

[1007, ctc_tms_qinq_port_vlan_default_entry_set]  exit succeed ,[rv: 0]

[3967, ctc_tms_vlan_mapping_both_del]  exit succeed ,[rv: 0]

[535, ctcQinQSwitchStructDel]  exit succeed ,[rv: 0]
```



### switchport vlan-mapping double-tagging untag add svlan outer 200 cos 2 inner 100 cos 3

**ROS_HW_SS_QINQ_VLAN_MAPPING_TRIPLE_TAG_ADD**

```
[406, ctcQinQSwitchStructSet]  ifIndex 272670747,  SnmpPort 47,cmd  50451

Enter Ingress VLAN Triple Tag Add:
qinq->ulIfindex,	272670747
first_vlan_in_celist,	-1
qinq->ulCVlanOuter,	0
qinq->ulCVlanOuterCos,	0
qinq->ulSVlanInner,	100
qinq->ulSVlanInnerCos,	3
qinq->ulSVlanOuter,	200
qinq->ulSVlanOuterCos,	2
qinq->ulTransFlag,	0x78
qinq->ulRetId,	0

[2453, ctc_tms_vlan_mapping_untag_add_double_set]  gport=25

[2470, ctc_tms_vlan_mapping_untag_add_double_set]  ingress config set

[2290, _untag_add_double_ingress]  gport=25,ulSAddinnerVid=100,ulSinnerCos=3,ulSAddoutVid=200,ulSouterCos=2,ulCAddVid=0,ulCcos=0

[2350, _untag_add_double_ingress]  exit succeed ,[rv: 0]

[2472, ctc_tms_vlan_mapping_untag_add_double_set]  [Succeed]

[1775, _qinq_save_tcam_ingress_triple_data]  Set succeed ,save data [eid 0] [ipv6_eid 1] [id 0], Exit

[2473, ctc_tms_vlan_mapping_untag_add_double_set]  [Succeed]

[2475, ctc_tms_vlan_mapping_untag_add_double_set]  egress config set

[2411, _untag_add_double_egress]  egress ,untag add double egress

[2425, _untag_add_double_egress]  egress : 200 , 100 -> 0(0)

[2426, _untag_add_double_egress]  entry num 0:


Para write to hardware:
old_cvid 100
old_svid 200

[2428, _untag_add_double_egress]  [Succeed]

[2359, _untag_add_double_save_egress_data]  Set succeed ,save data, Exit

[2371, _untag_add_double_save_egress_data]  snmp_port=47

[2429, _untag_add_double_egress]  [Succeed]

[2431, _untag_add_double_egress]  Set ok ,Exit

[2478, ctc_tms_vlan_mapping_untag_add_double_set]  end

[477, ctcQinQSwitchStructSet]  exit succeed ,[rv: 0]
```



### no switchport vlan-mapping double-tagging untag

**ROS_HW_SS_QINQ_VLAN_MAPPING_TRIPLE_TAG_ADD**

```
[491, ctcQinQSwitchStructDel]  ifIndex 272670747,  cmd  50451

[2618, ctc_tms_vlan_mapping_untag_add_double_del]  snmp_port: 47,  ulIfindex: 272670747, rule_id 0

[2622, ctc_tms_vlan_mapping_untag_add_double_del]  gport=25

[2663, ctc_tms_vlan_mapping_untag_add_double_del]  qinq del succeed

[2674, ctc_tms_vlan_mapping_untag_add_double_del]  qinq egress del start

[2507, _untag_add_double_egress_del]  snmp_port: 47,  ulIfindex: 272670747, rule_id 0

[2523, _untag_add_double_egress_del]  gport=25

[2550, _untag_add_double_egress_del]  Delete: Match Outer-Vid( 200 ) + Inner-Vid( 100 ) !

[2574, _untag_add_double_egress_del]  qinq del succeed

[2591, _untag_add_double_egress_del]  exit succeed ,[rv: 0]

[2679, ctc_tms_vlan_mapping_untag_add_double_del]  exit succeed ,[rv: 0]

[535, ctcQinQSwitchStructDel]  exit succeed ,[rv: 0]

[406, ctcQinQSwitchStructSet]  ifIndex 272670747,  SnmpPort 47,cmd  50453

[616, ctc_tms_set_port_miss_drop_enable]  entry! snmpPort=47,Enable=2,pPort->dot1qEnable= 0

[628, ctc_tms_set_port_miss_drop_enable]  already set

[477, ctcQinQSwitchStructSet]  exit succeed ,[rv: 0]
```


### switchport vlan-mapping double-tagging cvlan 100 add svlan outer 300 cos 2 inner 200 cos 1

**ROS_HW_SS_QINQ_VLAN_MAPPING_TRIPLE_TAG_ADD**

```
[406, ctcQinQSwitchStructSet]  ifIndex 272670747,  SnmpPort 47,cmd  50451

Enter Ingress VLAN Triple Tag Add:
qinq->ulIfindex,	272670747
first_vlan_in_celist,	100
qinq->ulCVlanOuter,	0
qinq->ulCVlanOuterCos,	0
qinq->ulSVlanInner,	200
qinq->ulSVlanInnerCos,	1
qinq->ulSVlanOuter,	300
qinq->ulSVlanOuterCos,	2
qinq->ulTransFlag,	0x79
qinq->ulRetId,	0

[3212, ctc_tms_vlan_mapping_cvlan_add_double_set]  gport=25

[3214, ctc_tms_vlan_mapping_cvlan_add_double_set]  ingress config set

Para write to hardware:
old_svid 100
new_cvid 200
new_svid 300
new_ccos 1
new_scos 2

[3267, _cvlan_add_double_ingress]  [Succeed]

[1823, _qinq_save_ingress_triple_data]  Set succeed ,save data, Exit

[3269, _cvlan_add_double_ingress]  [Succeed]

[3271, _cvlan_add_double_ingress]  Set succeed ! ,Exit !

[3216, ctc_tms_vlan_mapping_cvlan_add_double_set]  [Succeed]

[3222, ctc_tms_vlan_mapping_cvlan_add_double_set]  egress config set

[2411, _untag_add_double_egress]  egress ,untag add double egress

[2425, _untag_add_double_egress]  egress : 300 , 200 -> 0(0)

[2426, _untag_add_double_egress]  entry num 0:


Para write to hardware:
old_cvid 200
old_svid 300

[2428, _untag_add_double_egress]  [Succeed]

[2359, _untag_add_double_save_egress_data]  Set succeed ,save data, Exit

[2371, _untag_add_double_save_egress_data]  snmp_port=47

[2429, _untag_add_double_egress]  [Succeed]

[2431, _untag_add_double_egress]  Set ok ,Exit

[3225, ctc_tms_vlan_mapping_cvlan_add_double_set]  end

[477, ctcQinQSwitchStructSet]  exit succeed ,[rv: 0]
```



### no switchport vlan-mapping double-tagging cvlan 100

**ROS_HW_SS_QINQ_VLAN_MAPPING_TRIPLE_TAG_ADD**

```
[491, ctcQinQSwitchStructDel]  ifIndex 272670747,  cmd  50451

[3301, ctc_tms_vlan_mapping_cvlan_add_double_del]  snmp_port: 47,  ulIfindex: 272670747, rule_id 0

[3305, ctc_tms_vlan_mapping_cvlan_add_double_del]  gport=25

[3331, ctc_tms_vlan_mapping_cvlan_add_double_del]  Delete: Match Outer-Vid ( 100 )!

[3346, ctc_tms_vlan_mapping_cvlan_add_double_del]  qinq del succeed

[3361, ctc_tms_vlan_mapping_cvlan_add_double_del]  qinq egress del start

[2507, _untag_add_double_egress_del]  snmp_port: 47,  ulIfindex: 272670747, rule_id 0

[2523, _untag_add_double_egress_del]  gport=25

[2550, _untag_add_double_egress_del]  Delete: Match Outer-Vid( 300 ) + Inner-Vid( 200 ) !

[2574, _untag_add_double_egress_del]  qinq del succeed

[2591, _untag_add_double_egress_del]  exit succeed ,[rv: 0]

[3364, ctc_tms_vlan_mapping_cvlan_add_double_del]  exit succeed ,[rv: 0]

[535, ctcQinQSwitchStructDel]  exit succeed ,[rv: 0]

[406, ctcQinQSwitchStructSet]  ifIndex 272670747,  SnmpPort 47,cmd  50453

[616, ctc_tms_set_port_miss_drop_enable]  entry! snmpPort=47,Enable=2,pPort->dot1qEnable= 0

[628, ctc_tms_set_port_miss_drop_enable]  already set

[477, ctcQinQSwitchStructSet]  exit succeed ,[rv: 0]
```



### switchport vlan-mapping triple-tagging untag add svlan outer 100 cos 1 inner 200 cos 2  cvlan 300 cos 3（不支持）

**ROS_HW_SS_QINQ_VLAN_MAPPING_TRIPLE_TAG_ADD**

```
[406, ctcQinQSwitchStructSet]  ifIndex 272670747,  SnmpPort 47,cmd  50451

Enter Ingress VLAN Triple Tag Add:
qinq->ulIfindex,	272670747
first_vlan_in_celist,	-1
qinq->ulCVlanOuter,	300
qinq->ulCVlanOuterCos,	3
qinq->ulSVlanInner,	200
qinq->ulSVlanInnerCos,	2
qinq->ulSVlanOuter,	100
qinq->ulSVlanOuterCos,	1
qinq->ulTransFlag,	0x7e
qinq->ulRetId,	0

[2453, ctc_tms_vlan_mapping_untag_add_double_set]  gport=25

[2470, ctc_tms_vlan_mapping_untag_add_double_set]  ingress config set

[2290, _untag_add_double_ingress]  gport=25,ulSAddinnerVid=200,ulSinnerCos=2,ulSAddoutVid=100,ulSouterCos=1,ulCAddVid=300,ulCcos=3

[2350, _untag_add_double_ingress]  exit succeed ,[rv: 0]

[2472, ctc_tms_vlan_mapping_untag_add_double_set]  [Succeed]

[1775, _qinq_save_tcam_ingress_triple_data]  Set succeed ,save data [eid 0] [ipv6_eid 1] [id 0], Exit

[2473, ctc_tms_vlan_mapping_untag_add_double_set]  [Succeed]

[2475, ctc_tms_vlan_mapping_untag_add_double_set]  egress config set

[2411, _untag_add_double_egress]  egress ,untag add double egress

[2425, _untag_add_double_egress]  egress : 100 , 200 -> 0(0)

[2426, _untag_add_double_egress]  entry num 0:


Para write to hardware:
old_cvid 200
old_svid 100

[2428, _untag_add_double_egress]  [Succeed]

[2359, _untag_add_double_save_egress_data]  Set succeed ,save data, Exit

[2371, _untag_add_double_save_egress_data]  snmp_port=47

[2429, _untag_add_double_egress]  [Succeed]

[2431, _untag_add_double_egress]  Set ok ,Exit

[2478, ctc_tms_vlan_mapping_untag_add_double_set]  end

[477, ctcQinQSwitchStructSet]  exit succeed ,[rv: 0]
```


### no switchport vlan-mapping triple-tagging untag

**ROS_HW_SS_QINQ_VLAN_MAPPING_TRIPLE_TAG_ADD**

```
[491, ctcQinQSwitchStructDel]  ifIndex 272670747,  cmd  50451

[2618, ctc_tms_vlan_mapping_untag_add_double_del]  snmp_port: 47,  ulIfindex: 272670747, rule_id 0

[2622, ctc_tms_vlan_mapping_untag_add_double_del]  gport=25

[2663, ctc_tms_vlan_mapping_untag_add_double_del]  qinq del succeed

[2674, ctc_tms_vlan_mapping_untag_add_double_del]  qinq egress del start

[2507, _untag_add_double_egress_del]  snmp_port: 47,  ulIfindex: 272670747, rule_id 0

[2523, _untag_add_double_egress_del]  gport=25

[2550, _untag_add_double_egress_del]  Delete: Match Outer-Vid( 100 ) + Inner-Vid( 200 ) !

[2574, _untag_add_double_egress_del]  qinq del succeed

[2591, _untag_add_double_egress_del]  exit succeed ,[rv: 0]

[2679, ctc_tms_vlan_mapping_untag_add_double_del]  exit succeed ,[rv: 0]

[535, ctcQinQSwitchStructDel]  exit succeed ,[rv: 0]

[406, ctcQinQSwitchStructSet]  ifIndex 272670747,  SnmpPort 47,cmd  50453

[616, ctc_tms_set_port_miss_drop_enable]  entry! snmpPort=47,Enable=2,pPort->dot1qEnable= 0

[628, ctc_tms_set_port_miss_drop_enable]  already set

[477, ctcQinQSwitchStructSet]  exit succeed ,[rv: 0]
```



### switchport vlan-mapping triple-tagging cvlan 100 add svlan outer 300 cos 3 inner 200 cos 1  cvlan 400 cos 2（不支持）

**ROS_HW_SS_QINQ_VLAN_MAPPING_TRIPLE_TAG_ADD**

```
[406, ctcQinQSwitchStructSet]  ifIndex 272670747,  SnmpPort 47,cmd  50451

Enter Ingress VLAN Triple Tag Add:
qinq->ulIfindex,	272670747
first_vlan_in_celist,	100
qinq->ulCVlanOuter,	400
qinq->ulCVlanOuterCos,	2
qinq->ulSVlanInner,	200
qinq->ulSVlanInnerCos,	1
qinq->ulSVlanOuter,	300
qinq->ulSVlanOuterCos,	3
qinq->ulTransFlag,	0x7f
qinq->ulRetId,	0

[3212, ctc_tms_vlan_mapping_cvlan_add_double_set]  gport=25

[3214, ctc_tms_vlan_mapping_cvlan_add_double_set]  ingress config set

Para write to hardware:
old_svid 100
new_cvid 200
new_svid 300
new_ccos 1
new_scos 3

[3267, _cvlan_add_double_ingress]  [Succeed]

[1823, _qinq_save_ingress_triple_data]  Set succeed ,save data, Exit

[3269, _cvlan_add_double_ingress]  [Succeed]

[3271, _cvlan_add_double_ingress]  Set succeed ! ,Exit !

[3216, ctc_tms_vlan_mapping_cvlan_add_double_set]  [Succeed]

[3222, ctc_tms_vlan_mapping_cvlan_add_double_set]  egress config set

[2411, _untag_add_double_egress]  egress ,untag add double egress

[2425, _untag_add_double_egress]  egress : 300 , 200 -> 0(0)

[2426, _untag_add_double_egress]  entry num 0:


Para write to hardware:
old_cvid 200
old_svid 300

[2428, _untag_add_double_egress]  [Succeed]

[2359, _untag_add_double_save_egress_data]  Set succeed ,save data, Exit

[2371, _untag_add_double_save_egress_data]  snmp_port=47

[2429, _untag_add_double_egress]  [Succeed]

[2431, _untag_add_double_egress]  Set ok ,Exit

[3225, ctc_tms_vlan_mapping_cvlan_add_double_set]  end

[477, ctcQinQSwitchStructSet]  exit succeed ,[rv: 0]
```



### no switchport vlan-mapping triple-tagging cvlan 100

**ROS_HW_SS_QINQ_VLAN_MAPPING_TRIPLE_TAG_ADD**

```
[491, ctcQinQSwitchStructDel]  ifIndex 272670747,  cmd  50451

[3301, ctc_tms_vlan_mapping_cvlan_add_double_del]  snmp_port: 47,  ulIfindex: 272670747, rule_id 0

[3305, ctc_tms_vlan_mapping_cvlan_add_double_del]  gport=25

[3331, ctc_tms_vlan_mapping_cvlan_add_double_del]  Delete: Match Outer-Vid ( 100 )!

[3346, ctc_tms_vlan_mapping_cvlan_add_double_del]  qinq del succeed

[3361, ctc_tms_vlan_mapping_cvlan_add_double_del]  qinq egress del start

[2507, _untag_add_double_egress_del]  snmp_port: 47,  ulIfindex: 272670747, rule_id 0

[2523, _untag_add_double_egress_del]  gport=25

[2550, _untag_add_double_egress_del]  Delete: Match Outer-Vid( 300 ) + Inner-Vid( 200 ) !

[2574, _untag_add_double_egress_del]  qinq del succeed

[2591, _untag_add_double_egress_del]  exit succeed ,[rv: 0]

[3364, ctc_tms_vlan_mapping_cvlan_add_double_del]  exit succeed ,[rv: 0]

[535, ctcQinQSwitchStructDel]  exit succeed ,[rv: 0]

[406, ctcQinQSwitchStructSet]  ifIndex 272670747,  SnmpPort 47,cmd  50453

[616, ctc_tms_set_port_miss_drop_enable]  entry! snmpPort=47,Enable=2,pPort->dot1qEnable= 0

[628, ctc_tms_set_port_miss_drop_enable]  already set

[477, ctcQinQSwitchStructSet]  exit succeed ,[rv: 0]
```








