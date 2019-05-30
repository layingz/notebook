
## 介绍

    NTP服务服务端和客户端共存的软件

## NTP服务配置说明

    配置文件：/etc/ntp.conf
    权限控制-仅服务端段设置：
    restrict IP mask netmask_IP parameter
    IP: 软件地址，也可以是 default ，default 就类似 0.0.0.0
    paramter：
        ignore:     关闭所有的 NTP 联机服务
        nomodify：  Client 端不能更改 Server 端的时间参数，不过，
        Client 端仍然可以透过 Server 端来进行网络校时。
        notrust：   该 Client 除非通过认证，否则该 Client 来源将被视为不信任网域
        noquery：   不提供 Client 端的时间查询
        notrap：    不提供trap这个远程事件登入

    若paramter为空，表示该 IP (或网域)“没有任何限制”

    例子：
    restrict default nomodifynotrapnoquery　# 关闭所有的 NTP 要求封包
    restrict 127.0.0.1　　　 #这是允许本级查询
    restrict 192.168.0.1 mask 255.255.255.0 nomodify

    服务设定：
    server [IP|HOST Name] [prefer]
    perfer:     NTP主机主要以该部主机来作为时间校正的对应
    iburst：    如果在一个标准的轮询间隔内没有应答，客户端会发送一定数量的包（八个包而不是通常的一个）给 NTP 服务器。如果在短时间内呼叫 NTP 服务器几次，没有出现可辨识的应答，那么本地时间将不会变化。
    driftfile： 联机花费的时间记录文件
    stratum     时间服务器的层次。设为0则为顶级，如果要向别的NTP服务器更新时间，不要把它设为0

    例子
    server 2.pool.ntp.org
    server cn.pool.ntp.org prefer
    server  127.127.1.0     # local clock
    fudge   127.127.1.0 stratum 10
    driftfile /var/lib/ntp/drift
    broadcastdelay  0.008
    keys /etc/ntp/keys

## NTP相关命令

    ntpq -p     查询NTP Server状态
    hwclock --systohc   系统时间写到硬件
    hwclock --hctosys   硬件时间写到系统
    ntpdate *.*.*.*     强制更新系统时间
    hwclock --set --date="mm/dd/yy hh:mm:ss"  更新硬件时间
    date -s "dd/mm/yyyy hh:mm:ss"   更新系统时间

## ntpq -p命令

    remote           refid      st t when poll reach   delay   offset  jitter
    +193.60.199.75   193.62.22.98     2 u   52   64  377    8.578   10.203 289.032
    *mozart.musicbox 192.5.41.41      2 u   54   64  377   19.301  -60.218 292.411

    remote: 它指的就是本地机器所连接的远程NTP服务器
    refid: 它指的是给远程服务器(e.g. 193.60.199.75)提供时间同步的服务器
    st: 远程服务器的层级别（stratum）. 由于NTP是层型结构,有顶端的服务器,多层的Relay Server再到客户端. 所以服务器从高到低级别可以设定为1-16. 为了减缓负荷和网络堵塞,原则上应该避免直接连接到级别为1的服务器的.
    t: 这个.....我也不知道啥意思^_^
    when: 我个人把它理解为一个计时器用来告诉我们还有多久本地机器就需要和远程服务器进行一次时间同步
    poll: 本地机和远程服务器多少时间进行一次同步(单位为秒). 在一开始运行NTP的时候这个poll值会比较小,那样和服务器同步的频率也就增加了,可以尽快调整到正确的时间范围.之后poll值会逐渐增大,同步的频率也就会相应减小
    reach: 这是一个八进制值,用来测试能否和服务器连接.每成功连接一次它的值就会增加
    delay: 从本地机发送同步要求到服务器的round trip time
    offset: 这是个最关键的值, 它告诉了我们本地机和服务器之间的时间差别. offset越接近于0,我们就和服务器的时间越接近
    jitter: 这是一个用来做统计的值. 它统计了在特定个连续的连接数里offset的分布情况. 简单地说这个数值的绝对值越小我们和服务器的时间就越精确    

## 其他

    1、NTP和ntpupdate加cron组合比较优势

    使用ntpd服务，要好于ntpdate加cron的组合。因为，ntpdate同步时间，会造成时间的跳跃，对一些依赖时间的程序和服务会造成影响。比如sleep，timer等。而且，ntpd服务可以在修正时间的同时，修正cpu tick。理想的做法为，在开机的时候，使用ntpdate强制同步时间，在其他时候使用ntpd服务来同步时间。

    2、NTP端口为UDP的123端口

    3、NTP安装包有以下两个：
        ntpdate-*.rpm
        ntp-*.rpm