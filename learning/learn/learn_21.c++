#include <iostream>

int main() {
    std::string car[] = {"Camaro", "Mustang", "Porsche", "Hyundai", "Toyota"};

    car[0] = "Camaro";
    car[1] =  "Mustang";
    car[2] = "Mclaren";
    car[3] = "Porche";

    std::cout << car[0] << '\n';
    std::cout << car[1] << '\n';
    std::cout << car[2] << '\n';

    return 0;
}