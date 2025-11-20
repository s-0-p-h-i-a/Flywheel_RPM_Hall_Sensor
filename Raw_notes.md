TEST / PROOF OF CONCEPT: hall flywheel sensor emulator

# 13/10

GOAL:

- First implementation / test of servo + push button + hall sensor setup
- Write base program as basis for future developments

SETUP:

- Arduino uno board
- V5 sensor shield
- Digital push button module
- Digital hall sensor module
- SG90 servo motor
- Cardboard disc with one magnet on the edge, mounted on the servo

BEHAVIOUR:

- Servo motor: continuous speed
- Push button: controls motor, pressed = ON, not pressed = OFF
- Hall sensor: reads when the magnet passes by
- Track time between current and previous instance of magnet detection by the hall sensor
- Calculate current rpm based on time interval: 60 000ms / currentTimeWindow
- Calculate average with last rpm to smooth signal
- Display rpm in serial plotter

LOG:

- Considered scope for the variables involved to decide whether to declare globally or in loop
- Hall sensor does react to fridge magnet, experimented with different positions:
	- Magnet passing right below the module gives most reliable sensor reaction
	- Researched why: conductive elements on module board may influence magnet EMF
- Looked at serial plotter reaction to magnet movement
- Tested RPM calculation + result display via plotter and monitor with manual movement
- Servo not working:
	- Hall sensor OK
	- Button OK
- Must be the servo
- Initially assumed blocking servo moving code (for loop) may be causing issues
- Tested setup with simple servo-only code: still did not work
- Issue: did not know servo cannot be powered via arduino board, do not have the material to connect it to usb adapter
- Researched powering and risk issues:
	- Arduino board power not sufficient to properly power servo, may affect other parts of hardware setup
	- Have the material to power modules via 9v battery, but voltage too high for servo
- No terminal adapter for usb at hand -> cut and stripped usb wire to power servo via wall adapter
- Setup not working: issue narrowed down to servo motor no getting enough current
- Will rewire tomorrow then start again

# 14/10  
- Next day, considered potential servo powering issues across the setup, narrowed down to two main probable issues:
1. Stripped usb cable: insufficient breadboard slot contact and too many broken copper wires
2. Wall adapter known to have fluctuating current: when used to charge phone, charge times vary a lot and can be very long
- Cut and stripped cable repeatedly until figuring out proper technique to strip without breaking copper wires
- Contacted electronics repair shop to ask about copper soldering to strengthen the wires: they do not do this specific type of soldering at their shop


# 13/11

- Clean up / rewrite main program
- Write script for test1: simple slow continuous servo motion, 1° increments
- For now, keeping main program in one file for simplicity, will refactor into multiple files once full behaviour is
implemented
- Write button test script, 10° increments
- Write potentiometer test script: level 1 continuous movement (10° increments) for any non zero values
- Level 2, map speed to pot level: 12° increments, delay range 20ms - 80ms -> 60ms interval
	-> delay = 80 - potLevel * (60/1023)
- It may be easier to round down max speed to 10° increments... will have to check how this affects RPM calc math
- ChatGPT check suggests logic and math errors in main and test script drafts
- TO-DO:
	- Check logic and math in all code
	- Move RPM calculation to separate function


# 15/11

- Successfully spliced USB and hi-fi cables:
	- Protective tubing turned out not to be heat shrink tubing, it is a crimp insulation sleeve
	- Used electrical tape + zip ties to secure splice
	- Used electrical tape to secure + protect the thin exposed power+gnd wires from the usb cable (no exposed copper, but the plastic casing felt too flimsy to stay exposed)
- Multimeter test: stable 5.09V reading, no fluctuation
- New issue: hi-fi cable copper wiring too thick for breadboard or dupont
	- Noticed that copper wires get tightly pressed together when getting cut:
	- -> twist wires, make diagonal cut, press together with clamp
	- Still doesn't fit
- Alternative: use previously stripped usb cable kept as backup
- Was able to directly slot backup cable wires into breadboard rails using jumper wire tip
- Multimeter probes do not fit into breadboard slots? Reading 0V
- -> move on to LED test -> did not work
- Solution found:
	- Backup cable copper wires were too weak to slot into female dupont
	- -> tested other wires, found slightly looser dupont
	- Inserted copper wires by screwing female dupont onto them, following same direction as the wires twist, added electrical tape
- Multimeter test: current fluctuates a lot, but does reach 5V
	- Maybe due to tape not properly sealing the thin wires?
	- Use zip ties to reinforce insulation
- New idea: splice hi-fi and jumper wire
- It works! Stable 5.09V reading when the wires are immobilised
- Breadboard test: stable 5.08V
- LED test also ok: no flicker
- Ran into issues powering the servo, solved: breadboard vcc and gnd rails have gaps in the middle -> move all components to the same half of the board
- Previously assumed servo could spin continuously, it can only sweep back and forth
	- -> need to edit main and test scripts to reflect that
	- RPM calculation: value needs to be halved to account for 2 magnet passes for each rev

# 16/11

/** FLYWHEEL HALL SENSOR EMULATION V1.0 DRAFT
 * 
 * NOTE: CODE MAY NOT BE FUNCTIONAL, HAS NOT BEEN TESTED OR REVIEWED YET
 * COMMITTING TO REPO FOR PROGRESS TRACKING
 * 
 * 15/11
 * 
 * V0 was not functional but this is a departure from the original concept, so v1.0
 * 
 * CHANGES:
 * - Speed regulation: using angle instead of delay:
 * 	- if potValue > 100: servoAngle =  round(potValue/103.0)
 * 		-> between 1° and 10°, division by 103.0 to avoid max angle values above 10°
 * 
 * CONCEPT:
 * - Since servo sweeps back and forth, Hall sensor response logic needs to change:
 * 	- Issues: sensor triggered twice per 'revolution', time interval of magnet passing below sensor
 * 	would make RPM calculation out of sync with real-life behaviour
 * 	-> Only count first sensor pass then every other pass
 * 
 * - Implemented state machine logic for secondPass:
 * 	-> Was initially inside getRPM, then its own function, then moved secondPass update inside void loop:
 * 		this state needs to be re-checked and tracked at every loop regardless of RPM calculation
 * 	-> Intentional global variables for values that need 'memory' outside of individual loop runs
 * 
 * - Sequential circuit based logic for thisRPM, lastRPM, currentSensorRead, lastSensorRead:
 * 	-> values initialised at 0 = sending initial 0 signal to set circuit
 * 
 * - Defined 'safety range' for pot values to avoid irregularities due to flickering input current:
 * Only values above 100 trigger servo movement
 * 
 * - Split servo movement into separate sweepServo and moveServo functions for logical separation and 
 * modularity for future changes
 * 
 * - Defined easier to implement logic for speed regulation + state machine concept for program execution and
 * cross-loop state tracking -> straightforward refactor process, no conceptual/logic issue points
* 
 * - Trying out if-based logic for immediate response to speed regulation (previous for loops made servo finish
 * sweep before updating speed)
 * 
 * 
 * DATA VISUALISATION:
 * - Signal processing math for RPM, angle and pot value display: so that all variable variations can be visualised
 * at once without overlap + without loss of detail (angle, RPM, and pot value all have different magnitudes)
 * 
 * **/

# 19/11
## Pot & speed control:

- Button module worked
## Rebuild Sketch, Part 1:
-  Add potentiometer:
	-  Run [servoTestPot](./Tests/servoTestPor.ino)
	- Use read values to define (inversely proportional) length of delay in servo.move
	- If pot reads <100: empty if branch, does not call servo.move
	- Else: map pot values 100 - 1023 to angle values 1° - 12° (potValue/85.0)
	- Add hysteresis for smoother behaviour?: speed only changes if potValue changes more than ±10

Test this version, if issues appear work back stepwise to isolate problem.

Notes:
- Tried different logic for servomove, servo responded to pot input changes but not as desired
- Test had to be stopped due to pot getting warm
- Alternative solution: 2 buttons:
	- 1 Button module for on/off
	- 2 Breadboard buttons for faster/slower
- Idea for later testing: joystick 

Write servoTest3Buttons with new speed regulation logic

Issues:
- Breadboard buttons not getting picked up
- LED stays on with dim light, no response to buttons
- Tried different pins, different wiring configurations

Main things today:
- Servo powering via button worked
- Potentiometer was causing servo response, probably from code. Did not figure out this issue as pot started getting warm
- Rewrote some tests
- Added 3 buttons test script
- Trying new servo movement logic ideas
