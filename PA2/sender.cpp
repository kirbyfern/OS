#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <cstdlib>
#include <sys/wait.h>

using namespace std;

struct Message {
    long messageType;
    char text[100]; // Adjust the message size as needed
};

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Usage: ./sender <message_queue_id>" << endl;
        exit(1);
    }

    int msgQueueID = atoi(argv[1]);

    // Create a message structure
    Message message;
    message.messageType = 1; // You can use a different message type

    cout << "Sender, PID " << getpid() << ", begins execution" << endl;
    cout << "Sender with PID " << getpid() << " received message queue id " << msgQueueID << " through command-line argument" << endl;

    cout << "Sender with PID " << getpid() << ": Please input your message" << endl;
    cin.getline(message.text, sizeof(message.text));

    // Send the message to the message queue
    msgsnd(msgQueueID, &message, sizeof(message.text), 0);

    cout << "Sender with PID " << getpid() << " sent the following message into message queue:" << endl;
    cout << message.text << endl;

    // Wait for an acknowledgement from the receiver
    Message acknowledgement;
    msgrcv(msgQueueID, &acknowledgement, sizeof(acknowledgement.text), 2, 0);

    cout << "Sender with PID " << getpid() << " receives the following acknowledgement message:" << endl;
    cout << acknowledgement.text << endl;

    cout << "Sender with PID " << getpid() << " terminates" << endl;

    return 0;
}
