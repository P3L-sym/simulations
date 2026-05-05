#include <iostream>
#include <ctime>

void d4(int amountDice) {
    int j = 0;

    for (int i = 1; i <= amountDice; i++) {
        int num = (rand() % 4) + 1;
        j++;
        std::cout << j << ": " << num << '\n';
    }
}

void d6(int amountDice) {
    int j = 0;

    for (int i = 1; i <= amountDice; i++) {
        int num = (rand() % 6) + 1;
        j++;
        std::cout << j << ": " << num << '\n';
    }
}

void d8(int amountDice) {
    int j = 0;

    for (int i = 1; i <= amountDice; i++) {
        int num = (rand() % 8) + 1;
        j++;
        std::cout << j << ": " << num << '\n';
    }
}

void d10(int amountDice) {
    int j = 0;

    for (int i = 1; i <= amountDice; i++) {
        int num = (rand() % 10) + 1;
        j++;
        std::cout << j << ": " << num << '\n';
    }
}

void d100(int amountDice) {
    int j = 0;

    for (int i = 1; i <= amountDice; i++) {
        int num = (rand() % 100) + 1;
        j++;
        std::cout << j << ": " << num << '\n';
    }
}

void d12(int amountDice) {
    int j = 0;

    for (int i = 1; i <= amountDice; i++) {
        int num = (rand() % 12) + 1;
        j++;
        std::cout << j << ": " << num << '\n';
    }
}

void d20(int amountDice) {
    int j = 0;

    for (int i = 1; i <= amountDice; i++) {
        int num = (rand() % 20) + 1;
        j++;
        std::cout << j << ": " << num << '\n';
    }
}

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
        d4(amountDice);
    } else if (typeDice == 6) {
        d6(amountDice);
    } else if (typeDice == 8) {
        d8(amountDice);
    } else if (typeDice == 10) {
        d10(amountDice);
    } else if (typeDice == 100) {
        d100(amountDice);
    } else if (typeDice == 12) {
        d12(amountDice);
    } else if (typeDice == 20) {
        d20(amountDice);
    } else {
        std::cout << "Please Enter a Valid Dice!";
    }

    std::cout << "============================================================\n";

    return 0;
}