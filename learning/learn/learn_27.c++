#include <iostream>
#include <string>

int searchFoodArray(std::string array[], int size, std::string element) {
    for (int i = 0; i < size; i++) {
        if (array[i] == element)
        {
            return i;
        }
    }
    return -1;
}

int searchArray(int array[], int size, int element) {
    for (int i = 0; i < size; i++) {
        if (array[i] == element)
        {
            return i;
        }
    }
    return -1;
}

int main() {
    std::string foods[] = { "Pizza", "Hamburger", "Dumpling", "Hotdog", "Stirfry"};
    int foodSize = sizeof(foods)/sizeof(std::string);
    int foodIndex;
    std::string myFoods;

    int numbers[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int size = sizeof(numbers) / sizeof(int);
    int index;
    int myNum;

    std::cout << "======================================================================\n";
    std::cout << "Enter a food to search for: \n";
    std::cout << "======================================================================\n";
    std::getline(std::cin, myFoods);
    std::cout << "======================================================================\n";


    std::cout << "======================================================================\n";
    std::cout << "Enter a element to search for: \n";
    std::cout << "======================================================================\n";
    std::cin >> myNum;
    std::cout << "======================================================================\n";

    foodIndex = searchFoodArray(foods, foodSize, myFoods);
    index = searchArray(numbers, size, myNum);

    if (foodIndex != -1) {
        std::cout << myFoods << " is at index " << foodIndex << '\n';
    } else {
        std::cout << myFoods << " is not in the array!\n";
    }

    std::cout << "======================================================================\n";


    if (index != -1) {
        std::cout << myNum << " is at index " << index << '\n';
    } else {
        std::cout << myNum << " is not in the array!\n";
    }

    std::cout << "======================================================================\n";

    return 0;
}