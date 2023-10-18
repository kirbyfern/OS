#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <cerrno>

// header file that defines the shared memory structure
#include "myShm.h"

using namespace std;

int main(int argc, char* argv[]) {
    // Check if the correct number of command-line arguments is provided
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <number of child processes> <shared memory name>" << endl;
        return 1;
    }

    // Parse the number of child processes and the shared memory name from the command line
    int numChildProcesses = atoi(argv[1]);
    const char* sharedMemoryName = argv[2];

    // Create a new shared memory segment or open an existing one with read and write permissions
    int sharedMemoryFileDescriptor = shm_open(sharedMemoryName, O_CREAT | O_RDWR, 0666);
    if (sharedMemoryFileDescriptor == -1) {
        cerr << "shm_open failed: " << strerror(errno) << endl;
        return 1;
    }

    // Set the size of the shared memory segment
    if (ftruncate(sharedMemoryFileDescriptor, sizeof(CLASS)) == -1) {
        cerr << "ftruncate failed: " << strerror(errno) << endl;
        return 1;
    }

    // Map the shared memory segment into the process's address space
    CLASS* sharedMemory = static_cast<CLASS*>(mmap(NULL, sizeof(CLASS), PROT_READ | PROT_WRITE, MAP_SHARED, sharedMemoryFileDescriptor, 0));
    if (sharedMemory == MAP_FAILED) {
        cerr << "mmap failed: " << strerror(errno) << endl;
        return 1;
    }

    // Initialize the shared memory structure
    sharedMemory->index = 0;
    for (int i = 0; i < 10; i++) {
        sharedMemory->response[i] = 0;
    }

    // Output a message indicating the start of the program
    cout << "Master begins execution" << endl;
    cout << "Master created a shared memory segment named " << sharedMemoryName << endl;
    cout << "Master created " << numChildProcesses << " child processes to execute slave" << endl;

    // Create child processes
    for (int i = 0; i < numChildProcesses; i++) {
        // Fork a new child process
        pid_t childProcessId = fork();

        if (childProcessId == 0) {  // In the child process
            // Execute the slave program in the child process, passing necessary arguments
            execl("./slave", "./slave", to_string(i + 1).c_str(), sharedMemoryName, nullptr);
            return 1; // If execl fails, return an error code
        }
    }

    // Output a message indicating that the master process is waiting for child processes to terminate
    cout << "Master waits for all child processes to terminate" << endl;

    // Wait for all child processes to terminate
    for (int i = 0; i < numChildProcesses; i++) {
        int status;
        wait(&status);
    }

    // Output a message indicating that all child processes have terminated
    cout << "Master received termination signals from all " << numChildProcesses << " child processes" << endl;

    // Output the updated content of the shared memory structure
    cout << "Updated content of shared memory segment after access by child processes:" << endl;
    cout << "--- content of shared memory ---" << endl;
    for (int i = 0; i < 10; i++) {
        cout << "Response[" << i << "]: " << sharedMemory->response[i] << endl;
    }

    // Unmap the shared memory segment from the process's address space and close the file descriptor
    munmap(sharedMemory, sizeof(CLASS));
    close(sharedMemoryFileDescriptor);

    // Output a message indicating the removal of the shared memory segment and program exit
    cout << "Master removed the shared memory segment and is exiting" << endl;

    return 0;
}
