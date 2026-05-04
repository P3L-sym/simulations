#include <iostream>

double square(double length){
    double result = length * length;
    return result;
}

double cube(double length){
    double result = length * length * length;
    return result;
}

std::string cStrings(std::string fName, std::string lName){
    std::string fString = fName + " " + lName;
    return fString;
}

int main() {
    double length;
    std:: string fName;
    std:: string lName;

    std::cout << "============================================================\n";

    std::cout << "What is your First Name: ";
    std::cin >> fName;

    std::cout << "What is your Last Name: ";
    std::cin >> lName;

    std::cout << "============================================================\n";

    std::cout << "What length do you want to use: ";
    std::cin >> length;

    double area = square(length);
    double volume = cube(length);
    std:: string name = cStrings(fName, lName);

    std::cout << "============================================================\n";

    std::cout << "Hello " << name << "!\n";
    std::cout << "Your starting length is " << length << "!\n";

    std::cout << "Area: " << area << "cm^2\n";
    std::cout << "Volume: " << volume << "cm^3\n";

    std::cout << "============================================================\n";

    return 0;
}