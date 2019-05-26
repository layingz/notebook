Systemd是一个系统管理守护进程、工具和库的集合,用于集中管理和配置类UNIX系统,相比sysV init启动方式，最大改进就是引入并行启动，提高了启动效率，并行启动是因为systemd为每个启动的程序建立套接字，不同守护进程可以交互

### 简介
    1、systemd单位为unit，通过systemctl list-unit-files命令列出所有service以及状态

    2、target是一种将多个service聚合在一起以致于将它们同时启动的方式，
    service通过WantBy选项和target关联在一起，系统默认是multi-user状态，用systemctl isolate **.target 命令可以切换target，切换后，会启动该target的绑定的service

    3、程序默认把systemd放在/usr/lib/systemd/system目录下，通过systemctl enable/disable命令建立或者删除到/etc/systemd/system目录下的软连接

    4、启动时候加载/etc/systemd/system目录下的server

## systemd其他常用命令    
    systemctl start/stop/restart/kill   #启动、停止、重启、杀掉
    systemctl cat/status                #查看service文件、状态
    systemctl daemon-reload             #重新加载服务

## service配置文件
    $ systemctl cat sshd.service

    [Unit]
    Description=OpenSSH server daemon
    Documentation=man:sshd(8) man:sshd_config(5)
    After=network.target sshd-keygen.service
    Wants=sshd-keygen.service

    [Service]
    EnvironmentFile=/etc/sysconfig/sshd
    ExecStart=/usr/sbin/sshd -D $OPTIONS
    ExecReload=/bin/kill -HUP $MAINPID
    Type=simple
    KillMode=process
    Restart=on-failure
    RestartSec=42s

    [Install]
    WantedBy=multi-user.target

[Unit]

    Description 服务描述
    Documentation   字段文档位置
    Before/After   在以下服务启动之前/之后才能启动该服务
    Wants/Requires 弱依赖/强依赖以下的服务 强依赖表示被依赖的服务退出/启动该服务也会退出/启动

[Service]

    EnvironmentFile 环境参数

    ExecStart       启动程序执行的命令，$OPTIONS 来自EnvironmentFile的参数
    ExecReload      重启服务时执行的命令
    ExecStop        停止服务时执行的命令
    ExecStartPre    启动服务之前执行的命令
    ExecStartPost   启动服务之后执行的命令
    ExecStopPost    停止服务之后执行的命令

    Type            启动设置字段
        simple（默认值）：ExecStart字段启动的进程为主进程
        forking：ExecStart字段将以fork()方式启动，此时父进程将会退出，子进程将成为主进程
        oneshot：类似于simple，但只执行一次，Systemd 会等它执行完，才启动其他服务
        dbus：类似于simple，但会等待 D-Bus 信号后启动
        notify：类似于simple，启动结束后会发出通知信号，然后 Systemd 再启动其他服务
        idle：类似于simple，但是要等到其他任务都执行完，才会启动该服务。
            一种使用场合是为让该服务的输出，不与其他服务的输出相混合
    
    KillMode        停止设置字段
        control-group（默认值）：当前控制组里面的所有子进程，都会被杀掉
        process：只杀主进程
        mixed：主进程将收到 SIGTERM 信号，子进程收到 SIGKILL 信号
        none：没有进程会被杀掉，只是执行服务的 stop 命令。
    
    Restart         重启设置字段
        no（默认值）：退出后不会重启
        on-success：只有正常退出时（退出状态码为0），才会重启
        on-failure：非正常退出时（退出状态码非0），包括被信号终止和超时，才会重启
        on-abnormal：只有被信号终止和超时，才会重启
        on-abort：只有在收到没有捕捉到的信号终止时，才会重启
        on-watchdog：超时退出，才会重启
        always：不管是什么退出原因，总是重启
    
    RestartSec      表示 Systemd 重启服务之前，需要等待的秒数。

[Install]

    WantedBy        表示该服务所在的 Target


## Target配置文件

    $ systemctl cat multi-user.target

    [Unit]
    Description=Multi-User System
    Documentation=man:systemd.special(7)
    Requires=basic.target
    Conflicts=rescue.service rescue.target
    After=basic.target rescue.service rescue.target
    AllowIsolate=yes

    Requires、Conflicts、After   类似service
    AllowIsolate：允许使用systemctl isolate命令切换到multi-user.target。



