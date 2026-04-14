#include <iostream>

int main() {
    std::string intro = "Hello!!!!!";
    std::string means = "This is the first commit to my C++ Journey, I have started learning C++ and I know C# and Java but Im still new to this.";
    std::string doing = "For this first one im just gonna do these formulas, A=P(1+ni) and A=P(1-ni), and then see if it works out.";
    
    double p = 15;
    double n = 5;
    double i = 7;
    double A1 = p * (1 + n * i);
    double A2 = p * (1 - n * i);

    std::cout << intro << '\n';
    std::cout << means << '\n';
    std::cout << doing << '\n';
    std::cout << "A=P(1+ni) | " << "A: " << A1 << '\n';
    std::cout << "A=P(1-ni) | " << "A: " << A2 << '\n';
    
    return 0;
}