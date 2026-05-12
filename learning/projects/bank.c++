#include <iostream>
#include <iomanip>
#include <limits>

void showBalance(double balance)
{
    std::cout << "Your Balance is: $" << std::setprecision(2) << std::fixed << balance << "\n";
    std::cout << "===============================================================\n";
    std::cout << '\n';
}

double deposit()
{
    double amount = 0;

    std::cout << "Enter Amount to be deposited: $";
    std::cin >> amount;

    std::cout << "===============================================================\n";

    if (amount > 0)
    {
        return amount;
    }
    else
    {
        std::cout << "Please Enter a valid amount!\n";
        return 0;
    }
}

double withdraw(double balance)
{
    double amount = 0;

    std::cout << "Enter Amount to be withdrawn: $";
    std::cin >> amount;

    std::cout << "===============================================================\n";

    if (amount > balance)
    {
        std::cout << "Insufficient Funds!\n";
        return 0;
    }
    else if (amount < 0)
    {
        std::cout << "Thats not a valid amount!\n";
        return 0;
    }
    else 
    {
        return amount;
    }
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

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice)
        {
        case 1:
            showBalance(balance);
            break;
        case 2:
            balance += deposit();
            showBalance(balance);
            break;
        case 3:
            balance -= withdraw(balance);
            showBalance(balance);
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
            break;
        }
    } while (choice != 4);

    std::cout << "===============================================================\n";

    return 0;
}