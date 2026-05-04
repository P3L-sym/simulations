#include <iostream>

void happyBirthday(std::string name) {
    std::cout << "Happy birthday to you!\n";
    std::cout << "Happy birthday to you!\n";
    std::cout << "Happy birthday dear " << name << "!\n";
    std::cout << "Happy birthday to you!\n\n";
}

int main() {

    std::string name = "Petri";

    happyBirthday(name);

    return 0;
}