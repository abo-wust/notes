
18.04版本默认安装了python3.x，python2需要自行安装，安装完成之后查看python默认的版本
```
$ sudo apt install python
$ python --version
Python 2.7.15+
```

切换python版本默认为python3
```
$ sudo update-alternatives --install /usr/bin/python python /usr/bin/python2 100
$ sudo update-alternatives --install /usr/bin/python python /usr/bin/python3 150
$ python --version
Python 3.6.8
```

切换默认版本到python2，前面有星号的就是当前的默认版本，想切换到python2，输入对应的数字即可。之后就可以用这个命令来切换版本了。
```
$ sudo update-alternatives --config python
There are 2 choices for the alternative python (providing /usr/bin/python).


  Selection    Path              Priority   Status
------------------------------------------------------------
* 0            /usr/bin/python3   150       auto mode
  1            /usr/bin/python2   100       manual mode
  2            /usr/bin/python3   150       manual mode


Press <enter> to keep the current choice[*], or type selection number:
```

