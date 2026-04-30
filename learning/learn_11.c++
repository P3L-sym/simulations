#include <iostream>
#include <string>

int main() {
    std::string name;

    while (name.empty()) {
        std::cout << "Enter your name: ";
        std::getline(std::cin, name); 
    }

    std::cout << "Hello " << name;

    while (1==1) {
        std::cout << "This is an infinite loop which is bad\n";
    }
}