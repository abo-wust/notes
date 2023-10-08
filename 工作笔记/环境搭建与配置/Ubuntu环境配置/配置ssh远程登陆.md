
首先telnet到远程的Linux系统，在Linux上安装lrzsz：
```
$ sudo apt install lrzsz
    
```

然后再本地的windows/Linux系统上生成密钥文件，下面的命令在Windows（power shell）和Linux下通用，注意在Windows下用该命令生成的公钥，要删除最后的字符串“username@xxx-pc”：
```
$ ssh-keygen -t rsa -b 4096
```

如果是本地系统是Windows，在secureCRT的telnet中输入rz命令，将本地生成的公钥id_rsa.pub上传到Linux的/home/liubo/.ssh/目录，然后将公钥重定向到authorized_keys文件中，并删除上传的id_rsa.pub公钥文件，
```
$ cat id_rsa.pub > authorized_keys
$ rm id_rsa.pub
```

如果不用rz命令，可以使用scp命令在power shell下从Windows传输公钥文件到远程Linux：
```
scp C:\Users\campn\.ssh\id_rsa.pub liubo@192.168.0.103:/home/liubo/.ssh
```

查看sshd是否启动
```
$ ps -e | grep ssh
6441 ?        00:00:00 ssh-agent
14634 ?        00:00:00 sshd
```

如果sshd没有启动，是因为Ubuntu系统自带openssh-client，服务端需要自行安装
```
$ sudo apt-get install openssh-server
Reading package lists... Done
Building dependency tree       
Reading state information... Done
Some packages could not be installed. This may mean that you have
requested an impossible situation or if you are using the unstable
distribution that some required packages have not yet been created
or been moved out of Incoming.
The following information may help to resolve the situation:


The following packages have unmet dependencies:
openssh-server : Depends: openssh-client (= 1:7.6p1-4)
                  Depends: openssh-sftp-server but it is not going to be installed
                  Recommends: ssh-import-id but it is not going to be installed
E: Unable to correct problems, you have held broken packages.
```

安装服务端口openssh-server时可能出现上面的错误，从报错的打印中可以知道是服务端依赖的客户端版本不一致导致的，重新安装上面指定的依赖版本，然后再安装服务端
```
$ sudo apt-get install openssh-client=1:7.6p1-4
$
$ sudo apt-get install openssh-server
$ 
$ dpkg -l | grep openssh-client
ii  openssh-client                             1:7.6p1-4                                    amd64        secure shell (SSH) client, for secure access to remote machines
```

安装openssh-server完成之后，重启sshd服务
```
$ systemctl restart sshd
```

然后就可以使用ssh命令远程登陆Linux了，或者使用串口软件新建连接，选择本地的私钥文件登陆：
```
ssh username@ip_addr
```

按照需求修改sshd的配置：
```
vim /etc/ssh/sshd_config
```

