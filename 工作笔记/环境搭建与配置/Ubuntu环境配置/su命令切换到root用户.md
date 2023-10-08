在Terminal中使用su命令切换到root时会提示

```
$ su
Password:
su: Authentication failure
```

原因是系统默认以普通用户登陆，root账户是未启用的，可以使用sodo -i命令来切换到root用户，但是sudo -i切换到root的路径是/root，而su命令切换到root的路径是/home/username（这里的username是系统普通用户名）。

解决方法是输入sudo passwd root，然后按提示设置密码就行了。

  
```
$ sudo passwd root
[sudo] password for _username_ :
Enter new UNIX password :
Retype new UNIX password :
passswd: password updated successfully
$
```