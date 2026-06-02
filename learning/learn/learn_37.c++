#include <iostream>

int main() {
    //dynamic memory == memory that is allocated after the program
    //                  is already compiled and running
    //                  memory in the heap rather than the stack

    //                  Useful when we dont know how much memory
    //                  we will need. Makes our programs more flexible
    //                  especially when accepting user input
    int *pNum = NULL;

    pNum = new int;

    *pNum = 21;

    std::cout << "Address: " << pNum << '\n';
    std::cout << "Value: " << *pNum << '\n';

    delete pNum;

    return 0;
}