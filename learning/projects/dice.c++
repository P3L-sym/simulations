#include <iostream>
#include <ctime>
//use functions to update the app

int main() {
    int typeDice;
    int amountDice;

    srand(time(NULL));

    std::cout << "============================================================\n";

    std::cout << "What Type of Dice do you want to roll(4, 6, 8, 10, 100, 12, 20): ";
    std::cin >> typeDice;

    std::cout << "============================================================\n";

    std::cout << "How many dice do you want to roll: ";
    std::cin >> amountDice;

    std::cout << "============================================================\n";

    if (typeDice == 4){
        for (int i = 1; i <= amountDice; i++) {
            int num = (rand() % 4) + 1;
            std::cout << num << '\n';
        }
    } else if (typeDice == 6) {
        for (int i = 1; i <= amountDice; i++) {
            int num = (rand() % 6) + 1;
            std::cout << num << '\n';
        }
    } else if (typeDice == 8) {
        for (int i = 1; i <= amountDice; i++) {
            int num = (rand() % 8) + 1;
            std::cout << num << '\n';
        }
    } else if (typeDice == 10) {
        for (int i = 1; i <= amountDice; i++) {
            int num = (rand() % 10) + 1;
            std::cout << num << '\n';
        }
    } else if (typeDice == 100) {
        for (int i = 1; i <= amountDice; i++) {
            int num = (rand() % 100) + 1;
            std::cout << num << '\n';
        }
    } else if (typeDice == 12) {
        for (int i = 1; i <= amountDice; i++) {
            int num = (rand() % 12) + 1;
            std::cout << num << '\n';
        }
    } else if (typeDice == 20) {
        for (int i = 1; i <= amountDice; i++) {
            int num = (rand() % 20) + 1;
            std::cout << num << '\n';
        }
    } else {
        std::cout << "Please Enter a Valid Dice!";
    }

    std::cout << "============================================================\n";

    return 0;
}