#include <iostream>

int main() {
    bool thought;
    int grade;
    double f;
    double p;
    
    std::cout << "======================================================================" << '\n';

    std::cout << "Do you thin you passed (y/n): ";
    std::cin >> thought;

    if (thought == "y") {
        thought = true;
    } else {
        thought = false;
    }

    std::cout << "======================================================================" << '\n';

    std::cout << "What is your grade: ";
    std::cin >> grade;

    std::cout << "======================================================================" << '\n';

    thought ? std::cout << "You think you passed" << '\n' : std::cout << "You think you failed" << '\n';

    std::cout << "======================================================================" << '\n';

    grade % 2 == 1 ? std::cout << "Your grade is ODD" << '\n' : std::cout << "Your grade is EVEN" << '\n';

    f = 60 - grade;
    p = grade - 60;

    std::cout << "======================================================================" << '\n';

    grade >= 60 ? std::cout << "You pass" << '\n' << "You passed with " << p << "%" << '\n' : std::cout << "You failed" << '\n' << "You needed " << f << "% to pass" << '\n';

    std::cout << "======================================================================" << '\n';

    return 0;
}