
![[Pasted image 20250911230902.png]]


## 一、驱动介绍

Linux驱动属于内核的一部分，学习驱动开发时将驱动设计为[内核模块](https://zhida.zhihu.com/search?content_id=513378013&content_type=Answer&match_order=1&q=%E5%86%85%E6%A0%B8%E6%A8%A1%E5%9D%97&zhida_source=entity)，内核模块是一种可以在系统运行时加载和卸载的机制。

内核编程的注意事项：
	1.不能使用C标准库和C标准头文件
	2.使用GNU C
	3.没有内存保护机制
	4.不能处理浮点运算
	5.注意并发互斥和可移植性问题

  
## 1.内核模块

Linux 驱动有两种运行方式，第一种是将驱动编译进 Linux 内核中，当 Linux 内核启动的时就会自动运行驱动程序。第二种是将驱动编译成模块(Linux 下模块扩展名为.ko)，在Linux 内核启动以后使用[insmod](https://zhida.zhihu.com/search?content_id=513378013&content_type=Answer&match_order=1&q=insmod&zhida_source=entity)或者[modprobe](https://zhida.zhihu.com/search?content_id=513378013&content_type=Answer&match_order=1&q=modprobe&zhida_source=entity)命令加载驱动模块。在调试驱动的时候一般都选择将其编译为模块。

模块的加载和卸载注册函数如下
```
#include <linux/init.h>
#include <linux/module.h> //需要包含头文件

module_init(xxx_init); //注册模块加载函数
module_exit(xxx_exit); //注册模块卸载函数
```

module_init 函数用来向 Linux 内核注册一个模块加载函数，参数 xxx_init 就是需要注册的具体函数，当使用insmod命令加载驱动的时候， xxx_init 这个函数就会被调用。 module_exit()函数用来向 Linux 内核注册一个模块卸载函数，参数 xxx_exit 就是需要注册的具体函数，当使用“rmmod”命令卸载具体驱动的时候 xxx_exit 函数就会被调用。

字符设备驱动模块加载和卸载模板如下所示：
```
#include <linux/init.h>
#include <linux/module.h>
 
/* 驱动入口函数 */
static int __init xxx_init(void)
{
    /* 入口函数具体内容 */
 
    return 0;
}
 
 
/* 驱动出口函数 */
static void __exit xxx_exit(void)
{
    /* 出口函数具体内容 */
}
 
/* 将上面两个函数指定为驱动的入口和出口函数 */
module_init(xxx_init);
module_exit(xxx_exit)
 
MODULE_LICENSE("GPL");//GPL模块许可证
```

``
注：在编写模块时必须加上模块许可证，防止内核被污染，造成某些功能无法使用。

驱动编译完成以后扩展名为.ko，有两种命令可以加载驱动模块： insmod和 modprobe。
	insmod drv.ko //加载驱动模块
	rmmod drv.ko //卸载驱动模块
	modprobe drv.ko //加载或者卸载驱动模块

区别：insmod 命令不能解决模块的依赖关系，但是 modprobe 不会存在这个问题， modprobe 会分析模块的依赖关系，然后会将所有的依赖模块都加载到内核中。比如 drv.ko 依赖 first.ko 这个模块，就必须先使用insmod 命令加载 first.ko 这个模块，然后再加载 drv.ko 这个模块。

modprobe 命令主要智能在提供了模块的依赖性分析、错误检查、错误报告等功能，推荐使用modprobe 命令来加载驱动。 modprobe 命令默认会去`/lib/modules/<kernel-version>`目录中查找模块。

同时 modprobe 命令也可以卸载掉驱动模块所依赖的其他模块，前提是这些依赖模块已经没有被其他模块所使用，否则就不能使用 modprobe 来卸载驱动模块。所以对于模块的卸载，推荐使用 rmmod 命令。

## 2.日志级别

printk在内核中用来记录日志信息的函数，只能在内核源码范围内使用。和printf非常相似。printk函数主要做两件事情：①将信息记录到log中 ②调用控制台驱动来将信息输出。
```
#define KERN_EMERG	"<0>"	/*系统不可用*/
#define KERN_ALERT	"<1>"	/*必须立即处理的错误信息*/
#define KERN_CRIT	"<2>"	/*严重错误信息*/
#define KERN_ERR	"<3>"	/*错误信息*/
#define KERN_WARNING	"<4>"	/*警告信息*/
#define KERN_NOTICE	"<5>"	/*需要注意的信息*/
#define KERN_INFO	"<6>"	/*一般信息*/
#define KERN_DEBUG	"<7>"	/*调试信息*/
```

```
printk(KERN_DEBUG"debug\r\n");
```

printk打印的内容是否显示取决于日志级别，只有当printk的日志级别高于内核默认打印级别时才打印(打印日志级别数值小于内核默认打印级别)。数字越小，优先级越高。

若是printk不提供打印级别使用默认打印级别，可以通过查看/proc/sys/kernel/printk来查看，第二个数字就是printk的默认打印级别。

![[Pasted image 20250911231740.png]]

而/proc/sys/kernel/printk中的**第一个数字就是内核默认打印级别，** 可以通过uboot的环境变量bootargs传递内核默认打印级别。在uboot的bootargs中加入loglevel=X指令。
![[Pasted image 20250911231746.png]]


内核的默认打印级别修改为8。

## 3.模块符号的导出

模块导出符号可以将模块中的变量/函数导出，供内核其他代码/模块使用。 内核中提供了相应的宏来实现模块的导出：

EXPORT_SYMBOL -------------- 使用无限制
EXPORT_SYMBOL_GPL ---------- 只有遵循GPL协议的代码才可以使用

需注意的是， 如果一个模块使用了另一个模块的 变量/函数，该模块依赖于另一个模块，加载模块时必须先加载依赖的模块，如果一个模块被内核使用，该模块不得卸载。


## 4.内核模块参数

内核的模块参数**不但可以在编写代码时设置其的值， 还可以在加载模块时设置其的值，甚至可以再加载模块后修改其的值**。在模块中声明一些变量，使用以下语法将这些变量设置为模块参数：

module_param(模块参数名,模块参数类型,访问权限);
module_param_array(数组模块参数名,数组元素类型,NULL,访问权限);

在代码其他地方使用模块参数和使用普通变量没有区别。加载模块时可以通过 "模块参数名=值" 的方式来修改模块参数的值。当模块加载成功后，访问权限非0的模块参数就会出现在以下路径下 /sys/module/模块名/parameters 存在和模块参数名相同的文件，这些文件的权限来自于模块参数的权限，内容来自于模块参数的值。同时也可以通过修改文件中保存的数据来修改对应模块参数。


## 二、字符设备驱动(一)

驱动是沟通底层硬件和上层应用的桥梁，访问设备文件通过文件系统IO，在用户层访问设备文件和访问普通文件没有区别。

## 1.模块加载

模块加载和卸载模板：
```
#include <linux/init.h>
#include <linux/module.h>
 
/* 驱动入口函数 */
static int __init xxx_init(void)
{
    /* 入口函数具体内容 */
 
    return 0;
}
 
 
/* 驱动出口函数 */
static void __exit xxx_exit(void)
{
    /* 出口函数具体内容 */
}
 
/* 将上面两个函数指定为驱动的入口和出口函数 */
module_init(xxx_init);
module_exit(xxx_exit)
 
MODULE_LICENSE("GPL");//GPL模块许可证
```


## **2.注册字符设备驱动**

对于字符设备驱动而言，当驱动模块加载成功以后需要注册字符设备。卸载驱动模块的时也需要注销掉字符设备。

字符设备的注册和注销函数原型：
```
static inline int register_chrdev(unsigned int major, const char *name,
const struct file_operations *fops)
 
static inline void unregister_chrdev(unsigned int major, const char *name)
```


register_chrdev 函数用于注册字符设备，需要传入主设备号，设备名称和指向设备操作函数集合变量。这种注册函数会将后面所有的次设备号全部占用，而且主设备号需要我们自己去设置，现在不推荐这样使用。一般字符设备的注册在驱动模块的入口函数 xxx_init 中进行，字符设备的注销在驱动模块的出口函数 xxx_exit 中进行。
```
/*
 * @description		: 打开设备
 * @param - inode 	: 传递给驱动的inode
 * @param - filp 	: 设备文件，file结构体有个叫做private_data的成员变量
 * 					  一般在open的时候将private_data指向设备结构体。
 * @return 			: 0 成功;其他 失败
 */
static int led_open(struct inode *inode, struct file *filp)
{
	return 0;
}
 
/*
 * @description		: 从设备读取数据 
 * @param - filp 	: 要打开的设备文件(文件描述符)
 * @param - buf 	: 返回给用户空间的数据缓冲区
 * @param - cnt 	: 要读取的数据长度
 * @param - offt 	: 相对于文件首地址的偏移
 * @return 			: 读取的字节数，如果为负值，表示读取失败
 */
static ssize_t led_read(struct file *filp, char __user *buf, size_t cnt, loff_t *offt)
{
	return 0;
}
 
/*
 * @description		: 向设备写数据 
 * @param - filp 	: 设备文件，表示打开的文件描述符
 * @param - buf 	: 要写给设备写入的数据
 * @param - cnt 	: 要写入的数据长度
 * @param - offt 	: 相对于文件首地址的偏移
 * @return 			: 写入的字节数，如果为负值，表示写入失败
 */
static ssize_t led_write(struct file *filp, const char __user *buf, size_t cnt, loff_t *offt)
{
	return 0;
}
 
/*
 * @description		: 关闭/释放设备
 * @param - filp 	: 要关闭的设备文件(文件描述符)
 * @return 			: 0 成功;其他 失败
 */
static int led_release(struct inode *inode, struct file *filp)
{
	return 0;
}
 
//设备操作函数 
static struct file_operations led_fops = {
	.owner = THIS_MODULE,
	.open = led_open,
	.read = led_read,
	.write = led_write,
	.release = 	led_release,
};
 
//驱动入口函数
static int __init led_init(void)
{
	int retvalue = 0;
 
	/*注册字符设备驱动 */
	retvalue = register_chrdev(LED_MAJOR, LED_NAME, &led_fops);
	if(retvalue < 0){
		printk("register chrdev failed!\r\n");
		return -EIO;
	}
	return 0;
}
//驱动出口函数
static void __exit led_exit(void)
{
 
	/* 注销字符设备驱动 */
	unregister_chrdev(LED_MAJOR, LED_NAME);
}
 
module_init(led_init);
module_exit(led_exit);
MODULE_LICENSE("GPL");
```


## 3.内存映射

在Linux中不能直接访问寄存器，要想要操作寄存器需要完成物理地址到虚拟空间的映射。
```
#include <linux/io.h>
#include <mach/platform.h>
 
#define ioremap(cookie,size) __arm_ioremap((cookie), (size),
MT_DEVICE)
 
void __iomem * __arm_ioremap(phys_addr_t phys_addr, size_t size,
unsigned int mtype)
{
    return arch_ioremap_caller(phys_addr, size, mtype,
    __builtin_return_address(0));
}
```


返回值： __iomem 类型的指针，指向映射后的虚拟空间首地址。

建立映射：映射的虚拟地址 = ioremap(IO内存起始地址，映射长度)；一旦映射成功，访问对应的虚拟地址就相当于访问对应的IO内存 。

```
void iounmap (volatile void __iomem *addr)
```

```text
/* 寄存器物理地址 */
#define CCM_CCGR1_BASE				(0X020C406C)	
 
/* 映射后的寄存器虚拟地址指针 */
static void __iomem *IMX6U_CCM_CCGR1;
 
/* 寄存器地址映射 */
IMX6U_CCM_CCGR1 = ioremap(CCM_CCGR1_BASE, 4);
if(IS_ERR_OR_NULL(IMX6U_CCM_CCGR1))
{
    //...
}
```

解除映射：
```text
void iounmap (volatile void __iomem *addr)
 
iounmap(IMX6U_CCM_CCGR1);
```

内存映射后，就可以通过I/O 内存访问函数对寄存器进行相应的操作，和裸机开发那部分差不多一样。编写完成后，在写应用测试程序，但是在测试的时候需要手动创建设备文件，对应开发非常不方便。以上就是一个最简单的字符设备驱动框架。

## 三、字符设备驱动(二)

前面的字符设备驱动框架比较简单，不灵活。不仅需要设定主设备号，在测试时还需要手动创建设备文件。新字符设备驱动框架刚好能解决这两个大问题。

## 1.模块加载

  这一步和前面是一样的。

## 2.申请设备号

前面设备号的申请是开发者检查当前系统中所有被使用了的设备号，然后挑选一个没有使用的设备号给驱动。这样很不方便。Linux 社区推荐使用动态分配设备号，在注册字符设备之前先申请一个设备号，系统会自动给你一个没有被使用的设备号。

动态申请设备号：

```text
int alloc_chrdev_region(dev_t *dev, unsigned baseminor, unsigned count, const char *name)
```

dev：保存申请到的设备号。
baseminor： 次设备号起始地址，该函数可以申请一段连续的多个设备号，初始值一般为0
count： 要申请的设备号数量。
name：设备名字。

静态申请设备号：
```text
int register_chrdev_region(dev_t from, unsigned count, const char *name);
```

from - 要申请的起始设备号
count - 设备号个数
name - 设备号在内核中的名称
返回0申请成功，否则失败

注销字符设备之后要释放掉设备号，设备号释放函数

```text
void unregister_chrdev_region(dev_t from, unsigned count)
```

from：要释放的设备号。
count： 表示从 from 开始，要释放的设备号数量。

新字符设备驱动模板:

  ```text
//创建设备号 
if (newchrled.major)   //定义了设备号就静态申请
{		
	newchrled.devid = MKDEV(newchrled.major, 0);
	register_chrdev_region(newchrled.devid, NEWCHRLED_CNT, NEWCHRLED_NAME);
} 
else   //没有定义设备号就动态申请
{		
  
	alloc_chrdev_region(&newchrled.devid, 0, NEWCHRLED_CNT, NEWCHRLED_NAME);//申请设备号 
	newchrled.major = MAJOR(newchrled.devid);	//获取分配号的主设备号
	newchrled.minor = MINOR(newchrled.devid);	// 获取分配号的次设备号
}
```

## 3.注册字符设备

在 Linux 中使用 cdev 结构体表示一个字符设备， cdev 结构体在 include/linux/cdev.h 文件中的定义如下
```text
struct cdev {
	struct kobject kobj;
	struct module *owner;
	const struct file_operations *ops;//操作函数集合
	struct list_head list;
	dev_t dev;//设备号
	unsigned int count;
};
```

在 cdev 中有两个重要的成员变量:ops 和 dev，字符设备文件操作函数集合file_operations 以及设备号 dev_t。

向Linux内核添加字符设备

**①初始化cdev结构体变量**

定义好 cdev 变量以后就要使用 cdev_init 函数对其进行初始化， cdev_init 函数原型如下：

```text
void cdev_init(struct cdev *cdev, const struct file_operations *fops);
```

参数 cdev 就是要初始化的 cdev 结构体变量，参数 fops 就是字符设备文件操作函数集合。
```text
struct cdev testcdev;
 
//设备操作函数
static struct file_operations test_fops = {
    .owner = THIS_MODULE,
    //其他具体的初始项 
};
 
testcdev.owner = THIS_MODULE;
 
//初始化 cdev 结构体变量
cdev_init(&testcdev, &test_fops); 
```

**② 将设备添加到内核**

cdev_add 函数用于向 Linux 系统添加字符设备(cdev 结构体变量)，首先使用 cdev_init 函数完成对 cdev 结构体变量的初始化，然后使用 cdev_add 函数向 Linux 系统添加这个字符设备。

```text
int cdev_add(struct cdev *p, dev_t dev, unsigned count)
```

p - 要添加的cdev结构
dev - 绑定的起始设备号
count - 设备号个数

```text
cdev_add(&testcdev, devid, 1); //添加字符设备
```

将cdev添加到内核同时绑定设备号。

其实这里申请设备号和注册设备在第一中驱动中直接使用register_chrdev函数完成者两步操作。卸载也是一样的。

卸载驱动的时候一定要使用 cdev_del 函数从 Linux 内核中删除相应的字符设备

```text
void cdev_del(struct cdev *p);
 
cdev_del(&testcdev); //删除 cdev
```

## 4.自动创建设备节点

上面的驱动框架，当使用 modprobe 加载驱动程序以后还需要使用命令mknod手动创建设备节点。在驱动中实现自动创建设备节点的功能以后，使用 modprobe 加载驱动模块成功的话就会自在/dev 目录下创建对应的设备文件。

在 Linux 下通过 udev 来实现设备文件的创建与删除，但是在嵌入式 Linux 中使用mdev 来实现设备节点文件的自动创建与删除， Linux 系统中的热插拔事件也由 mdev 管理。具体关于mdev和udev可以参考前面的笔记。

自动创建设备节点的工作是在驱动程序的入口函数中完成的，一般在 cdev_add 函数后面添  
加自动创建设备节点相关代码。

①首先要创建一个 class 类，定义在文件include/linux/device.h 里面。

```text
struct class *class_create(struct module *owner, const char *name);
```

class_create 一共有两个参数，参数 owner 一般为 THIS_MODULE，参数 name 是类名字。设备类名对应 /sys/class 目录的子目录名。返回值是个指向结构体 class 的指针，也就是创建的类。

卸载驱动程序的时候需要删除掉类，类删除函数为 class_destroy

```text
void class_destroy(struct class *cls); // cls要删除的类
```

②创建设备：

创建好类以后还不能实现自动创建设备节点，还需要在类下创建一个设备,使用 device_create 函数在类下面创建设备。

```text
struct device *device_create(struct class *class,
    struct device *parent,
    dev_t devt,
    void *drvdata,
    const char *fmt, ...)
```

简化后

```text
struct device *device_create(设备类指针, 父设备指针,设备号, 额外数据, 设备文件名);
```

成功会在 /dev 目录下生成设备文件。

卸载驱动的时候需要删除掉创建的设备，设备删除函数为 device_destroy

```text
void device_destroy(struct class *class, dev_t devt);
```

参数 class 是要删除的设备所处的类，参数 devt 是要删除的设备号

小结：
> a.创建设备类 ---------------------- class_create  
> struct class *class_create(struct module *owner, const char *name);  
> //创建成功会生产该路径： /sys/class/设备类名  
> b.创建设备文件(设备节点) ------------------ device_create  
> struct device *device_create(设备类指针, 父设备指针,设备号, 额外数据, 设备文件名);  
> //成功会在 /dev 目录下生成设备文件  
> 销毁设备类 --------------------- class_destroy  
> 销毁设备文件 ------------------- device_destroy

```text
struct class *class; /* 类 */
struct device *device; /* 设备 */
dev_t devid; /* 设备号 */
 
/* 驱动入口函数 */
static int __init led_init(void)
{
	...
	/* 创建类 */
	class = class_create(THIS_MODULE, "xxx");
	/* 创建设备 */
	device = device_create(class, NULL, devid, NULL, "xxx");
	...
	return 0;
}
 
/* 驱动出口函数 */
static void __exit led_exit(void)
{
	...
	/* 删除设备 */
	device_destroy(newchrled.class, newchrled.devid);
	/* 删除类 */
	class_destroy(newchrled.class);
	...
}
 
module_init(led_init);
module_exit(led_exit);
```

## 5.文件私有数据

每个硬件设备都有一些属性，比如主设备号(dev_t)，类(class)、设备(device)，在编写驱动的时候可以将这些属性全部写成变量的形式，但对于一个设备的所有属性信息最好将其做成一个结构体,编写驱动 open 函数的时候将设备结构体作为私有数据添加到设备文件中。

```text
/* newchrled设备结构体 */
struct newchrled_dev{
	dev_t devid;			/* 设备号 	 */
	struct cdev cdev;		/* cdev 	*/
	struct class *class;		/* 类 		*/
	struct device *device;	/* 设备 	 */
	int major;				/* 主设备号	  */
	int minor;				/* 次设备号   */
};
 
struct newchrled_dev newchrled;	/* led设备 */
 
/*
 * @description		: 打开设备
 * @param - inode 	: 传递给驱动的inode
 * @param - filp 	: 设备文件，file结构体有个叫做private_data的成员变量
 * 					  一般在open的时候将private_data指向设备结构体。
 * @return 			: 0 成功;其他 失败
 */
static int led_open(struct inode *inode, struct file *filp)
{
	filp->private_data = &newchrled; /* 设置私有数据 */
	return 0;
}

static ssize_t led_read(struct file *filp, char __user *buf, size_t cnt, loff_t *offt) 
{
	struct newchrled_dev *dev = (struct newchrled_dev *)filp->private_data;
	return 0;
}
```

在 open 函数里面设置好私有数据后，在 write、 read、 close 函数中直接读取 private_data即可得到设备结构体。


## 6.应用层和内核传递数据

应用层和内核层是不能直接进行数据传输的。要想进行数据传输，要借助下面的两个函数：
```
static inline long copy_from_user(void *to, const void __user *from, unsigned long n)
static inline long copy_to_user(void __user *to, const void *from, unsigned long n)
```

用户空间-->内核空间
![[Pasted image 20250911233328.png]]
```
copy_from_user(void *to, const void __user *from, unsigned long n)
```

to：目标地址（内核空间）
from：源地址（用户空间）
n：将要拷贝数据的字节数
返回值：成功返回0，失败返回没有拷贝成功的数据字节数

内核空间-->用户空间
![[Pasted image 20250911233451.png]]
```
copy_to_user(void __user *to, const void *from, unsigned long n)
```

to：目标地址（用户空间）
from：源地址（内核空间）
n：将要拷贝数据的字节数
返回值：成功返回0，失败返回没有拷贝成功的数据字节数


## 总结

Linux系统将设备分为3类：字符设备、块设备、网络设备。
![[Pasted image 20250911232712.png]]

Linux中所有的设备文件在/dev目录下，内核中有很多的字符设备驱动，这些字符设备驱动和字符设备文件匹配的方式是通过设备号。

![[Pasted image 20250911232723.png]]

在应用层调用open函数打开设备文件，**对于上层open调用到内核时会发生一次软中断中断号是0X80，从用户空间进入到内核空间。**

![[Pasted image 20250911232733.png]]


open会调用到system_call(内核函数)，system_call会根据设备文件中的设备名，去找出要操作的设备号。

然后调到虚拟文件VFS （为了上层调用到确切的硬件统一化）,调用VFS里的sys_open，sys_open会找到在驱动链表(管理所有设备的驱动)里面，根据主设备号和次设备号找到字符设备驱动，然后驱动函数里面有通过寄存器操控IO口的代码，进而可以控制IO口实现相关功能。

大致流程：
![[Pasted image 20250911232749.png]]


补充

1.在Linux文件系统中，每个文件都用一个struct inode结构体来描述，这个结构体记录了这个文件的所有信息，例如文件类型，访问权限等。

2.在linux操作系统中，每个驱动程序在应用层的/dev目录或者其他如/sys目录下都会有一个文件与之对应。

3.在linux操作系统中, 每个驱动程序都有一个设备号

4.在linux操作系统中，每打开一次文件，Linux操作系统会在VFS层分配一个struct file结构体来描述打开的文件

**大致驱动原理：**

(1) 当open函数打开设备文件时，可以根据设备文件对应的struct inode结构体描述的信息，可以知道接下来要操作的设备类型（字符设备还是块设备），还会分配一个struct file结构体。

(2) 根据struct inode结构体里面记录的设备号，可以找到对应的驱动程序。在Linux操作系统中每个字符设备都有一个struct cdev结构体。此结构体描述了字符设备所有信息，其中最重要的一项就是字符设备的操作函数接口

(3) 找到struct cdev结构体后，linux内核就会将struct cdev结构体所在的内存空间首地址记录在struct inode结构体i_cdev成员中，将struct cdev结构体中的记录的函数操作接口地址记录在struct file结构体的f_ops成员中。

![[Pasted image 20250911232812.png]]

(4) 任务完成，VFS层会给应用返回一个文件描述符（fd）。这个fd是和struct file结构体对应的。接下来上层应用程序就可以通过fd找到struct file，然后在struct file找到操作字符设备的函数接口file_operation了。

其中，cdev_init和cdev_add在驱动程序的入口函数中就已经被调用，分别完成字符设备与file_operation函数操作接口的绑定，和将字符驱动注册到内核的工作。



