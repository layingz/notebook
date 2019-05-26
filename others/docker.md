
# windows10下docker使用

1、windows启用hyper-v,启用后需要重启电脑

    控制面板->程序->启动或关闭Windows功能

2、下载安装 Docker for Windows

[下载地址](https://github.com/boot2docker/windows-installer/releases)

3、安装完成
 
    docker --version

4、设置镜像加速，国内连接不稳定，设置阿里云进行加速

[阿里云地址](https://cr.console.aliyun.com/)

    1、注册或者进入账户
    2、进入镜像中心-镜像加速器目录
    3、拷贝加速器地址
    4、在系统右下角托盘图标内右键菜单选择 Settings，打开配置窗口后左侧导航菜单选择 Docker Daemon。编辑窗口内的JSON串，填写下方加速器地址
    5、编辑完成后点击 Apply 保存按钮，等待Docker重启并应用配置的镜像加速器

5、启动centos
    
    在cmd或者powershell命令行操作
    1、docker pull centos ##下载centos
    2、docker images ##查看是否可用
    3、docker network create --subnet=10.0.0.0/8 mynetwork #创建网络
    4、docker network ls #查看网络
    5、启动
    docker run -v /C:/Users/shared：/root/shared -h centosN1 --name centosN1 --net mynetwork --ip 10.0.75.2  -tdi --privileged centos init ##固定up
    docker run -p 23:22 -v /C:/Users/shared：/root/shared -h centosN1 --name centosN1 -tdi --privileged centos init #ssh通过23端口连接
     
    6、docker exec -it centosN1 bash #连接centos
    7、passwd   #设置密码
    8、yum install -y openssh-server
    9、/etc/ssh/sshd_config添加PermitRootLogin yes
    10、ssh通过物理机ip加上端口23进行登陆

    命令行参数：
    -p 物理机端口：容器端口
    -h 主机名
    --name 容器名
    --net 网络名
    --ip 容器网络ip
    -v 容器centos系统和本地目录共享