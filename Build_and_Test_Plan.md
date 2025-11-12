# Part 1: Servo power supply debugging

Looked up wire splicing tutorials.
Bought materials and planned sturdier power supply wiring for SG90 servo:

- Multimeter for circuit testing
- USB-A cables (1 for use, 1 for backup)
- Hi-fi power + GND cables roll
- Heat-shrink tubing
- Electrical tape
- Zip ties

---
## Build plan:
1. Strip USB-A cable, separate and strip power and GND wires
2. Separate and strip power and GND wires on hi-fi cable
3. Twist exposed copper wires together
4. Cover with heat-shrink tube (with slack for the wires to prevent strain)
5. Use electrical tape and zip ties as needed to reinforce the splice

---
## Test plan:

### Test A: Splice test
1. Strip small section of the hi-fi cables
2. Connect USB wall adapter to spliced cable and plug it
3. Use multimeter to test via short exposed copper wires

If there are issues: try using another wall adapter (same V and A).

If Test A passes:

---
### Test B1: Breadboard connections test level 1
1. Strip more off hi-fi cables
2. Test if twisted exposed wires are strong enough to go into breadboard slots:
	1. If yes and contact is solid: continue with this setup
	2. Otherwise, strip more wire and connect to dupont wire
3. Plug into breadboard power and GND rails
4. Test breadboard connection with multimeter
5. Add LED + 220 Ω resistor to test for continuous power

If there are issues: check wiring, try a different LED, resistor, or breadboard.

---
### Test B2: Breadboard connections test level 2
1. Add relevant resistor and push button and/or potentiometer to breadboard wiring
2. Test turning LED on/off via push button and/or potentiometer

This is to test power supply across the whole board.

If issues: check wiring, try different components.

Sensor shield and arduino board have been proven to work, move on to testing servo.

---
### If Tests B1 and B2 pass:
1. Write short test function with slow movement (light load)
2. Connect servo power and GND to breadboard, signal to arduino board
3. Connect arduino to computer, upload sketch, test servo

If it doesn't work:

---
### Test C:
1. Check pin:
	1. Servo connected to digital pin?
	2. Correct pin number constant in sketch?
2. Servo.h included?
3. Servo object is created?
4. Servo is correctly initialised in void setup?
5. Code in void loop uses correct pin number constant name?
6. Correct use of servo.write?

If it still doesn't work: retrace steps until issue found + solved.

---
# Part 2: Progressive rebuild & test procedure

If servo test works: Hall sensor has been tested and proven to work
-> introduce control button/potentiometer into sketch first before integrating sensor so that they can more easily be isolated.

---
## Reference & Calculation values:
Checked data sheet for servo speed and calculated max RPM and angle covered in one step:
- 0.1s / 60° -> 0.6s / 360°
- 60s / 0.6s = 100 RPM
- 0.1s / 60° and minimum delay of 20ms -> 100ms / 20 ms = 5 -> maximum 60 / 5 = 12° movement between delays -> use to define step increment movement between delays

---
## Rebuild sketch, part 1:
1. Edit servo.move function for slightly faster movement then test run
2. If test run ok: edit servo.move so the steps are 12°
3. Add control:
	1. Button: simple on/off, fixed speed. Button ON -> servo.move function runs. Speed can be manually slowed down by pressing and releasing button in intervals
	2. Potentiometer:
		- Use read values to define (inversely proportional) length of millis delay in servo.move
		- If pot reads 0: empty if branch, does not call servo.move
		- Else: map pot values from 0 - 1023 to 80ms - 20ms (isolate pot.read == 0 into separate if branch)
4. Implement 'virtual' RPM value reading and display in serial plotter:
	- RPM = (20 / delay) * 100
	- Define getRPM function using this formula
5. Implement pot values display into serial plotter:
	- Display value : pot.read / 20 so it fits within RPM range but doesn't overlap or intersect with RPM value

Test this version, if issues appear work back stepwise to isolate problem.

---
## Rebuild sketch, part 2:
1. Integrate Hall sensor reading into sketch: 
	- Define pin constant
	- Initialise sensor
	- Define getTimeWindow function to measure time between sensor LOW reads
2. Change getRPM to use Hall sensor reading for calculation:
	- RPM = 60000 / getTimeWindow
	- Average with last read RPM to smooth variations

---
## Rebuild hardware setup (disconnected from power):
1. Secure disc with magnet to servo arms
2. Connect Hall sensor to circuit
3. Position servo and sensor so that the sensor can read the magnet, secure in place

Connect circuit to power + computer, upload sketch, test setup and serial plotter display.
If issues appear, work back stepwise to fix them, then repeat steps until everything works!

---
Notes:
- What about trying static 20ms delay and vary step angle instead?
	-> write & test alternative servo.move using new getStepAngle function to convert pot reading into 0° - 12° range 
