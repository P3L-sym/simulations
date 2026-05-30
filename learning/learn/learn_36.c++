#include <iostream>

int main() {
    int *pointer = nullptr;
    int x = 123;

    pointer = &x;

    if(pointer == nullptr) {
        std::cout << "address was not assigned";
    } else {
        std::cout << "adress was assigned\n";
        std::cout << *pointer;
    }

    return 0;
}