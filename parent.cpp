#include <iostream>        // Include necessary header for input and output operations.
#include <string>          // Include necessary header for working with strings.
#include <vector>          // Include necessary header for using vectors.
#include <sys/types.h>     // Include necessary header for system types.
#include <sys/wait.h>      // Include necessary header for wait functions.
#include <unistd.h>        // Include necessary header for system calls.

using namespace std;       // Use the standard C++ namespace.

int main(int argc, char* argv[]) {  // Main function with command-line arguments.
    if (argc < 2) {                // Check if there are not enough command-line arguments.
        cerr << "Usage: " << argv[0] << " name_1 name_2 ... name_k" << endl;  // Print an error message with usage instructions.
        return 1;                   // Exit the program with an error code.
    }

    int numChildren = argc - 1;    // Calculate the number of child processes.
    vector<string> childNames;     // Create a vector to store child names.

    // Store child names in a vector
    for (int i = 1; i < argc; ++i) {
        childNames.push_back(argv[i]);  // Add each child name to the vector.
    }

    cout << "I have " << numChildren << " children." << endl;  // Display the number of children.

    // Create child processes
    for (int i = 0; i < numChildren; ++i) {
        pid_t pid = fork();  // Create a child process.

        if (pid == -1) {
            cerr << "Fork failed." << endl;  // If fork fails, print an error message.
            return 1;                         // Exit the program with an error code.
        } else if (pid == 0) {
            // Child process
            execl("./child", "./child", to_string(i + 1).c_str(), childNames[i].c_str(), nullptr);
            // Execute the child program with appropriate arguments.

            // If execl() fails
            cerr << "Exec failed for child process " << i + 1 << endl;  // Print an error message.
            return 1;  // Exit the child process with an error code.
        }
    }

    // Wait for all child processes to terminate
    for (int i = 0; i < numChildren; ++i) {
        int status;
        wait(&status);  // Wait for a child process to finish.
    }

    cout << "All child processes terminated. Parent exits." << endl;  // Display a message when all children are done.

    return 0;  // Exit the program with a success code.
}