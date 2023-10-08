参考资料： [https://mp.weixin.qq.com/s/GoYDsfy9m0wRoXi_NCfCmg](https://mp.weixin.qq.com/s/GoYDsfy9m0wRoXi_NCfCmg)

收包之前Linux内核的准备工作：

1) 创建ksoftirqd进程，设置好自己用来处理软中断的线程函数，CPU的每个核都有自己的软中断处理线程。
2) 协议栈注册，比如arp，icmp，ip，tcp，udp等，每个协议都注册自己的处理函数。
3) 网卡驱动初始化，每个驱动都有一个初始化函数。内核初始化驱动的过程中，分配DMA，并注册poll函数处理收包，因特尔的igb网卡内核中注册的就是igb_poll函数。
4) 启动网卡，分配RX，TX队列，注册中断对应的处理函数。

上述步骤完成后就可以打开硬中断准备处理网卡接收的网络数据包了。

网卡收包->DMA方式存入内存->硬中断通知CPU->CPU响应硬中断，并发出软中断->ksoftirqd软中断进程处理软中断，调用网卡驱动注册的收包函数处理收包->数据包从ring buffer取出->协议栈处理报文->送到用户态应用程序处理
![](images/Pasted%20image%2020221201214844.png)
*   网卡将数据帧DMA到内存的RingBuffer中，然后向CPU发起中断通知
-   CPU响应中断请求，调用网卡启动时注册的中断处理函数
-   中断处理函数几乎没干啥，就发起了软中断请求
-   内核线程ksoftirqd线程发现有软中断请求到来，先关闭硬中断
-   ksoftirqd线程开始调用驱动的poll函数收包
-   poll函数将收到的包送到协议栈注册的ip_rcv函数中
-   ip_rcv函数再讲包送到udp_rcv函数中（对于tcp包就送到tcp_rcv）