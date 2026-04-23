#include <iostream>

int main() {
    double temp;
    char stUnit;
    char finUnit;
    double ans;
    
    std::cout << "========================Temperature Conversion========================\n";
    std::cout << "F = Fahrenheit\n";
    std::cout << "C = Celsius\n";
    std::cout << "K = Kelvin\n";
    std::cout << "======================================================================\n";

    std::cout << "What is your starting temperature (F/C/K): ";
    std::cin >> stUnit;

    std::cout << "======================================================================\n";

    std::cout << "What Temperature do you want to Convert to (F/C/K): ";
    std::cin >> finUnit;

    std::cout << "======================================================================\n";
    
    if((stUnit == 'F' || stUnit == 'f') && (finUnit == 'C' || finUnit == 'c')) {
        std::cout << "Your temperature (F): ";
        std::cin >> temp;

        std::cout << "======================================================================\n";

        ans = (temp - 32) * 5 / 9;
        std::cout << ans << " C\n";
    } else if ((stUnit == 'F' || stUnit == 'f') && (finUnit == 'K' || finUnit == 'k')) {
        std::cout << "Your temperature (F): ";
        std::cin >> temp;

        std::cout << "======================================================================\n";

        ans = (temp + 459.67) * 5 / 9;
        std:: cout << ans << " K\n";
    } else if((stUnit == 'C' || stUnit == 'c') && (finUnit == 'F' || finUnit == 'f')) {
        std::cout << "Your temperature (C): ";
        std::cin >> temp;

        std::cout << "======================================================================\n";

        ans = (temp * 9 / 5) + 32;
        std::cout << ans << " F\n";
    } else if ((stUnit == 'C' || stUnit == 'c') && (finUnit == 'K' || finUnit == 'k')) {
        std::cout << "Your temperature (C): ";
        std::cin >> temp;

        std::cout << "======================================================================\n";

        ans = temp + 273.15;
        std::cout << ans << " K\n";
    } else if ((stUnit == 'K' || stUnit == 'k') && (finUnit == 'F' || finUnit == 'f')) {
        std::cout << "Your temperature (K): ";
        std::cin >> temp;

        std::cout << "======================================================================\n";

        ans = (temp * 9/5) - 459.67;
        std::cout << ans << " F\n";
    } else if ((stUnit == 'K' || stUnit == 'k') && (finUnit == 'C' || finUnit == 'c')) {
        std::cout << "Your temperature (K): ";
        std::cin >> temp;

        std::cout << "======================================================================\n";

        ans = temp - 273.15;
        std::cout << ans << " C\n";
    } else {
        std::cout << "Please Enter a Valid Unit\n";
    }

    std::cout << "======================================================================\n";

    return 0;
}