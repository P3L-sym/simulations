#include <iostream>

class Rocket{
    private:
        int fuelTemp = 0;

    public:
    Rocket(int fuelTemp){
        setFuelTemp(fuelTemp);
    }

    int getFuelTemp(){
        return fuelTemp;
    }
    void setFuelTemp(int fuelTemp){
        if(fuelTemp < 0){
            this->fuelTemp = 0;
        } else if(fuelTemp > 100000) {
            this->fuelTemp = 100000;
        } else {
            this->fuelTemp = fuelTemp;
        }
    }
};

int main() {
    Rocket rocket(0);

    rocket.setFuelTemp(15);

    std::cout << "The fuel temperature is " << rocket.getFuelTemp() << "C\n";
    return 0;
}