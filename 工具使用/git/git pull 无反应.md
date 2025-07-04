
有时候提交有冲突，解决冲突或者回退代码后切换分支更新代码时会出现 git pull 无法更新远程仓库的代码，git fetch 会提示
```
...... canot lock ref refs/remote......
```

原因是本地存在与远程分支同名的残留引用，或者是引用文件锁定异常，直接清理：
```
git remote prune origin
```

或者强制清理并更新：
```
git fetch --prune
```

然后重新拉取更新：
```
git pull
```


其他方法：
```
git update-ref -d refs/remotes/origin/冲突分支名
```

```
git reset --hard origin/分支名
git pull
```


