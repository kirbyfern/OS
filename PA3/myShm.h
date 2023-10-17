// myShm.h
#ifndef MYSHM_H
#define MYSHM_H

struct CLASS {
    int index;       // index to the next available response slot
    int response[10]; // each child writes its child number here
};

#endif
