# 🚀 Space Simulations

> *Learning C++ by simulating the universe — one body at a time.*

This repository documents my journey learning C++ through the lens of space simulations. Starting from zero C++ knowledge, I'm building progressively complex simulations — from a single orbiting planet all the way to full n-body gravitational systems.

---

## 🌌 The Goal

Build realistic, physics-based space simulations in C++ while learning the language from the ground up. Every simulation here is a milestone — a step forward in both my understanding of C++ and the mechanics of the cosmos.

---

## 🗺️ Roadmap

### Phase 1 — C++ Foundations *(starting here)*
- [X] Setting up a C++ development environment
- [X] Variables, data types, and control flow
- [X] Functions and basic OOP (classes & objects)
- [X] Compiling and running my first program
- [X] Running my first OpenGl program
- [ ] Running my first 3D OpenGl program

### Phase 2 — First Simulations
- [X] Simulating 2D gravity
- [X] Simulating a single planet orbiting a star (Euler method)
- [X] Simulating a Solar System
- [ ] Plotting orbital paths to the console
- [X] Adding a second body — the Moon

### Phase 3 — Physics Engine Basics
- [ ] Implementing Newton's law of universal gravitation
- [ ] Runge-Kutta 4 integration for accuracy
- [ ] Energy and momentum conservation checks

### Phase 4 — N-Body Simulations
- [ ] Simulating the solar system (all 8 planets)
- [ ] Handling collisions and mergers
- [ ] Performance optimisation with the Barnes-Hut algorithm

### Phase 5 — Visualisation
- [X] Rendering simulations with a graphics library (SFML or OpenGL)
- [ ] Real-time interactive simulation viewer
- [ ] Recording and exporting simulation data
- [ ] All of the above for the 3D simulation

---

## 📁 Project Structure

```
simulation/
│
├── cmake/                        # CMake configuration modules
├── dependencies/                 # External libraries and dependencies
├── src/
│    └── gravsim/
│         ├── 2D_gravsim/         # 2D n-body gravity simulation
│         │    ├── bodies/        # Body struct — mass, position, velocity, acceleration
│         │    ├── camera/        # Pan, zoom and focus controls
│         │    ├── physics/       # Integrator and force calculations
│         │    └── window/        # GLFW window creation and input callbacks
│         └── 3D_gravsim/         # 3D n-body gravity simulation (planned)
│
├── learning/                     # C++ concepts and language fundamentals
│    ├── learn/                   # Topic exercises — syntax, patterns, and idioms
│    │    ├── arrays/
│    │    ├── basics/
│    │    ├── enums/
│    │    ├── functions/
│    │    ├── inheritance/
│    │    ├── loops/
│    │    ├── pointers/
│    │    ├── recursion/
│    │    ├── structs/
│    │    └── templates/
│    └── projects/                # Small self-contained practice projects
│
├── glad.c                        # OpenGL loader
├── CMakeLists.txt                # Root build configuration
└── README.md
```

---

## 🛠️ Built With

- **Language:** C++ (C++17)
- **Build System:** CMake
- **Visualisation (planned):** OpenGL
- **Physics:** Einstein's Theory of Relativity, Newtonian gravity, numerical integration

---

## 📚 Learning Resources

Resources I'm using along the way:

- [learncpp.com](https://www.learncpp.com/) — free, comprehensive C++ tutorials
- [C++ Full Course for free by BroCode](https://www.youtube.com/watch?v=-TkoO8Z07hI)
- [cppreference.com](https://en.cppreference.com/) — C++ standard library reference
- *The C++ Programming Language* — Bjarne Stroustrup
- [NASA Horizons System](https://ssd.jpl.nasa.gov/horizons/) — real planetary data
- [Three-Body Problem — Wikipedia](https://en.wikipedia.org/wiki/Three-body_problem)
- [Simulating Gravity in C++](https://www.youtube.com/watch?v=_YbGWoUaZg0)
- [CMake Project Setup (Windows, OpenGL, GLFW, GLAD)](https://www.youtube.com/watch?v=Z2MiPOepwPE&list=PLn3eTxaOtL2PHxN8EHf-ktAcN-sGETKfw&index=2)

---

## 📝 Dev Log

| Entry | Topic | Notes |
|-------|-------|-------|
| #001 | Repo created | The journey begins 🚀 |
| #002 | Started working on the fundementals of C++ | Its been alot of fun learning a new language and seeing how old knowledge help you understand the new language quicker |
| #003 | Completed all of the C++ basics and OOP | I completed the C++ course that I was doing and am now ready to start moving on to learning how to make a opengl window and render planets and move on to the gravity sim  |
| #004 | Made a Window | I created a window wile using OpenGl |
| #005 | Generated a 2D object | I generated a 2D object and added some garavity(that is abit broken still) |
| #006 | Generated 2 2D objects with gravity | I created 2 objects with the data from the sun and the earth and simulated gravity with those 2 bodies |
| #007 | Solar System | I added all of the 8 planets in out solar system and i added the moon aswel |
| #008 | Mars Moons | I added the 2 Moons that orbit Mars |
| #009 | Jupiter Moons | I added 8 of Jupiters's Moons |
| #010 | Saturns Moons | I added 13 of Saturns's Moons |
| #011 | Uranus Moons | I added 5 of Uranus's Moons |
| #012 | Neptune Moons | I added 8 of Neptune's Moons |

*This table will grow with each simulation and milestone.*

---

## ⚠️ Disclaimer

I'm learning as I go. And I'm not using an LLM for the learning nor the coding of this project. The code in this repo will be messy, wrong sometimes, and then fixed. That's the point. Every commit is progress. 

---

## 📬 Contact

If you're on a similar journey or want to share resources, feel free to open an issue or reach out!

---

*"The cosmos is within us. We are made of star-stuff." — Carl Sagan*
