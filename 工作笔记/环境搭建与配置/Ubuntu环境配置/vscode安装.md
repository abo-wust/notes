
在官网 https://code.visualstudio.com/ 下选择Linux版本，根据指导安装即可。    

使用命令行安装的方法：

1. 将vscode的源添加到系统
```
$ echo "deb [arch=amd64] http://packages.microsoft.com/repos/vscode stable main" | sudo \
   tee /etc/apt/sources.list.d/vs-code.list
```

2. 然后将密钥导入到系统
```
$ curl https://packages.microsoft.com/keys/microsoft.asc | gpg --dearmor > microsoft.gpg
$ sudo mv microsoft.gpg /etc/apt/trusted.gpg.d/microsoft.gpg
```

3. 最后更新软件库，安装vscode
```
$ sudo apt update
$ sudo apt install code
```

