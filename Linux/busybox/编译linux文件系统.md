
### 下载busybox

```
wget https://busybox.net/downloads/busybox-1.35.0.tar.bz2
wget https://busybox.net/downloads/busybox-1.35.0.tar.bz2.sha256
wget https://busybox.net/downloads/busybox-1.35.0.tar.bz2.sig 
```


### 解压

```
tar -xvf busybox-1.35.0.tar.bz2
cd busybox-1.35.0
```

### 设置交叉编译工具路径

```
export ARCH=arm
export CROSS_COMPILE=/opt/xilinx-gcc/bin/arm-xilinx-linux-gnueabi-
```

`CROSS_COMPILE`根据本地交叉编译链工具所在目录设置。

### 修改配置文件

```
make menuconfig
```

命令弹出手动修改配置的图形界面

![](images/Pasted%20image%2020230111161205.png)

回车键进入`Settings`设置静态编译（空格选中，方括号会显示星号`*`，表示已选中）：
[* ] Build static binary (no shared libs) 

![](images/Pasted%20image%2020230111162141.png)

如果不需要其他配置，`Exit`回退到上一级页面，进入文件系统工具设置

![](images/Pasted%20image%2020230111162333.png)

将`[ ] nsenter (6.5 kb)`选项去掉（空格去掉星号`*`）

![](images/Pasted%20image%2020230111162526.png)

`Exit`回退到上一级页面，进入`Coreutils`设置

![](images/Pasted%20image%2020230111162828.png)

将`[ ] sync (3.8 kb)`选项去掉（空格去掉星号`*`）

![](images/Pasted%20image%2020230111162909.png)

如果不配置上面`nsenter`和`sync`，`make install`会出现下面的编译错误

![](images/Pasted%20image%2020230111162103.png)

继续`Exit`退出配置界面时选择保存配置

![](images/Pasted%20image%2020230111161927.png)


### 编译安装

```
make install
```
编译完成后会在当前目录自动生成`_install`目录

![](images/Pasted%20image%2020230111163440.png)

`_install`目录包含了编译好的文件系统需要的命令集合

![](images/Pasted%20image%2020230111164138.png)

如果需要将`make install`编译生成的文件存放到指定目录，可以在`make menuconfig`的图形界面中设置，在`Settings->Installation Options`中设置路径，`busybox`默认存放编译生成文件的目录如图所示，在`_install`：

![](images/Pasted%20image%2020230111164543.png)

`Ctrl+Backspace`删除默认的`_install/`目录，然后输入指定的目录，比如：`/home/liubo/work/rootfs`，然后保存配置。

![](images/Pasted%20image%2020230111164942.png)

如果不想使用图形配置界面，可以在`make install`命令行指定编译文件存放的目录
```
make install CONFIG_PREFIX=/home/huahuan/work/rootfs
```

### 完善文件系统

参考：
https://zhuanlan.zhihu.com/p/146580665
https://blog.csdn.net/guoyiyan1987/article/details/80243324
https://www.shuzhiduo.com/A/WpdK6K2Z5V/

#### 创建所需文件夹

进入`_install`，根据需要创建文件夹

	```
	mkdir dev etc home lib mnt opt proc sys tmp var root
	ls
	bin  dev  etc  home  lib  linuxrc  mnt  opt  proc  root  sbin  sys  tmp  usr  var
	```

#### 添加配置文件

即配置/etc/目录下的文件

	```
	mkdir -p etc/init.d
	```
	
1）rcS文件

在`_install/etc/init.d`目录下新建`rcS`文件，根据需要写入配置。可以写入以下内容来测试：

```shell
#!/bin/sh

echo "Starting rcS..."
mkdir -p /proc
mkdir -p /tmp  
mkdir -p /sys  
mkdir -p /mnt  
/bin/mount -a  
mkdir -p /dev/pts  
mount -t devpts devpts /dev/pts  
echo /sbin/mdev > /proc/sys/kernel/hotplug  
mdev –s

ifconfig lo 127.0.0.1
ifconfig eth0 192.168.1.100
telnetd -p 2650

```

2）/etc/fstab文件

```
proc            /proc           proc    defaults                0       0
tmpfs           /tmp            tmpfs   defaults                0       0
sysfs           /sys            sysfs   defaults                0       0
tmpfs           /dev            tmpfs   defaults                0       0
var             /dev            tmpfs   defaults                0       0
ramfs           /dev            ramfs   defaults                0       0
```
从左至右分别各列参数说明：`device  mount-point  type  options  dump  fsck order`

3）/etc/inittab文件

```
::sysinit:/etc/init.d/rcS
::respawn:-/bin/sh
::restart:/sbin/init
::askfirst:-/bin/sh
::ctrlaltdel:/bin/umount -a -r
::shutdown:/bin/umount -a -r
::shutdown:/sbin/swapoff -a
```

4）

在`_install/dev`目录中创建如下设备节点：
```
mknod console c 5 1  
mknod null c 1 3
```

5）/etc/profile文件

在`/etc/inittab`文件中，`::respawn:-/bin/sh`这一行配置表示在启动脚本`/etc/init.d/rcS`执行完后将在终端启动一个shell。Shell启动过程中会根据文件`/etc/profile`配置登陆环境。

添加profile文件，内容如下：

```
USER="`id -un`"  
LOGNAME=$USER
PS1='[\u@\h \W]# '
PATH=$PATH

HOSTNAME=`/bin/hostname`

export USER LOGNAME PS1 PATH
```

`PS1='[\u@\h \W]# '`这个显示的是命令行下的主机名和用户名格式，如：`[root@Blue etc]#`  。


### 编译内核

下载Linux内核
```
wget https://mirrors.tuna.tsinghua.edu.cn/kernel/v5.x/linux-5.4.5.tar.gz
tar -zxvf linux-5.4.5.tar.gz
```

将上一步busybox编译安装生成并且已经完善过的`_install`目录拷贝到解压后的内核目录
```
cp ../busybox-1.35.0/_install/ ./ -r
```

文件系统完善之后，进入内核目录编译内核

```
cd linux-5.4.5
export ARCH=arm
export CROSS_COMPILE=/opt/xilinx-gcc/bin/arm-xilinx-linux-gnueabi-
make vexpress_defconfig
make menuconfig
```

![](images/Pasted%20image%2020230203095706.png)

`make menuconfig`设置编译选项，在`General setup`的`Initramfs source file(s)`选项中回车输入``\_install`。
![](images/Pasted%20image%2020230203100119.png)

设置完源文件目录后，返回上一层界面，进入`Boot options`，将`Default kernel command string`选项中默认的内容`console=ttyAMA0`清空。
![](images/Pasted%20image%2020230203100544.png)

返回上一层界面，进入`Kernel Features`修改`Memory split`为`3G/1G user/kernel split`，原先默认为`2G/2G user/kernel split `，这个可以根据需要选择。然后将`High Memory Support`选项空格勾选上。
![](images/Pasted%20image%2020230203101008.png)

配置完成后开始编译内核
```
make bzImage ARCH=arm CROSS_COMPILE=/opt/xilinx-gcc/bin/arm-xilinx-linux-gnueabi-
make dtbs
```

![](images/Pasted%20image%2020230203101924.png)

出现上图错误后，将`usr/gen_initramfs_list.sh`文件的131行多加一对双引号，修改为：
```
local dev="`LC_ALL=C ls -l "${location}"`"
```

原来的内容如下图：
![](images/Pasted%20image%2020230203102139.png)


编译完成后会有如下提示，并显示编译后内核的存储路径。
![](images/Pasted%20image%2020230203111741.png)


### 运行

1.  **运行 QEMU**  
    如下所示，输入 QEMU 启动命令，成功启动 QEMU，注意需指定 bzImage路径，并注意使用当前命令与 bzImage 路径的关系。  
    qemu-system-arm -M vexpress-a9 -m 256M -kernel arch/arm/boot/zImage -append "rdinit=/linuxrc console=ttyAMA0 loglevel=8" -dtb arch/arm/boot/dts/vexpress-v2p-ca9.dtb -nographic  
    以上命令中参数含义如下； -M：指定硬件芯片框架 -m：指定运行内存大小 -kernel：指定运行的内核镜像 -dtb：指定具体芯片的配置信息 -nographic：指定不使用图形界面
2.  **完成**  
    如下图可以看到，成功运行了我们刚刚编译的新内核
    ![](images/Pasted%20image%2020230203111842.png)


### 编译错误

1）make bzImage错误
![](images/Pasted%20image%2020230203111537.png)

