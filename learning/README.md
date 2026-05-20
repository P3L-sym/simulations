# 📓 C++ Learning Notes

> *Translating software engineering foundations into C++ and astrophysics.*

This directory serves as my digital brain for C++ concepts. Since I already have a background in software engineering, these notes focus less on basic programming logic and more on C++ specific mechanics, memory management, and high-performance computing required for physics simulations.

---

## 📝 Notes Directory

| Topic | Description | Status |
|-------|-------------|--------|
| [Memory Management](./memory_management.md) | Pointers, references, heap vs. stack, and smart pointers. | In Progress |
| [Object-Oriented C++](./oop_basics.md) | Classes, constructors, destructors, and operator overloading (crucial for Vector math). | Planned |
| [CMake & Build Systems](./cmake_setup.md) | How to compile and link C++ projects properly. | Planned |
| [Performance & Optimisation](./performance.md) | Pass-by-reference, inline functions, and `constexpr`. | Planned |
| [Math & Physics in C++](./physics_math.md) | Handling floating-point precision, `<cmath>`, and numerical stability. | Planned |

---

## 📚 References & Cheat Sheets

Quick links to documentation and articles I frequently reference while studying:

- [cppreference.com](https://en.cppreference.com/) — The ultimate standard library reference.
- [BroCode](https://www.youtube.com/watch?v=-TkoO8Z07hI&t=8204s) — Full C++ course by BroCode on YouTube
- [LearnCpp.com](https://www.learncpp.com/) — My primary structured learning path.
- [w3schools.com](https://www.w3schools.com/cpp/) - Extra structured learning
- [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines) — Best practices by Bjarne Stroustrup and Herb Sutter.
- [Floating Point Math](https://0.30000000000000004.com/) — Essential reading for physics simulations to understand precision limits.
- [NASA JPL Horizons](https://ssd.jpl.nasa.gov/horizons/) — For when I need real planetary masses and distances to test my math.

---

*"Equipped with his five senses, man explores the universe around him and calls the adventure Science." — Edwin Hubble*

---

# 🛠️ C++ Practice Projects

> *Small, self-contained experiments to build C++ muscle memory before tackling the cosmos.*

This directory contains my foundational C++ mini-projects. Before integrating complex systems into the main `gravsim` engine, I built these console applications to master the basics of the language: control flow, standard input/output, random number generation, and function design. 

---

## 🏗️ Project Directory

| File | Project Name | Concept Focus | Status |
|------|--------------|---------------|--------|
| **`bank.c++`** | Space-Bank ATM | `do-while` loops, `switch` statements, and state management (balance tracking). | ✅ Completed |
| **`cal.c++`** | Console Calculator | Basic arithmetic operations and `switch` statement logic. | ✅ Completed |
| **`dice.c++`** | Multi-sided Dice Roller | Functions, logic branching, and procedural random generation (`<ctime>`, `rand()`). | ✅ Completed |
| **`numgame.c++`** | Number Guessing Game | Loop control, user input validation, and comparative conditionals. | ✅ Completed |
| **`rectangle.c++`** | ASCII Shape Drawer | Nested `for` loops and character output. | ✅ Completed |
| **`rps.c++`** | Rock, Paper, Scissors | Complex conditional logic, modular design (separating choices and winner calculation into functions). | ✅ Completed |
| **`temp.c++`** | Temperature Converter | Mathematical conversions (F/C/K) and isolating logic into specialized `void` functions. | ✅ Completed |
| **`6die.c++`** | Simple D6 Roller | Introduction to `for` loops and modulo arithmetic for bounds. | ✅ Completed |
| **`20die.c++`** | Simple D20 Roller | Variations on basic random number generation. | ✅ Completed |
| **`rdmgen.c++`** | RNG Test | Bare-minimum random number seeding. | ✅ Completed |

---

## 🚀 Why These Matter for Astro-Physics

While these are basic console apps, the mechanics learned here are the building blocks of the physics engine:
* **Random Number Generation (`<ctime>`, `rand()`):** Essential for procedurally generating star fields or initializing particles with random velocities.
* **Nested Loops (`rectangle.c++`):** The exact same logic used to iterate through 2D and 3D grids or arrays of celestial bodies.
* **State Management (`bank.c++`):** Keeping track of a bank balance uses the same core logic as keeping track of a planet's energy or momentum frame over frame.

---

## 📚 References & Tooling

Resources specifically for building, linking, and structuring C++ projects:

- [LearnCpp.com](https://www.learncpp.com/) — My primary structured learning path.
- [cppreference.com - <ctime>](https://en.cppreference.com/w/cpp/header/ctime) — Reference for time-based random seeding.
- [cppreference.com - <iomanip>](https://en.cppreference.com/w/cpp/header/iomanip) — For formatting output (like precision in the bank app).

---

*"The universe is under no obligation to make sense to you." — Neil deGrasse Tyson*

---
