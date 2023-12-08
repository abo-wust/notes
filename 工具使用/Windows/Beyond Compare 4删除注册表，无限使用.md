
快到期时找到注册表中的`Beyond Compare 4`，删除`CacheID`项，再打开软件会发现试用期恢复到30天。

或者将下述代码保存为`.bat`脚本：
```
reg delete "HKEY_CURRENT_USER\Software\Scooter Software\Beyond Compare 4" /v CacheID /f
```

然后在`Windows`中搜索打开`任务计划程序`，在面板右侧选择`创建基本任务`，按窗口提示操作即可，在`操作`选项中选择`启动程序`，然后选择上述保存的`.bat`文件。

