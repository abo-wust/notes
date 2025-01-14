
需要回退已经提交的修改，首先找到提交前的commit号，然后reset到该指定commit：
```
git reset --hard <commit id>
```

这样之前提交的修改都会还原，所有提交信息会被删掉，然后再push到远程仓库：
```
git push -f origin <origin branch name>
```

需要加上`-f`参数强制推送，否则会提示本地分支版本落后于远程仓库。push之前如果需要其他修改，重新commit修改后的代码，再push就行。



