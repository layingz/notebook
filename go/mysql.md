
## 介绍

## 使用


## 性能测试

环境：服务器处理器志强Bronze 3104 6核, 内存64g，hdd盘，mysql版本14.14

测试结果

    ---bulk  100 time single GOROUTINE-----
    use time: 2.272308629s eve(100 count/ms): 22.723086ms
    ----bulk  100 multi GOROUTINE----
    use time: 1.0041488s eve(100 count/ms): 10.041488ms
    ---txn ALL ONCE----
    txn all: 1.903638822s insert single time: 190.363?s
    ---txn  100  time SINGLE GOROUTINE----
    txn all: 3.923673472s insert single time: 39.236734ms
    ---txn  100  time Multi GOROUTINE----
    txn all: 1.117633679s insert single time: 11.176336ms

测试时候mysql程序 cpu占用6%，内存1%

分析
    
插入相同个数事务性提交比批量插入性能要差一些，但是在多给goroutine下，性能相差变小
