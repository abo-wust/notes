

## 问题

在Windows10上使用terminal远程到Linux之后，敲backspace键会发出Windows上常见的提示声音，比较烦人。

## 解决方法：

关闭提示声的方法：
（1）远程登陆后输入命令：bind "set bell-style none"。注意这种方法只对本次连接有效，断开远程重新连接登陆后，提示音仍然存在，如何在远程登陆之后默认就关掉提示音看第二种方法。
```
$ bind "set bell-style none"
```

（2）远程登陆后在用户目录下创建文件.inputrc，将方法1种的命令(set bell-style none)输入并保存，然后断开远程重新连接登陆，再敲backspace键后就不会发出提示音了。
```
$  pwd
/home/abo
$ vim .inputrc
```

切换到root权限后，仍然会有提示音，在/root/目录下同样创建.inputrc文件，并保存set bell-style none即可。

（3）用vim编辑文件时敲击backspace或者CTRL+C等组合键仍然会有提示音，修改vim的配置文件（如果系统有创建配置文件）就行。
```
$ vim .vim/vimrc

set visualbell
set t_vb=
```

