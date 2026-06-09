#include <iostream>

int main()
{
    std::string car[6];

    car[0] = "Camaro";
    car[1] = "Mustang";
    car[2] = "Mclaren";
    car[3] = "Porche";
    car[4] = "Hyundai";
    car[5] = "Toyota";

    std::cout << car[0] << '\n';
    std::cout << car[1] << '\n';
    std::cout << car[2] << '\n';
    std::cout << car[3] << '\n';
    std::cout << car[4] << '\n';
    std::cout << car[5] << '\n';

    return 0;
}