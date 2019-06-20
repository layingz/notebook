## 一、介绍

MariaDB是MySQL的分支版本。它主要是由于MySQL在被Oracle公司收购时出现的问题而开发的。MariaDB是一个通用的数据库管理系统（DBMS），它具有可扩展的架构，可通过可插拔存储引擎支持大量的用例。它使用不同的存储引擎来支持不同的用例 

MariaDB由MySQL的创始人麦克尔·维德纽斯主导开发，他早前曾以10亿美元的价格，将自己创建的公司MySQL卖给了SUN，此后，随着SUN被甲骨文收购，MySQL的所有权也落入Oracle的手中。

MariaDB名称来麦克尔·维德纽斯的女儿玛丽亚（英语：Maria）的名字。

## 二、mariadb和mysql比较

优点

    MariaDB针对性能进行了优化，对于大型数据集，它比MySQL强大得多。从其他数据库系统可以优雅的迁移到MariaDB是另一个好处。
    从MySQL切换到MariaDB相对容易，这对于系统管理员来说好像是一块蛋糕。
    MariaDB通过引入微秒级精度和扩展用户统计数据提供更好的监控。
    MariaDB增强了KILL命令，使您可以杀死用户的所有查询（KILL USER 用户名）或杀死查询ID（KILL QUERY ID query_id）。MariaDB也转而使用Perl兼容的正则表达式（PCRE），它提供比标准MySQL正则表达式支持更强大和更精确的查询。
    MariaDB为与磁盘访问，连接操作，子查询，派生表和视图，执行控制甚至解释语句相关的查询应用了许多查询优化。
    MariaDB纯粹是开源的，而不是MySQL使用的双重授权模式。一些仅适用于MySQL Enterprise客户的插件在MariaDB中具有等效的开源实现。
    与MySQL相比，MariaDB支持更多的引擎（SphinxSE，Aria，FederatedX，TokuDB，Spider，ScaleDB等）。
    MariaDB提供了一个用于商业用途的集群数据库，它也支持多主复制。任何人都可以自由使用它，并且不需要依赖MySQL Enterprise系统。

缺点

    从版本5.5.36开始，MariaDB无法迁移回MySQL。
    对于MariaDB的新版本，相应的库（用于Debian）不会及时部署，由于依赖关系，这将导致必需升级到较新的版本。
    MariaDB的群集版本不是很稳定。

## 三、安装启动Mariadb

    1、安装mariadb，将mariadb文件夹rpm拷贝到设备，执行
        rpm -ivh * --forece
    2、启动
        systemctl enable maridb
        systemctl start maridb
    3、进入mariadb，执行
        mysql
## 四、性能测试

环境：cpu: 志强e3,内存64g
软件版本：
    mariadb 5.5.60版本 引擎InnoDB
    mysql 5.6.44社区版本 引擎InnoDB
数据库表头：
    id int
    name varchar(20)
    age varchar(10)
    sex varchar(10)
插入操作：
mariadb

    ---bulk  100 time single GOROUTINE-----
    use time: 710.639862ms eve(100 count/ms): 7.106398ms
    ----bulk  100 multi GOROUTINE----
    use time: 211.927958ms eve(100 count/ms): 2.119279ms
    ---txn ALL ONCE----
    txn all: 1.331180851s insert single time: 133.118µs
    ---txn  100  time SINGLE GOROUTINE----
    txn all: 2.100493824s insert single time: 21.004938ms
    ---txn  100  time Multi GOROUTINE----
    txn all: 349.005952ms insert single time: 3.490059ms

mysql

    ---bulk  100 time single GOROUTINE-----
    use time: 2.243023416s eve(100 count/ms): 22.430234ms
    ----bulk  100 multi GOROUTINE----
    use time: 1.116943016s eve(100 count/ms): 11.16943ms
    ---txn ALL ONCE----
    txn all: 1.956498492s insert single time: 195.649µs
    ---txn  100  time SINGLE GOROUTINE----
    txn all: 3.448137774s insert single time: 34.481377ms
    ---txn  100  time Multi GOROUTINE----
    txn all: 1.179982725s insert single time: 11.799827ms

结论：在10000次测试中，无论批量插入、事务插入，mariadb性能都比mysql好

查询操作：

mariadb

    QUERY time: 27.052324ms
    QUERY 1000 key time: 59.210674ms

mysql

    QUERY time: 28.514422ms
    QUERY 1000 key time: 67.58057ms

结论：一次查询10000个key的速度差不多，每次查询3个key查询1000次mariadb要比mysql快

## 五、语法

    显示库
    show database;
    使用tesst数据库
    use test;
    显示表 
    show tables;
    查看user表头
    desc user; 
    查看当前引擎
    show variables like '%storage_engine%';
    创建表
    CREATE TABLE table_name (column_name column_type);
    删除表
    DROP TABLE table_name;
    插入数据
    INSERT INTO table_name (field1, field2,...fieldN)VALUES(value1，value2,...valueN); 
    查询数据
    SELECT column_name,column_name FROM table_name [WHERE Clause][LIMIT N][ OFFSET M];
    更新数据
    UPDATE table_name SET field1=new-value1, field2=new-value2[WHERE Clause];
    删除数据
    DELETE FROM table_name [WHERE Clause];
    清空表
    truncate table user；
