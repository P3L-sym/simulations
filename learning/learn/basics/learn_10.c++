#include <iostream>
#include <string>
#include <algorithm>

int main()
{
    std::string name;

    std::cout << "Enter your name: ";
    std::getline(std::cin, name);

    if (name.length() > 12) {
        std::cout << "Your name cant be over 12 Characters";
    } else if (name.empty()) {
        std::cout << "You didnt enter your name";
    } else {
        std::cout << "Welcome " << name << "\n";
        name.erase(remove_if(name.begin(), name.end(), isspace), name.end());
        name.append("@gmail.com");
        std::cout << "Your username is: " << name;
    }

    return 0;
}