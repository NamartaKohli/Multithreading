#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>
#include <stdlib.h>

#define SHAREDMEM_FILENAME "shared_file2"
typedef struct
{
    // Synchronisation objects
    pthread_mutex_t ipc_mutex;
    pthread_mutexattr_t mutex_attr;
    // Shared data
    int number;
    char data[1024];
} shared_data_t;
