#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <aio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <getopt.h>
#include <pthread.h>
#include "atomic_pub.h"
#include <sys/time.h>
#include <signal.h>
#include "aioBase.h"

static struct option long_options[] =
{
    {"len",     1,  0,  'l'},
    {"srcfile",    2,  0,  's'},
    {0, 0, 0, 0},
};

int newaioInfo(struct aioInfo **aio, int size, char *src, char *des)
{
    //struct aiocb ** aiocblist;
    int count = 0, i = 0;
    struct aioInfo * aioinfo; 

    count = ((size + BUFFERSIZE)/ BUFFERSIZE) -1;
    aioinfo = (struct aioInfo *)malloc(sizeof(struct aioInfo));
    if(aioinfo == NULL)
    {
        printf("malloc aioinfo\n");
        return -1;
    }
    memset(aioinfo, 0x0, sizeof(struct aioInfo));
    aioinfo->src_file = (char*)malloc(64);
    aioinfo->des_file = (char*)malloc(64);

    aioinfo->aio_list = (struct aiocb **)calloc(count, sizeof(struct aiocb*));
    if(aioinfo->aio_list == NULL)
    {
        printf("malloc aioinfo aiolist\n");
        free(aioinfo);
        return -1;
    }
    
    for(i = 0; i < count; i++)
    {
        aioinfo->aio_list[i] = (struct aiocb *)malloc(sizeof(struct aiocb));
        if(aioinfo->aio_list[i] == NULL)
        {
            printf("malloc aioinfo aiolist, i:%d\n", i);
            //free aio_list[i]
            free(aioinfo->aio_list);
            free(aioinfo);
            return -1;
        }
    }

    aioinfo->size = size;
    aioinfo->count = count;
    aioinfo->src_file = src;
    aioinfo->des_file = des;

//#ifndef CALLBACK_THREAD
    sigemptyset(&aioinfo->sig_act.sa_mask);
    aioinfo->sig_act.sa_flags = SA_SIGINFO;
    aioinfo->sig_act.sa_sigaction = aio_read_completion_handler;
    //sigaction(SIGIO, &aioinfo->sig_act, NULL);
//#endif
    atomic_set(&aioinfo->cnt, 0);

    *aio = aioinfo;

    return 0;
}

int main(int argc, char* argv[])
{
    int32_t opt;
    int rfd = -1;
    int wfd = -1;
    int ret = 0, i = 0, j = 0, option_index = 0;
    char *srcfile = SRC_DEFAULT;
    struct aiocb *aio_ctl;
    struct aioInfo *aioinfo;
    struct timeval read_start_time;
    struct timeval write_start_time;
    struct timeval read_end_time;
    struct timeval write_end_time;
    int position = 0, len = 0, count = 0;
    double total_time;
    void * buff;
    struct sigevent aio_sigevent;

    atomic_set(&cnt, 0);

    while((opt = getopt_long(argc, argv, "l:s:", long_options, &option_index)) != -1)
    {
        switch(opt)
        {
            case 'l':
                len = atoi(optarg);
                break;
            case 's':
                srcfile = optarg;
                break;
            default:
                printf("unknow param.\n");
                break;
        }
    }
    if(len == 0)
    {
        perror("param null");
        return -1;
    }
    printf("len[%dMB],srcfile[%s],desfile[%s]\n", len, srcfile, DES_DEFAULT);
   
    rfd = open(srcfile, O_RDONLY);
    if (rfd == -1) 
    {
        perror("rfd open failed");
        return -1;
    }
    wfd = open(DES_DEFAULT, O_CREAT|O_WRONLY);
    if (wfd == -1) 
    {
        perror("wfd open failed");
        return -1;
    }

    len = len * 1024 * 1024;
    ret = newaioInfo(&aioinfo, len, srcfile, DES_DEFAULT);
    if (ret < 0)
    {
        perror("init aioinfo failed.");
        return 0;
    }


    for(i = 0; i < aioinfo->count; i++)
    {
        aio_ctl = aioinfo->aio_list[i];

        buff = malloc(BUFFERSIZE);
        if (!buff)
        {
            perror("malloc");
            goto out;
        }
        memset(buff, 0x0 , BUFFERSIZE);
        aio_ctl->aio_buf = buff;

        aio_ctl->aio_fildes = rfd;
        aio_ctl->aio_nbytes = BUFFERSIZE;
        aio_ctl->aio_offset = i * BUFFERSIZE;
        aio_ctl->aio_lio_opcode = LIO_READ;
        aio_ctl->aio_sigevent.sigev_value.sival_ptr = aio_ctl; 
        //aioinfo->aio_list[i]->aio_sigevent.sigev_value.sival_int = rfd;
        aio_ctl->aio_reqprio = 0;
        aio_ctl->aio_sigevent.sigev_notify_function = read_finish; 

    #ifndef CALLBACK_THREAD
        //内核通过信号量回调
        aio_ctl->aio_sigevent.sigev_notify = SIGEV_SIGNAL;
        aio_ctl->aio_sigevent.sigev_signo = SIGIO;
    #else
        //用户态系统回调函数
        aio_ctl->aio_sigevent.sigev_notify = SIGEV_THREAD;
        aio_ctl->aio_sigevent.sigev_notify_attributes = NULL;
    #endif
        
        atomic_inc(&aioinfo->cnt);
        atomic_inc(&cnt);
    }

    sigaction(SIGIO, &aioinfo->sig_act, NULL);

    aio_sigevent.sigev_notify = SIGEV_SIGNAL;
    aio_sigevent.sigev_signo = SIGIO;

    gettimeofday(&read_start_time, NULL);

    ret = lio_listio(LIO_NOWAIT, aioinfo->aio_list, aioinfo->count, NULL);
    if (ret < 0)
    {
        printf("lio_listio read failed,ret:%d,errno:%d\n", ret, errno);
        goto out;
    }

    gettimeofday(&read_end_time, NULL);

    //aio_fsync()

    total_time = read_end_time.tv_sec - read_start_time.tv_sec + (read_end_time.tv_usec - read_start_time.tv_usec) / 1000000.0;
    printf("read finish, cost time:%f ms\n", total_time * 1000);

    //#ifndef CALLBACK_THREAD
    while(1)
    {       
        if (atomic_read(&cnt) == 0)
        {
    //#endif 
            for(i = 0; i < aioinfo->count; i++)
            {
                aioinfo->aio_list[i]->aio_fildes = wfd;
                aioinfo->aio_list[i]->aio_lio_opcode = LIO_WRITE;
                aioinfo->aio_list[i]->aio_sigevent.sigev_notify_function = write_finish;
            }
            aioinfo->sig_act.sa_sigaction = aio_write_completion_handler;

            gettimeofday(&write_start_time, NULL);
            ret = lio_listio(LIO_WAIT, aioinfo->aio_list, aioinfo->count, NULL);
            if (ret < 0)
            {
                printf("lio_listio write failed, errno:%d\n", ret, errno);;
                //return -1;
                break;
            }  
            gettimeofday(&write_end_time, NULL);

            total_time = write_end_time.tv_sec - write_start_time.tv_sec + (write_end_time.tv_usec - write_start_time.tv_usec) / 1000000.0;
            printf("write finish, cost time:%f ms\n", total_time * 1000);
    //#ifndef CALLBACK_THREAD
            break;
        }
        
        //sleep(1);
    }
    //#endif

    /*
    while (aio_error(aio_ctl) == EINPROGRESS );
  
    if ((ret = aio_return(aio_ctl)) > 0)
    {

    } else {
        perror("aio_ctl write error");
    } */
out:
/*   
    for(i = 0; i < count; i++)
    {
        for(j = 0; j < aioList[i]->count; j++)
        {
            if(!aioList[i]->aio_list[j]->aio_buf)
            {
                free(&aioList[i]->aio_list[j]->aio_buf);
            }
            free(aioList[i]->aio_list[j]);
        }
    }
*/
    close(wfd);
    close(rfd);
    return 0;   
}

#if 0
int main(int argc, char* argv[])
{
    int32_t opt;
    int rfd = -1;
    int wfd = -1;
    int ret = 0, i = 0, count = 0, option_index = 0;
    char *file = SRC_DEFAULT, *position = NULL, *len = NULL;
    struct aiocb *aio_list[MAXLIST];
    struct aiocb *aio_ctl;
    int start, size;
    struct sigaction sig_act;
    struct timeval read_start_time;
    struct timeval write_start_time;
    struct timeval read_end_time;
    struct timeval write_end_time;

    bzero((char *)aio_list, sizeof(aio_list));
    atomic_set(&cnt, 0);

    while((opt = getopt_long(argc, argv, "p:l:f:", long_options, &option_index)) != -1)
    {
        switch(opt)
        {
            case 'p':
                position = optarg;
                break;
            case 'l':
                len = optarg;
                break;
            case 'f':
                file = optarg;
                break;
            default:
                printf("unknow param.\n");
                break;
        }
    }
    if(position == NULL || len == NULL){
        perror("param null");
        return -1;
    }
    printf("start[%sKB],len[%sKB],file[%s]\n", position, len, file);

    start = atoi(position) * 1024;
    size = atoi(len) * 1024;
   
    rfd = open(file, O_RDONLY);
    if (rfd == -1) 
    {
        perror("rfd open failed");
        return -1;
    }
    wfd = open(DES_DEFAULT, (O_CREAT|O_RDWR));
    if (wfd == -1) 
    {
        perror("wfd open failed");
        return -1;
    }
    



    sigemptyset(&sig_act.sa_mask);
    sig_act.sa_flags = SA_SIGINFO;
    sig_act.sa_sigaction = aio_read_completion_handler;

    count = ((size - start + BUFFERSIZE )/ BUFFERSIZE) -1;
    for(i = 0; i< count; i++)
    {
        aio_ctl = (struct aiocb *)malloc(sizeof(struct aiocb));
        bzero((char *)aio_ctl, sizeof(struct aiocb));
        aio_ctl->aio_buf = malloc(BUFFERSIZE+1);
        if (!aio_ctl->aio_buf)
        {
            perror("malloc");
            goto out;
        }
        aio_ctl->aio_fildes = rfd;
        aio_ctl->aio_nbytes = BUFFERSIZE;
        aio_ctl->aio_offset = i * BUFFERSIZE;
        aio_ctl->aio_lio_opcode = LIO_READ;

        
    #ifdef CALLBACK_THREAD
        //用户态系统回调函数
        aio_ctl->aio_sigevent.sigev_notify = SIGEV_THREAD;
        aio_ctl->aio_sigevent.sigev_notify_function = read_finish; 
        aio_ctl->aio_sigevent.sigev_notify_attributes = NULL;
    #else
        //内核通过信号量回调
        aio_ctl->aio_sigevent.sigev_notify = SIGEV_SIGNAL;
        aio_ctl->aio_sigevent.sigev_signo = SIGIO;
        ret = sigaction(SIGIO, &sig_act, NULL);
    #endif

        aio_ctl->aio_sigevent.sigev_value.sival_ptr = aio_ctl; 
        //aio_ctl->aio_sigevent.sigev_value.sival_int = rfd;
        aio_ctl->aio_reqprio = 0;

        aio_list[i] = aio_ctl;
        atomic_inc(&cnt);
    }

    gettimeofday(&start_time, NULL);
    ret = lio_listio(LIO_WAIT, aio_list, count, NULL);
    if (ret < 0)
    {
        perror("lio_listio");
        goto out;
    }

    sig_act.sa_sigaction =aio_write_completion_handler;
    while(1)
    {
        if (atomic_read(&cnt) == 0)
        { 
            for(i = 0; i< count; i++)
            {
                aio_list[i]->aio_fildes = wfd;
                aio_list[i]->aio_lio_opcode = LIO_WRITE;
            #ifdef CALLBACK_THREAD
                aio_list[i]->aio_sigevent.sigev_notify_function = write_finish;
            #endif               
            }
            
            ret = lio_listio(LIO_WAIT, aio_list, count, NULL);
            if (ret < 0)
            {
                perror("lio_listio");
                goto out;
            }
            break;
        }
    }

    /*
    while (aio_error(aio_ctl) == EINPROGRESS );
  
    if ((ret = aio_return(aio_ctl)) > 0)
    {

    } else {
        perror("aio_ctl write error");
    } */
out:            
    for(i = 0; i< count; i++)
    {
        if(!aio_list[i]->aio_buf)
        {
            free(&aio_list[i]->aio_buf);
        }
        free(aio_list[i]);
    }

    close(wfd);
    close(rfd);
    return 0;
}
#endif