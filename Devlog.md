# Flywheel RPM Sensor Emulation – Development Log

**Project Dates:** 13/10–12/11/2025  
**Objective:** Proof-of-concept for measuring rotational speed using a Hall effect sensor, exploring embedded sensor timing, PWM control, and iterative debugging.

---
## Latest:

### 16/11 Main Program Refactor:
- New logic for speed regulation (angle- instead of delay-based) and servo control
- Use of state machine and sequential circuit to initialise and keep track of variables across loop
- New servo control logic for immediate response
- Signal processing for clear visualisation on serial monitor

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
## Day 3 – Build & Test Plan Development, Materials Acquisition (12/11)

**Focus:** Prepare a comprehensive plan for safe servo powering, servo power supply testing, breadboard testing, and control logic.

**New Components Acquired:**

- Additional USB-A cables (for splicing and backup)
- Hi-fi power + GND wire roll (sturdier than USB cores)
- Heat-shrink tubing for insulated splices
- Electrical tape and zip ties for eventual mechanical reinforcement
- Multimeter for voltage/current verification

**Build & Test Plan Concepts:**

- **Stepwise, incremental testing:**

    1. Verify spliced wires for reliable power delivery.
    2. Confirm breadboard connections using LEDs and resistors.
    3. Add control elements (button, potentiometer) and test logic.
    4. Integrate SG90 servo with safe power supply and signal control.
    5. Introduce Hall sensor for RPM measurement and averaging.

- **Safety-first wiring practices:**
    
    - Minimal exposed copper for initial tests
    - Heat-shrink and tape to prevent shorts
    - Zip ties to relieve mechanical strain

- **Servo speed and control planning:**
    
    - Maximum speed derived from datasheet: 0.1 s / 60° → 100 RPM
    - Step increments (e.g., 12° per 20 ms) chosen to match ideal max speed
    - Button: on/off control
    - Potentiometer: mapped to 20–80 ms delay for variable speed

- **Virtual RPM display:**
    
    - Formula based on step size and delay to simulate idealised virtual RPM values (based on datasheet max speed and min delay interval) in Serial Plotter before Hall sensor integration

**Outcome:**

- Build & Test Plan drafted as a central reference for all wiring, control logic, and testing steps.
- Materials purchased and organised for next stage of hands-on implementation.
- Plan provides both **safe testing workflow** and **guidance for iterative development**.

---
## 13/11: Code Clean-Up and Test Script Drafts

- Cleaned up & implemented new logic idea in main program
- Wrote 3 servo test script drafts: simple continuous movement (servoTest1), button-controlled movement (servoTestButton), potentiometer-controlled movement (servoTestPot)

**NOTE:** main and test scripts still contain logic mistakes to be fixed later

---
## 15/11 – Servo Powering Solved

- Successfully spliced hi-fi and USB cables; secured with electrical tape and zip ties. Metal crimp sleeves provide solid contact.

- Multimeter tests: stable 5.09 V at first splice.

- Resolved breadboard fit issues: thick hi-fi wires incompatible with Dupont/breadboard slots, solved by splicing to jumper wires and immobilizing with tape/zip ties.

- Multimeter tests: stable 5.09 V at second splice, 5.08 V at breadboard; LED test confirms steady power.

- Servo initially did not move due to split Vcc/GND rails; moving all connections to same breadboard half solved the issue.

- Confirmed SG90 is a standard servo (sweeps 0–180°), not continuous rotation; will update logic and RPM calculation to account for sweeping motion and two sensor passes per sweep.

---
## 16/11 Main Program Refactor

- Decided to refactor instead of trying to fix messy v0 code

- Main concept changes: 
	- Different logic for speed control, now angle-based instead of delay-based
	- Servo sweep motion issues for RPM calculation: 
		- Hall sensor triggered twice each "revolution"
		- Time interval of the magnet passing below sensor
		- Solution: only track first sensor triggering event + every other event after that
	- State machine and sequential circuit logic for tracking sensor passes, servo movement angle and RPM calculation values
- Potentiometer input noise prevention: servo only triggered for potValue >= 100 
- Signal processing math for display values to adjust for magnitude differences and avoid overlap
- If-branch based logic for servo response:
	- Previous for loops forced servo to finish sweep cycle before responding to speed regulation
	- Goal: immediate response behaviour, adjust speed or stop moving right away depending on potentiometer input changes
- Planned: after testing, break main up into multiple files and learn how to build & upload multi-file programs to arduino 

---
## Insights & Relevance

- Documenting a structured plan enhances **reproducibility** and **debug efficiency**.
- Pre-planned testing steps reduce risk of hardware damage and help isolate software/hardware issues.
- Understanding servo max speed and mapping to delays/steps ensures predictable control before sensor feedback is added.

---
## Next Steps

- Add push button and potentiometer control for SG90.
- Validate RPM readings in Serial Plotter.
