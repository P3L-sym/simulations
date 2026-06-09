#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>

std::string toLower(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c)
                   { return std::tolower(c); });
    return str;
}

std::string trim(std::string str)
{
    size_t first = str.find_first_not_of(" \t\n\r");

    if (first == std::string::npos)
    {
        return "";
    }

    size_t last = str.find_last_not_of(" \t\n\r");

    return str.substr(first, (last - first + 1));
}

int searchFoodArray(std::string array[], int size, std::string element)
{
    for (int i = 0; i < size; i++)
    {
        if (toLower(array[i]) == toLower(element))
        {
            return i + 1;
        }
    }
    return -1;
}

int searchArray(int array[], int size, int element)
{
    for (int i = 0; i < size; i++)
    {
        if (array[i] == element)
        {
            return i + 1;
        }
    }
    return -1;
}

int main()
{
    std::string foods[] = {"Pizza", "Hamburger", "Dumplings", "Hotdog", "Stirfry"};
    int foodSize = sizeof(foods) / sizeof(foods[0]);
    int foodIndex;
    std::string myFoods;

    int numbers[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int size = sizeof(numbers) / sizeof(numbers[0]);
    int index;
    int myNum;

    std::cout << "======================================================================\n";
    std::cout << "Enter a food to search for: \n";
    std::cout << "======================================================================\n";
    std::getline(std::cin, myFoods);
    std::cout << "======================================================================\n";

    myFoods = trim(myFoods);

    std::cout << "======================================================================\n";
    std::cout << "Enter a element to search for: \n";
    std::cout << "======================================================================\n";
    std::cin >> myNum;
    std::cout << "======================================================================\n";

    foodIndex = searchFoodArray(foods, foodSize, myFoods);
    index = searchArray(numbers, size, myNum);

    if (foodIndex != -1)
    {
        std::cout << myFoods << " is at index " << foodIndex << '\n';
    }
    else
    {
        std::cout << myFoods << " is not in the array!\n";
    }

    std::cout << "======================================================================\n";

    if (index != -1)
    {
        std::cout << myNum << " is at index " << index << '\n';
    }
    else
    {
        std::cout << myNum << " is not in the array!\n";
    }

    std::cout << "======================================================================\n";

    return 0;
}