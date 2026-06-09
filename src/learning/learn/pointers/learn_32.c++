#include <iostream>

int main(){
    std::string name = "Petri";
    int age = 21;
    bool student = true;
    
    std::cout << &name << '\n' << &age << '\n' << &student << '\n';

    return 0;
}