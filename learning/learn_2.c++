#include <iostream>

int main() {
    //implicit
    double x = (int) 3.14;
    char y = 100;

    std::cout << x << '\n';
    std::cout << y << '\n';

    //explicit
    std::cout << (char)120 << '\n';
    std::cout << '\n';

    int correct = 8;
    int questions = 10;
    double score = correct / (double)questions * 100;

    std::cout << "The students mark is: " << score << "%";

    return 0;
}