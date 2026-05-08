#include <iostream>

void showBalance(double balance)
{
}

double deposit()
{
}

double withdraw(double balance)
{
}

int main()
{
    double balance = 0;
    int choice = 0;

    do
    {
        std::cout << "===============================================================\n";
        std::cout << "Welcome to Space-Bank\n";
        std::cout << "===============================================================\n";
        std::cout << "Enter Your Choice:\n";
        std::cout << "===============================================================\n";

        std::cout << "1. Show Balance\n";
        std::cout << "2. Deposit Money\n";
        std::cout << "3. Withdraw Money\n";
        std::cout << "4. Exit\n";
        std::cout << "===============================================================\n";
        std::cin >> choice;
        std::cout << "===============================================================\n";

        switch (choice)
        {
        case 1:
            showBalance(balance);
            break;
        case 2:
            balance += deposit();
            break;
        case 3:
            balance -= withdraw(balance);
            break;
        case 4:
            std::cout << R"( _____                            ______             _    )" << "\n";
            std::cout << R"(/  ___|                           | ___ \           | |   )" << "\n";
            std::cout << R"(\ `--. _ __   __ _  ___ ___ ______| |_/ / __ _ _ __ | | __)" << "\n";
            std::cout << R"( `--. \ '_ \ / _` |/ __/ _ \______| ___ \/ _` | '_ \| |/ /)" << "\n";
            std::cout << R"(/\__/ / |_) | (_| | (_|  __/      | |_/ / (_| | | | |   < )" << "\n";
            std::cout << R"(\____/| .__/ \__,_|\___\___|      \____/ \__,_|_| |_|_|\_\ )" << "\n";
            std::cout << R"(      | |                                                   )" << "\n";
            std::cout << R"(      |_|                                                   )" << "\n";
            std::cout << R"( _____                 _______            _ _  )" << "\n";
            std::cout << R"(|  __ \               | | ___ \          | | | )" << "\n";
            std::cout << R"(| |  \/ ___   ___   __| | |_/ /_   _  ___| | | )" << "\n";
            std::cout << R"(| | __ / _ \ / _ \ / _` | ___ \ | | |/ _ \ | | )" << "\n";
            std::cout << R"(| |_\ \ (_) | (_) | (_| | |_/ / |_| |  __/_|_| )" << "\n";
            std::cout << R"( \____/\___/ \___/ \__,_\____/ \__, |\___(_|_)  )" << "\n";
            std::cout << R"(                                __/ |            )" << "\n";
            std::cout << R"(                               |___/             )" << "\n";
            break;
        default:
            std::cout << "Please Enter a valid number!\n";
        }
    } while (choice != 4);

    std::cout << "===============================================================\n";

    return 0;
}