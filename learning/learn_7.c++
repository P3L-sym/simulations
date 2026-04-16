#include <iostream>

int main() {
    int month;

    std::cout << "======================================================================" << '\n';
    
    std::cout << "Enter the month (1-12): ";
    std::cin >> month;

    std::cout << "======================================================================" << '\n';

    if (month == 1) {
        std::cout << "It is January!" << '\n';
    } else if ( month == 2) {
        std::cout << "It is February!" << '\n';
    } else if ( month == 3) {
        std::cout << "It is March!" << '\n';
    } else if ( month == 4) {
        std::cout << "It is April!" << '\n';
    } else if ( month == 5) {
        std::cout << "It is May!" << '\n';
    } else if ( month == 6) {
        std::cout << "It is June!" << '\n';
    } else if ( month == 7) {
        std::cout << "It is July!" << '\n';
    } else if ( month == 8) {
        std::cout << "It is August!" << '\n';
    } else if ( month == 9) {
        std::cout << "It is September!" << '\n';
    } else if ( month == 10) {
        std::cout << "It is October!" << '\n';
    } else if ( month == 11) {
        std::cout << "It is November!" << '\n';
    } else {
        std::cout << "It is December!" << '\n';
    }

    std::cout << "======================================================================" << '\n';

    return 0;
}