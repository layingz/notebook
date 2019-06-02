# 介绍

jemalloc是facebook优化glibc的malloc的内存管理技术，主要在以下几个方面进行优化

    Jmalloc小对象也根据size-class，但是它使用了低地址优先的策略，来降低内存碎片化。

    Jemalloc大概需要2%的额外开销。（tcmalloc 1%， ptmalloc最少8B）

    Jemalloc和tcmalloc类似的线程本地缓存，避免锁的竞争

    相对未使用的页面，优先使用dirty page，提升缓存命中

优点：小块性能提升明显，特别是在使用多线程时候


缺点：对于重复释放内存文件检查不出来

# 使用

    1、安装好，用ldconfig库能够加载
    2、头文件包含#include <jemalloc/jemalloc.h>
    3、编译选项添加-ljemalloc
    4、对编译好的二进制进行ldd命令查看释放链接到jemalloc库