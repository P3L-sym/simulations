#include <iostream>
#include <cmath>

int main() {
    double c;
    double a;
    double b;

    std::cout << "======================================================================" << '\n';
    std::cout << "Solve the hypotenuse of a right angle triangle" << '\n';

    std::cout << "Declare a: ";
    std::cin >> a;
    std::cout << "Declare b: ";
    std::cin >> b;

    c = sqrt(pow(a, 2) + pow(b, 2));

    std::cout << "======================================================================" << '\n';

    std::cout << "c = " << c << '\n';
    std::cout << "======================================================================" << '\n';
    return 0;
}