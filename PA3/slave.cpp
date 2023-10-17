// slave.cpp
#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include "myShm.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <child number> <shared memory name>" << endl;
        return 1;
    }

    int childNumber = atoi(argv[1]);
    const char* shmName = argv[2];

    int shm_fd = shm_open(shmName, O_RDWR, 0666);
    if (shm_fd == -1) {
        cerr << "shm_open failed: " << strerror(errno) << endl;
        return 1;
    }

    CLASS* shm = static_cast<CLASS*>(mmap(NULL, sizeof(CLASS), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0));
    if (shm == MAP_FAILED) {
        cerr << "mmap failed: " << strerror(errno) << endl;
        return 1;
    }

    // Write child number to shared memory
    int index = shm->index;
    shm->response[index] = childNumber;
    shm->index = index + 1;

    cout << "Slave begins execution" << endl;
    cout << "I am child number " << childNumber << ", received shared memory name " << shmName << endl;
    cout << "I have written my child number to slot " << index << " and updated index to " << index + 1 << endl;
    cout << "Child " << childNumber << " closed access to shared memory and terminates" << endl;

    // Clean up shared memory
    munmap(shm, sizeof(CLASS));
    close(shm_fd);

    return 0;
}
