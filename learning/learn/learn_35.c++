#include <iostream>

int main(){
    // pointer = variable that stores a memory address of another variable 
    //           sometimes its easier to work with an address 

    // & address-of operator
    // * dereference operator 

    std::string name = "Petri";
    int age = 21;
    std::string pizza[5] = { "pizza1", "pizza2", "pizza3", "pizza4", "pizza5" };

    std::string *pName = &name;
    int *pAge = &age;
    std::string *pPizza = pizza;

    std::cout << pName << '\n' << *pName << '\n';
    std::cout << pAge << '\n' << *pAge << '\n';
    std::cout << pizza << '\n' << *pPizza << '\n';

    return 0;
}