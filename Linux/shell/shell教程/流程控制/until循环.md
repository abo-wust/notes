until 循环执行一系列命令直至条件为 true 时停止。

until 循环与 while 循环在处理方式上刚好相反。

一般 while 循环优于 until 循环，但在某些时候—也只是极少数情况下，until 循环更加有用。

until 语法格式：
```shell
until condition
do
    command
done
```

`condition`一般为条件表达式，如果返回值为`false`，则继续执行循环体内的语句，否则跳出循环。

以下实例我们使用 until 命令来输出`0 ~ 9`的数字：
```shell
#!/bin/bash

a=0

until [ ! $a -lt 10 ]
do
    echo $a
    a=`expr $a + 1`
done
```

输出结果为：
```
0
1
2
3
4
5
6
7
8
9
```

或者读取键盘信息，并判断是否满足条件。只有当输入的`ans`不是`yes`时，循环终止：
```shell
ans=yes

until [ "$ans" != "yes" ]
do
    read -p "please input yes to exit loop:" ans
done
```

