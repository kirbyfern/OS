#include <iostream>  // Include necessary header for input and output operations.
#include <string>     // Include necessary header for working with strings.

using namespace std; // Use the standard C++ namespace.

// Main function with command-line arguments.
int main(int argc, char* argv[]) { // argc holds the no. of command line arguments, argv stores the integer of the command line arguments into a vector
    if(argc != 3) { // Check if the number of command-line arguments is not 3.
      cerr << "Usage: " << argv[0] << " child_number name_of_child" << endl; // Print an error message with usage instructions.
      return 1; // Exit the program with an error code.
    }

    int childNumber = stoi(argv[1]); // Convert the first argument to an integer and store it as childNumber.
    string childName = argv[2];      // Store the second argument as childName.

    cout << "I am child number " << childNumber << ", and my name is " << childName << "." << endl; // Display the child's number and name.

    return 0; // Exit the program with a success code.
}