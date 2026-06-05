#include <iostream>

//recusive
void walk(int steps){
    if(steps > 0) {
        std::cout << "#" << steps << " You take a step\n";
        walk(steps - 1);
    }
}

int factorial(int num){
    if(num > 1){
        return num * factorial(num - 1);
    } else {
        return 1;
    }
}

int main() {

    // walk(100);

    std::cout << factorial(10);

    return 0;
}

// recursion = a programming technique where a function invokes itself from within 
// break a complex concept into a repeatable single steps

// (iterative vs recusive)

//advantages = less code and is cleaner 
//           = useful for sorting and searching algorithms

//disadvantages = uses more memory
//              = slower