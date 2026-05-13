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
    srand(time(0));
    int num = rand() % 3 + 1;

    switch(num){
        case 1: 
            return 'r';
        case 2:
            return 'p';
        case 3:
            return 's';
    }

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

    switch(player){
        case 'r': case 'R':
            if(computer == 'r') {
                std::cout << "Its a Tie!!!\n";
            } else if(computer == 'p') {
                std::cout << "You Lose!!!\n";
                std::cout << "The Computer Win!!!\n";
            } else {
                std::cout << "You Win!!!\n";
            }
            break;
        case 'p': case 'P':
            if(computer == 'r') {
                std::cout << "Its a Win!!!\n";
            } else if(computer == 'p') {
                std::cout << "Its a Tie!!!\n";
            } else {
                std::cout << "You Lose!!!\n";
                std::cout << "The Computer Win!!!\n";
            }
            break;
        case 's': case 'S':
            if(computer == 'r') {
                std::cout << "You Lose!!!\n";
                std::cout << "The Computer Win!!!\n";
            } else if(computer == 'p') {
                std::cout << "Its a Win!!!\n";
            } else {
                std::cout << "Its a Tie!!!\n";
            }
            break;
    }
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
    std::cout << "Computers Choice: ";
    showChoice(computer);
    std::cout << "======================================================================\n";

    chooseWinner(player, computer);

    std::cout << "======================================================================\n";

    return 0;
}