
系统刚刚安装时，ifconfig命令无法使用
```
$ ifconfig

Command 'ifconfig' not found, but can be installed with:

sudo apt install net-tools
```

安装时可能会出现dpkg的问题，按提示修复即可：
```
$ sudo apt install net-tools
E: dpkg was interrupted, you must manually run 'sudo dpkg --configure -a' to correct the problem.
```

注意：可能需要先更新软件包库
```
sudo apt update
```


