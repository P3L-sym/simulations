#include <iostream>

int main() {
    char an;
    bool thought;
    int grade;
    double f;
    double p;
    
    std::cout << "======================================================================" << '\n';

    std::cout << "Do you thin you passed (y/n): ";
    std::cin >> an;

    // switch(an) {
    //     case 'y':
    //         thought = true;
    //         break;
    //     case 'n':
    //         thought = false;
    //         break;
    //     default:
    //         std::cout << "Please enter a valid letter" << '\n';
    //         break;
    // }

    if(an == 'y') {
        thought = true;
    } else if(an == 'n') {
        thought = false;
    }

    std::cout << "======================================================================" << '\n';

    std::cout << "What is your grade: ";
    std::cin >> grade;

    std::cout << "======================================================================" << '\n';

    // thought ? std::cout << "You think you passed" << '\n' : std::cout << "You think you failed" << '\n';
    std::cout << (thought ? "You think you passed" : "You think you failed") << '\n';

    std::cout << "======================================================================" << '\n';

    grade % 2 == 1 ? std::cout << "Your grade is ODD" << '\n' : std::cout << "Your grade is EVEN" << '\n';

    f = 60 - grade;
    p = grade - 60;

    std::cout << "======================================================================" << '\n';

    grade >= 60 ? std::cout << "You passed" << '\n' << "You passed with " << p << "%" << '\n' : std::cout << "You failed" << '\n' << "You needed " << f << "% to pass" << '\n';

    std::cout << "======================================================================" << '\n';

    return 0;
}