#include <iostream>

class Alien{
    public:
        bool alive = true;
    
    void eat(){
        std::cout << "This creature is eating\n";
    }
};

class Human : public Alien{
    public:

    void speak() {
        std::cout << "The human can speak\n";
    }
};

class Eridian : public Alien{
    public:

    void sing() {
        std::cout << "The eridian can sing\n";
    }
};

int main(){
    Human human;
    Eridian eridian;

    std::cout << "Human\n";
    std::cout << "Alive(0/1): " << human.alive << '\n';
    human.eat();
    human.speak();

    std::cout << '\n';

    std::cout << "Eridian\n";
    std::cout << "Alive(0/1): " << eridian.alive << '\n';
    eridian.eat();
    eridian.sing();

    return 0;
}