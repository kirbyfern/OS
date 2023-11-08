#include <iostream>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>
#include "myShm.h"

using namespace std;

int main(int argc, char *argv[]) {
    // Check the command line arguments
    if (argc != 4) {
        cerr << "Usage: " << argv[0] << " x /shm_name /sem_name" << endl;
        return 1;
    }

    // Parse command line arguments
    int child_number = stoi(argv[1]);
    const char* shm_name = argv[2];
    const char* sem_name = argv[3];

    // Open the shared memory
    int shm_fd = shm_open(shm_name, O_RDWR, 0666);
    struct CLASS* shared_data = (struct CLASS*)mmap(0, sizeof(struct CLASS), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    // Open the semaphore
    sem_t* semaphore = sem_open(sem_name, 0);

    // Print initial messages
    cout << "Slave begins execution" << endl;
    cout << "I am child number " << child_number << ", received shared memory name " << shm_name << " and " << sem_name << endl;

    int lucky_number;
    cout << "Child number " << child_number << ": What is my lucky number? ";
    cin >> lucky_number;

    // Ensure mutual exclusion with semaphore
    sem_wait(semaphore);

    // Write data to shared memory
    shared_data->response[shared_data->index] = child_number;
    shared_data->response[shared_data->index + 1] = lucky_number;
    shared_data->index += 2;

    // Release the semaphore
    sem_post(semaphore);

    // Close access to shared memory
    munmap(shared_data, sizeof(struct CLASS));

    // Print the termination message
    cout << "Child " << child_number << " closed access to shared memory and terminates" << endl;

    return 0;
}
