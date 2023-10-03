#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <cstdlib>
#include <sys/wait.h>

using namespace std;

// Define a structure for your message
struct Message {
    long messageType;
    char text[100]; // Adjust the message size as needed
};

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Usage: ./master <number_of_senders_and_receivers>" << endl;
        exit(1);
    }

    int numProcesses = atoi(argv[1]);

    // Create a message queue
    key_t msgQueueKey = ftok(".", 'a'); // You can choose a different key
    int msgQueueID = msgget(msgQueueKey, 0666 | IPC_CREAT);

    // Create sender processes
    for (int i = 0; i < numProcesses; i++) {
        pid_t senderPID = fork();
        if (senderPID == 0) {
            // Child process, execute sender.cpp
            execl("./sender", "./sender", to_string(msgQueueID).c_str(), NULL);
            exit(0);
        }
    }

    // Create receiver processes
    for (int i = 0; i < numProcesses; i++) {
        pid_t receiverPID = fork();
        if (receiverPID == 0) {
            // Child process, execute receiver.cpp
            execl("./receiver", "./receiver", to_string(msgQueueID).c_str(), NULL);
            exit(0);
        }
    }

    // Output master process information
    cout << "Master, PID " << getpid() << ", begins execution" << endl;
    cout << "Master acquired a message queue, id " << msgQueueID << endl;
    cout << "Master created " << numProcesses << " child processes to serve as sender/receiver" << endl;
    cout << "Master waits for all child processes to terminate" << endl;

    // Wait for all child processes to terminate
    for (int i = 0; i < numProcesses * 2; i++) {
        int status;
        wait(&status);
    }

    // Remove the message queue
    msgctl(msgQueueID, IPC_RMID, NULL);

    cout << "Master received termination signals from all child processes, removed message queue, and terminates" << endl;

    return 0;
}
