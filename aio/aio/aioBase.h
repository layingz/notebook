
#include <aio.h>
#include "atomic_pub.h"
#include <sys/time.h>

atomic_t cnt;

#define BUFFERSIZE (64 * 1024)
#define MAXLIST 16
#define AIOLIST    1024
#define SRC_DEFAULT "/root/testDate/test1MB"
#define DES_DEFAULT "/root/desDate/testDate"
//#define CALLBACK_THREAD 0


struct aioInfo 
{
    struct sigaction sig_act;
    struct aiocb **aio_list;
    int start;
    int size;
    int count;
    char *src_file;
    char *des_file;

    atomic_t cnt;

    //struct timeval read_start_time;
    //struct timeval write_start_time;
    //struct timeval read_end_time;
    //struct timeval write_end_time;
};


void read_finish(union sigval para);
void write_finish(union sigval para);
void aio_read_completion_handler(int signo, siginfo_t *info, void *context);
void aio_write_completion_handler(int signo, siginfo_t *info, void *context);