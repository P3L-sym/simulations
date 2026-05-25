#include <iostream>

int main(){
    const int size = 30;
    std::string foods[size];

    fill(foods, foods + size/3, "dumplings");
    fill(foods + size/3, foods + (size/3) * 2, "pizza");
    fill(foods + (size/3) * 2, foods + size,  "burger");

    for(std::string food : foods){
        std::cout << food << '\n';
    }
    return 0;
}