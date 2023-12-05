#include <iostream>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/wait.h>
#include "myShm.h"

using namespace std;

/*
* To compile:
* g++ master.cpp -o master -lrt -lpthread
* g++ slave.cpp -o slave -lrt -lpthread
*/

/*
After running the master program, it will clean up the shared memory and semaphores. However, if you encounter issues, you can manually remove the shared memory and semaphore using the following commands:

ipcrm -M /my_shared_memory  # Remove shared memory
ipcrm -S /my_named_semaphore  # Remove named semaphore
*/

int main(int argc, char *argv[]) {
    // Check the command line arguments
    if (argc != 4) {
        cerr << "Usage: " << argv[0] << " n /shm_name /sem_name" << endl;
        return 1;
    }

    // Parse command line arguments
    int num_child_processes = stoi(argv[1]);
    const char* shm_name = argv[2];
    const char* sem_name = argv[3];

    // Create and initialize shared memory
    int shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(struct CLASS));
    struct CLASS* shared_data = (struct CLASS*)mmap(0, sizeof(struct CLASS), PROT_WRITE, MAP_SHARED, shm_fd, 0);

    // Initialize the shared data structure
    shared_data->index = 0;
    sem_init(&(shared_data->unnamed_semaphore), 1, 1); // Initialize unnamed semaphore

    // Print initial messages
    cout << "./master " << num_child_processes << " " << shm_name << " " << sem_name << endl;
    cout << "Master begins execution" << endl;
    cout << "Master created a shared memory segment named " << shm_name << " [ " << shm_name << " is from commandline ]" << endl;
    cout << "Master initializes index in the shared structure to zero" << endl;

    // Create and initialize named semaphore for I/O control
    sem_t* io_semaphore = sem_open(sem_name, O_CREAT, 0666, 1);
    if (io_semaphore == SEM_FAILED) {
        perror("sem_open");
        return 1;
    }
    cout << "Master created a semaphore named " << sem_name << " [ " << sem_name << " is from commandline ]" << endl;

    // Create child processes
    pid_t child_pids[num_child_processes];

    for (int i = 0; i < num_child_processes; ++i) {
        pid_t child_pid = fork();
        if (child_pid == 0) {
            // Execute the slave program with child-specific arguments
            execlp("./slave", "./slave", to_string(i).c_str(), shm_name, sem_name, NULL);
            perror("execlp");
            return 1;
        } else if (child_pid > 0) {
            child_pids[i] = child_pid;
        } else {
            perror("fork");
            return 1;
        }
    }

    // Print status messages
    cout << "Master created " << num_child_processes << " child processes to execute slave [ The number n is from commandline ]" << endl;
    cout << "Master waits for all child processes to terminate" << endl;

    // Wait for child processes to terminate
    for (int i = 0; i < num_child_processes; ++i) {
        waitpid(child_pids[i], NULL, 0);
    }

    // Print final status messages and shared memory content
    cout << "Master received termination signals from all " << num_child_processes << " child processes" << endl;
    cout << "Updated content of shared memory segment after access by child processes:" << endl;
    cout << "--- content of shared memory --- [ Current content of shared memory ]" << endl;
    for (int i = 0; i < shared_data->index; ++i) {
        cout << "Child number " << shared_data->response[i] << ": " << shared_data->response[i + 1] << endl;
        i++; // Skip to the next pair
    }

    // Clean up and exit
    sem_close(io_semaphore);
    sem_unlink(sem_name);
    munmap(shared_data, sizeof(struct CLASS));
    shm_unlink(shm_name);

    cout << "Master removed the semaphore" << endl;
    cout << "Master closed access to shared memory, removed shared memory segment, and is exiting" << endl;

    return 0;
}
