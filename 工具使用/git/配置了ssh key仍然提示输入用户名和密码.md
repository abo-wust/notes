
查看本地的`/home/liubo/.ssh/id_rsa.pub`和远程服务器上已经添加的`SSH Key`没有问题，但是每次使用`git pull`或者`git push`时都提示输入用户名和密码。

1、第一种情况
`git`拉取项目时使用了`http`协议访问远程仓库，将远程地址换成`SSH`协议的地址

查看配置使用的协议
```
# git remote -v
origin  http://192.168.34.231/soft3_ctc/product_esw_ritp(fetch)
origin  http://192.168.34.231/soft3_ctc/product_esw_ritp(push)
```

确认是使用了`http`协议后，修改为`SSH`协议即可
```
# git config remote.origin.url git@192.168.34.231/soft3_ctc/product_esw_ritp.git
# git remote -v
origin  git@192.168.34.231/soft3_ctc/product_esw_ritp(fetch)
origin  git@192.168.34.231/soft3_ctc/product_esw_ritp(push)
```

2、第二种情况
如果查看`git`使用的是`SSH`协议，那么直接把服务器上配置的`SSH Key`删掉，本地的密钥也删掉，重新生成后添加到服务器端。






