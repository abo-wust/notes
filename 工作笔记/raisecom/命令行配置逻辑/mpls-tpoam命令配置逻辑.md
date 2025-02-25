

## mpls-tp cfm domain level 7
## mpls-tp service ma1 level 7
## service tunnel 1/1/1

前面两条不会下发驱动，第三条命令才会下发到驱动，打印下面的日志
```
SSP_mplsoam_switch_struct_set @201: normal create MA
SSP_mplsoam_switch_struct_set @202: normal iMaHwId=1
SSP_mplsoam_switch_struct_set @204: normal aucMaId[0]=1
SSP_mplsoam_switch_struct_set @205: normal aucMaId[1]=20
SSP_mplsoam_switch_struct_set @206: normal aucMaId[2]=d
SSP_mplsoam_switch_struct_set @207: normal aucMaId[raiscmma1]
SSP_mplsoam_switch_struct_set @229: normal ulLevel=7
SSP_mplsoam_switch_struct_set @230: normal eMaType=7
SSP_mplsoam_switch_struct_set @231: normal ulMplsType=1
SSP_mplsoam_switch_struct_set @232: normal eCcmInterval=4
SSP_mplsoam_switch_struct_set @233: normal ulIfIndex=0
SSP_mplsoam_switch_struct_set @234: normal usSVlan=0
SSP_mplsoam_switch_struct_set @235: normal ulLspInLabel=4000
SSP_mplsoam_switch_struct_set @236: normal ulLspOutLabel=900
SSP_mplsoam_switch_struct_set @237: normal ulReLspInLabel=0
SSP_mplsoam_switch_struct_set @238: normal ulReLspOutLabel=0
SSP_mplsoam_switch_struct_set @239: normal ulPwInLabel=0
SSP_mplsoam_switch_struct_set @240: normal ulPwOutLabel=0
SSP_mplsoam_switch_struct_set @241: normal ulRePwInLabel=0
SSP_mplsoam_switch_struct_set @242: normal ulRePwOutLabel=0
SSP_mplsoam_switch_struct_set @243: normal ucInTtl=1
SSP_mplsoam_switch_struct_set @244: normal ucOutTtl=255
SSP_mplsoam_switch_struct_set @245: normal ucInTc=7
SSP_mplsoam_switch_struct_set @246: normal ucOutTc=7
SSP_mplsoam_switch_struct_set @247: normal ulTunnelIf=155189248
SSP_mplsoam_switch_struct_set @248: normal ulXcIndex=7
SSP_mplsoam_switch_struct_set @249: normal ulVpnId=0
SSP_mplsoam_switch_struct_set @250: normal ulVp=0
SSP_mplsoam_switch_struct_set @251: normal eflag=0
SSP_mplsoam_ma_set @8435: normal Create pSspTpoamMa=88058160

```


## service cc interval 10ms
```
SSP_mplsoam_switch_struct_set @201: normal create MA
SSP_mplsoam_switch_struct_set @202: normal iMaHwId=1
SSP_mplsoam_switch_struct_set @204: normal aucMaId[0]=0
SSP_mplsoam_switch_struct_set @205: normal aucMaId[1]=0
SSP_mplsoam_switch_struct_set @206: normal aucMaId[2]=0
SSP_mplsoam_switch_struct_set @207: normal aucMaId[]
SSP_mplsoam_switch_struct_set @229: normal ulLevel=0
SSP_mplsoam_switch_struct_set @230: normal eMaType=0
SSP_mplsoam_switch_struct_set @231: normal ulMplsType=0
SSP_mplsoam_switch_struct_set @232: normal eCcmInterval=2
SSP_mplsoam_switch_struct_set @233: normal ulIfIndex=0
SSP_mplsoam_switch_struct_set @234: normal usSVlan=0
SSP_mplsoam_switch_struct_set @235: normal ulLspInLabel=0
SSP_mplsoam_switch_struct_set @236: normal ulLspOutLabel=0
SSP_mplsoam_switch_struct_set @237: normal ulReLspInLabel=0
SSP_mplsoam_switch_struct_set @238: normal ulReLspOutLabel=0
SSP_mplsoam_switch_struct_set @239: normal ulPwInLabel=0
SSP_mplsoam_switch_struct_set @240: normal ulPwOutLabel=0
SSP_mplsoam_switch_struct_set @241: normal ulRePwInLabel=0
SSP_mplsoam_switch_struct_set @242: normal ulRePwOutLabel=0
SSP_mplsoam_switch_struct_set @243: normal ucInTtl=0
SSP_mplsoam_switch_struct_set @244: normal ucOutTtl=0
SSP_mplsoam_switch_struct_set @245: normal ucInTc=0
SSP_mplsoam_switch_struct_set @246: normal ucOutTc=0
SSP_mplsoam_switch_struct_set @247: normal ulTunnelIf=0
SSP_mplsoam_switch_struct_set @248: normal ulXcIndex=0
SSP_mplsoam_switch_struct_set @249: normal ulVpnId=0
SSP_mplsoam_switch_struct_set @250: normal ulVp=0
SSP_mplsoam_switch_struct_set @251: normal eflag=0
SSP_mplsoam_ma_set @8593: normal iMaHwId=1 do not have lmep

```


## service remote-mep 2

先创建`rmep`时会提示`lmep`未创建，后续逻辑跳过直接返回
```
SSP_mplsoam_switch_struct_set @301: normal create RMEP
SSP_mplsoam_switch_struct_set @302: normal iMaHwId=1
SSP_mplsoam_switch_struct_set @303: normal iRMepHwId=1
SSP_mplsoam_switch_struct_set @304: normal usRMepId=2
SSP_mplsoam_switch_struct_set @305: normal eflag=0
SSP_mplsoam_rmep_set @9288: normal pSspTpoamRmep=880D3710
SSP_mplsoam_rmep_set @9301: normal tpoam hwid=1 lmep not create 
```

后创建`rmep`时会正常走到`remp`的创建接口
```
SSP_mplsoam_switch_struct_set @301: normal create RMEP
SSP_mplsoam_switch_struct_set @302: normal iMaHwId=1
SSP_mplsoam_switch_struct_set @303: normal iRMepHwId=1
SSP_mplsoam_switch_struct_set @304: normal usRMepId=2
SSP_mplsoam_switch_struct_set @305: normal eflag=0
SSP_mplsoam_rmep_set @9288: normal pSspTpoamRmep=880DAF70
_sdk_tpoam_rmep_create @1498: normal _sdk_tpoam_rmep_update_aps TUNNEL apsGroup=-1,return.
```


## service mep mpid 1 

```
SSP_mplsoam_switch_struct_set @275: normal create LMEP
SSP_mplsoam_switch_struct_set @276: normal iMaHwId=1
SSP_mplsoam_switch_struct_set @277: normal iMepHwId=1
SSP_mplsoam_switch_struct_set @278: normal usMepId=1
SSP_mplsoam_switch_struct_set @279: normal eMaType=7
SSP_mplsoam_lmep_set @8842: normal pSspTpoamMep=880D0710
_SSP_mplstpoam_for_mplstp_tunnel_set @8139: normal masterXCindex=7 group_NHI=2 master_NHI=3hotxc_NHI=0 aps_group=-1
_SSP_mplstpoam_tunnel_nhi_set @2712: normal pMplstpoamSspInfo->mplstpoam_lsp_nhi=16385 
_sdk_tpoam_lmep_lm_cos_set @1084: normal lm cos type set, type = 12, value = 0
_sdk_tpoam_lmep_lm_type_set @1119: normal lm type set, type = 11, value = 2
_sdk_tpoam_rmep_create @1498: normal _sdk_tpoam_rmep_update_aps TUNNEL apsGroup=-1,return.
SSP_mplsoam_switch_struct_set @393: normal SET MPLSTP OAM LM PRIORITY SERVICE ENABLE
SSP_mplsoam_switch_struct_set @394: normal iMaHwId = 1
SSP_mplsoam_switch_struct_set @395: normal iMepHwId = 1
SSP_mplsoam_lm_cos_set @11430: normal hwapi set lm cos = 0.
SSP_mplsoam_lm_cos_set @11431: normal ssp set lm cos = 1.
SSP_mplsoam_lm_cos_set @11441: normal ssp set lm enable = 0.
_sdk_tpoam_lmep_lm_cos_set @1084: normal lm cos type set, type = 12, value = 2

```


## service cc enable mep 1
```
SSP_mplsoam_switch_struct_set @319: normal set CC 
SSP_mplsoam_switch_struct_set @320: normal iMaHwId=1
SSP_mplsoam_switch_struct_set @321: normal iMepHwId=1
SSP_mplsoam_switch_struct_set @322: normal bCcmEnable=1

```



## 业务接口down

接口状态`down`时，`oam`相关的配置会删掉，等接口状态`link`后再重新配置一遍
```
iTN165-2G_8.106(debug)#
iTN165-2G_8.106(debug)#show ssp mpls  cache 
[NO.1]1970-01-01,10:30:05.988.311, _ssp_mplstpoam_intr@10415, rmep hwid 1 down events=34112
[NO.2]1970-01-01,10:30:05.988.378, _SSP_mplstpoam_quick_notify@9824, quick sw tunnelif=155189248
[NO.3]1970-01-01,10:30:05.988.441, mplstpoamNotifyRos@9910, ulMaHwId=1 uiRmepId=2 ulIntrErrBitMap=0x40
[NO.4]1970-01-01,10:30:05.988.459, ulMaHwId=1 uiRmepId=2 ulIntrErrBitMap=0x40
[NO.5]1970-01-01,10:30:06.002.077, BFD IPV4 CB:ipadd_BE=0x64010100 vrf=0 masklen=24 op=2
[NO.6]1970-01-01,10:30:06.002.209, FDI IPV4 DEL:rc=0.
[NO.7]1970-01-01,10:30:06.002.224, SSP ROUTE DEL INERN:hwnhi=8194.
[NO.8]1970-01-01,10:30:06.002.251, BFD IPV4 CB:ipadd_BE=0x64010100 vrf=0 masklen=24 op=3
[NO.9]1970-01-01,10:30:06.002.278, SSP IP DEL:vrf=0.ip=100.1.1.0/24.rc=0
[NO.10]1970-01-01,10:30:06.002.941, SSP NHP DEL INERN:nhi=9.port=48.hwnhi=8194.
[NO.11]1970-01-01,10:30:06.002.995, SSP NHI DEL:nhi=9.type=6.rc=0
[NO.12]1970-01-01,10:30:06.006.075, BFD IPV4 CB:ipadd_BE=0x64010101 vrf=0 masklen=32 op=2
[NO.13]1970-01-01,10:30:06.006.187, FDI IPV4 DEL:rc=0.
[NO.14]1970-01-01,10:30:06.006.200, SSP ROUTE DEL INERN:hwnhi=2.
[NO.15]1970-01-01,10:30:06.006.225, BFD IPV4 CB:ipadd_BE=0x64010101 vrf=0 masklen=32 op=3
[NO.16]1970-01-01,10:30:06.006.250, SSP IP DEL:vrf=0.ip=100.1.1.1/32.rc=0
[NO.17]1970-01-01,10:30:06.089.213, SSP NHLFE DELETE:xc=7,rc=0
[NO.18]1970-01-01,10:30:06.094.402, BFD pw CB:Event=3 vpnid=12288 vpid=2 inlabel=0 rc=0
[NO.19]1970-01-01,10:30:06.094.781, SSP NNI UPDATE:vpn=12288.vp=2.flags=2.xc=0.type=2.access=0.rc=0
[NO.20]1970-01-01,10:30:06.095.013, SSP_mplsoam_switch_struct_del@539, SET MPLSTP OAM LM PRIORITY SERVICE DISENABLE.
[NO.21]1970-01-01,10:30:06.095.060, SSP_mplsoam_switch_struct_del@540, iMaHwId = 1
[NO.22]1970-01-01,10:30:06.095.096, SSP_mplsoam_switch_struct_del@541, iMepHwId = 1
[NO.23]1970-01-01,10:30:06.095.144, SSP_mplsoam_lm_cos_set@11430, hwapi set lm cos = 1.
[NO.24]1970-01-01,10:30:06.095.181, SSP_mplsoam_lm_cos_set@11431, ssp set lm cos = 0.
[NO.25]1970-01-01,10:30:06.095.219, SSP_mplsoam_lm_cos_set@11441, ssp set lm enable = 0.
[NO.26]1970-01-01,10:30:06.095.261, _sdk_tpoam_lmep_lm_cos_set@1084, lm cos type set, type = 12, value = 0
[NO.27]1970-01-01,10:30:06.095.404, TPOAM DEL LM PRIORITY:maHwid=1,mepHwid=1,rc=0
[NO.28]1970-01-01,10:30:06.100.477, SSP_mplsoam_switch_struct_del@493, DEL LMEP
[NO.29]1970-01-01,10:30:06.100.529, SSP_mplsoam_switch_struct_del@494, iMaHwId=1
[NO.30]1970-01-01,10:30:06.100.565, SSP_mplsoam_switch_struct_del@495, iMepHwId=1
[NO.31]1970-01-01,10:30:06.100.889, _SSP_mplstpoam_tunnel_nhi_inused@1916, mplstp_tunnel_nhi_share NOT Find!
[NO.32]1970-01-01,10:30:06.100.940, _SSP_mplstpoam_tunnel_nhi_del@2729, del lsp_nhi_id = 16385
[NO.33]1970-01-01,10:30:06.101.004, _SSP_mplstpoam_tunnel_nhi_del@2745, free lsp_nhi_id
[NO.34]1970-01-01,10:30:06.101.036, TPOAM DEL LMEP:maHwid=1,mepid=1,rc=0
[NO.35]1970-01-01,10:30:06.102.357, BFD pw CB:Event=7 vpnid=12288 vpid=2 inlabel=100 rc=0
[NO.36]1970-01-01,10:30:06.102.402, _SSP_mplstpoam_ilm_del_callback ros_inlabel=100
[NO.37]1970-01-01,10:30:06.102.541, SSP ILM DEL:MplsInLabel=100.rc=0
[NO.38]1970-01-01,10:30:06.104.222, SSP_mplsoam_switch_struct_del@507, DEL RMEP
[NO.39]1970-01-01,10:30:06.104.277, SSP_mplsoam_switch_struct_del@508, iMaHwId=1
[NO.40]1970-01-01,10:30:06.104.313, SSP_mplsoam_switch_struct_del@509, iRMepHwId=1
[NO.41]1970-01-01,10:30:06.104.352, SSP_mplsoam_switch_struct_del@510, usRMepId=60900
[NO.42]1970-01-01,10:30:06.104.387, TPOAM DEL RMEP:maHwid=1,mepid=1,rc=0
[NO.43]1970-01-01,10:30:06.104.677, BFD pw CB:Event=3 vpnid=12288 vpid=2 inlabel=0 rc=0
[NO.44]1970-01-01,10:30:06.104.716, SSP NNI UPDATE:vpn=12288.vp=2.flags=2.xc=0.type=0.access=0.rc=0
[NO.45]1970-01-01,10:30:06.105.646, SSP_mplsoam_switch_struct_del@482, DEL MA
[NO.46]1970-01-01,10:30:06.105.698, SSP_mplsoam_switch_struct_del@483, maHwid=1
[NO.47]1970-01-01,10:30:06.105.811, TPOAM DEL MA:maHwid=1,rc=0
[NO.48]1970-01-01,10:30:06.106.446, SSP NHLFE DELETE:xc=9,rc=0
[NO.49]1970-01-01,10:30:06.107.025, SSP TUNNEL UPDATE:tunnel=155189248.type=2.xc=0.rc=0,pTunnel->group_NHI=2,pTunnel->master_NHI=3.
[NO.50]1970-01-01,10:30:06.107.342, SSP TUNNEL UPDATE:tunnel=155189248.type=2.xc=0.rc=0,pTunnel->implicit_null_master_nhi=5,pTunnel->master_NHI=3.
[NO.51]1970-01-01,10:30:06.107.557, FDI EMPTY NHI UPDATE:nh_index_empty = 4,nh_index = 5.rc=0.
[NO.52]1970-01-01,10:30:06.107.840, FDI EMPTY NHI UPDATE:nh_index_empty = 2,nh_index = 3.rc=0.
[NO.53]1970-01-01,10:30:06.108.171, BFD lsp CB:Event=12 HwNhi=2
[NO.54]1970-01-01,10:30:06.108.209, SSP TUNNEL UPDATE:tunnel=155189248.type=2.owner=1.xc=0.rc=0
[NO.55]1970-01-01,10:30:07.209.149, BFD IPV4 CB:ipadd_BE=0x66666666 vrf=0 masklen=32 op=2
[NO.56]1970-01-01,10:30:07.209.272, FDI IPV4 DEL:rc=0.
[NO.57]1970-01-01,10:30:07.209.287, SSP ROUTE DEL INERN:hwnhi=8210.
[NO.58]1970-01-01,10:30:07.209.315, BFD IPV4 CB:ipadd_BE=0x66666666 vrf=0 masklen=32 op=3
[NO.59]1970-01-01,10:30:07.209.344, SSP IP DEL:vrf=0.ip=102.102.102.102/32.rc=0
[NO.60]1970-01-01,10:30:07.210.554, FDI IP NHI SET:nhIndex = 8211,arp = 0, vlan=0,innervlan=0.flag=0.port=48,trunk=0.rc=0.
[NO.61]1970-01-01,10:30:07.210.607, SSP NHP SET INERN:nhi=85750.port=48.hwnhi=8211.
[NO.62]1970-01-01,10:30:07.210.628, SSP NHI SET:nhi=85750.type=1.port=272646144.if=272646144.mac=000E.5E66.0009.vlan=0/0.flag=0.ucMasterActive=0.rc=0.
[NO.63]1970-01-01,10:30:07.211.253, FDI IPV4 SET:nhi = 8211,rc=0.
[NO.64]1970-01-01,10:30:07.211.283, SSP ROUTE UPDATE INERN:hwnhi=8211.
[NO.65]1970-01-01,10:30:07.211.300, BFD IPV4 CB:ipadd_BE=0x64010102 vrf=0 masklen=32 op=1
[NO.66]1970-01-01,10:30:07.211.330, SSP IP SET:vrf=0.ip=100.1.1.2/32.nhitype=1.nhi=85750.flag=0.rc=0
[NO.67]1970-01-01,10:30:07.211.574, FDI IP NHI DEL:nhIndex = 8210,arp=0.rc=0.
[NO.68]1970-01-01,10:30:07.211.597, SSP NHP DEL INERN:nhi=10.port=48.hwnhi=8210.
[NO.69]1970-01-01,10:30:07.211.620, SSP NHI DEL:nhi=10.type=1.rc=0
[NO.70]1970-01-01,10:30:08.015.302, FDI IP NHI SET:nhIndex = 8210,arp = 0, vlan=4093,innervlan=0.flag=0.port=0,trunk=0.rc=0.
[NO.71]1970-01-01,10:30:08.015.360, SSP NHP SET INERN:nhi=11.port=0.hwnhi=8210.
[NO.72]1970-01-01,10:30:08.015.381, SSP NHI SET:nhi=11.type=1.port=272654336.if=543166460.mac=000E.5E66.000F.vlan=4093/0.flag=0.ucMasterActive=0.rc=0.
[NO.73]1970-01-01,10:30:08.016.405, FDI IPV6 SET:nhi = 8210,rc=0.
[NO.74]1970-01-01,10:30:08.016.439, SSP V6ROUTE UPDATE INERN:hwnhi=8210.
[NO.75]1970-01-01,10:30:08.016.459, BFD IPV6 CB:ipv6addr_BE=0xfe800000:00000000:020e5eff:fe66000f vrf=1 masklen=128 op=1
[NO.76]1970-01-01,10:30:08.016.500, SSP IP SET:vrf=1.ip=FE80::20E:5EFF:FE66:F/128.nhitype=1.nhi=11.flag=0.rc=0
[NO.77]1970-01-01,10:30:08.016.941, FDI IP NHI DEL:nhIndex = 8209,arp=0.rc=0.
[NO.78]1970-01-01,10:30:08.016.971, SSP NHP DEL INERN:nhi=85729.port=0.hwnhi=8209.
[NO.79]1970-01-01,10:30:08.016.999, SSP NHI DEL:nhi=85729.type=1.rc=0
[NO.80]1970-01-01,10:30:08.017.877, FDI IPV6 SET:nhi = 8210,rc=0.
[NO.81]1970-01-01,10:30:08.017.917, SSP V6ROUTE UPDATE INERN:hwnhi=8210.
[NO.82]1970-01-01,10:30:08.017.936, BFD IPV6 CB:ipv6addr_BE=0x00000000:00000000:020001ff:fe020304 vrf=1 masklen=128 op=1
[NO.83]1970-01-01,10:30:08.017.975, SSP IP SET:vrf=1.ip=::200:1FF:FE02:304/128.nhitype=1.nhi=11.flag=0.rc=0
[NO.84]1970-01-01,10:30:08.018.547, FDI IPV6 SET:nhi = 8210,rc=0.
[NO.85]1970-01-01,10:30:08.018.584, SSP V6ROUTE UPDATE INERN:hwnhi=8210.
[NO.86]1970-01-01,10:30:08.018.602, BFD IPV6 CB:ipv6addr_BE=0x00000000:00000000:020e5eff:fe161601 vrf=1 masklen=128 op=1
[NO.87]1970-01-01,10:30:08.018.642, SSP IP SET:vrf=1.ip=::20E:5EFF:FE16:1601/128.nhitype=1.nhi=11.flag=0.rc=0
[NO.88]1970-01-01,10:30:08.020.186, FDI IPV6 SET:nhi = 8210,rc=0.
[NO.89]1970-01-01,10:30:08.020.226, SSP V6ROUTE UPDATE INERN:hwnhi=8210.
[NO.90]1970-01-01,10:30:08.020.245, BFD IPV6 CB:ipv6addr_BE=0x00000000:00000000:020e5eff:fe660001 vrf=1 masklen=128 op=1
[NO.91]1970-01-01,10:30:08.020.287, SSP IP SET:vrf=1.ip=::20E:5EFF:FE66:1/128.nhitype=1.nhi=11.flag=0.rc=0
[NO.92]1970-01-01,10:30:08.021.781, FDI IPV6 SET:nhi = 8210,rc=0.
[NO.93]1970-01-01,10:30:08.021.812, SSP V6ROUTE UPDATE INERN:hwnhi=8210.
[NO.94]1970-01-01,10:30:08.021.831, BFD IPV6 CB:ipv6addr_BE=0x00000000:00000000:027800ff:fe020001 vrf=1 masklen=128 op=1
[NO.95]1970-01-01,10:30:08.021.872, SSP IP SET:vrf=1.ip=::278:FF:FE02:1/128.nhitype=1.nhi=11.flag=0.rc=0
[NO.96]1970-01-01,10:30:08.022.776, FDI IP NHI SET:nhIndex = 8209,arp = 0, vlan=4093,innervlan=0.flag=0.port=48,trunk=0.rc=0.
[NO.97]1970-01-01,10:30:08.022.822, SSP NHP SET INERN:nhi=85729.port=48.hwnhi=8209.
[NO.98]1970-01-01,10:30:08.022.840, SSP NHI SET:nhi=85729.type=1.port=272646144.if=541069308.mac=000E.5E66.0009.vlan=4093/0.flag=0.ucMasterActive=0.rc=0.
[NO.99]1970-01-01,10:30:08.023.340, FDI IPV6 SET:nhi = 8209,rc=0.
[NO.100]1970-01-01,10:30:08.023.368, SSP V6ROUTE UPDATE INERN:hwnhi=8209.
[NO.101]1970-01-01,10:30:08.023.385, BFD IPV6 CB:ipv6addr_BE=0xfe800000:00000000:020e5eff:fe660009 vrf=1 masklen=128 op=1
[NO.102]1970-01-01,10:30:08.023.447, SSP IP SET:vrf=1.ip=FE80::20E:5EFF:FE66:9/128.nhitype=1.nhi=85729.flag=0.rc=0
[NO.103]1970-01-01,10:30:08.023.885, FDI IP NHI DEL:nhIndex = 8208,arp=0.rc=0.
[NO.104]1970-01-01,10:30:08.024.099, SSP NHP DEL INERN:nhi=8.port=48.hwnhi=8208.
[NO.105]1970-01-01,10:30:08.024.142, SSP NHI DEL:nhi=8.type=1.rc=0
[NO.106]1970-01-01,10:30:19.336.560, BFD IPV4 CB:ipadd_BE=0x64010102 vrf=0 masklen=32 op=2
[NO.107]1970-01-01,10:30:19.336.690, FDI IPV4 DEL:rc=0.
[NO.108]1970-01-01,10:30:19.336.705, SSP ROUTE DEL INERN:hwnhi=8211.
[NO.109]1970-01-01,10:30:19.336.730, BFD IPV4 CB:ipadd_BE=0x64010102 vrf=0 masklen=32 op=3
[NO.110]1970-01-01,10:30:19.336.756, SSP IP DEL:vrf=0.ip=100.1.1.2/32.rc=0
[NO.111]1970-01-01,10:30:19.336.983, FDI IP NHI DEL:nhIndex = 8211,arp=0.rc=0.
[NO.112]1970-01-01,10:30:19.337.005, SSP NHP DEL INERN:nhi=85750.port=48.hwnhi=8211.
[NO.113]1970-01-01,10:30:19.337.027, SSP NHI DEL:nhi=85750.type=1.rc=0
[NO.114]1970-01-01,10:30:19.409.299, BFD IPV6 CB:ipv6addr_BE=0xfe800000:00000000:020e5eff:fe660009 vrf=1 masklen=128 op=2
[NO.115]1970-01-01,10:30:19.409.452, FDI IPV6 DEL:rc=0.
[NO.116]1970-01-01,10:30:19.409.467, SSP V6ROUTE DEL INERN:hwnhi=8209.
[NO.117]1970-01-01,10:30:19.409.491, BFD IPV6 CB:ipv6addr_BE=0xfe800000:00000000:020e5eff:fe660009 vrf=1 masklen=128 op=3
[NO.118]1970-01-01,10:30:19.409.524, SSP IP DEL:vrf=1.ip=FE80::20E:5EFF:FE66:9/128.rc=0
[NO.119]1970-01-01,10:30:19.409.755, FDI IP NHI DEL:nhIndex = 8209,arp=0.rc=0.
[NO.120]1970-01-01,10:30:19.409.778, SSP NHP DEL INERN:nhi=85729.port=48.hwnhi=8209.
[NO.121]1970-01-01,10:30:19.409.799, SSP NHI DEL:nhi=85729.type=1.rc=0
[NO.122]1970-01-01,10:30:47.394.189, FDI IPV4 SET:nhi = 2,rc=0.
[NO.123]1970-01-01,10:30:47.394.235, SSP ROUTE SET INERN:hwnhi=2.
[NO.124]1970-01-01,10:30:47.394.278, BFD IPV4 CB:ipadd_BE=0x64010101 vrf=0 masklen=32 op=0
[NO.125]1970-01-01,10:30:47.394.314, SSP IP SET:vrf=0.ip=100.1.1.1/32.nhitype=1.nhi=81630.flag=8.rc=0
iTN165-2G_8.106(debug)#   
iTN165-2G_8.106(debug)#
iTN165-2G_8.106(debug)#
```



