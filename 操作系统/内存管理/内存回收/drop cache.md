https://zhuanlan.zhihu.com/p/93962657

Linux有自己完备的一套内存回收机制，并不需要人为的干预，但它同时也提供了一种手动释放的手段，可以让我们在调试的时候使用，方法是通过设置"/proc/sys/vm/drop_cache"参数。

这个参数可接收3个数字的输入，分别是1, 2和3，写入1代表只释放page cache的可回收部分，写入2代表只释放slab cache中的可回收部分。可回收的slab cache是指在调用"kmem_cache_create"函数向slab分配器申请内存时，使用了"SLAB_RECLAIM_ACCOUNT"标志位，主要就是dentry cache和inode cache。

写入3代表同时释放page cache和reclaimable slab cache。很好记，1+2=3嘛，事实上，1就是二级制的01，2就是二进制的10，运行drop cache就是靠这两个置1的bit判断的。具体的实现函数是"drop_caches_sysctl_handler"（代码位于/fs/drop_caches.c)：
```
int drop_caches_sysctl_handler(...)
{
    if (sysctl_drop_caches & 1) {
	iterate_supers(drop_pagecache_sb, NULL);
	count_vm_event(DROP_PAGECACHE);
    }
    if (sysctl_drop_caches & 2) {
        drop_slab();
	count_vm_event(DROP_SLAB);
    }
    ...
}
```

写入后只会执行一次，后续不用再去管它，不需要写入0去清除什么的（其实你写入0也是不会成功的），如果非强迫症犯了想把它清掉，那就写入4好了，4是"100"，末尾两位是0就行。

【page cache的释放】

在执行"echo 1 > /proc/sys/vm/drop_caches"命令之前和之后，分别查看"meminfo"中page cache的数量：


