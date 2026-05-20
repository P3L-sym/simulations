#include <iostream>

void bakePizza() {
    std::cout << "Here is your Pizza!\n";
}

void bakePizza(std::string topping1) {
    std::cout << "Here is your " << topping1 << " Pizza!\n";
}

void bakePizza(std::string topping1, std::string topping2) {
    std::cout << "Here is your " << topping1 << " and " << topping2 << " Pizza!\n";
}

int main() {
    // if you leave the brackets empty then it will do the first bakePizza
    // if you add a topping into the brackets it will display the second function and so on
    bakePizza("bacon", "avo");

    return 0;
}