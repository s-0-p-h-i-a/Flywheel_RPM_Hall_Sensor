# Flywheel RPM Sensor Emulation (Proof-of-Concept)

**Goal:**  
Proof-of-concept Arduino project simulating flywheel RPM calculation with a Hall sensor, practicing **embedded signal acquisition, sensor processing, and iterative debugging**. Explores hardware constraints, applies on-the-fly learning, and parallels real automotive flywheel sensors.

---
## Latest Update (v0.1 – 04/12): Refactor and Timing Improvements

- **Centralized timing:** introduced a global `int now` for consistent timestamp across all functions in a loop iteration.

- **RPM calculation:** `getRPM()` and `getTimeWindow()` now reference the global timestamp rather than local `millis()`, improving consistency and reducing desync issues.

- **Loop logic restructured:** main loop now follows a deterministic order for predictable data flow:

    1. Handle flywheel edge angles
    2. Update speed using `canAccelerate`/`canDecelerate` functions
    3. Spin flywheel
    4. Move servo
    5. Check and update sensor read state
    6. Compute time window
    7. Check flywheel idle state
    8. Calculate RPM
    9. Plot data
    10. Update `now` for the next iteration

- **Global time window:** `timeWindow` and `idleInterval` added to track system idle status and centralize timing information.

- **Button pin setup:** all pins now configured in `setup()`, separating hardware interface initialization from flywheel object logic.

- **Multi-file structure:** `flywheel_lib` now `.cpp` to support the state machine controlling the servo actuator, while other libraries are mostly C.

- **Build environment:** project can now compile outside the Arduino IDE using `gcc-avr` and a Makefile.

- ⚠️ **Hardware testing has not been performed yet**; this version focuses on code structure, timing consistency, and logical flow.

---
## Hardware

- Arduino Uno + V5 sensor shield
- Breadboard
- Hall effect sensor module
- Digital push button module
- 2 breadboard push buttons
- SG90 standard servo motor (0–180° sweep)
- Cardboard disc with single magnet attached to the edge, mounted on the servo
- Hi-fi power + GND wire roll (for reliable servo wiring)
- Jumper and Dupont wires
- Additional USB-A cables for splicing and backup
- Crimp insulation tubes, electrical tape, zip ties
- Multimeter for verification and troubleshooting

> ⚠️ Note: Servo power uses spliced USB/hi-fi wires to address breadboard Vcc/GND gaps.

---
## Functionality

- Push button: turns servo on/off
- Breadboard buttons: varies servo speed by adjusting step angle (1°–12°)
- Hall sensor: detects magnet passing and calculates instantaneous and averaged RPM
- Serial output: prints RPM and potentiometer speed for real-time monitoring

> ⚠️ Note: The SG90 is a standard servo; it cannot rotate continuously. RPM calculation now accounts for **two sensor passes per sweep cycle**.

---
## Observations / Automotive Relevance

> This project emulates the RPM measurement principle of a flywheel Hall sensor in a simplified form: a single magnet disc produces a discrete pulse per rotation, analogous to automotive flywheel sensors with ferrous teeth and missing gaps. The setup develops understanding of **sensor signal timing, pulse counting, averaging, and RPM calculation principles** in a controlled experiment.

---
## Learning & Debugging Highlights

Hardware integration was the first focus. The Hall sensor, SG90 servo with mounted magnet disc, and control buttons were wired and verified. Power delivery and wiring reliability were critical: breadboard Vcc/GND gaps were addressed using spliced USB/hi-fi wires, and voltages were monitored at key points (≈5.08–5.09 V) to ensure stable operation. Sensor placement, alignment, and response accuracy were carefully tested, and the push buttons were verified for consistent control input.

Once the hardware was stable, attention shifted to software and logic. Initial tests revealed anomalies in RPM readings, speed jitter, and unexpected servo behavior. These issues were traced to timing inconsistencies: multiple `millis()` calls across modules created desynchronized snapshots of shared state, and minor sweep-back logic changes propagated hidden conflicts between servo control and RPM computation.

To address this, the loop was refactored with a deterministic order of operations, and a global `now` timestamp was introduced so all functions in a loop reference the same time. A global time window and clear state management in `flywheel_lib` improved synchronization between RPM calculation, servo actuation, and control states. Iterative telemetry analysis and mental modeling of internal states guided debugging, helping isolate subtle interactions and stabilize system behavior.

---
## Usage & Testing

> ⚠️ Note: v0.1 is untested on hardware. The focus is on logic and loop timing; actual behavior may differ.

**1. Hardware Setup**

- Connect Arduino Uno to sensor shield
- Mount SG90 servo with magnet disc
- Wire Hall sensor and push button according to standard pins
- Wire push buttons on breadboard
- Ensure reliable power via USB/hi-fi cable splices

**2. Upload Sketch**

- Open Arduino IDE or build via Makefile
- Upload main sketch

**3. Verify Controls**

- Press push button: servo should start/stop
- Faster/slower push buttons: servo speed should reflect changes

**4. Check Sensor Output**

- Observe Hall sensor pulses on Serial Plotter
- Verify RPM readings once sensor is integrated

**5. Troubleshooting Tips**

- Check wiring, power supply, and pin assignments if servo or sensors misbehave
- Use Serial Plotter to monitor virtual and sensor RPM values

---
## Next Steps

- Test v0.1 on hardware for timing and RPM calculation validation
- Verify Serial Plotter outputs for angle, speed, Hall sensor input, and RPM
- Implement visual RPM display via LEDs or alternative output
- Progress toward realistic automotive flywheel sensor emulation