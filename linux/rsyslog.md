

2、日志文件描述

    /var/log/messages
    大多数系统日志消息记录在此处，除了与身份验证、电子邮件处理相关的定期运行左右的消息以及纯粹与调试相关的信息
    /var/log/secure
    安全和身份验证相关的消息和错误日志文件
    /var/log/maillog
    与邮件服务器相关的消息和错误的日志文件
    /var/log/cron
    与定期执行任务相关的日志文件
    /var/log/boot.log
    与系统启动相关的日志

3、日志优先级

    debug           调试级别日志 
    info            信息日志 
    notice          正常但是重要的事件
    warn(warning)   警告
    err(error)      非严重错误情况 
    crit(critical)  严重情况
    alert           必须立即采取措施
    emerg(panic)    系统不可用

2、rsyslog配置文件(/etc/rsyslog.conf)

    MODULES
    这个部分是针对接收配置的，主要是指定接收日志的协议和端口。若要配置日志服务器，则需要将相应的配置项去掉注释。
    GLOBAL DIRECTIVES
    这个部分主要用来配置模板，模板的作用是指定你希望在日志文件中保存的日志格式
    RULES
    规则


[RULES]

格式为

facilty.priority     target

    facilty 表示设备设施，表示从功能或程序上对日志收集进行分类，一般有下面这几类
    auth, authpriv, cron, daemon, kern, lpr, mail, mark, news, security, user, uucp, local0-local7, syslog

    priority 表示优先级，有下面几类
    debug, info, notice, warn(warning), err(error), crit(critical), alert, emerg(panic)

    target 表示将日志信息发送到哪里
        文件：记录日志事件于指定的文件中；通常应该位于/var/log目录下；文件路径之前的”-“表示异步写入；
        用户：将日志事件通知给指定的用户；是通过将信息发送给登录到系统上的用户的终端进行的；
        日志服务器：@host，把日志送往指定的服务器主机； host：即日志服务器地址，监听在tcp或udp协议的514端口以提供服务；
        管道： | COMMAND



