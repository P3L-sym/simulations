#include <iostream>
#include <ctime>

int main() {
    srand(time(0));
    int randNum = (rand() % 5) + 1;
    std::string ans;

    std::cout << "============================================================\n";

    std::cout << "What will happen to the earth!\n";

    std::cout << "============================================================\n";

    std::cout << "Do you want to continue (y/n): ";
    std::cin >> ans;


    std::cout << "============================================================\n";

    if (ans == "y" || ans == "Y"){
        std::cout << "Cool!!!" << '\n' << "Lets see what happens\n";

        std::cout << "============================================================\n";

        switch(randNum){
            case 1: 
                std::cout << "An astroid hit the Earth!\n";
                break; 
            case 2: 
                std::cout << "The astroid misses the Earth!\n";
                break;
            case 3:
                std::cout << "A Black Hole consumes the Earth!" << "\n" << "Oh Noooo!!!\n";
                break;
            case 4:
                std::cout << "The Moon crashes into the Earth!\n";
                break;
            case 5:
                std::cout << "Humanity thrives and develop interstellar travel\n";
                break;
        }
    } else if (ans == "n" || ans == "N") {
        std::cout << "Sorry to hear that" << '\n' << "See you later!\n";
    } else {
        std::cout << "Next time enter a valid answer!\n";
    }

    std::cout << "============================================================\n";

    return 0;
}