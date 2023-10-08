### if 语句语法格式

```shell
if condition
then

    command1

    command2
    ...
    commandN
fi
```

写成一行（适用于终端命令提示符）：
```shell
if [ $(ps -ef | grep -c "ssh") -gt 1 ]; then echo "true"; fi
```

### if else语法格式
```shell
if condition

then
    command1
    command2
    ...
    commandN
else
    command
fi
```

注意，如果`else`分支没有语句执行，也就是`else`下面没有`command`这一语句，就不用写`else`，直接使用`if`语句。

### if else-if else 语法格式

```shell
if condition1
then
    command1
elif condition2
then
    command2
else
    commandN
fi 
```

if else 的 [...] 判断语句中大于使用 -gt，小于使用 -lt。
```shell
if [ "$a" -gt "$b" ]; then
    ...
fi
```

如果使用 `((...))` 作为判断语句，大于和小于可以直接使用 > 和 <。
```shell
if (( a > b )); then
    ...
fi
```

以下实例判断两个变量是否相等：
```shell
a=10
b=20

if [ $a == $b ]
then
    echo "a 等于 b"
elif [ $a -gt $b ]
then
    echo "a 大于 b"
elif [ $a -lt $b ]
then
    echo "a 小于 b"
else
    echo "没有符合的条件"
fi
```

输出结果：
```
a 小于 b 
```

**使用 ((...)) 作为判断语句：**

```shell
a=10
b=20

if (( $a == $b ))
then
    echo "a 等于 b"
elif (( $a > $b ))
then
    echo "a 大于 b"
elif (( $a < $b ))
then
    echo "a 小于 b"
else
    echo "没有符合的条件"
fi
```

输出结果：
```
a 小于 b
```

**if else 语句经常与 test 命令结合使用**，如下所示：
```shell
num1=$[2*3]
num2=$[1+5]

if test $[num1] -eq $[num2]
then
    echo '两个数字相等!'
else
    echo '两个数字不相等!'
fi
```

输出结果：
```
两个数字相等!
```

### 多个条件如何使用，两种方式

方式一：
```shell
if [ 10 -gt 5 -o 10 -gt 4 ];then
     echo "10>5 or 10>4"
fi 
```

方式二：
```shell
if [ 10 -gt 5 ] || [ 10 -gt 4 ];then
     echo "10>5 or 10>4"
fi
```

其中，-o或者||表示或。这里也有一些常见的条件判定。

总结如下：（笔记：[布尔运算符和逻辑运算符]()）
- -o or 或者，同||
- -a and 与，同&&
- ! 非

整数判断：（笔记：[关系运算符]()）
- -eq 两数是否相等
- -ne 两数是否不等
- -gt 前者是否大于后者（greater then）
- -lt 前面是否小于后者（less than）
- -ge 前者是否大于等于后者（greater then or equal）
- -le 前者是否小于等于后者（less than or equal） *

字符串判断str1 exp str2：（笔记：[字符串运算符]()）
- -z "$str1" str1是否为空字符串
- -n "$str1" str1是否不是空字符串
- "\$str1" == "$str2" str1是否与str2相等
- "\$str1" != "$str2" str1是否与str2不等
- "$str1" =~ "str2" str1是否包含str2

特别注意，字符串变量最好用引号引起来，因为一旦字符串中有空格，这个表达式就错了，有兴趣的可以尝试当str1="hello world"，而str2="hello"的时候进行比较。

文件目录判断：filename（笔记：[文件测试运算符]()）
- -f $filename 是否为文件
- -e $filename 是否存在
- -d $filename 是否为目录
- -s $filename 文件存在且不为空
- ! -s $filename 文件是否为空

特别需要注意以下几点：

- []前面要有空格，它里面是逻辑表达式
- `if elif`后面要跟`then`，然后才是要执行的语句
- 如果想打印上一条命令的执行结果，最好的做法是将==\$?==赋给一个变量，因为一旦执行了一条命令，==\$?==（传递参数）的值就可能会变。

一般说明，如果命令执行成功，则其返回值为0，否则为非0。因此，可以通过下面的方式判断上条命令的执行结果：
```shell
if [ $? -eq 0 ]
then
    echo "success"
elif [ $? -eq 1 ]
then
    echo "failed, error code is 1"
else
    echo "other code"
fi
```

