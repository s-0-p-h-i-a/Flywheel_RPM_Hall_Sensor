# Flywheel RPM Sensor Emulation Tests (Proof-of-Concept)

**Goal:**  
Sketch test scripts to prepare for hardware debugging and testing.

See [Build & Test Plan](Build_and_Test_Plan.md) for a detailed step-by-step guide to wiring, component testing, and control logic implementation.

**NOTE:** main and test scripts still contain logic mistakes to be fixed later

---

## Tests:

### Simple Servo Functionality Test:

- [servoTest1.ino](./Test_Drafts/servoTest1.ino): continuous movement in 1° increments

---
### Button Control Test:

- [servoTestButton.ino](./Test_Drafts/servoTestButton.ino): continuous movement in 10° or 12° increments

---
### Potentiometer Control Test:

- [servoTestPot.ino](./Test_Drafts/servoTestPot.ino): level 1 continuous movement function, level 2 maps servo speed to potentiometer reading
