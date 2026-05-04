#include <iostream>

// string name doesnt have to be the same as 
void happyBirthday(std::string me) {
    std::cout << "Happy birthday to you!\n";
    std::cout << "Happy birthday to you!\n";
    std::cout << "Happy birthday dear " << me << "!\n";
    std::cout << "Happy birthday to you!\n\n";
}

int main() {

    std::string name = "Petri";

    happyBirthday(name);

    return 0;
}