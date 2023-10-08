
在Ubuntu 18.04中安装tftp

1、安装tftp客户端和服务端
```
$ sudo apt install tftp-hpa
$ sudo apt install tftpd-hpa
$ sudo apt install xinetd
```

2、配置文件
```
$ sudo vim /etc/xinetd.d/tftp
service tftp
{
        socket_type     = dgram
        protocol        = udp
        wait            = yes
        user            = huahuan                    #Ubuntu系统的用户名
        server          = /home/huahuan/work/tftp    #tftp服务器的下载目录，要下载到设备的文件都保存在该目录下
        server_args     = -s /tftp -c
        disable         = no
        per_source      = 11
        cps             = 100 2
        flags           = IPv4
}

$ sudo vim /etc/default/tftpd-hpa
# /etc/default/tftpd-hpa

TFTP_USERNAME="tftp"
TFTP_DIRECTORY="/home/huahuan/work/tftp"
TFTP_ADDRESS=":69"
TFTP_OPTIONS="--secure"
```

3、创建服务器的下载目录
```
$ sudo mkdir /home/huahuan/work/tftp
$ sudo chmod -R 777 tftp
```

4、重启tftp服务
```
$ sudo service tftpd-hpa restart
$ sudo /etc/init.d/xinetd restart
```

重启tftpd-hpa可能会出现错误，解决方法先停止xinetd服务，再先后启动tftpd-hpa服务和xinetd服务。
```
$ sudo service tftpd-hpa restart
Job for tftpd-hpa.service failed because the control process exited with error code.
See "systemctl status tftpd-hpa.service" and "journalctl -xe" for details.

$ /etc/init.d/xinetd stop
[ ok ] Stopping xinetd (via systemctl): xinetd.service.
$ /etc/init.d/tftpd-hpa start
[ ok ] Starting tftpd-hpa (via systemctl): tftpd-hpa.service.

$ netstat -a | grep tftp
udp        0      0 0.0.0.0:tftp            0.0.0.0:*                          
udp6       0      0 [::]:tftp               [::]:*
```

5、测试
检查能否ping通设备，如果有防火墙则关闭
```
$ sudo ufw disable
```

检查tftp服务是否启动，有如下显示说明已经启动
```
$ netstat -a|grep tftp
udp        0      0 0.0.0.0:tftp            0.0.0.0:*                          
udp6       0      0 [::]:tftp               [::]:*                          
```

在tftp服务器的下载目录创建一个文件，然后切到其他目录下测试，如果/home/huahuan/work目录下有test_server.txt，并且文件内容正确，说明tftp下载能正常使用。
```
$ pwd
/home/huahuan/work/tftp
$ vim test_server.txt
$ cd ../
$ pwd
$ /home/huahuan/work
$ 
$ ifconfig enp3s0
enp3s0: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 192.168.10.123  netmask 255.255.255.0  broadcast 192.168.10.255
        inet6 fe80::2e0:4cff:fe99:1542  prefixlen 64  scopeid 0x20<link>
        ether 00:e0:4c:99:15:42  txqueuelen 1000  (Ethernet)
        RX packets 22  bytes 4571 (4.5 KB)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 138  bytes 14564 (14.5 KB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

$ tftp 192.168.10.123                //或者直接用 tftp localhost 也可以
tftp>get test_server.txt
tftp>q

$ pwd
/home/huahuan/work
```

测试上传功能
```
$ vim test.txt
$ 
$ tftp 192.168.10.123
tftp> put test.txt
Error code 1: File not found
tftp>
```

出现这种错误的原因是tftp服务器的下载目录当中必须已经存在test.txt文件，并且有读写的权限，否则还会报权限不够的错误
```
$ tftp 192.168.10.123
tftp> put test.txt
Error code 0: Permission denied
tftp>
```

所以先切到tftp的服务器下载目录创建一个test.txt文件，并给最大的权限，然后再切回其他的目录进行测试
```
$ cd /home/huahuan/work/tftp
$ vim test.txt
$ chmod 777 test.txt
$
$ cd ..
$
$ tftp 192.168.10.123
tftp> put test.txt
tftp>
```


如果get出现transfer timed out，重启tftp服务
```
$ sudo service tftpd-hpa restart
$ sudo /etc/init.d/xinetd reload
$ sudo /etc/init.d/xinetd restart
```

如果还不行，杀掉xinetd进程再重启tftp服务
```
$ netstat -pan | grep 69
```


