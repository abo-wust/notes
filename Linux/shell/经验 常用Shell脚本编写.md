已剪辑自: [https://mp.weixin.qq.com/s/2NHk8\_d1gel1pNym4Sw8Cg](https://mp.weixin.qq.com/s/2NHk8_d1gel1pNym4Sw8Cg)



**本章写的Shell脚本例子都比较实用，在面试题中也经常出现，希望大家参考着多动手写写，不要复制粘贴就拿来跑，这样是学不会的！**

**注意事项**

1) 开头加解释器：#!/bin/bash

2) 语法缩进，使用四个空格;多加注释说明。

3) 命名建议规则：变量名大写、局部变量小写，函数名小写，名字体现出实际作用。

4) 默认变量是全局的，在函数中变量local指定为局部变量，避免污染其他作用域。

5) 有两个命令能帮助我调试脚本：set -e 遇到执行非0时退出脚本，set-x 打印执行过程。

6) 写脚本一定先测试再到生产上。

**1 获取随机字符串或数字**

获取随机8位字符串：
![](images/Pasted%20image%2020221201224110.png)

获取随机8位数字：
![](images/Pasted%20image%2020221201224122.png)

cksum：打印CRC效验和统计字节

**2 定义一个颜色输出字符串函数**
![](images/Pasted%20image%2020221201224139.png)

function关键字定义一个函数，可加或不加。

**3 批量创建用户**
![](images/Pasted%20image%2020221201224152.png)

**4 检查软件包是否安装**
![](images/Pasted%20image%2020221201224158.png)

**5 检查服务状态**
![](images/Pasted%20image%2020221201224204.png)

**6 检查主机存活状态**

方法1： 将错误IP放到数组里面判断是否ping失败三次
![](images/Pasted%20image%2020221201224210.png)

方法2： 将错误次数放到FAIL\_COUNT变量里面判断是否ping失败三次
![](images/Pasted%20image%2020221201224217.png)

方法3： 利用for循环将ping通就跳出循环继续，如果不跳出就会走到打印ping失败
![](images/Pasted%20image%2020221201224222.png)

**7 监控CPU、内存和硬盘利用率**

1)CPU

借助vmstat工具来分析CPU统计信息。
![](images/Pasted%20image%2020221201224242.png)

2)内存
![](images/Pasted%20image%2020221201224248.png)

3)硬盘
![](images/Pasted%20image%2020221201224253.png)

**8 批量主机磁盘利用率监控**

前提监控端和被监控端SSH免交互登录或者密钥登录。

写一个配置文件保存被监控主机SSH连接信息，文件内容格式：IP User Port
![](images/Pasted%20image%2020221201224259.png)

**9 检查网站可用性**

1)检查URL可用性
![](images/Pasted%20image%2020221201224308.png)

2)判断三次URL可用性

思路与上面检查主机存活状态一样。
![](images/Pasted%20image%2020221201224315.png)

本章写的Shell脚本例子都比较实用，在面试题中也经常出现，希望大家参考着多动手写写，不要复制粘贴就拿来跑，这样是学不会的！





