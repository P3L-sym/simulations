#include <iostream>

int main()
{
    std::string cars[3][3] = {{"Mustang", "Escape", "F-150"},
                              {"Camaro", "Implala", "Corvette"},
                              {"Challenger", "Durango", "Ram"}};

    // you have to use the elements to do the right math for the arrays
    int rows = sizeof(cars) / sizeof(cars[0]);
    int columns = sizeof(cars[0]) / sizeof(cars[0][0]);

    std::cout << rows << " Rows" << '\n';
    std::cout << columns << " Columns" << '\n'
              << '\n';

    // memory addresses
    for (int i = 0; i < rows; i++)
    {
        std::cout << cars[i] << ' ';
    }

    std::cout << '\n';

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            std::cout << cars[i][j] << '\n';
        }
        std::cout << '\n';
    }

    // std::cout << "Ford " << cars[0][0] << '\n';
    // std::cout << "Ford " << cars[0][1] << '\n';
    // std::cout << "Ford " << cars[0][2] << '\n' << '\n';
    // std::cout << "Chevy " << cars[1][0] << '\n';
    // std::cout << "Chevy " << cars[1][1] << '\n';
    // std::cout << "Chevy " << cars[1][2] << '\n' << '\n';
    // std::cout << "Dodge " << cars[2][0] << '\n';
    // std::cout << "Dodge " << cars[2][1] << '\n';
    // std::cout << "Dodge " << cars[2][2] << '\n';

    return 0;
}