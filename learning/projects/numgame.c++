#include <iostream>
#include <ctime>

int main() {
    int num;
    int guess;
    int tries;

    srand(time(NULL));
    num = (rand() % 100) + 1;

    std::cout << "======================================================================\n";
    std::cout << "Number guessing game\n";
    std::cout << "======================================================================\n";

    return 0;
}