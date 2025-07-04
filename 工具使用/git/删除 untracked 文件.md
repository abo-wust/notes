
检查未跟踪文件，预览将被删除的文件：
```
git clean -n -d
```

安全删除：
```
# 删除所有未跟踪文件，保留.gitignore 忽略的文件
git clean -f

#删除包括未跟踪目录在内的所有内容，文件+目录
git clean -fd
```


强制删除，包括 .gitignore 文件：
```
# 删除所有未跟踪文件，包括 .gitignore 忽略的文件
git clean -fx

# 删除文件+目录，包括 .gitignore 忽略的内容
git clean -fdx
```

选择性删除：
```
# 删除指定类型文件
git clean -f "*.log"

# 交互式删除，逐个文件确认
git clean -f -i
```


存档备份后删除：
```
# 创建备份目录
mkdir ../untracked_backup

# 移动所有未跟踪文件到备份目录
git ls-files --others --exclude-standard -z | xargs -0 -I {} mv {} ../untracked_backup
```
