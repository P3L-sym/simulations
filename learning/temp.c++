#include <iostream>

int main() {
    double temp;
    char unit;
    double ans;
    

    std::cout << "======================================================================" << '\n';

    std::cout << "What is your starting temperature (F/C): ";
    std::cin >> unit;

    std::cout << "======================================================================" << '\n';
    
    if(unit == 'F' || unit == 'f') {
        std::cout << "Your temperature: ";
        std::cin >> temp;

        std::cout << "======================================================================" << '\n';

        ans = (temp * 9 / 5) + 32;
        std::cout << ans << " C" << '\n';
    } else if(unit == 'C' || unit == 'c') {
        std::cout << "Your temperature: ";
        std::cin >> temp;

        std::cout << "======================================================================" << '\n';

        ans = (temp -32) * 5 / 9;
        std::cout << ans << " F" << '\n';
    }

    std::cout << "======================================================================" << '\n';

    return 0;
}