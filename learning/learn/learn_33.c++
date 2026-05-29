#include <iostream>

//pass by value is creating copies of the initial values 
//pass by refrenbce uses the memory adress of the values
//use a "&" when you want to pass by refrence 

void swap(std::string &x, std::string &y){
    std::string temp; 

    temp = x;
    x = y;
    y = temp;
}

int main()
{
    std::string x = "Root-Beer";
    std::string y = "Water";
    

    std::cout << "==========================================\n";
    std::cout << "==============Initial=Values==============\n";
    std::cout << "==========================================\n";

    std::cout << "initial x: " << x << '\n';
    std::cout << "initial y: " << y << '\n';

    std::cout << "==========================================\n";
    std::cout << "==============Swapped=Values==============\n";
    std::cout << "==========================================\n";

    swap(x, y);

    std::cout << "x: " << x << '\n';
    std::cout << "y: " << y << '\n';
    
    std::cout << "==========================================\n";

    return 0;
}