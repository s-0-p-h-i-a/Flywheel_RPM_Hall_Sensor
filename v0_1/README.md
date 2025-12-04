# Flywheel Hall Sensor Project – v0.1

**Status:** Compiles successfully, **not yet tested on hardware**

## Overview

This project emulates a flywheel RPM calculation system using a Hall effect sensor and controls a servo actuator based on the flywheel state. It also provides a plotting interface for visualization.

v0.1 is primarily a refactor of the previous version, focusing on timing consistency, data flow, and code clarity.

---
## Changes in v0.1

- Introduced a **global `int now`** as a centralized timestamp to reduce desynchronization and ensure all time-dependent functions reference the same value within a loop iteration.

- Refactored `getRPM()` and `getTimeWindow()` to use the global `now` instead of local `millis()` calls, improving consistency in RPM calculation and idle detection.

- Moved **state and angle handling** (`handleEdgeAngles`) and **speed updates** (`updateSpeed`) to the main loop to provide a clearer flow of control.

- Main loop call order revised for predictable data flow, see summary.

- Introduced a **global `timeWindow` variable** and an `idleInterval` constant to track idle status.

- All **button pin modes** moved to `setup()` for clearer separation between hardware interface and flywheel object initialization.

- Code is now buildable **outside the Arduino IDE** using `gcc-avr` and a Makefile, while remaining Arduino-compatible.

---
## Concepts:

- **Virtual flywheel model:**

    - `flywheelAngle` represents the flywheel’s current angle.
    - Servo sweeps back and forth between 0–180° to emulate rotation.
    - Button input increments/decrements the "angle step size" (1°-12°); sweep direction toggles when reaching boundaries.

- **Hall sensor logic:**
    
    - Designed to handle sensor being triggered twice per “revolution.”
    - Only counts the first pass per cycle to avoid RPM miscalculations.
    - `secondPass` state tracked globally to ensure consistent updates across loop iterations.

---
## Notes

- v0.1 is intended as a logical and structural refactor.
- **Hardware behavior has not been tested yet**; the focus so far is on building, compiling, and clarifying data flow.
- Timing and RPM calculations may still need validation once the system is running on a physical setup.

---
## File Structure

- `global.h/.c` – central definitions for `now` and `timeWindow` (central 'clock')
- `flywheel_lib.h/.cpp` – flywheel state machine and actuator interface
- `hall_lib.h/cpp` – Hall sensor interface
- `plotter_lib.h/cpp` – visualization interface
- `rpmlib.h/cp` – RPM calculation and time window management
- `servolib.h/cpp` – servo control

---
## Loop Flow Summary

Each iteration of the main loop performs the following steps:

1. **Handle angles:** updates flywheel angle edge states

2. **Update speed:** accelerates or decelerates flywheel if speed control buttons are pressed

3. **Spin flywheel:** applies updated angle (adjusted for speed) to flywheel state

4. **Move servo:** adjusts servo according to flywheel state

5. **Sensor update:** reads Hall sensor and updates internal state

6. **Compute time window:** centralizes timing information for RPM calculation

7. **Idle check:** updates flywheel idle status based on time window

8. **Calculate RPM:** based on sensor input, time window and state

9. **Plotter display:** visualizes data

10. **Update `now`:** prepares timestamp for the next loop iteration