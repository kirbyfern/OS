#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <cstdlib>
#include <sys/wait.h>
#include <string.h>
#include <cstring>

using namespace std;

struct Message {
    long messageType;
    char text[100]; // Adjust the message size as needed
};

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Usage: ./receiver <message_queue_id>" << endl;
        exit(1);
    }

    int msgQueueID = atoi(argv[1]);

    // Create a message structure
    Message message;
    message.messageType = 1; // You can use a different message type

    cout << "Receiver, PID " << getpid() << ", begins execution" << endl;
    cout << "Receiver with PID " << getpid() << " received message queue id " << msgQueueID << " through command-line argument" << endl;

    // Receive a message from the message queue
    msgrcv(msgQueueID, &message, sizeof(message.text), 1, 0);

    cout << "Receiver with PID " << getpid() << " retrieved the following message from message queue:" << endl;
    cout << message.text << endl;

    // Send an acknowledgement message to the message queue
    strcpy(message.text, "Receiver acknowledges receipt of message");
    message.messageType = 2; // Use a different message type for acknowledgements
    msgsnd(msgQueueID, &message, sizeof(message.text), 0);

    cout << "Receiver with PID " << getpid() << " terminates" << endl;

    return 0;
}
