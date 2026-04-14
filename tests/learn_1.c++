#include <iostream>

using text_t = std::string;

int main() {
    text_t intro = "Hello!!!!!";
    text_t means = "This is the first commit to my C++ Journey, I have started learning C++ and I know C# and Java but Im still new to this.";
    text_t doing = "For this first one im just gonna do these formulas, A=P(1+ni) and A=P(1-ni), and then see if it works out.";
    text_t cool = "Oke now that i have the language working properly and I can run my code I want to move on to bigger things.";
    text_t next = "Next im going to complete a C++ course (https://youtu.be/-TkoO8Z07hI?si=ZQGAwtocEP9y1Ke0) and then Ill move on to new bigger tasks.";
    
    const double p = 15;
    const double n = 5;
    const double i = 0.07;
    double A1 = p * (1 + n * i);                        // the answer is 20.25
    double A2 = p * (1 - n * i);                        // the answer is 9.75

    const double a = 20;
    const double d = 30;
    double T = a + (n - 1) * d;                        // the answer is 140
    double S = n / 2 * (2 * a + (n - 1) * d);          // the abswer is 400

    int planets = 20;
    int stars = 20;
    int solarSystems = 20;
    double astroids = 20;

    planets = planets + 1;
    stars--;
    solarSystems*=2;
    astroids/=3;
    
    // planets == 21 cause of line 28
    int remainder = planets % 2;

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
    std::cout << '\n';

    std::cout << cool << '\n';
    std::cout << next << '\n';

    std::cout << '\n';
    std::cout << "Sum of Planets: " << planets << '\n';
    std::cout << "Stars left: " << stars << '\n';
    std::cout << "Product of Solar Systems: " << solarSystems << '\n';
    std::cout << "How many Astroids will it all become: " << astroids << '\n';
    std::cout << "The remainder of the Planets are: " << remainder << '\n';

    return 0;
}