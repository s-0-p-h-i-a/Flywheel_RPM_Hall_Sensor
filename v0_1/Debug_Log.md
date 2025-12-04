# Flywheel RPM Sensor Emulation – v0 → v0.1 Transition Log

## 1. Initial Observations (v0)

- Hardware integrated successfully: Hall sensor, SG90 servo with magnet disc, push buttons functional.
- Basic system behavior mostly correct: control and sweep-back logic operational.
- Telemetry anomalies noted:
    - RPM occasionally “stuck” or decayed slower than expected.
    - Speed values exhibited jitter.
    - Minor sweep-back logic changes unexpectedly affected servo movement.

---
## 2. Identified Challenges

- **Timing & Data Flow Conflicts:**
    - Multiple `millis()` calls across modules led to desynchronized time references.
    - Independent sampling caused subtle race conditions in shared variables.

- **Shared State Conflicts in `flywheel_lib`:**
    - Single module handling both RPM calculation and servo control propagated changes unpredictably.
    - Trying to fix one symptom (RPM decay) would break servo behavior.

- **Edge-Case & Asynchronous Behavior:**
    - Minor logic changes revealed latent failures under specific input/state combinations.
    - Telemetry revealed internal conflicts not evident from hardware output alone.

---
## 3. Conceptual Insights Applied

- Developed mental models of: Hall pulse timing, servo sweep angles, RPM calculation, and control states (idle, accelerate, decelerate, sweep-back).

- Recognized need for **deterministic read-process-actuate order** in the main loop.

- Centralized timestamp per loop (`now`) identified as critical for synchronizing module computations.

- Understood the impact of mechanical inertia and actuator delays on perceived system behavior.

---
## 4. v0.1 Refactor & Solutions

- **Centralized `now` timestamp**: all functions in loop reference the same value to reduce timing inconsistencies.
- **Global time window (`timeWindow`)** introduced for idle detection and centralized timing.
- This way `now` and `timeWindow` act as respectively a central clock and as a central 'latch' enabled by valid sensor input.

- **Loop order restructured** for predictable data flow:

    1. Handle edge angles
    2. Update speed (`canAccelerate`/`canDecelerate`)
    3. Spin flywheel
    4. Move servo
    5. Update sensor read state
    6. Compute time window
    7. Check flywheel idle state
    8. Calculate RPM
    9. Plot data
    10. Update `now` for next iteration

- **State machine refinement**: clearly defined control flow for idle, accelerate, decelerate, and sweep-back.

- **Module separation & build improvements**:
    
    - `flywheel_lib` converted to `.cpp` for state machine and actuator integration.
    - Other libraries remain C for practice and modularity.
    - Buildable outside Arduino IDE via `gcc-avr` and Makefile.

---
## 5. Next Steps

- Test v0.1 on hardware to validate loop logic, timing, and RPM calculation.
- Instrument telemetry: Hall pulse intervals, computed RPM, servo angles, control states, and time-since-last-Hall.
- Apply filtering and timeout policies to stabilize RPM and servo behavior.
- Conduct systematic test sequences to isolate latent edge-case failures.