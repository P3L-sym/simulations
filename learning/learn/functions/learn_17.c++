#include <iostream>

// string name doesnt have to be the same as 
void happyBirthday(std::string name, int age) {
    std::cout << "Happy birthday to you!\n";
    std::cout << "Happy birthday to you!\n";
    std::cout << "Happy birthday dear " << name << "!\n";
    std::cout << "Happy birthday to you!\n";
    std::cout << "Its your " << age << "st!\n";
}


int main() {

    std::string name = "Petri";
    int age = 21;

    happyBirthday(name, age);

    return 0;
}