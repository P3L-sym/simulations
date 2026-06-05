#include <iostream>

void printInfo(const std::string name, const int age) {
    std::cout << name << '\n';
    std::cout << age << '\n';
}

int main()
{
    std::string name = "Petri";
    int age = 21;

    printInfo(name, age);

    return 0;
} 