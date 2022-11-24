1.新建一个监控用户（monitor）
    [root@mylab ~]# useradd monitor
    [root@mylab ~]# passwd monitor

2.配置sudo
    [root@mylab ~]# cp /etc/sudoers /etc/sudoers.bak.`date +'%F-%T'`
    [root@mylab ~]# echo "monitor ALL=(root) NOPASSWD: /usr/sbin/dmidecode" >> /etc/sudoers
    [root@mylab ~]# vim /etc/sudoers
    # Defaults    requiretty
    将上面行注释掉

3.上传脚本文件至/home/monitor
    mail.py #发送邮件脚本
    monitor.sh #监控脚本

4.配置文件说明
    mail.py #调用第三方邮箱发送邮件
        以QQ邮件为例修改mail.py文件：（其他平台修改参数即可,一般公司都有邮件接口或者自己内网搭建一台邮件服务器）
        gserver = 'smtp.qq.com'
        gport = 465
        sendqqmail('123456','gkhcvklnjyjsbgic','123456@qq.com',to,subject,content)
            QQ邮箱用户名    IMAP/SMTP第三方登录授权码（QQ邮箱设置生成）QQ邮箱

    monitor.sh #可监控负载、CPU、内存、SWAP、硬盘、网卡流量、网站PV/UV(需要根据需求修改)
        ###Mailbox configuration###
        ACCENT_MAIL="123456@126.com" #告警接受邮箱

        告警阀值，可根据需要修改
        ###The alarm threshold###
        LOAD_AVERAGE_ALARM=2 #15分钟内的平均负载，参考：一般小于机器物理CPU个数
        CPU_ALARM=70 #CPU使用率
        MEMORY_ALARM=70 #内存使用率
        SWAP_ALARM=30 #虚拟内存使用率
        ROOT_PART_ALARM=70 #/挂载点硬盘使用率
        IN_NETWORK_TRAFFIC_ALARM=167772160 #网卡每分钟接收流量20M
        OUT_NETWORK_TRAFFIC_ALARM=167772160 #网络每分钟发送流量20M

        应用日志路径
        ###Log variable###
        ACCESS_LOG_PATH=localhost_access_log.${BEFORE_DAY}.txt

    Monitor_history.log #日志历史记录

5.定时发送邮件
    ####Timed task###
    TIMING=08 #每天08点发生邮件


6.添加自动任务
    [monitor@mylab ~]$ crontab  -e
    1 * * * *  /bin/sh  -c  "/home/monitor/monitor.sh &>/dev/null" #每个小时的第一分钟执行一次