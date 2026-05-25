#include <iostream>

int main()
{
    // will always stay the same no matter how big the value is
    std::string name = "Petri";
    double gpa = 2.5;
    char grade = 'F';
    bool student = true;
    // since there are more than 1 value it will be bigger depending on how many values you assign to the array
    char grades[] = {'A', 'B', 'C', 'D', 'F'};
    std::string names[] = {"Petri", "Ashlynn", "Ryan", "Jayden", "Niel", "Noe", "Vaughn"};

    std::cout << "Size of string is " << sizeof(name) << " bytes\n";
    std::cout << "Size of double is " << sizeof(gpa) << " bytes\n";
    std::cout << "Size of char is " << sizeof(grade) << " bytes\n";
    std::cout << "Size of bool is " << sizeof(student) << " bytes\n";
    std::cout << "Size of an array with char is " << sizeof(grades) << " bytes\n";
    std::cout << "There is " << sizeof(grades) / sizeof(char) << " elements in the array\n";
    std::cout << "Size of an array with strings is " << sizeof(names) << " bytes\n";
    std::cout << "There is " << sizeof(names) / sizeof(std::string) << " elements in the array\n";

    return 0;
}