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
#include <signal.h>
#include "aioBase.h"

void read_finish(union sigval para)
{
    int ret =0;
    struct aiocb *req;
    req = (struct aiocb *)para.sival_ptr;

    /* Did the request complete? */
    if (aio_error(req) == 0) {
        /* Request completed successfully, get the return status */
        ret = aio_return(req);
        if(ret <= 0){
            perror("read return failed");
        }
        else
        {
            printf("read finish cnt:%d\n", atomic_dec_return(&cnt));
        }
    }
    else
    {
        fprintf(stderr, "read aio error, err:%d\n", aio_error(req));
    }
}

void write_finish(union sigval para)
{
    int ret =0;
    struct aiocb *req;

    req = (struct aiocb *)para.sival_ptr;

    /* Did the request complete? */
    if (aio_error(req) == 0) {
        /* Request completed successfully, get the return status */
        ret = aio_return(req);
        if(ret <= 0){
            perror("write return failed");
        }
        else
        {
            //printf("write finish cnt");
        }
    }
    else
    {
        fprintf(stderr, "write aio error, err:%d\n", aio_error(req));
    }
}

void aio_read_completion_handler(int signo, siginfo_t *info, void *context)
{
    int ret =0;
    struct aiocb *req;
    /* Ensure it's our signal */
    if (info->si_signo == SIGIO)
    {
        req = (struct aiocb *)info->si_value.sival_ptr;
        /* Did the request complete? */
        while (aio_error(req) == EINPROGRESS);
        if (aio_error(req) == 0)
        {
            /* Request completed successfully, get the return status */
            ret = aio_return(req);
            printf("completion finish read cnt:%d,req:%d\n", atomic_dec_return(&cnt), ret);
        }
        else
        {
            printf("completion read error:%d\n", aio_error(req));
        }
    }
    else
    {
        printf("signo %d\n", info->si_signo);
    }

    return;
}

void aio_write_completion_handler(int signo, siginfo_t *info, void *context)
{
    int ret =0;
    struct aiocb *req;

    /* Ensure it's our signal */
    if (info->si_signo == SIGIO)
    {
        req = (struct aiocb *)info->si_value.sival_ptr;
        /* Did the request complete? */
        if (aio_error(req) == 0)
        {
            /* Request completed successfully, get the return status */
            ret = aio_return(req);
            printf("completion finish write cnt:%d\n", ret);
        }
        else
        {
            printf("completion write error:%d\n", aio_error(req));
        }
        
    }

  return;
}