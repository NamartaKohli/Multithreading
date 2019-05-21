#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define SHARED_SYNC_FILENAME "shared_sync_file"
#define SHARED_DATA_FILENAME "shared_data_file"
typedef struct
{
    /* Synchronisation objects */
    pthread_mutex_t ipc_mutex;
    pthread_mutexattr_t mutex_attr;
    pthread_cond_t  ipc_condvar;
    pthread_condattr_t cond_attr;
} shared_sync_t;

typedef struct 
{
   /* shared data */
   int status; 
   char msg[25];
}shared_data_t;
 
