#include <iostream>

class Human{
    public:
        std::string fName;
        std::string lName;
        std::string occupation;
        std::string homePlanet;
        int age;

        void eat(){
            std::cout << fName << " " << lName<< " is eating\n";
        }

        void drink(){
            std::cout << fName << " "  << lName<< " is drinking\n";
        }

        void sleep(){
            std::cout << fName << " "  << lName<< " watches Rocky sleep\n";
        }
};

class Alien{
    public:
        std::string name;
        std::string occupation;
        std::string homePlanet;
        int age;

        void eat(){
            std::cout << name << " eats\n";
        }

        void sleep(){
            std::cout << name << " watches Grace sleep\n";
        }
};

int main() {
    Human human1;
    Human human2;
    Alien alien1;

    human1.fName = "Ryland";
    human1.lName = "Grace";
    human1.occupation = "Micro-Biologist";
    human1.homePlanet = "Earth";
    human1.age = 35;

    human2.fName = "Eva";
    human2.lName = "Stradt";
    human2.occupation = "Micro-Biologist";
    human2.homePlanet = "Earth";
    human2.age = 45;

    alien1.name = "Rocky";
    alien1.occupation = "Engineer";
    alien1.homePlanet = "40-Eridani";
    alien1.age = 291;

    std::cout << "Name: " << human1.fName << ' ' << human1.lName << '\n' << "Occupation: " << human1.occupation << '\n' << "Home Planet: " << human1.homePlanet << '\n' << "Age: " << human1.age << '\n';
    std::cout << '\n';

    human1.eat();
    human1.drink();
    human1.sleep();

    std::cout << "====================================================================================================\n";

    std::cout << "Name: " << human2.fName << ' ' << human2.lName << '\n' << "Occupation: " << human2.occupation << '\n' << "Home Planet: " << human2.homePlanet << '\n' << "Age: " << human2.age << '\n';
    std::cout << '\n';

    human2.eat();

    std::cout << "====================================================================================================\n";

    std::cout << "Name: " << alien1.name << '\n' << "Occupation: " << alien1.occupation << '\n' << "Home Planet: " << alien1.homePlanet << '\n' << "Age: " << alien1.age << '\n';
    std::cout << '\n';

    alien1.eat();
    alien1.sleep();

    return 0;
}