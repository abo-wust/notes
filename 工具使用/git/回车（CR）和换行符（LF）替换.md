
Windows下使用git add或者git diff查看修改内容时会有警告
```
warning: in the working copy of 'xxx.md', LF will be replaced by CRLF the next time Git touches it
```


原因是Windows系统默认的换行符是：回车（CR）+换行（LF），即“\r\n”，而Linux系统默认是：换行（LF），即“\n”。

设置方法一：
```
git config --global core.autocrlf true
```

适用于Windows系统，一般为默认设置，会在提交时对换行符进行CRLF->LF的转换，checkout检出时进行LF->CRLF的转换。


设置方法二：
```
git config --global core.autocrlf input
```

适用于Linux系统，所有换行符都会进行CRLF->LF的转换，但操作时不会转换回CRLF。


设置方法三：
```
git config --global core.autocrlf false
```

适用于Windows系统，且只在Windows上开发使用的情况。在提交、检出时都不会对CRLF/LF换行符进行转换。


文件提交时进行safecrlf检查：
```
#拒绝提交包含混合换行符的文件
git config --global core.safecrlf true

#允许提交包含混合换行符的文件
git config --global core.safecrlf false

#提交包含混合换行符的文件时给出警告
git config --global core.safecrlf warn
```












