#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <cstdlib>
#include <unistd.h>
#include <cstring>

// header file that defines the shared memory structure
#include "myShm.h"

using namespace std;

int main(int argc, char* argv[]) {
    // Check if the correct number of command-line arguments is provided
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <child number> <shared memory name>" << endl;
        return 1;
    }

    // Parse the child number and shared memory name from the command line
    int childNumber = atoi(argv[1]);
    const char* sharedMemoryName = argv[2];

    // Open the existing shared memory segment with read and write permissions
    int sharedMemoryFileDescriptor = shm_open(sharedMemoryName, O_RDWR, 0666);
    if (sharedMemoryFileDescriptor == -1) {
        cerr << "shm_open failed: " << strerror(errno) << endl;
        return 1;
    }

    // Map the shared memory segment into the process's address space
    CLASS* sharedMemory = static_cast<CLASS*>(mmap(NULL, sizeof(CLASS), PROT_READ | PROT_WRITE, MAP_SHARED, sharedMemoryFileDescriptor, 0));
    if (sharedMemory == MAP_FAILED) {
        cerr << "mmap failed: " << strerror(errno) << endl;
        return 1;
    }

    // Write the child number to the shared memory segment
    int index = sharedMemory->index;
    sharedMemory->response[index] = childNumber;
    sharedMemory->index = index + 1;

    // Output messages to indicate the start and end of the child process
    cout << "Slave begins execution" << endl;
    cout << "I am child number " << childNumber << ", received shared memory name " << sharedMemoryName << endl;
    cout << "I have written my child number to slot " << index << " and updated index to " << index + 1 << endl;
    cout << "Child " << childNumber << " closed access to shared memory and terminates" << endl;

    // Clean up shared memory by unmapping and closing
    munmap(sharedMemory, sizeof(CLASS));
    close(sharedMemoryFileDescriptor);

    return 0;
}
