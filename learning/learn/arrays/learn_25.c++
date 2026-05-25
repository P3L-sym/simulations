#include <iostream>

int main()
{
    std::string students[] = {"Petri", "Ashlynn", "Ryan", "Jayden", "Niel", "Noe", "Vaughn", "Ulrigh"};
    int grades[] = {75, 30, 55, 85, 99};

    for (std::string student : students)
    {
        for (int grade : grades)
        {
            std::cout << student << " " << grade << '\n';
        }
    }

    return 0;
}