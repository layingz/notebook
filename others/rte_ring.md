# 介绍

    rte_ring是的的dpdk里的无锁队列，是一个FIFO的环形队列，实质是用inter的原子操作CAS实现的乐观锁，是一个多生产者和多消费者的模式

# api

创建

struct rte_ring *rte_ring_create(constchar *name, unsigned count, int socket_id, unsignedflags)

count: 队列长度必须是2的幂次方(保证溢出也是正常)

flags： 0表示多生产者、多消费者

入队

rte_ring_enqueue

出队

rte_ring_dequeue

# 入队出队逻辑

    生产者和消费者都有两个head和tail两个指针，向后先移动head，多线程的话就需要去竞争提交移动head，提交成功则移动，失败重试。移动完head移动tail

# 性能
rte_ring与mutex互斥锁性能测试结果    

    单元素和多核情况下sp/sc入队出队性能比mp/mc入队出队好
    桶和单核情况下sp/sc入队出队性能要比mp/mc入队出队差

    mutex性能要不如rte_ring，特别是在多线程或者多生产、消费者时候


