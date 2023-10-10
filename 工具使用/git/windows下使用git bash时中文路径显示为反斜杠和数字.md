
这是因为在`Windows`中，`git`默认使用`UTF-8`编码，而`Windows`使用的是`ANSI`编码。

设置`Git`的文件名和路径编码格式，在`Git Bash`中，设置编码格式为`UTF-8`：

```
git config --global core.quotepath false
```

如果上述`git`编码格式设置后仍然显示反斜杠+数字，或者`xx%`的格式，则需要确保`Windows`系统的本地编码设置与`Git`的编码一致。更改`Windows`系统的本地编码格式为`UTF-8`：
1. `WIN` + `R`，打开运行窗口，输入`regedit`，打开注册表；
2. 找到注册表路径：`HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Nls\CodePage`；
3. 双击右侧窗口中的`ACP`，将“数值数据”更改为`65001`；
4. 关闭注册表编辑器，重启电脑使更改生效。









