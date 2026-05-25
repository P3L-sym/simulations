#include <iostream>

int main() {
    int temp;
    int spstemp;
    bool overheat;

    std::cout << "======================================================================" << '\n';

    std::cout << "Enter the temperature (Celsius): ";
    std::cin >> temp;

    std::cout << "======================================================================" << '\n';

    std::cout << "Enter the SpaceShip temperature (Celsius): ";
    std::cin >> spstemp;

    std::cout << "======================================================================" << '\n';

    //and
    if(temp > 0 && temp < 30) {
        std::cout << "The temperature is Good" << '\n';
    } else {
        std::cout << "The temperature is Bad" << '\n';
    }

    //or
    if(spstemp <=30 || spstemp >=150) {
        std::cout << "The SpaceShip temperature is Bad" << '\n';
    } else {
        std::cout << "The SpaceShip temperature is Good" << '\n';
    }

    //not
    if (!overheat) {
        std::cout << "The SpaceShip is not overheating" << '\n';
    } else {
        std::cout << "The SpaceShip is overheating" << '\n';
    }

    std::cout << "======================================================================" << '\n';

    return 0;
}