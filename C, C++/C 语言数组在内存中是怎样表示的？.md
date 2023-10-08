
![](images/Pasted%20image%2020221221183846.png)

这个是经典的Linux进程内存布局，通常我们使用的数据存在这样几个地方：

-   栈区，Stack
    
-   全局区，Global
    
-   堆区，Heap
    

接下来我们分别看一下C语言中的数组在这几个区域是怎样表示的，注意，小风哥的机器是x86 64位。

## 数组与栈区

来看一段极其简单的代码：

```c
void arr_on_stack() {  
    int arr[6];  
    
    arr[0]=100;  
    arr[1]=200;  
    arr[2]=300;  
    arr[3]=400;  
    arr[4]=500;  
    arr[5]=600;  
  
    int a = arr[0];  
}
```

我们定义了一个局部变量arr作为int类型的数组，然后分别将100-600写到了数组中，那么数组arr在内存中是怎样表示的呢？

首先我们编译一下：

```
# gcc -g -fno-stack-protector a.c
```

注意，-fno-stack-protector选项是为了禁止堆栈保护，让汇编更容易懂些，关于堆栈保护这个话题可以参考这篇文章《[黑客攻防：缓冲区溢出攻击与堆栈保护](http://mp.weixin.qq.com/s?__biz=Mzg4OTYzODM4Mw==&mid=2247487070&idx=1&sn=5c7a6362362e97e046d99dab7008d6f8&chksm=cfe992def89e1bc82b188559dd302b9027a845a484e4d33a9665c19419c62011906e552374fb&scene=21#wechat_redirect)》。

好啦，一切准备就绪，可以庖丁解牛啦，使用的刀就是gdb，代码面前了无秘密，gdb面前程序的运行时(run time)了无秘密。

用gdb来调试刚刚编译出来的程序，这里看一下arr_on_stack函数的汇编指令：

```
(gdb) disassemble arr_on_stack  
Dump of assembler code for function arr_on_stack:  
   0x0000000000400526 <+0>:     push   %rbp  
   0x0000000000400527 <+1>:     mov    %rsp,%rbp  
   0x000000000040052a <+4>:     movl   $0x64,-0x20(%rbp)  
   0x0000000000400531 <+11>:    movl   $0xc8,-0x1c(%rbp)  
   0x0000000000400538 <+18>:    movl   $0x12c,-0x18(%rbp)  
   0x000000000040053f <+25>:    movl   $0x190,-0x14(%rbp)  
   0x0000000000400546 <+32>:    movl   $0x1f4,-0x10(%rbp)  
   0x000000000040054d <+39>:    movl   $0x258,-0xc(%rbp)  
=> 0x0000000000400554 <+46>:    mov    -0x20(%rbp),%eax  
   0x0000000000400557 <+49>:    mov    %eax,-0x4(%rbp)  
   0x000000000040055a <+52>:    nop  
   0x000000000040055b <+53>:    pop    %rbp  
   0x000000000040055c <+54>:    retq  
End of assembler dump.
```

我们在之前的文章《[函数在内存中是怎样表示的？](http://mp.weixin.qq.com/s?__biz=Mzg4OTYzODM4Mw==&mid=2247485714&idx=1&sn=1a315fe4da87fde2758fc9dd5366ba01&chksm=cfe99592f89e1c84b87ebfbfbbbd9868ddf3e945666168122f57aa793e87787528b513e46e5d&scene=21#wechat_redirect)》多次提到过，每个函数在运行起来后都有属于自己的栈帧，栈帧组成栈区，此时arr_on_stack这个函数的栈区在哪里呢？答案就在寄存器rbp中。

我们来看一下rbp寄存器指向了哪里？

```
(gdb) p $rbp
$3 = (void *) 0x7ffffffee2a0
```

啊哈，原来栈帧在0x7ffffffee2a0这个地方，那么我们的数组arr在哪里呢？别着急，这条指令会告诉我们答案：

```
0x000000000040052a <+4>:     movl   $0x64,-0x20(%rbp)
```

这行指令的含义是说把100(0x64)放到rbp寄存器减去0x20的地方，显然这就是数组的开头，让我们来计算一下rbp寄存器减去0x20：

```
0x7ffffffee2a0(%rbp) - 0x20 =  0x7ffffffee280
```

因此，我们预测arr应该在0x7ffffffee280这个位置上。

接下来我们用gdb验证一下：

```
(gdb) p &arr
$2 = (int (*)[6]) 0x7ffffffee280
```

哈哈，怎么样，是不是和我们猜想的一样，数组arr的确就放在了0x7ffffffee280这个位置，是这样存储的：

![](images/Pasted%20image%2020221221184143.png)

这就是C语言中所谓的数组了，**无非就是从0x7ffffffee280 到 0x7ffffffee298这一段内存嘛**，数组在栈区就是这么表示的！

## 数组与全局区

同样看一段代码：

```c
int global_array[6];  
  
void arr_on_global() {  
    global_array[0]=1;  
    global_array[1]=2;  
    global_array[2]=3;  
    global_array[3]=4;  
    global_array[4]=5;  
    global_array[5]=6;  
  
    int b = global_array[0];  
}
```

同样使用# gcc -g -fno-stack-protector a.c编译，然后用gdb加断点在int b = global_array[0]这行代码，看下全局变量global_array的内存位置：

```
(gdb) p &global_array$12 = (int (*)[6]) 0x601050 <global_array>
```

gdb告诉我们数组global_array存放在内存0x601050这个地址上。

注意0x601050这个地址和刚才看到的0x7ffffffee280这个地址相去甚远，为什么呢？

再看下开局那张图:

![](images/Pasted%20image%2020221221183846.png)

全局区几乎在最底部，栈区在最顶部，所以相差很远。

接下来让我们看看0x601050这个内存区域中到底保存了些啥？

我们使用命令x/6wd 0x601050，这个命令告诉gdb从0x601050这个位置开始以32bit为单位用10进制依次打印6次，让我们来看看打印的是什么？

```
(gdb) x/6wd 0x601050
0x601050 <global_array>:        1      2      3      4
0x601060 <global_array+16>:     5      6
```

怎么样，是不是正是全局变量global_array中存放的内容：

![](images/Pasted%20image%2020221221184605.png)

这就是C语言中所谓的数组了，**无非就是从 0x601050到 0x601068这一段内存嘛**，数组在全局区就是这么表示的！

## 数组与堆区

来段代码：

```c
void array_on_heap() {  
    int* arr = (int*)malloc(sizeof(int) * 6);  
    arr[0] = 100;  
    arr[1] = 200;  
    arr[2] = 300;  
    arr[3] = 400;  
    arr[4] = 500;  
    arr[5] = 600;  
  
    int a = arr[0];  
}
```

使用gdb加断点在int a = arr[0];这行代码，然后打印数组arr的地址：

```
(gdb) p arr$20 = (int *) 0x602010
```

注意0x602010这个地址，这个地址和刚才的全局数组global_array的地址0x601050比较接近，因为堆区和全局区挨得比较近，可以再回过头看一下开局那张图。

然后我们同样使用x命令查看这个区域的内存内容：

```
(gdb)  x/6wd 0x602010
0x602010:       100     200     300     400
0x602020:       500     600
```

依然不出我们所料，这个区域保存的正是数组的值。

![](images/Pasted%20image%2020221221184756.png)

这就是C语言中所谓的数组了，**无非就是从 0x602010到 0x602028这一段内存嘛**，数组在堆区就是这么表示的！

现在你应该明白了吧，C语言中所谓的数组是怎么表示的？很简单，其实也没啥表示，**无非就是内存中一段连续的空间**，仅此而已。

