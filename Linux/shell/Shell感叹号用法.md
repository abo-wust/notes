
### 执行上一条命令

如果想执行上次的命令，我们除了可以在控制台输入方向键向上的按键外，还可以使用`!!`来执行上一次命令：

```
[root@iZ2zecnsfqf16bnmk6lrd1Z ~]# ls
dump.rdb  files  test.sh  tmp
[root@iZ2zecnsfqf16bnmk6lrd1Z ~]# !!
ls
dump.rdb  files  test.sh  tmp
```

如果我们想查看一个文件内容，但是可能忘记输入其他的管道命令，我们可以如下来使用：

```
# 忘记输入其他命令
[root@iZ2zecnsfqf16bnmk6lrd1Z ~]# cat 111.txt 
1
2
3
4
# 这一步可以补上
[root@iZ2zecnsfqf16bnmk6lrd1Z ~]# !! |grep '2'
cat 111.txt  |grep '2'
2
```

是不是特别的方便。

### 使用第一个参数

有时候我们输入了一长串的命令，有可能输入了很长的参数，然后再执行下一个命令的时候需要执行上一次的第一个参数，这个时候我们使用`!^`，如下所示：

```
[root@iZ2zecnsfqf16bnmk6lrd1Z ~]# ls /root/tmp/ttt/temp.txt -al
-rw-r--r-- 1 root root 6 Dec  1 10:20 /root/tmp/ttt/temp.txt
[root@iZ2zecnsfqf16bnmk6lrd1Z ~]# cat !^
cat /root/tmp/ttt/temp.txt
Hello
```

### 使用最后一个参数

与上面的相反，我们也可以使用`!$`来获取上一个命令的最后一个参数，如下所示：

```
[root@iZ2zecnsfqf16bnmk6lrd1Z ~]# cat -A /root/tmp/ttt/temp.txt
Hello$
[root@iZ2zecnsfqf16bnmk6lrd1Z ~]# ls -al !$
ls -al /root/tmp/ttt/temp.txt
-rw-r--r-- 1 root root 6 Dec  1 10:20 /root/tmp/ttt/temp.txt
```

### 去掉最后一个参数

如果想去掉上一个命令的最后一个参数，可以使用`!:-`，如下所示：

```
[root@iZ2zecnsfqf16bnmk6lrd1Z ~]# cat /root/tmp/ttt/temp.txt -A
Hello$
[root@iZ2zecnsfqf16bnmk6lrd1Z ~]# !:-
cat /root/tmp/ttt/temp.txt
Hello
```

_注意：这个指令会将命令也会输出。_

### 使用所有的参数

有时候我们不小心将命令输入错误了，但是又不想重复的输入，或者使用退格键按半天，那么就可以使用`!*`来获取所有参数，重新输入正确的指令，如下所示：

```
[root@iZ2zecnsfqf16bnmk6lrd1Z ~]# sl -al /root/tmp/ttt/temp.txt
-bash: sl: command not found
[root@iZ2zecnsfqf16bnmk6lrd1Z ~]# ls !*
ls -al /root/tmp/ttt/temp.txt
-rw-r--r-- 1 root root 6 Dec  1 10:20 /root/tmp/ttt/temp.txt
```

### 获取指定位置的参数

如果想获取到上一个命令指定第N个参数，可以使用`!:N`，如下所示：

```
[root@iZ2zecnsfqf16bnmk6lrd1Z ~]# ls /root/tmp/ttt/temp.txt -a -l
-rw-r--r-- 1 root root 6 Dec  1 10:20 /root/tmp/ttt/temp.txt
# 去除所有参数
[root@iZ2zecnsfqf16bnmk6lrd1Z ~]# !:0
ls
111.txt  dump.rdb  files  test.sh  tmp
# 获取第一个参数
[root@iZ2zecnsfqf16bnmk6lrd1Z ~]# ls !:1
ls /root/tmp/ttt/temp.txt
/root/tmp/ttt/temp.txt
```

也可以匹配最近指令的第N个参数，使用`!command:N`，如下使用：

```
[root@iZ2zecnsfqf16bnmk6lrd1Z ~]# ls /root/tmp/ttt/temp.txt -a -l
-rw-r--r-- 1 root root 6 Dec  1 10:20 /root/tmp/ttt/temp.txt
[root@iZ2zecnsfqf16bnmk6lrd1Z ~]# cat /root/tmp/ttt/temp.txt -A
Hello$
[root@iZ2zecnsfqf16bnmk6lrd1Z ~]# ls !ls:3
ls -l
total 24
-rw-r--r-- 1 root root    8 Dec  1 10:11 111.txt
-rw-r--r-- 1 root root 4851 Nov 20 21:42 dump.rdb
drwxr-xr-x 2 root root 4096 Sep 20 16:21 files
```

相当于查找上一次`ls`命令的参数了。

如果想获取多个参数的话，可以使用`!:N-M`，如下所示：

```
[root@iZ2zecnsfqf16bnmk6lrd1Z ~]# cat /root/tmp/ttt/test.txt -A -b
     1  Shell$
[root@iZ2zecnsfqf16bnmk6lrd1Z ~]# cat 111.txt !:2-3
cat 111.txt -A -b
     1  1$
     2  2$
     3  3$
     4  4$
```

还有下面的使用方法：

```
# 获取从第1个参数开始所有参数
[root@iZ2zecnsfqf16bnmk6lrd1Z ~]# cat !:1*
cat 111.txt -A -b
     1  1$
     2  2$
     3  3$
     4  4$
# 上同
[root@iZ2zecnsfqf16bnmk6lrd1Z ~]# cat !:1-$
cat 111.txt -A -b
     1  1$
     2  2$
     3  3$
     4  4$
# 
[root@iZ2zecnsfqf16bnmk6lrd1Z ~]# cat !:1-
cat 111.txt -A
1$
2$
3$
4$
```

### 删除尾路径和前导路径

我们可以移除路径的文件名，可以使用`!:$:h`，如下所示：

```
[root@iZ2zecnsfqf16bnmk6lrd1Z ~]# ls /root/tmp/ttt/test.txt
/root/tmp/ttt/test.txt
[root@iZ2zecnsfqf16bnmk6lrd1Z ~]# ls -l !:$:h 
ls -l /root/tmp/ttt
total 8
-rw-r--r-- 1 root root 6 Dec  1 10:20 temp.txt
-rw-r--r-- 1 root root 6 Dec  1 11:11 test.txt
```

想删除掉文件后缀，可以使用`!:$:r`，如下所示：

```
[root@iZ2zecnsfqf16bnmk6lrd1Z ~]# ls /root/111.txt 
/root/111.txt
[root@iZ2zecnsfqf16bnmk6lrd1Z ~]# ls -l !:$:r
ls -l /root/111
total 4
-rw-r--r-- 1 root root 3 Dec  1 13:57 hello.txt
```

### 使用history中的命令

`history`命令可以查看之前执行过的命令，除了使用方向键外，还可以使用感叹号`!N`，如下所示：

```
[root@iZ2zecnsfqf16bnmk6lrd1Z ~]# history
...
 1083  ls -l
 1084  ls /root/tmp/ttt/temp.txt -a -l
 1085  cat /root/tmp/ttt/temp.txt -A
 1086  ls -l
 1087  history |more
 1088  history
# 执行第1086条命令
[root@iZ2zecnsfqf16bnmk6lrd1Z ~]# !1086
ls -l
total 24
-rw-r--r-- 1 root root    8 Dec  1 10:11 111.txt
-rw-r--r-- 1 root root 4851 Nov 20 21:42 dump.rdb
drwxr-xr-x 2 root root 4096 Sep 20 16:21 files
```

### 执行倒数第N条命令，可以使用`!-N`，如下所示：

```
[root@iZ2zecnsfqf16bnmk6lrd1Z ~]# cat /root/tmp/ttt/temp.txt -A
Hello$
[root@iZ2zecnsfqf16bnmk6lrd1Z ~]# ls /root/tmp/ttt/temp.txt -a
/root/tmp/ttt/temp.txt
[root@iZ2zecnsfqf16bnmk6lrd1Z ~]# !-2
cat /root/tmp/ttt/temp.txt -A
Hello$
```

### 执行上次指定关键字开头的命令

如果想执行上一条以指定开头的命令，可以使用`!command`，如下所示：

```
[root@iZ2zecnsfqf16bnmk6lrd1Z ~]# cat /root/tmp/ttt/temp.txt -A
Hello$
[root@iZ2zecnsfqf16bnmk6lrd1Z ~]# ls /root/tmp/ttt/temp.txt -a 
/root/tmp/ttt/temp.txt
[root@iZ2zecnsfqf16bnmk6lrd1Z ~]# !cat
cat /root/tmp/ttt/temp.txt -A
Hello$
```

### 执行包含指定关键字的命令

如果想执行上次包含指定关键字的命令，可以使用`!?command?`，如下所示：

```
[root@iZ2zecnsfqf16bnmk6lrd1Z ~]# cat /root/tmp/ttt/temp.txt -A
Hello$
[root@iZ2zecnsfqf16bnmk6lrd1Z ~]# ls /root/tmp/ttt/temp.txt -a 
/root/tmp/ttt/temp.txt
[root@iZ2zecnsfqf16bnmk6lrd1Z ~]# !?at?
cat /root/tmp/ttt/temp.txt -A
Hello$
```

我们还可以使用`%`来获取`!?command`匹配的单词，如下所示：

```
[root@iZ2zecnsfqf16bnmk6lrd1Z ~]# ls /root/tmp/ttt/temp.txt    
/root/tmp/ttt/temp.txt
[root@iZ2zecnsfqf16bnmk6lrd1Z ~]# !?temp
ls /root/tmp/ttt/temp.txt
/root/tmp/ttt/temp.txt
[root@iZ2zecnsfqf16bnmk6lrd1Z ~]# cat !%
cat /root/tmp/ttt/temp.txt
Hello
```

### 替换上次指令中的关键字

有时候我们输入了指令想重复执行，但是又想替换掉其中某些字符，可以使用`!:gs/old/new`，如下所示：

```
[root@iZ2zecnsfqf16bnmk6lrd1Z ~]# cat /root/tmp/ttt/temp.txt -A
Hello$
[root@iZ2zecnsfqf16bnmk6lrd1Z ~]# !:gs/temp/test/
cat /root/tmp/ttt/test.txt -A
Shell$
```

如果已经成功执行了如上所示的替换，则可以使用`!:g&`再次快速重复相同的替换。

```
[root@iZ2zecnsfqf16bnmk6lrd1Z ~]# tar -zcvf temp.tar.gz /root/tmp/ttt/test.txt 
tar: Removing leading `/' from member names
/root/tmp/ttt/test.txt
[root@iZ2zecnsfqf16bnmk6lrd1Z ~]# !:g&
tar -zcvf test.tar.gz /root/tmp/ttt/test.txt 
tar: Removing leading `/' from member names
/root/tmp/ttt/test.txt
[root@iZ2zecnsfqf16bnmk6lrd1Z ~]# ls
```

### 只查看不执行命令

有时候我们不确定使用感叹号之后获得命令是否正确，可以使用`:p`来查看替换后的命令：

```
[root@iZ2zecnsfqf16bnmk6lrd1Z ~]# tar cvf home-dir.tar 111.txt test.sh 
111.txt
test.sh
[root@iZ2zecnsfqf16bnmk6lrd1Z ~]# tar cvfz new-file.tar !tar:3-:p
tar cvfz new-file.tar 111.txt
[root@iZ2zecnsfqf16bnmk6lrd1Z ~]# ls
111  111.txt  dump.rdb  files  home-dir.tar  test.sh  tmp
```

如上可以看到，真正的压缩命令并没有执行。

### 总结

我们这里将上面所有的使用方法在此总结到一个表格中，方便大家查阅使用方法：

|命令|说明|
|----|--|
|!! | 执行上次命令|
|!^ | 使用上次命令的第一个参数|
|!$ | 使用上次命令的最后一个参数|
|!:- | 使用上次命令（包含命令），但去掉最后一个参数|
|!* | 使用上次命令的所有参数|
|!:N | 使用上次命令的第N个参数|
|!command | 使用上次指令开头的命令|
|!command:N | 使用上次指定命令的第N个参数（也可以用于上述命令组合，如!command:^）|
|!:N-M | 使用上次命令的第N-M的参数|
|!:1* | 使用上次命令的第1个参数开始的所有参数|
|!:1-$ | 上同，使用上次命令的第1个参数到最后一个参数|
|!:1- | 使用上次命令的第1个参数到最后第二个参数的之间的所有参数|
|!:$:h | 使用上次命令的最后一个参数并移除尾随路径名|
|!:$:r | 使用上次命令的最后一个参数并移除后缀名|
|!N | 使用history中的第N个命令|
|!-N | 使用history中的最后第N个命令|
|!?command? | 使用上次指定关键词的命令|
|!% |使用上次!?command匹配的完整单词|
|!:gs/old/new | 替换上次命令中的关键词|
|!:g& | 如果上次替换成功，则使用上次替换的规则替换上一个命令中的关键词|
|:p |只查看并不执行感叹号替换后的命令（tar cvfz aaa.tar !tar:3-:p 并不会被执行）|



