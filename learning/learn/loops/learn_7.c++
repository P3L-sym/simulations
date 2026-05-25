#include <iostream>

int main() {
    int month;
    char grade;

    std::cout << "======================================================================" << '\n';
    
    std::cout << "Enter the month (1-12): ";
    std::cin >> month;

    std::cout << "What Letter Grade: ";
    std::cin >> grade;

    std::cout << "======================================================================" << '\n';

    switch (month) {
        case 1:
            std::cout << "It is January!" << '\n'; 
            break;
        case 2:
            std::cout << "It is February!" << '\n';
            break;
        case 3:
            std::cout << "It is March!" << '\n'; 
            break;
        case 4:
            std::cout << "It is April!" << '\n';
            break;
        case 5:
            std::cout << "It is May!" << '\n';
            break;
        case 6:
            std::cout << "It is June!" << '\n';
            break;
        case 7:
            std::cout << "It is July!" << '\n'; 
            break;
        case 8:
            std::cout << "It is August!" << '\n';
            break;
        case 9:
            std::cout << "It is September!" << '\n'; 
            break;
        case 10:
            std::cout << "It is October!" << '\n';
            break;
        case 11:
            std::cout << "It is November!" << '\n';
            break;
        case 12:
            std::cout << "It is December!" << '\n';
            break;
        default:
            std::cout << "Please Enter in only number 1 through 12" << '\n';
    }

    switch (grade) {
        case 'A':
            std::cout << "You did great!" << '\n'; 
            break;
        case 'B':
            std::cout << "You did good" << '\n';
            break;
        case 'C':
            std::cout << "You did okay" << '\n'; 
            break;
        case 'D':
            std::cout << "You didnot do good" << '\n';
            break;
        case 'F':
            std::cout << "YOU FAILED" << '\n';
            break;
        default:
            std::cout << "Please Enter in your Letter Grade" << '\n';
    }

    // if (month == 1) {
    //     std::cout << "It is January!" << '\n';
    // } else if ( month == 2) {
    //     std::cout << "It is February!" << '\n';
    // } else if ( month == 3) {
    //     std::cout << "It is March!" << '\n';
    // } else if ( month == 4) {
    //     std::cout << "It is April!" << '\n';
    // } else if ( month == 5) {
    //     std::cout << "It is May!" << '\n';
    // } else if ( month == 6) {
    //     std::cout << "It is June!" << '\n';
    // } else if ( month == 7) {
    //     std::cout << "It is July!" << '\n';
    // } else if ( month == 8) {
    //     std::cout << "It is August!" << '\n';
    // } else if ( month == 9) {
    //     std::cout << "It is September!" << '\n';
    // } else if ( month == 10) {
    //     std::cout << "It is October!" << '\n';
    // } else if ( month == 11) {
    //     std::cout << "It is November!" << '\n';
    // } else {
    //     std::cout << "It is December!" << '\n';
    // }

    std::cout << "======================================================================" << '\n';

    return 0;
}