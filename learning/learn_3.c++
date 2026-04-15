#include <iostream>
#include <string>

int main() {
    std::string name;
    int age;

    std::cout << "Whats your full name?: ";
    std::getline(std::cin, name);
    
    std::cout << "What is your age?: ";
    std::cin >> age;

    std::cout << "Hello " << name << "!" << '\n';
    std::cout << "You are " << age << " years old!";

    return 0;
}