
系统需要安装openbsd-inetd、telnetd，然后重启openbsd-inetd服务。
```
liubo@abo:~$ sudo apt-get install openbsd-inetd
liubo@abo:~$ sudo apt-get install telnetd 
liubo@abo:~$ sudo /etc/init.d/openbsd-inetd restart
[ ok ] Restarting openbsd-inetd （via systemctl)：openbsd-inetd.service.
liubo@abo:~$
liubo@abo:~$ sudo netstat -a | grep telnet
tcp    0    0 0.0.0.0:telnet    0.0.0.0:*    LISTEN
liubo@abo:~$
```

