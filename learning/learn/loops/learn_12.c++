#include <iostream>

int main()
{
    // int does NOT have a default value 
    // it uses the last stored data if it is unitilized 
    // hence why im getting 32759 instead of 0 as expected
    int number;

    // while (number < 0) {
    //     std::cout << "Enter a positive number: ";
    //     std::cin >> number;
    // }

    do {
        std::cout << "Enter a positive number: ";
        std::cin >> number;
    } while (number < 0);

    std::cout << "Your number is " << number << "\n";

    return 0;
}