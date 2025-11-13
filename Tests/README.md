# Flywheel RPM Sensor Emulation Tests (Proof-of-Concept)

**Goal:**  
Sketch test scripts to prepare for hardware debugging and testing.

See [Build & Test Plan](Build_and_Test_Plan.md) for a detailed step-by-step guide to wiring, component testing, and control logic implementation.

---

## Tests:

### Simple Servo Functionality Test:

- [servoTest1.ino](./Tests/servoTest1.ino): continuous movement in 1° increments

---
### Button Control Test:

- [servoTestButton.ino](./Tests/servoTestButton.ino): continuous movement in 10° or 12° increments

---
### Potentiometer Control Test:

- [servoTestPot.ino](./Tests/servoTestPot.ino): level 1 continuous movement function, level 2 maps servo speed to potentiometer reading
