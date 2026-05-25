#include <iostream>

int main() {
    std::string students[] = { "Petri", "Ashlynn", "Ryan", "Jayden", "Niel", "Noe", "Vaughn", "Ulrigh"};
    char grades[] = { 'A', 'B', 'C', 'D', 'F' };

    for(int i = 0; i < sizeof(students)/sizeof(std::string); i++) {
        for(int j = 0; j <sizeof(grades)/sizeof(grades[0]); j++) {
            std::cout << students[i] << " " << grades[j] << '\n';
        }
    }

    return 0; 
}