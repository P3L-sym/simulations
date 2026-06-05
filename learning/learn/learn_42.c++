#include <iostream>

struct Planet{
    int planetId;
    std::string name;
    int size;
    std::string star;
    std::string moon;
};

void printPlanet(Planet &planet){
    // std::cout << &planet << '\n';
    // std::cout << '\n';
    std::cout << "#" << planet.planetId << ": " << planet.name << '\n' << planet.size << "km Diameter!\n" << "Star Name: " << planet.star << '\n' << "Moon: " << planet.moon << '\n';
    
}

void planetMoon(Planet &planet, std::string moon){
    planet.moon = moon;
}

int main(){
    Planet planet1;
    planet1.planetId = 1;
    planet1.name = "Saturn";
    planet1.size = 120536;
    planet1.star = "Sol";

    Planet planet2;
    planet2.planetId = 2;
    planet2.name = "Kepler-22 b";
    planet2.size = 30325;
    planet2.star = "Kepler-22";

    planetMoon(planet1, "Titan");
    planetMoon(planet2, "No Moon That We Know Off");

    // std::cout << &planet1 << '\n';
    // std::cout << &planet2 << '\n';
    
    printPlanet(planet1);
    std::cout << '\n';
    printPlanet(planet2);

    return 0;
}