
### SCL（Service Classfication List）

在芯片中对应`UserId`模块

ingress方向查表：
![](images/Pasted%20image%2020230522111239.png)

egress方向查表：
![](images/Pasted%20image%2020230522111949.png)



### Nexthop



### Ad index



### UDF

`UDF`缩写：`User Defined Field`

这个模块的目的是获取到自定义位置的报文信息，供后续的逻辑（如SCL或ACL）等使用。UDF可以用于SCL、IPfix、FIB、Hash、Acl，与之密切相关的模块还包括Parser、MPLS和Tunnel。总的逻辑如图：

![](images/Pasted%20image%2020230519162931.png)




























