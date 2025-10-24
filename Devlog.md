# Flywheel RPM Sensor Emulation – Development Log

**Project Dates:** 13/10–14/10/2025  
**Objective:** Proof-of-concept for measuring rotational speed using a Hall effect sensor, exploring embedded sensor timing, PWM control, and iterative debugging.

---
## Day 1 – Initial Setup & Testing (13/10)

**Goal:**

- First implementation/test of servo + push button + Hall sensor setup.
- Write base program as foundation for future development.

**Setup:**

- Arduino Uno + V5 sensor shield
- Hall sensor module
- Digital push button
- SG90 servo motor
- Cardboard disc with one magnet mounted on the servo


**Planned Behaviour:**

- Servo: continuous rotation controlled by push button
- Hall sensor: detect magnet passage, calculate current RPM
- RPM calculation: `currentRPM = 60000 ms / currentTimeWindow`
- Display average RPM in Serial Plotter


**Observations & Debugging:**

- Considered variable scope for correct timing and RPM calculation.

- Hall sensor responded to fridge magnet; experimented with positioning:

    - Best detection when magnet passes directly below module.

    - Investigated: conductive elements on sensor module may influence EMF.

- Tested RPM calculation and Serial Plotter output by moving magnet manually.

- Servo not moving:

    - Hall sensor and button confirmed working

    - Initially suspected blocking loops in servo code; tested servo in isolation → still not moving

- Identified power issue: Arduino 5V insufficient for SG90 under load.

- Researched alternative powering:
    
    - 9V battery too high for servo

    - Attempted wall adapter via USB cable → fluctuating current, unreliable

    - Cut/stripped USB wire for temporary connection

**Outcome:**

- Servo still not moving; root cause narrowed to insufficient current.

- Planned to rewire and continue troubleshooting the next day.

---
## Day 2 – Servo Powering & On-the-Fly Hardware Learning (15/10)

**Focus:** Identify and address servo powering issues while continuing RPM measurement development.

**Troubleshooting Steps:**

- Narrowed probable issues to:
    1. Stripped USB cable: poor contact, broken wires
    2. Wall adapter: known fluctuating current

- Practiced proper wire stripping technique; learned to avoid exposed copper and ensure solid connections.
- Contacted electronics repair shop regarding soldering options → they do not provide the specific service needed.

**Key Learnings:**

- Gained hands-on experience with **basic electronics troubleshooting**.

- Applied **structured reasoning** to isolate hardware vs. software issues.

- Applied **iterative problem-solving**: tested multiple hypotheses, documented outcomes, and planned next steps.

---
## Insights & Relevance

- Successfully demonstrated **signal acquisition, timing, and RPM calculation**, despite incomplete servo operation.

- Project served as learning process on hardware handling and safe/effective wiring practices.

- Single-magnet disc setup parallels automotive flywheel sensors:

    - Sensor reacts to a magnet (opposite of magnetized flywheel Hall sensor)

    - Single pulse per rotation is analogous to double-LOW signal from missing teeth in real flywheels

---
## Next Steps

- Implement reliable servo powering for continuous rotation.