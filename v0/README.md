# Flywheel Hall Sensor Emulation – v0

**Status:** Compiles and builds, preliminary logic verified in code, **hardware behaviour verified but not validated**

## Overview

v0 implements a basic flywheel emulation system using a Hall effect sensor and a servo actuator. This version focuses on establishing the core logic for speed regulation, angle handling, and RPM calculation in a modular, multi-file structure.

---
## Key Features / Concepts

- **Virtual flywheel model:**

    - `flywheelAngle` represents the flywheel’s current angle.
    - Servo sweeps back and forth between 0–180° to emulate rotation.
    - Button input increments/decrements the "angle step size" (1°-12°); sweep direction toggles when reaching boundaries.

- **Hall sensor logic:**
    
    - Designed to handle sensor being triggered twice per “revolution.”
    - Only counts the first pass per cycle to avoid RPM miscalculations.
    - `secondPass` state tracked globally to ensure consistent updates across loop iterations.

- **RPM calculation and timing:**
    
    - Sequential logic using global variables: `thisRPM`, `lastRPM`, `currentSensorRead`, `lastSensorRead`.
    - Initial values set to 0 to provide a deterministic starting point for calculations.

- **Separation of concerns:**
    
    - Multi-file structure separates flywheel object, servo control, Hall sensor, RPM calculation, and plotting.
    - Servo control functions no longer require passing the servo as an argument; all functions reference a central `serv0`.
    - `spinFlywheel()` and `moveServo()` handle updates and actuation.
    - `plotterDisplay()` visualizes sensor and flywheel data.

---
## Design Notes

- v0 is a **refactor of a previous draft**, consolidating logic into a clean, modular structure.
- Focused on **building the foundation** before integrating button-based speed control: initial speed input is requested during setup, now changed due to i/o issues.
- Progressive debugging was necessary to improve hardware response, program 'broke' while fine tuning and further debugging/minor refactoring is done in v0.1.
- Multi-file approach facilitates easier debugging and clearer logic separation.

---
## Loop Flow Summary

1. `spinFlywheel()` – updates flywheel angle based on virtual model and speed logic.
2. `moveServo()` – moves central servo to match flywheel angle, handling sweep direction.
3. `plotterDisplay()` – visualizes the current angle, sensor readings, and RPM.
4. `checkReadState()` – updates sensor states and global tracking variables for RPM calculation.

---
## File Structure

- `flywheel_lib.h/cpp` – flywheel state and virtual angle logic
- `hall_lib.h/cpp` – Hall sensor reading and edge detection
- `servolib.h/cpp` – servo actuation and sweep logic
- `rpmlib.h/cpp` – RPM calculation using sequential logic and sensor input
- `plotter_lib.h/cpp` – plotting interface for data visualization

---

v0 establishes the **core functional skeleton** of the system, ready for incremental updates like button-based speed regulation, timing improvements, and RPM smoothing in subsequent versions.