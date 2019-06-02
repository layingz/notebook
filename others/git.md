# 介绍
git是一个开源的分布式版本控制系统，用于项目版本管理

[官网文档](https://git-scm.com/book/zh/v2)

# 使用

## 1、下载安装
## 2、常用命令
    #初始化git仓库
    git init
    #下载远程仓库
    git clone username@host:/path/to/repository
    #添加文件
    git add *
    #提交代码
    git commit -m "message"
    #推送到远程master分支
    git push origin master
    #创建并切换分支
    git checkout -b feat_
    #删除远程分支
    git branch -d feat_
    #更新并且合并远端master到本地仓库
    git pull master
    #获取远端master改动
    git fetch master
    #合并其他分支代码到本地
    git merge *
    #创建标签
    git tag 标签号 提交id前10位字符
    #获取提交id
    git log

    #初始化子模块
    git submodule init
    #添加子模块
    git submodule add
    #下载包含子模块的代码
    git clone --recursive 地址
    or
    git submodule foreach git pull
    #切换子模块分支
    git submodule foreach git checkout feat_
    #合并子模块改动
    git submodule update --remote --merge
    #包含子模块推送代码
    git push --recurse-submodules=check
    #删除子模块
    git rm -r 路径/子模块

# git原理

