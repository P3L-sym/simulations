#include <iostream>

int myNum = 3;

void printNum() {
    std::cout << myNum << '\n';
}

int main() {
    //local variables == declared inside a function or block {}
    //global variables == declared outside of all functions

    printNum();
    std::cout << myNum << " From int main(){}\n 2:23:07";

    return 0;
}