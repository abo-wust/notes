


## 清空所有 untracked files

清空所有未跟踪的文件：
```
git clean -fd

-f：表示强制执行
-d：表示同时删除未跟踪的目录
```

先查看将被删除的文件，只显示不实际执行删除操作：
```
git clean -n
```

示例脚本：
```
#!/bin/bash
echo "将要删除的文件："
git clean -n
read -p "确认要删除这些文件吗？（y/n）" -n 1 -r
#read -p "确认要删除这些文件吗？（y/n）" choice
#if [[ "$choice" == [Yy] ]]; then
if [[ $REPLY == [Yy] ]]; then
	git clean -fd
	echo "clean done"
else
	echo "action cancel"
fi

```


## 生成 patch 文件

当前所有修改的 patch：
```
git diff > changes.patch
```

当前分支修改文件很多，但是只需要生成其中部分文件修改的 patch：
```
git diff file1 file2 > changes.patch
```

特定 commit 的 patch：
```
git format-patch -1 <commit-hash>
```

生成从某个提交开始的系列 patch：
```
git format-patch <commit-hash>
```

生成最近 n 个提交的系列 patch：
```
git format-patch -n
```

指定生成 patch 文件的目录：
```
git format-patch -o /path/to/dir <commit-hash>
```

生成带完整提交信息的 patch：
```
git format-patch --attach
```

生成分支间的差异 patch：
```
git diff branch1..branch2 > differences.patch
```

应用 patch：
```
git apply changes.patch

#自动提交并保留提交信息
git am changes.patch
```





