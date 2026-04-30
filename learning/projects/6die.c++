#include <iostream>
#include <ctime>

int main() {

    int die;

    srand(time(NULL));

    std::cout << "============================================================\n";
    
    std::cout << "How many die do you want to roll: ";
    std::cin >> die;

    std::cout << "============================================================\n";

    for (int i = 1; i <= die; i++){
        int num = (rand() % 6) + 1;
        std::cout << num << '\n';
    }

    std::cout << "============================================================\n";

    return 0;
}