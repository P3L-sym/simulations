#include <iostream>
#include <ctime>

int main() {

    srand(time(NULL));

    int num = rand();

    std::cout << num;

    return 0;
}