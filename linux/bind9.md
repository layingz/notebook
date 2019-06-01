1、介绍

    bind9是linux下的dns服务的软件，可以主从部署，位于tcp/ip网络协议的应用层，osi模型的应用层，默认是53端口，核心是用socket和epoll的方式对查询、查询、更新、通知进行边缘触发

[官方介绍](https://www.centos.bz/manual/BIND9-CHS.pdf)

2、配置文件/etc/named.conf

3、添加解析域配置

    在/etc/named.rfc1912.zones添加正向和反向解析内容，添加完成使用named-checkconf检查添加规则是否正确
    主服务器配置：
    zone "test.com" IN {
        type master;
        file "test.com.zone";
        allow-query {any;};
        allow-transfer{ 
            192.168.36.189;
        };
        notify yes;
        also-notify{
            192.168.36.189;
        };
    };

    zone "2.168.192.in-addr.arpa" IN {
            type master;
            file "192.168.2.arpa";
            allow-transfer {
                192.168.36.189;
            };
            allow-query {any;};
    };
    从服务器配置：
    zone "test.com" IN {
        type slave;
        file "test.com.zone";
        masters{
                192.168.36.54;
        };
    };
    zone "2.168.192.in-addr.arpa" IN {
            type slave;
             masters{
                192.168.36.54;
            };
            file "192.168.2.arpa";
            allow-query {any;};
    };

4、创建正向解析
    
    在/var/named/目录下创建test.com.zone
    设置权限    chown named:named test.com.zone
    添加以下内容：
    $TTL 1D
    @	IN SOA	test.com. rname.invalid. (
                        0	; serial
                        1D	; refresh
                        1H	; retry
                        1W	; expire
                        3H )	; minimum
            NS      @
            A       127.0.0.1
            AAAA    ::1
    ns	IN	A	192.168.2.244
    node1	IN	A	192.168.2.245

    检查配置是否正确
    named-checkzone "test.com" /var/named/test.com.zone 

5、创建反向解析

    在/var/named/目录下添加192.168.2.arpa反向文件
    修改权限    chown root:named 192.168.2.arpa
    $TTL 1D
    @	IN SOA	test.com. rname.invalid. (
                        0	; serial
                        1D	; refresh
                        1H	; retry
                        1W	; expire
                        3H )	; minimum
        IN	NS	@
        A	127.0.0.1
        AAAA	::1
        PTR	localhost.
    ns	IN	A	192.168.2.244
    244	IN	PTR	ns.test.com

    检查配置是否正确
    named-checkzone "192.168.2.in-addr.arpa" /var/named/192.168.2.arpa

    第一行配置解释：

    name  [ttl]  IN  RRT  value

    name：资源记录名称
    ttl：资源记录缓存时长（可以省略）
    RRT（resouce record type）：资源记录类型
    vlaue：该条资源记录的值
    
    参数解释：

    SOA（起始授权记录）：定义了该区域谁是主DNS服务器、管理员邮箱地址、否定答案的缓存时间，且定义了该区域中的主从DNS服务器直接的数据如何同步。

    NS（域名服务器记录）：定义了该区域内的DNS服务器，可以有多条。（需要有正向解析记录）

    MX（邮件服务器记录）：定义了该区域内的邮件服务器，可以有多条。（需要有正向解析记录）

    A（正向解析记录）：定义了该区域内的主机的主机名与IP地址的对应关系，可以有多条。

    PTR（反向解析记录）：定义了该区域内的主机的IP地址与I主机名的对应关系，可以有多条

6、重启服务并用dig命令测试是否成功