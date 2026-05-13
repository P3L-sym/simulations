#include <iostream>
#include <ctime>

char getUserChoice() {
    char player;

    do {
        std::cout << "======================================================================\n";
        std::cout << "R for Rock\n"; 
        std::cout << "P for Paper\n"; 
        std::cout << "S for Scissors\n";
        std::cout << "======================================================================\n";
        std::cin >> player;
        std::cout << "======================================================================\n";
    } while(player != 'r' && player != 'R' && player != 'p' && player != 'P' && player != 's' && player != 'S');

    return player;
}

char getComputerChoice() {
    return 0;
}

void showChoice(char choice) {
    switch(choice) {
        case 'r': case 'R':
            std::cout << "Rock\n";
            break;
        case 'p': case 'P':
            std::cout << "Paper\n";
            break;
        case 's': case 'S':
            std::cout << "Scissors\n";
            break;
    }
}

void chooseWinner(char player, char computer) {

}

int main() {
    char player;
    char computer;

    std::cout << "======================================================================\n";
    std::cout << "Rock, Paper, Scissors!!!!!\n";
    std::cout << "======================================================================\n";

    player = getUserChoice();
    std::cout << "Your Choice: ";
    showChoice(player);
    std::cout << "======================================================================\n";
    
    computer = getComputerChoice();

    return 0;
}