// myShm.h
#ifndef MYSHM_H
#define MYSHM_H

#include <semaphore.h>

struct CLASS {
    sem_t index_sem;  // unnamed semaphore for guarding access to index
    int index;        // index to the next available response slot
    int response[10]; // each child writes its child number here
};

#endif
