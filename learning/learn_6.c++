#include <iostream>
#include <cmath>
#include <string>

int main() {
    std::string name;
    int age;
    int a;
    std::cout << "======================================================================" << '\n';
    std::cout << "Want to sign up for the Space Program" << '\n';
    std::cout << "======================================================================" << '\n';

    std::cout << "Enter your full name: ";
    std::getline(std::cin, name);
    std::cout << "Enter your age: ";
    std::cin >> age;

    a = 21 - age;

    std::cout << "======================================================================" << '\n';

    if (age >= 21 && age <= 80){
        std::cout << "Thank You for signing up" << '\n';
        std::cout << "Welcome to the Space Program" << '\n';
    } else if (age <= 21){
        std::cout << "Sorry you are too Young" << '\n';
        std::cout << "Please come back in " << a << " years!" << '\n';
    } else {
        std::cout << "Sorry you are too Old" << '\n';
    }

    std::cout << "======================================================================" << '\n';

    return 0;
}