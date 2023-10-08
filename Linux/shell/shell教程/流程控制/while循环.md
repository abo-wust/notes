while 循环用于不断执行一系列命令，也用于从输入文件中读取数据。其语法格式为：
```shell
while condition
do
    command
done
```

以下是一个基本的 while 循环，测试条件是：如果 int 小于等于 5，那么条件返回真。int 从 1 开始，每次循环处理时，int 加 1。运行上述脚本，返回数字 1 到 5，然后终止。

示例：
```shell
#!/bin/bash

int=1

while(( $int<=5 ))
do
    echo $int
    let "int++"
done
```

运行脚本，输出：
```
1
2
3
4
5
```

以上实例使用了`Bash let`命令，它用于执行一个或多个表达式，变量计算中不需要加上`$`来表示变量，具体可查阅：[Bash let 命令](https://www.runoob.com/linux/linux-comm-let.html)。

`while`循环可用于读取键盘信息。下面的例子中，输入信息被设置为变量`FILM`，按`<Ctrl-D>`结束循环。
```shell
echo '按下 <CTRL-D> 退出'
echo -n '输入你最喜欢的网站名: '

while read FILM
do
    echo "是的！$FILM 是一个好网站"
done
```

运行脚本，输出类似下面：
```
按下 <CTRL-D> 退出
输入你最喜欢的网站名: GitHub
是的！GitHub 是一个好网站
```

或者读取键盘信息，并判断是否满足条件。只有当输入yes时，循环才会退出：
```shell
while [ "$ans" != "yes" ]
do
    read -p "please input yes to exit loop:" ans
done
```

### 无限循环

无限循环语法格式：
```shell
while :
do
    command
done
```

或者：
```shell
while true
do
    command
done
```

或者：
```shell
for (( ; ; ))
```

