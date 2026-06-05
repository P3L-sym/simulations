#include <iostream>

//iterative
void walk(int steps){
    for(int i = 0; i < steps; i++) {
        std::cout << "#" << i + 1 << " You take a step\n";
    }
}

int factorial(int num){
    int result = 1;
    for(int i = 1; i <= num; i++){
        result = result * i;
    }
    return result;
}

int main() {

    // walk(5);

    std::cout << factorial(6);

    return 0;
}

// recursion = a programming technique where a function invokes itself from within 
// break a complex concept into a repeatable single steps

// (iterative vs recusive)

//advantages = less code and is cleaner 
//           = useful for sorting and searching algorithms

//disadvantages = uses more memory
//              = slower