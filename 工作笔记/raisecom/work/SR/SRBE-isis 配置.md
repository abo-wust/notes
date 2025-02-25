
## 三台设备拓扑

```
                     -----                             -----                             -----
                    |     |                           |     |                           |     |
           gi 1/3/2 |     |   g 1/1/2        g 1/1/2  |     |  g 1/2/2         g 1/2/2  |     |  g 1/3/8
        ----------->| 106 | ------------------------> | 107 | ------------------------> | 102 | ----------->
                    |     |                           |     |                           |     |            
                    |     |                           |     |                           |     |            
                     -----                             -----                             -----             
```

### 106 设备

#### 配置
```
mpls lsr-id 106.106.106.106
mpls enable

ip vrf v1
rd 1:1
tunnel-policy sr
static local-label 106
route-target import 1:1
route-target export 1:1
exit

router isis 1
net 49.0000.0000.0000.0106.00
metric-style wide
segment-routing mpls
segment-routing global-block 10241 10999
exit

tunnel-policy sr
tunnel select-seq sr-be-lsp
exit

segment-routing
global-block 11000 12000
ipv4 adjacency local-ip-addr 100.1.1.1 remote-ip-addr 100.1.1.2 sid 40001
exit

interface loopback 2
ip address 106.106.106.106 255.255.255.255
ip router isis 1
isis prefix-sid index 10
exit

interface mtn-veth 1
bind fgchannel 1
ip address 100.1.1.1 255.255.255.0
ip router isis 1
exit

interface gigaethernet 1/3/2
mode l2
exit

interface gigaethernet 1/3/2.100
encapsulation dot1Q 100
ip vrf forwarding v1
ip address 120.1.1.1 255.255.255.0
exit

ip route vrf v1 120.1.2.0 255.255.255.0 vpn-label 102 vpn-peer 102.102.102.102


```

#### 删除
```
no interface gigaethernet 1/3/2.100

no interface mtn-veth 1

segment-routing
no global-block
no tunnel-prefer segment-routing
no ipv4 adjacency local-ip-addr 100.1.1.1 remote-ip-addr 100.1.1.2 sid 40001
exit

no interface loopback 2

no tunnel-policy sr
no router isis 1

no ip vrf v1


```



### 107 设备
#### 配置
```
mpls lsr-id 107.107.107.107
mpls enable

router isis 1
net 49.0000.0000.0000.0107.00
metric-style wide
segment-routing mpls
segment-routing global-block 10241 10999
exit


interface loopback 2
ip address 107.107.107.107 255.255.255.255
ip router isis 1
isis prefix-sid index 20
exit

segment-routing
global-block 11000 12000
ipv4 adjacency local-ip-addr 100.1.1.2 remote-ip-addr 100.1.1.1 sid 40001
ipv4 adjacency local-ip-addr 100.1.2.2 remote-ip-addr 100.1.2.1 sid 40002
exit

interface mtn-veth 1
bind fgchannel 1
ip address 100.1.1.2 255.255.255.0
ip router isis 1
exit

interface gigaethernet 1/2/2
mode l2
exit

interface gigaethernet 1/2/2.100
encapsulation dot1Q 100
mode l3
ip address 100.1.2.2 255.255.255.0
ip router isis 1
exit


```

#### 删除
```
no interface gigaethernet 1/2/2.100

no interface mtn-veth 1

segment-routing
no tunnel-prefer segment-routing
no ipv4 adjacency local-ip-addr 100.1.1.2 remote-ip-addr 100.1.1.1 sid 40001
no ipv4 adjacency local-ip-addr 100.1.2.2 remote-ip-addr 100.1.2.1 sid 40001
exit

no interface loopback 2

no router isis 1


```


### 102 设备
#### 配置
```
mpls lsr-id 102.102.102.102
mpls enable

ip vrf v1
rd 1:1
tunnel-policy sr
static local-label 102
route-target import 1:1
route-target export 1:1
exit

router isis 1
net 49.0000.0000.0000.0102.00
metric-style wide
segment-routing mpls
segment-routing global-block 10241 10999
exit

tunnel-policy sr
tunnel select-seq sr-be-lsp
exit

segment-routing
global-block 11000 12000
ipv4 adjacency local-ip-addr 100.1.2.1 remote-ip-addr 100.1.2.2 sid 40002
exit

interface loopback 20
ip address 102.102.102.102 255.255.255.255
ip router isis 1
isis prefix-sid index 30
exit

interface gigaethernet 1/2/2
mode l2
exit

interface gigaethernet 1/2/2.100
encapsulation dot1Q 100
mode l3
ip address 100.1.2.1 255.255.255.0
ip router isis 1
exit


interface gigaethernet 1/3/8.100
encapsulation dot1Q 100
mode l3
ip vrf forwarding v1
ip address 120.1.2.1 255.255.255.0
exit

ip route vrf v1 120.1.1.0 255.255.255.0 vpn-label 106 vpn-peer 106.106.106.106


```

#### 删除
```
no interface gigaethernet 1/3/8.100

segment-routing
no tunnel-prefer segment-routing
no ipv4 adjacency local-ip-addr 100.1.2.1 remote-ip-addr 100.1.2.2 sid 40002
exit

no interface loopback 20

no tunnel-policy sr
no router isis 1

no ip vrf v1


```

