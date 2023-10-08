for循环一般格式为：
```shell
for var in item1 item2 ... itemN
do
    command1
    command2
    ...
    commandN
done
```

写成一行：
```shell
for var in item1 item2 ... itemN; do command1; command2… done;
```

当变量值在列表里，`for`循环即执行一次所有命令，使用变量名获取列表中的当前取值。命令可为任何有效的`shell`命令和语句。`in`列表可以包含替换、字符串和文件名。

`in`列表是可选的，如果不用它，`for`循环使用命令行的位置参数。

例如，**顺序输出当前列表中的数字：**
```shell
for loop in 1 2 3 4 5
do
    echo "The value is: $loop"
done
```

或者
```shell
for loop in {1..5}
do
    echo "The value is: $loop"
done
```

输出结果：
```
The value is: 1
The value is: 2
The value is: 3
The value is: 4
The value is: 5
```

### 间隔3输出数字：

```shell
for loop in {5..15..3}
do
    echo "The value is: $loop"
done
```

输出结果：
```
5
8
11
14
```

### 顺序输出字符串中的字符：

```shell
#!/bin/bash

for str in This is a string
do
    echo $str
done
```

输出结果：
```
This
is
a
string
```

### 类似C语言风格的循环打印0到9：

```shell
for ((i = 0; i < 10; i++))
do
    echo $i
done
```

