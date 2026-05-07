#include <iostream>

int myNum = 3;

void printNum() {
    //it will take from global cause of the ::
    int myNum = 2;
    std::cout << ::myNum << " From global\n";
}

int main() {
    //local variables == declared inside a function or block {}
    //global variables == declared outside of all functions

    int myNum = 1;

    printNum();
    std::cout << myNum << " From int main()\n";

    return 0;
}