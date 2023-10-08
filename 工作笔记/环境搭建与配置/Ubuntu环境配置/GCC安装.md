
Ubuntu 18.04库中默认可用的gcc版本是7.4.0，安装前需要先更新软件库。
```
liubo@abo:~$ sudo apt-get update
liubo@abo:~$ 
liubo@abo:~$ sudo apt install gcc
liubo@abo:~$ gcc --version
gcc (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0
Copyright (C) 2017 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

liubo@abo:~$
```

gcc目前可用的版本已经更新到9.2，目前7.4版本满足开发使用。