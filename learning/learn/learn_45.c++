#include <iostream>

class Alien{
    public:
        std::string name;
        std::string occupation;
        std::string homePlanet;
        int age;
        double size;

    Alien(std::string x, std::string y, std::string z, int b, double c){
        name = x;
        occupation = y;
        homePlanet = z;
        age = b;
        size = c;
    }
};

class Human{
    public:
        std::string fName;
        std::string lName;
        std::string occupation;
        std::string homePlanet;
        int age;
        double size;
    
    //i prefer to do constructers like this 
    Human(std::string fName, std::string lName, std::string occuptaion, std::string homePlanet, int age, double size){
        this->fName = fName;
        this->lName = lName;
        this->occupation = occupation;
        this->homePlanet = homePlanet;
        this->age = age;
        this->size = size;
    }
};

int main(){
    Alien alien1("Rocky", "Engineer", "40-Eridani", 291, 168.5);
    Human human1("Ryland", "Grace", "Micro-Biologist", "Earth", 35, 80);

    std::cout << "Name: " << alien1.name << '\n' << "Occupation: " << alien1.occupation << '\n' << "Home Planet: " << alien1.homePlanet << '\n' << "Age: " << alien1.age << '\n' << "Size: " << alien1.size << "KG\n";
    std::cout << '\n';
    std::cout << "Name: " << human1.fName << ' ' << human1.lName << "Occupation: " << human1.occupation << '\n' << "Home Planet: " << human1.homePlanet << '\n' << "Age: " << human1.age << '\n' << "Size: " << human1.size << "KG\n";

    return 0;
}