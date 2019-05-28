
rsysylog处理流程

    输入模块->预处理模块->主队列->过滤模块->执行队列->输出模块

    输入模块包括imklg、imsock、imfile、imtcp等，是消息来源

    预处理模块主要解决各种syslog协议实现间的差异，举例说明如果日志系统client端使用rsyslog、server端使用syslog-ng，如果自己不做特殊处理syslog-ng是无法识别的。但是反过来，rsyslog的server端就可以识别syslog-ng发过来的消息

    过滤模块模块处理消息的分析和过滤，rsyslog可以根据消息的任何部分进行过滤，后面会介绍到具体的做法

    输出模块包括omfile、omprog、omtcp、ommysql等。是消息的目的地

    Queue模块负责消息的存储，从Input传入的未经过滤的消息放在主队列中，过滤后的消息放入到不同action queue中，再由action queue送到各个输出模块

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

[MODULES]

    module(load="imfile")

[GLOBAL DIRECTIVES]

    template(name="ThisIsTemplateName" type="string" string="/tmp/custom_filename_%$year%-%$month%-%$day%.log")



[RULES]

    ruleset( name="ruleset11" ){
    action(type="omfwd" Target="10.116.1.1" Port="514" Protocol="tcp" template="nginx_a" )
    action(type="omfile" dynaFile="ThisIsTemplateName" fileOwner="develop" fileCreateMode="0655" template="nginx_b" )
    stop
    }




