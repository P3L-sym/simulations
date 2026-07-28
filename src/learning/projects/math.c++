#include <iostream>
#include <iomanip>
#include <limits>
#include <cmath>
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>

#ifdef _WIN32
    #include <cstdlib>
#endif

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

void clearScreen() {
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

std::string toScientific(double value, int precision = 6) {
    std::ostringstream oss;
    oss << std::scientific << std::setprecision(precision) << value;
    return oss.str();
}

std::string toFixedStr(double value, int precision = 6) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(precision) << value;
    return oss.str();
}

void printHeader(const std::string& title,
                  const std::string& formula,
                  const std::vector<std::string>& enteredSoFar) {
    clearScreen();
    std::cout << "============================================\n";
    std::cout << "  " << title << "\n";
    std::cout << "============================================\n";
    std::cout << "Formula: " << formula << "\n";
    if (!enteredSoFar.empty()) {
        std::cout << "--------------------------------------------\n";
        std::cout << "Values entered so far:\n";
        for (const auto& line : enteredSoFar) {
            if (!line.empty()) std::cout << "  " << line << "\n";
            else std::cout << "\n";
        }
    }
    std::cout << "--------------------------------------------\n";
    std::cout << "(Type 'b' at any prompt to go back)\n\n";
}

int readChoiceOrBack(const std::string& prompt, int minVal, int maxVal) {
    while (true) {
        std::cout << prompt;
        std::string line;
        std::cin >> line;
        if (line == "b" || line == "B") return -1;
        try {
            size_t idx;
            int val = std::stoi(line, &idx);
            if (idx != line.size()) throw std::invalid_argument("trailing characters");
            if (val < minVal || val > maxVal) {
                std::cout << "Please enter a number between " << minVal
                           << " and " << maxVal << " (or 'b' to go back).\n";
                continue;
            }
            return val;
        } catch (...) {
            std::cout << "Invalid input. Please enter a number between " << minVal
                       << " and " << maxVal << " (or 'b' to go back).\n";
        }
    }
}

bool readDoubleOrBack(const std::string& prompt, double& outValue) {
    while (true) {
        std::cout << prompt;
        std::string line;
        std::cin >> line;
        if (line == "b" || line == "B") return false; 
        try {
            size_t idx;
            double val = std::stod(line, &idx);
            if (idx != line.size()) throw std::invalid_argument("trailing characters");
            outValue = val;
            return true;
        } catch (...) {
            std::cout << "Invalid input. Please enter a numeric value (or 'b' to go back).\n";
        }
    }
}

bool waitForEnterOrBack() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string line;
    std::getline(std::cin, line);
    return (line == "b" || line == "B");
}

bool getMassInKgWizard(const std::string& title,
                        const std::string& formula,
                        const std::vector<std::string>& priorValues,
                        const std::string& label,
                        double& outMass) {
    int stage = 0;
    int methodChoice = 0;

    while (true) {
        std::vector<std::string> shown = priorValues;
        if (stage >= 1) {
            std::string methodStr = (methodChoice == 1) ? "kilograms"
                                   : (methodChoice == 2) ? "solar masses"
                                                          : "Earth masses";
            shown.push_back(label + " unit: " + methodStr);
        }
        printHeader(title, formula, shown);

        if (stage == 0) {
            std::cout << label << " - how do you want to enter this mass?\n";
            std::cout << "  1. Kilograms\n";
            std::cout << "  2. Solar masses (multiples of the Sun's mass)\n";
            std::cout << "  3. Earth masses (multiples of Earth's mass)\n";
            int choice = readChoiceOrBack("Choice (or 'b' to go back): ", 1, 3);
            if (choice == -1) return false;
            methodChoice = choice;
            stage = 1;
        } else {
            double val;
            std::string unitName = (methodChoice == 1) ? "kilograms"
                                  : (methodChoice == 2) ? "solar masses"
                                                         : "Earth masses";
            bool gotValue = readDoubleOrBack("Enter " + label + " in " + unitName + ": ", val);
            if (!gotValue) {
                stage = 0;
                continue;
            }
            switch (methodChoice) {
                case 1: outMass = val; break;
                case 2: outMass = val * Constants::M_sun; break;
                case 3: outMass = val * Constants::M_earth; break;
            }
            return true;
        }
    }
}

bool getDistanceInMetersWizard(const std::string& title,
                                const std::string& formula,
                                const std::vector<std::string>& priorValues,
                                double& outDistance) {
    int stage = 0;
    int unitChoice = 0;

    while (true) {
        std::vector<std::string> shown = priorValues;
        if (stage >= 1) {
            shown.push_back(std::string("Distance unit: ") +
                             (unitChoice == 2 ? "kilometers" : "meters"));
        }
        printHeader(title, formula, shown);

        if (stage == 0) {
            std::cout << "How do you want to enter the distance?\n";
            std::cout << "  1. Meters (m)\n";
            std::cout << "  2. Kilometers (km)\n";
            int choice = readChoiceOrBack("Choice (or 'b' to go back): ", 1, 2);
            if (choice == -1) return false;
            unitChoice = choice;
            stage = 1;
        } else {
            double val;
            std::string unitName = (unitChoice == 2) ? "kilometers" : "meters";
            bool gotValue = readDoubleOrBack("Enter distance in " + unitName + ": ", val);
            if (!gotValue) {
                stage = 0;
                continue;
            }
            outDistance = (unitChoice == 2) ? val * 1000.0 : val;
            return true;
        }
    }
}

void calcSchwarzschildRadius() {
    const std::string title = "Schwarzschild Radius";
    const std::string formula = "r_s = 2GM / c^2";

    while (true) {
        double mass;
        if (!getMassInKgWizard(title, formula, {}, "Mass", mass)) {
        }

        double r_s = (2.0 * Constants::G * mass) / (Constants::c * Constants::c);

        std::vector<std::string> results;
        results.push_back("Mass: " + toFixedStr(mass) + " kg  (" + toScientific(mass) + " kg)");
        results.push_back("Schwarzschild Radius: " + toFixedStr(r_s) + " m  (" + toScientific(r_s) + " m)");
        if (r_s >= 1000.0) {
            results.push_back("Schwarzschild Radius in km: " + toFixedStr(r_s / 1000.0) + " km");
        } else if (r_s < 1.0) {
            results.push_back("Schwarzschild Radius in mm: " + toFixedStr(r_s * 1000.0) + " mm");
        }

        printHeader(title, formula, results);
        std::cout << "\nPress Enter to return to the menu, or type 'b' then Enter to redo this calculation: ";
        bool goBack = waitForEnterOrBack();
        if (!goBack) return;
    }
}

void calcGravitationalForce() {
    const std::string title = "Newton's Law of Universal Gravitation";
    const std::string formula = "F = G * (M1 * M2) / r^2";

    enum Stage { M1_STAGE, M2_STAGE, DIST_STAGE, RESULT_STAGE };
    Stage stage = M1_STAGE;

    double M1 = 0.0, M2 = 0.0, r = 0.0;

    while (true) {
        if (stage == M1_STAGE) {
            if (!getMassInKgWizard(title, formula, {}, "Mass 1 (M1)", M1)) {
                return;
            }
            stage = M2_STAGE;

        } else if (stage == M2_STAGE) {
            std::vector<std::string> prior;
            prior.push_back("Mass 1 (M1): " + toFixedStr(M1) + " kg");
            if (!getMassInKgWizard(title, formula, prior, "Mass 2 (M2)", M2)) {
                stage = M1_STAGE;
                continue;
            }
            stage = DIST_STAGE;

        } else if (stage == DIST_STAGE) {
            std::vector<std::string> prior;
            prior.push_back("Mass 1 (M1): " + toFixedStr(M1) + " kg");
            prior.push_back("Mass 2 (M2): " + toFixedStr(M2) + " kg");
            if (!getDistanceInMetersWizard(title, formula, prior, r)) {
                stage = M2_STAGE;
                continue;
            }
            if (r <= 0.0) {
                prior.push_back("Distance (r): " + toFixedStr(r) + " m");
                printHeader(title, formula, prior);
                std::cout << "\nDistance must be greater than 0. Press Enter to re-enter the distance...";
                waitForEnterOrBack();
                continue; 
            }
            stage = RESULT_STAGE;

        } else {
            double F = Constants::G * ((M1 * M2) / (r * r));

            std::vector<std::string> results;
            results.push_back("Mass 1 (M1): " + toFixedStr(M1) + " kg");
            results.push_back("Mass 2 (M2): " + toFixedStr(M2) + " kg");
            results.push_back("Distance (r): " + toFixedStr(r) + " m  (" + toFixedStr(r / 1000.0) + " km)");
            results.push_back("");
            results.push_back("Gravitational Force: " + toFixedStr(F) + " N  (" + toScientific(F) + " N)");

            printHeader(title, formula, results);
            std::cout << "\nPress Enter to return to the menu, or type 'b' then Enter to redo the distance: ";
            bool goBack = waitForEnterOrBack();
            if (goBack) {
                stage = DIST_STAGE;
                continue;
            }
            return;
        }
    }
}

void showMenu() {
    clearScreen();
    std::cout << "============================================\n";
    std::cout << "      Physics & Math Calculator\n";
    std::cout << "============================================\n";
    std::cout << " 1. Schwarzschild Radius (r_s = 2GM/c^2)\n";
    std::cout << " 2. Newton's law of Universal Gravitation\n";
    std::cout << " 0. Exit\n";
    std::cout << "--------------------------------------------\n";
    std::cout << "Select an option: ";
}

int main() {
    bool running = true;
    while (running) {
        showMenu();
        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1:
                calcSchwarzschildRadius();
                break;
            case 2:
                calcGravitationalForce();
                break;
            case 0:
                clearScreen();
                std::cout << "Goodbye!\n";
                running = false;
                break;
            default:
                std::cout << "Invalid option, please try again. Press Enter to continue...";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::string dummy;
                std::getline(std::cin, dummy);
        }
    }

    return 0;
}