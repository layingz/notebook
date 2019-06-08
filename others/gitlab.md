# Gitlab介绍

GitLab：是一个基于Git实现的在线代码仓库托管软件，你可以用gitlab自己搭建一个类似于Github一样的系统，一般用于在企业、学校等内部网络搭建git私服。Gitlab 是一个提供代码托管、提交审核和问题跟踪的代码管理平台。对于软件工程质量管理非常重要。

Gitlab服务构成：

    Nginx：静态web服务器。
    gitlab-shell：用于处理Git命令和修改authorized keys列表。（Ruby）
    gitlab-workhorse: 轻量级的反向代理服务器。（go）
    logrotate：日志文件管理工具。
    postgresql：数据库。
    redis：缓存数据库。
    sidekiq：用于在后台执行队列任务（异步执行）。（Ruby）
    unicorn：An HTTP server for Rack applications，GitLab Rails应用是托管在这个服务器上面的。（Ruby Web Server,主要使用Ruby编写）


[官方文档](https://docs.gitlab.com/ee/README.html)

centos7安装

    依赖安装
    sudo yum install -y curl policycoreutils-python openssh-server
    sudo systemctl enable sshd
    sudo systemctl start sshd
    sudo firewall-cmd --permanent --add-service=http
    sudo systemctl reload firewalld
    下载包
    curl https://packages.gitlab.com/install/repositories/gitlab/gitlab-ee/script.rpm.sh | sudo bash
    安装
    rpm -ivh 

gitlab命令

    #初始化，就执行一次
    gitlab-ctl reconfigure 
    #查询状态
    gitlab-ctl status   