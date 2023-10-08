
18.04版本有时候会卡死在Ubuntu的logo界面无法进入桌面，解决方法：
https://blog.csdn.net/l821133235/article/details/79565541

1、查看磁盘是否异常
```
$ sudo fsck -f /
[sudo] password for abo:
fsck from util-linux 2.31.1
e2fsck 1.44.1 (24-Mar-2018)
/dev/sda1 is mounted.
e2fsck: Cannot continue, aborting.
```

2、修复
```
$ sudo fsck -nf /dev/sda1
fsck from util-linux 2.31.1
e2fsck 1.44.1 (24-Mar-2018)
Warning!  /dev/sda1 is mounted.
Warning: skipping journal recovery because doing a read-only filesystem check.
Pass 1: Checking inodes, blocks, and sizes
Pass 2: Checking directory structure
Pass 3: Checking directory connectivity
Pass 4: Checking reference counts
Pass 5: Checking group summary information
Free blocks count wrong (115846288, counted=115825479).
Fix? no

Free inodes count wrong (30156070, counted=30156067).
Fix? no

/dev/sda1: 375514/30531584 files (0.1% non-contiguous), 6249840/122096128 blocks
$
```

3、卸载挂载
```
$ sudo umount /dev/sda1
umount: /: target is busy.
$
```

4、解决target is busy
```
$ sudo fuser -kvm /dev/sda1
```

在卸载挂载时会出现umount: /: target is busy.错误，解决方法（shell会退出，重连即可）：

https://blog.csdn.net/ly_qiu/article/details/105842105


