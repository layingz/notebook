常用命令

1、grep

    grep / -name * 在/目录下查找文件名为*文件
    grep / -r * 在目录下递归查找包含*字符的文件
    grep -v 反向查找即过滤
2、awk

    awk '条件   动作'   文件名

    -F  截取字符
    FS  输入字段分割符 默认为空
    OFS 输出字段分隔符 默认为空
    RS  输入记录分隔符  默认为换行符
    ORS 输出记录分隔符  默认为换行符
    NF  当前多少列（字段）
    $NF 最后一列（字段）
    NR  第几行
    toupper()   大写
    olower()    字符转为小写
    length()    返回字符串长度
    substr()    返回子字符串
    sin()       正弦
    cos()       余弦
    sqrt()      平方根
    rand()      随机数
    ......

    '{print $3 $7}'  第三列和第七列

3、sed

    sed -e 4a\newline testfile      在第四行后添加新字符串
    sed '2,5d'                      删除第二到第五行

4、ps

    ps是Process Status的缩写，用来列出系统中当前运行的那些进程。ps命令列出的是当前进程的快照，就是执行ps命令这个时刻的进程，可以使用top命令获取动态的进程信息

    ps -A
    显示所有进程

    ps -ef
    显示所有进程包括命令行提示符信息

    ps -aux |grep *
    查看*进程所有信息状态

4、 netstat

    netstat 命令用于显示各种网络相关信息，如网络连接，路由表，接口状态(Interface Statistics),masquerade连接，多播成员(Multicast Memberships)等

    netstat -aux 显示所有状态下unix域的udp连接
    netstat -lt 显示所有listen状态的tcp连接

    netstat -c:    每隔1秒输出当前连接信息

    netstat -apn |grep  显示特定连接

5、lsof

    lsof，List Open Files 列出当前系统打开文件的工具。在linux环境下，任何事物都以文件的形式存在，通过文件不仅仅可以访问常规数据，还可以访问网络连接和硬件(lsof强大原因)。所以如传输控制协议 (TCP) 和用户数据报协议 (UDP) 套接字等，系统在后台都为该应用程序分配了一个文件描述符，无论这个文件的本质如何，该文件描述符为应用程序与基础操作系统之间的交互提供了通用接口

    常用
    lsof -i：* 显示*打开的端口/文件
    lsof -p * 列出进程号为*的文件

6、 watch

    实时监测命令
    -d 高亮显示
    -n  每隔几秒显示
    -t  关闭顶部显示的间隔时间

7、sar

    sar（System Activity Reporter系统活动情况报告）是目前Linux上最为全面的系统性能分析工具之一，可以从多方面对系统的活动进行报告，包括：文件的读写情况、系统调用的使用情况、磁盘I/O、CPU效率、内存使用状况、进程活动及IPC有关的活动等。

    sar -n DEV 1 2 显示网口信息，每秒一次，取2次

    IFACE：LAN接口
    rxpck/s：每秒钟接收的数据包
    txpck/s：每秒钟发送的数据包
    rxbyt/s：每秒钟接收的字节数
    txbyt/s：每秒钟发送的字节数
    rxcmp/s：每秒钟接收的压缩数据包
    txcmp/s：每秒钟发送的压缩数据包
    rxmcst/s：每秒钟接收的多播数据包

8、iostat

    主要用于监控系统设备的IO负载情况，iostat首次运行时显示自系统启动开始的各项统计信息，之后运行iostat将显示自上次运行该命令以后的统计信息。用户可以通过指定统计的次数和时间来获得所需的统计信息。iostat的数据的主要来源是/proc/partitions

    iostat -d 1 3
    显示io负载，每秒刷新一次，输出3次后退出

9、iotop

    用来监视磁盘I/O使用状况的top类工具。iotop具有与top相似的UI，其中包括PID、用户、I/O、进程等相关信息

    iotop常用快捷键
    左右箭头 --> 改变排序方式，默认是按IO排序
    r --> 改变排序顺序
    o --> 只显示有IO输出的进程
    p --> 进程/线程的显示方式的切换
    a --> 显示累积使用量
    q --> 退出




