// myShm.h
#ifndef MYSHM_H
#define MYSHM_H

#include <semaphore.h>

struct CLASS {
    int index;       // index to the next available response slot
    int response[10]; // each child writes its child number here
    sem_t unnamed_semaphore; // unnamed semaphore for index variable access
};

#endif
