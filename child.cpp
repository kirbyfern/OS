#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    if(argc != 3) {
      cerr << "Usage: " << argv[0] << " child_number name_of_child" << endl;

      return 1;
    }

    int childNumber = stoi(argv[1]);
    string childName = argv[2];

    cout << "I am child number " << childNumber << ", and my name is " << childName << "." << endl;

    return 0;
}
