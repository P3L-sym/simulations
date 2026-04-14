#include <iostream>

using text_t = std::string;

int main() {
    text_t intro = "Hello!!!!!";
    text_t means = "This is the first commit to my C++ Journey, I have started learning C++ and I know C# and Java but Im still new to this.";
    text_t doing = "For this first one im just gonna do these formulas, A=P(1+ni) and A=P(1-ni), and then see if it works out.";
    text_t cool = "Oke now that i have the language working properly and I can run my code I want to move on to bigger things.";
    text_t next = "Next im going to complete a C++ course (https://youtu.be/-TkoO8Z07hI?si=ZQGAwtocEP9y1Ke0) and then Ill move on to new bigger tasks.";
    
    double p = 15;
    double n = 5;
    double i = 0.07;
    double A1 = p * (1 + n * i);                        // the answer is 20.25
    double A2 = p * (1 - n * i);                        // the answer is 9.75

    double a = 20;
    double d = 30;
    double T = a + (n - 1) * d;                        // the answer is 140
    double S = n / 2 * (2 * a + (n - 1) * d);          // the abswer is 400

    std::cout << intro << '\n';
    std::cout << means << '\n';
    std::cout << doing << '\n';
    std ::cout << '\n';

    std::cout << "A=P(1+ni) | " << "A: " << A1 << '\n';
    std::cout << "A=P(1-ni) | " << "A: " << A2 << '\n';
    std::cout << '\n';

    std::cout << "Cool it worked now i want to do something else" << '\n';
    std::cout << "T=a+(n-1)d | " << "T: " << T << '\n';
    std::cout << "S=n/2(2a+(n-1)d) | " << "S: " << S << '\n';

    return 0;
}