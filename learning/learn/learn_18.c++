#include <iostream>

double square(double length){
    double result = length * length;
    return result;
}

int main() {
    double length;

    std::cout << "============================================================\n";

    std::cout << "What length do you want to use: ";
    std::cin >> length;

    double area = square(length);

    std::cout << "============================================================\n";

    std::cout << "Area: " << area << "cm^2\n";

    std::cout << "============================================================\n";

    return 0;
}