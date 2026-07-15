#include <iostream>
#include <iomanip>
#include <limits>
#include <cmath>
#include <string>

namespace Constants {
    constexpr double G = 6.67430e-11;             // Gravitational Constant, m^3 kg^-1 s^-2
    constexpr double c = 2.99792458e8;            // Speed of light, m/s
    constexpr double h = 6.62607015e-34;          // Plancks constant, J*s
    constexpr double hbar = 1.054571817e-34;      // Reduced Planck constant, J*s
    constexpr double k_B = 1.380649e-23;          // Boltzmann constant, J/K
    constexpr double N_A = 6.02214076e23;         // Avogadro's number, 1/mol
    constexpr double e_charge = 1.602176634e-19;  // Elementary charge, C
    constexpr double epsilon0 = 8.8541878128e-12; // Vacuum permittivity, F/m
    constexpr double M_sun = 1.98892e30;          // Solar mass, kg
    constexpr double M_earth = 5.9722e24;         // Earth mass, kg
}

double readDouble(const std::string& prompt) {
    double value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            return value;
        } else {
            std::cout << "Invalid input, please eenter a numeric value.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

double getMassInKg() {
    std::cout << "\nHow do you want to enter the mass?\n";
    std::cout << "  1. Enter mass directly in kilograms\n";
    std::cout << "  2. Use Solar masses (multiples of the Sun's mass)\n";
    std::cout << "  3. Use Earth masses (multiples of Earth's mass)\n";
    int choice;
    std::cout << "Choice: ";
    std::cin >> choice;

    switch (choice) {
        case 1: {
            return readDouble("Enter mass in kilograms: ");
        }
        case 2: {
            double solarMasses = readDouble("Enter mass in solar masses: ");
            return solarMasses * Constants::M_sun;
        }
        case 3: {
            double earthMasses = readDouble("Enter mass in Earth masses: ");
            return earthMasses * Constants::M_earth;
        }
        default: {
            return readDouble("Please enter a valid number");
        }
    }
}

void calcSchwarzschildRadius() {
    std::cout << "\n--- Schwarzschild Radius ---\n";
    std::cout << "Formula: r_s = 2GM / c^2\n";

    double mass = getMassInKg();

    double r_s = (2.0 * Constants::G * mass) / (Constants::c * Constants::c);

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "\nMass: " << mass << " kg\n";
    std::cout << "Schwarzschild Radius: " << r_s << " m\n";

    if (r_s >= 1000.0) {
        std::cout << r_s / 1000.0 << " km\n";
    } else if (r_s < 1.0) {
        std::cout << " " << r_s * 1000.0 << " mm\n";
    }
}

void showMenu() {
    std::cout << "\n============================================\n";
    std::cout << "      Physics & Math Calculator\n";
    std::cout << "============================================\n";
    std::cout << " 1. Schwarzschild Radius (r_s = 2GM/c^2)\n";
    std::cout << " 0. Exit\n";
    std::cout << "--------------------------------------------\n";
    std::cout << "Select an option: ";
}

int main() {
    std::cout << "Welcome to the Physics & Math Calculator!\n";
    std::cout << "This program will grow to include more equations over time.\n";

    bool running = true;
    while (running) {
        showMenu();
        int choice;
        std::cin >>choice;

        switch (choice) {
             case 1:
                calcSchwarzschildRadius();
                break;
            case 0:
                std::cout << "Goodbye!\n";
                running = false;
                break;
            default:
                std::cout << "Invalid option, please try again.\n";
        }
    }

    return 0;
}