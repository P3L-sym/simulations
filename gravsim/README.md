# 🌌 Gravity Simulation Engine (`gravsim`) The Core Physics Engine

> *Bridging software engineering and astrophysics through numerical integration.*

The `gravsim` directory contains the core application of this repository: a bespoke, C++ driven, N-body gravitational simulation engine. This project represents the transition from learning C++ syntax to applying it toward high-performance scientific computing and real-time OpenGL rendering.

---

## 🔭 Project Vision & Physics Goals

The ultimate goal of this engine is to simulate the mechanics of the cosmos with mathematical stability. To do this, the engine must solve the fundamental equations of motion for celestial bodies interacting via gravity over time.

At its core, the engine evaluates Newton's Law of Universal Gravitation for interacting bodies:

$$F = G \frac{m_1 m_2}{r^2}$$

Where:
* $F$ is the gravitational force between two masses.
* $G$ is the gravitational constant.
* $m_1, m_2$ are the masses of the bodies.
* $r$ is the distance between their centers of mass.

### The Integration Problem
Because gravity constantly changes as bodies move, the simulation cannot just jump from point A to point B. It requires **numerical integration** to approximate their paths step-by-step. A major focus of this engine will be comparing different integrators:
* **Euler Method:** Simple, but introduces massive energy drift over time (planets will spiral into stars or fly away).
* **Verlet / Symplectic Euler:** Better at conserving orbital energy over long periods.
* **Runge-Kutta 4 (RK4):** Highly accurate, calculating multiple derivatives per time step to ensure smooth, predictable orbits.

---

## 🔬 Advanced Physics Roadmap (Future Scope)

Once the core Newtonian engine is mathematically stable, the simulation will expand to support more complex astrophysical phenomena. 

**Orbital Mechanics & Geometry:**
* Keplerian Orbital Elements (Eccentricity, Semi-major axis, Inclination, Periapsis, Apoapsis)
* Orbital Resonance

**Multi-Body Dynamics:**
* Lagrange Points (L1–L5)
* Hill Spheres
* The Kozai Mechanism

**Perturbations & Real-World Mechanics:**
* Planetary Oblateness (J2 Perturbation)
* Atmospheric Drag
* Solar Radiation Pressure (Yarkovsky effect)

**Tidal & Extreme Physics:**
* Tidal Locking & Friction
* Roche Limits & Fragmentation
* Spaghettification

**Relativistic Physics:**
* General Relativity corrections (e.g., perihelion precession of Mercury)
* Time Dilation
* Schwarzschild Radii (Black hole mechanics)

---

## 📂 Engine Architecture & Modules

The simulation is built in progressive layers of complexity, ensuring the underlying math is rock-solid before adding more variables.

### 1. `single_orbit/` (The Proving Ground)
* **Focus:** A single planet orbiting a stationary, infinite-mass star.
* **Mechanics:** Testing the primary integration loop. Ensuring that vectors (position, velocity, acceleration) update correctly frame-over-frame.
* **Output:** Console-based data logging (exporting X/Y coordinates to CSV).

### 2. `two_body/` (Action & Reaction)
* **Focus:** The Earth-Moon system.
* **Mechanics:** Both bodies now exert force on each other. This module introduces the concept of the **Barycenter** (the center of mass around which both bodies orbit) and tests the conservation of momentum.

### 3. `solar_system/` (The N-Body Problem)
* **Focus:** Scaling up to 8 planets, moons, and asteroids.
* **Mechanics:** Calculating the forces of *every* body against *every other* body. 
* **Optimisation:** Standard N-body calculations require $O(N^2)$ time complexity. This module will eventually implement the **Barnes-Hut algorithm** (using an Octree/Quadtree data structure) to group distant masses and reduce the complexity to $O(N \log N)$.

### 4. `visualisation/` (The Window to the Universe)
* **Focus:** Real-time hardware-accelerated rendering.
* **Mechanics:** Decoupling the physics "tick" from the visual frame rate. Drawing 3D spheres, rendering orbital trails, and handling a movable camera to explore the system.

---

## 🛠️ Technical Stack & Dependencies

This engine relies on industry-standard C++ libraries for performance and rendering.

| Technology | Purpose in the Engine |
| :--- | :--- |
| **C++17** | Core engine logic, memory management, and object-oriented physics structures. |
| **CMake** | Cross-platform build generation. Ensures libraries link correctly. |
| **OpenGL** | The primary graphics API for rendering 2D/3D space. |
| **GLFW** | Creates the operating system window and handles user input (keyboard/mouse for camera). |
| **GLEW / GLAD** | Loads the modern OpenGL functions pointing to the GPU. |
| **GLM** | (OpenGL Mathematics) Provides highly optimized classes for Vectors, Matrices, and Quaternions required for 3D transformations. |

---

*“For small creatures such as we the vastness is bearable only through love.” — Carl Sagan*