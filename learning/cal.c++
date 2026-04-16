# include <iostream>

int main() {
    char op;
    double num1;
    double num2;
    double result;

    std::cout << "======================================================================" << '\n';
    std::cout << "Calculator" << '\n';
    std::cout << "======================================================================" << '\n';

    std::cout << "Enter Either ( + - * / ): ";
    std::cin >> op;

    std::cout << "Enter your first Number: ";
    std::cin >> num1;

    std::cout << "Enter your second Number: ";
    std::cin >> num2;

    switch (op) {
        case '+':
            result = num1 + num2;
            std::cout << num1 << " + " << num2 << " = " << result << '\n';
            break;
        case '-':
            result = num1 - num2;
            std::cout << num1 << " - " << num2 << " = " << result << '\n';
            break;
        case '*':
            result = num1 * num2;
            std::cout << num1 << " * " << num2 << " = " << result << '\n';
            break;
        case '/':
            result = num1 / num2;
            std::cout << num1 << " / " << num2 << " = " << result << '\n';
            break;
        default:
            std::cout << "Please Enter a valid mathematical operations" << '\n';
            break;
    }

    std::cout << "======================================================================" << '\n';

    return 0;
}