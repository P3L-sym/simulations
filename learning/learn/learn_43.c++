#include <iostream>
#include <string>

enum Planet{Mercury = 0, Venus = 1, Earth = 2, Mars = 3, Jupiter = 4, Saturn = 5, Uranus = 6, Neptune = 7};

Planet getPlanet(std::string input){
    if(input == "Mercury") return Mercury;
    if(input == "Venus")   return Venus;
    if(input == "Earth")   return Earth;
    if(input == "Mars")    return Mars;
    if(input == "Jupiter") return Jupiter;
    if(input == "Saturn")  return Saturn;
    if(input == "Uranus")  return Uranus;
    if(input == "Neptune") return Neptune;
    return (Planet)-1; 
}

int main(){
    std::string input;

    std::cout << "What is your favotire Planet in our solar system: ";
    std::getline(std::cin, input);

    Planet planets = getPlanet(input);

    switch(planets){
        case Mercury:
            std::cout << "Your favourite Planet is Mercury!!\n";
            break;
        case Venus:
            std::cout << "Your favourite Planet is Venus!!\n";
            break;
        case Earth:
            std::cout << "Your favourite Planet is Earth!!\n";
            break;
        case Mars:
            std::cout << "Your favourite Planet is Mars!!\n";
            break;
        case Jupiter:
            std::cout << "Your favourite Planet is Jupiter!!\n";
            break;
        case Saturn:
            std::cout << "Your favourite Planet is Saturn!!\n";
            break;
        case Uranus:
            std::cout << "Your favourite Planet is Uranus!!\n";
            break;
        case Neptune:
            std::cout << "Your favourite Planet is Neptune!!\n";
            break;
    }
    return 0;
}