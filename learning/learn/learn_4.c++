#include <iostream>
#include <cmath> //https://cplusplus.com/reference/cmath/

int main() {
    double x;
    double y;
    double n;
    double i;
    double w;
    double c;
    double t;
    double a;
    double u;
    double b;
    double max;
    double min;

    std::string answer;

    std::cout << "======================================================================" << '\n';

    std::cout << "Declare x: ";
    std::cin >> x;
    std::cout << "Declare y: ";
    std::cin >> y;

    max = std::max(x, y);
    min = std::min(x, y);

    n = pow(x, y);
    i = pow(y, x);

    w = sqrt(x);
    c = sqrt(y);

    std::cout << "Max or Min: ";
    std::cin >> answer;

    std::cout << "Declare t for absolute values (use negative numbers): ";
    std::cin >> t;

    std::cout << "Declare u to be rounded (use decimals): ";
    std::cin >> u;

    a = abs(t);

    //ceil will round it up and floor will always round down 
    b = round(u);

    std::cout << "======================================================================" << '\n';

    if (answer == "max") {
        std::cout << "Max: " << max << '\n';
    } else {
        std::cout << "Min: " << min << '\n';
    }

    std::cout << "x^y: n = " << n << '\n';
    std::cout << "y^x: i = " << i << '\n';
    std::cout << "sqrt of x: w = " << w << '\n';
    std::cout << "sqrt of y: c = " << c << '\n';

    std::cout << "ABS: " << a << '\n';
    std::cout << "Rounded u: " << b << '\n';

    std::cout << "======================================================================" << '\n';

    return 0;
}