#include<stdio.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
#include<libaio.h>
#include<errno.h>
#include<unistd.h>
#include <sys/time.h>
#include <sys/uio.h>
#include <sys/eventfd.h>
#include <sys/epoll.h>
//#include <linux/aio_abi.h>

#define SRC_DEFAULT "/root/testDate/4MB"
#define DES_DEFAULT "/root/desDate/des"
#define RWSIZE (4*1024*1024)

#define PAGE_SIZE 4096
#define RWLIST 256
#define AIOSIZE  (RWLIST * PAGE_SIZE)
#define MAXEVENTS 256

#define PADDED(x, y)	x, y

static void read_done(io_context_t ctx, struct iocb *iocb, long res, long res2)
{
    if (res2 != 0) {
        printf("aio read error\n");
    }
    printf("recv key:%d, flags:%d, res:%d\n", iocb->key, iocb->u.c.flags, iocb->u.c.resfd);
    if (res != (iocb->u.c.nbytes * PAGE_SIZE)) {
        printf("read missed bytes expect % d got % d\n", iocb->u.c.nbytes * PAGE_SIZE, res);
        exit(1);
    }

    //free(iocb->u.c.buf);
    //free(iocb);
}

static void write_done(io_context_t ctx, struct iocb *iocb, long res, long res2)
{
    if (res2 != 0) {
        printf("aio write error\n");
    }
    if (res != (iocb->u.c.nbytes * PAGE_SIZE)) {
        printf("write missed bytes expect % d got % d\n", iocb->u.c.nbytes * PAGE_SIZE, res);
        exit(1);
    }

    free(iocb->u.c.buf);
    free(iocb);
}

int main(void){
    int write_fd, read_fd;
    io_context_t read_ctx;
    io_context_t write_ctx;
    struct io_event *events = NULL;
    struct timespec timeout;
    struct iovec *iovec = NULL;
    int i = 0,j=0, count = 0, done = 0, ret = 0;
    int iovecsize = AIOSIZE, rwsize = RWSIZE;
    size_t iov_nr = 0, iov_size = 0;
    int aiocount = 0, offset = 0, tmpsize = 0;
    io_callback_t cb = read_done;
    int eventreadfd = 0, epfd = 0, eventwritefd = 0;
    struct iocb **pwrite, **pread;
    struct iocb *io;
    struct epoll_event epevent;
    uint64_t finished_aio =0;
    uint16_t a16 = 1;
    uint16_t b16 = 1;
    uint64_t a = 1;

    memset(&read_ctx,0,sizeof(read_ctx)); 
    memset(&write_ctx,0,sizeof(write_ctx)); 

    eventreadfd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    eventwritefd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);

    /*-------------------read---------------------------*/
#ifdef EPOLLEVENT
    epfd = epoll_create(1);

    epevent.events = EPOLLIN;
    epevent.data.ptr = NULL;
    printf("enter epoll read\n");
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, eventreadfd, &epevent);
    if (ret < 0)
    {
        printf("epoll_ctl failed, ret:%d\n", ret);
        return -1;
    }
#endif

    //准备读ctx
    ret = io_setup(MAXEVENTS, &read_ctx);
    if (ret != 0){
        printf("io_setup read error\n"); 
        return -1; 
    }

    read_fd = open(SRC_DEFAULT, O_RDONLY);
    if (read_fd < 0)
    {   
        perror("open src error");
        io_destroy(read_ctx);
        return -1; 
    }
    aiocount = ((RWSIZE - 1) / AIOSIZE + 1);
    
    pread = (struct iocb **)calloc(aiocount, sizeof(struct iocb*));
    //准备读
    for(i = 0; i < aiocount; i++)
    {
        //a += i;
        io = (struct iocb *)malloc(sizeof(struct iocb));
        memset(io, 0x0, sizeof(struct iocb));

        offset = i * AIOSIZE;
        tmpsize = rwsize > AIOSIZE ? AIOSIZE : rwsize;

        iov_nr = ((tmpsize - 1) / PAGE_SIZE + 1);

        iovec = (struct iovec *)calloc(RWLIST, sizeof(struct iovec));
        bzero(iovec, sizeof(struct iovec));

        iovecsize = tmpsize;
        for(j = 0; j < iov_nr; j++)
        {
            iov_size = iovecsize > PAGE_SIZE ? PAGE_SIZE : iovecsize;
            iovec[j].iov_base = calloc(1, PAGE_SIZE);
            iovec[j].iov_len = iov_size;
            iovecsize -= iov_size;
        }
        rwsize -= tmpsize;

        io_prep_preadv(io, read_fd, iovec, iov_nr, offset);
        io_set_callback(io, cb);
        //PADDED(io->key, io->__pad2) = a;
        //io->key = (a16 +i) << 4;
        //io->u.c.flags = b16 << 16;
        //io->u.c.flags += i;
        //io->u.c.resfd = i;
        io->key = i;
        //io->u.c.__pad3 = a;
        printf("send key:%d, res:%d\n", io->key, io->u.c.resfd);
        //设置evnetts
        io_set_eventfd(io, eventreadfd);
        pread[i] = io;
    }

    //读提交
    //while(ret < aiocount)
    //{
        ret += io_submit(read_ctx, aiocount, pread);
        if(ret != aiocount)
        {
            io_destroy(read_ctx);
            printf("io_submit preadv error, ret:%d, count:%d\n", ret, aiocount);
            return -1; 
        }
        //pread += 64;
        //printf("io_submit preadv error, ret:%d, count:%d\n", ret, aiocount);
    //}
    //printf("sleep before\n");
    //leep(10);
    //printf("sleep after\n");
    events = (struct io_event *)calloc(aiocount, sizeof(struct io_event));
#ifndef EPOLLEVENT
    //接受读完成信息
    while(done < aiocount){
        timeout.tv_sec=1;
        timeout.tv_nsec=500000000;//0.5s
        done += io_getevents(read_ctx, 0, aiocount, events, &timeout);
        if (done > 0)
        {
            //读回调    
            for(i = 0; i < done; i++)
            {
                ((io_callback_t)(events[i].data))(read_ctx, (struct iocb *)events[i].obj, events[i].res, events[i].res2);
                //printf("read events[%d]: obj = %p res = %ld res2 = %ld\n", i, events[i].obj, events[i].res, events[i].res2);
            }
        }
        printf("read FINISH %d io, evcount:%d\n", done, aiocount);       
        //sleep(1);
    }
#else

    i = 0;
    while(i < aiocount)
    {
        finished_aio = 0;
        ret = epoll_wait(epfd, &epevent, 1, -1);
        if (ret != 1)
        {
            printf("epoll wait error,ret:%d\n", ret);
            return -1;
        }

        ret = read(eventreadfd, &finished_aio, sizeof(finished_aio));
        if (ret != sizeof(finished_aio))
        {
            printf("read ret:%d, finish:%d\n", ret, sizeof(finished_aio));
            return -1;
        }

        while(finished_aio > 0)
        {
            timeout.tv_sec=0;
            timeout.tv_nsec=0;//0.5s
            done += io_getevents(read_ctx, 0, aiocount, events, &timeout);
            printf("read done:%d, aiocount:%ld\n", done, finished_aio);
            if (done > 0)
            {
                for(j = 0; j < done; j++)
                {
                    ((io_callback_t)(events[j].data))(read_ctx, events[j].obj, events[j].res, events[j].res2);
                }
                i += done;
                finished_aio -= done;
            } 
        }

    }
#endif
    /*-------------------write---------------------------*/
    pwrite = (struct iocb **)calloc(aiocount, sizeof(struct iocb*));
    cb = write_done;
    //准备写ctx
    ret = io_setup(MAXEVENTS, &write_ctx);
    if (ret != 0){
        printf("io_setup write error\n"); 
        return -1; 
    } 

    ret = (write_fd=open(DES_DEFAULT,O_CREAT|O_WRONLY));
    if (ret < 0)
    {   
        perror("open des error");
        io_destroy(write_ctx);
        return -1; 
    }

#ifdef EPOLLEVENT
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, eventwritefd, &epevent);
    if (ret < 0)
    {
        printf("epoll_ctl failed, ret:%d\n", ret);
    }
#endif

    //准备写
    for(i = 0; i < aiocount; i++)
    {
        io = (struct iocb *)malloc(sizeof(struct iocb));
        memset(io, 0x0, sizeof(struct iocb));
        io_prep_pwritev(io, write_fd, (struct iovec *)(pread[i]->u.c.buf), pread[i]->u.c.nbytes, i * AIOSIZE);
        io_set_callback(io, cb);
        pwrite[i] = io;
        io_set_eventfd(io, eventwritefd);
    }

    //写提交
    ret = io_submit(write_ctx, aiocount, pwrite);
    if (ret != aiocount)
    {
        io_destroy(write_ctx);
        printf("io_submit pwritev error, ret:%d, count:%d\n", ret, aiocount);    
        return -1;
    }

    bzero(events, sizeof(events));

#ifndef EPOLLEVENT
    //写等通知
    done = 0;
    while(done < aiocount){
        timeout.tv_sec=1;
        timeout.tv_nsec=500000000;//0.5s
        done += io_getevents(write_ctx, 0, aiocount, events, &timeout);
        if (done > 0)
        {
             //写回调
            for(i = 0; i < done; i++)
            {
                ((io_callback_t)(events[i].data))(write_ctx, (struct iocb *)events[i].obj, events[i].res, events[i].res2);

                //释放读的内存
                free(pread[i]);
                //printf("write events[%d]: obj = %p res = %ld res2 = %ld\n", i, events[i].obj, events[i].res, events[i].res2);
            }
        }
        printf("write FINISH %d io, count:%d\n", done, aiocount);       
        //sleep(1);
    }
#else

    i = 0;
    done = 0;
    while(i < aiocount)
    {
        finished_aio = 0;
        ret = epoll_wait(epfd, &epevent, 1, -1);
        if (ret != 1)
        {
            printf("epoll wait error,ret:%d\n", ret);
            return -1;
        }

        ret = read(eventwritefd, &finished_aio, sizeof(finished_aio));
        if (ret != sizeof(finished_aio))
        {
            printf("write ret:%d, finish:%d\n", ret, sizeof(finished_aio));
            return -1;
        }


        while(finished_aio > 0)
        {
            timeout.tv_sec=0;
            timeout.tv_nsec=0;//0.5s
            done += io_getevents(write_ctx, 0, aiocount, events, &timeout);
            printf("write done:%d, aiocount:%ld\n", done, finished_aio);
            if (done > 0)
            {
                for(j = 0; j < done; j++)
                {
                    ((io_callback_t)(events[j].data))(write_ctx, events[j].obj, events[j].res, events[j].res2);
                }
                i += done;
                finished_aio -= done;
            } 
        }
    }
    close(epfd);
#endif
    free(events);
    close(read_fd);   
    io_destroy(read_ctx);
    close(write_fd);
    io_destroy(write_ctx);
    return 0;
}