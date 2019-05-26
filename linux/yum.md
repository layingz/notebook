## yum原理和介绍



## yum重要目录

    /var/lib/yum：（yum的库）
    /usr/lib/yum-plugins ：插件的库（可执行二进制文件）
    /etc/logrotate.d/yum
    /etc/yum：yum配置存放的目录
    /etc/yum.conf ：yum全局配置
    /etc/yum.repos.d：仓库本地配置
    /etc/yum/fssnap.d：快照配置
    /etc/yum/pluginconf.d：插件配置
    /etc/yum/protected.d：保护包的配合
    /etc/yum/vars：yum可以使用的变量设置
    /etc/yum/version-groups.conf：帮助文件目录
    /var/cache/yum ：yum程序使用中的一些缓存文件

## yum命令

    1.列出所有可更新的软件清单
    yum check-update

    2.安装所有更新软件
    yum update

    3.仅安装指定的软件
    yum install

    4.删除软件
    yum remove

    5.列出所有可安裝的软件清单
    yum list

    6.清除原有的yum信息   
    yum clean all

    7.生成缓存
    yum makecache

    8.搜索软件
    yum search

    8.仅下载不安装，仅限于未安装的软件
    yum install --downloadonly --downloaddir=



## 本地yum源和nginx

1、创建/data/yum路径
2、yum配置文件放在/etc/yum.repos.d/，需要填写以下内容

    name=yum源名称
    baseurl=url路径,比如http://*.*.*.*/yum/
    #gpgkey = gpg密钥的地址(可不填)
    enabled= 1
    gpgcheck = 0 (是否检查，如果0表示不检查，1表示检查)

3、nginx添加以下配置

    server { 
            listen 80; 
            server_name *; 
            location ~ / {
                index index.html; 
                root /data;
            }
    }

4、把rpm放进/data/yum目录下，重新制作yum缓存


## 替换国内yum源(centos7)

阿里云源

curl -o /etc/yum.repos.d/CentOS-aliyun-Base.repo http://mirrors.aliyun.com/repo/Centos-7.repo

163源

curl -o /etc/yum.repos.d/CentOS-163-Base.repo http://mirrors.163.com/.help/CentOS7-Base-163.repo

epel

wget -O /etc/yum.repos.d/epel.repo http://mirrors.aliyun.com/repo/epel-7.repo 

elrepo源

rpm -Uvh http://www.elrepo.org/elrepo-release-7.0-2.el7.elrepo.noarch.rpm