#include <iostream>
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " name_1 name_2 ... name_k" << endl;
        return 1;
    }

    int numChildren = argc - 1;
    vector<string> childNames;

    // Store child names in a vector
    for (int i = 1; i < argc; ++i) {
        childNames.push_back(argv[i]);
    }

    cout << "I have " << numChildren << " children." << endl;

    // Create child processes
    for (int i = 0; i < numChildren; ++i) {
        pid_t pid = fork();

        if (pid == -1) {
            cerr << "Fork failed." << endl;
            return 1;
        } else if (pid == 0) {
            // Child process
            execl("./child", "./child", to_string(i + 1).c_str(), childNames[i].c_str(), nullptr);
            // If execl() fails
            cerr << "Exec failed for child process " << i + 1 << endl;
            return 1;
        }
    }

    // Wait for all child processes to terminate
    for (int i = 0; i < numChildren; ++i) {
        int status;
        wait(&status);
    }

    cout << "All child processes terminated. Parent exits." << endl;

    return 0;
}