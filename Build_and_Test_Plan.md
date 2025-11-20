# ✅ Part 1: Servo Power Supply Debugging

Looked up wire splicing tutorials.
Bought materials and planned sturdier power supply wiring for SG90 servo:

- Multimeter for circuit testing
- USB-A cables (1 for use, 1 for backup)
- Hi-fi power + GND cables roll
- Crimp insulation sleeves
- Electrical tape
- Zip ties

Existing material:

- Jumper wires or Dupont wires (see build plan)
- Arduino Uno
- V5 Sensor shield
- Hall sensor module
- SG90 mini servo
- Button/Button module
- Potentiometer
- 220 Ω resistor
- LED light

---
## ✅ Build plan:

 ⚠️ Never work on cables when connected to any power source

Note: crimp insulation sleeves will be used to cover splices. Always make sure the splices are shorter than the metal tubing in the tips, adapt stripped wires length accordingly if needed.
### Splice 1: USB-A <-> Hi-Fi
1. Strip USB-A power and GND cables (±1.5cm), cut USB-A signal cables. More length needs to be exposed on the thinner USB wires to account for the thicker hi-fi cable wires
2. Separate and strip hi-fi power and GND wires (±1cm)
3. Twist exposed copper/metal wires separately
4. Twist exposed wires together (pigtails)
5. Cover with insulation sleeve
6. Use electrical tape and zip ties to fully cover and reinforce the splice, including tube (be careful with mechanical strain when tightening zip ties)
7. Expose ±0.8cm off other end of spliced hi-fi cables, twist and form a hook small enough for multimeter probes to fit in neatly:
	- Use hooks to nest multimeter probes for testing before continuing
	- Reading should be stable ±5V using this method

Depending on the hi-fi wires' thickness, they can be slotted into female Dupont wire ends. In that case:
- If they can be slotted far enough into the Duponts for good contact:
	- Isolate and secure the connections with electrical tape (use zip ties if needed to fully seal, mind the mechanical strain).

If that does not work, as was the case for me:
### Splice 2: Hi-Fi <-> Jumper Wire
1. Slip insulation sleeves on hi-fi power and GND
2. Expose more wire off hi-fi cables (±1.5cm)
3. Twist exposed wires
4. 'Pigtail' hi-fi wires around jumper wire tip
5. Slip insulation sleeve over the splice
	- Note: splice may not fit neatly into the metal casing right away, in my case pushing the jumper wire in was needed
6. Making sure insulation sleeve stays over splice + splice stays together, use electrical tape to immobilise and cover splice (incl. tube)
7. Use zip ties to fully seal and reinforce

---
## Test plan:

### ✅ Test A: Splice test
1. Connect USB wall adapter to spliced USB+hi-fi cables and plug it
2. Immobilise jumper wire ends, test with multimeter probes

If there are issues: try using another wall adapter (same V and A).

### If Test A passes:
- Result: stable 5.09V reading

---
### ## ✅ Test B: Breadboard Test
1. Strip more off hi-fi cables
2. Test if twisted exposed wires are strong enough to go into breadboard slots:
	1. If yes and contact is solid: continue with this setup
	2. Otherwise, strip more wire and connect to dupont wire
3. Plug into breadboard power and GND rails
4. Test breadboard connection with multimeter
5. Add LED + 220 Ω resistor to test for continuous power

If there are issues: check wiring, try a different LED, resistor, or breadboard.

Sensor shield and arduino board have been proven to work, move on to testing servo.

---
### ✅  If Test B passes:
1. Connect servo power and GND to breadboard, signal to arduino board
2. Connect arduino to computer, run [servoTest1](./Tests/servoTest1.ino)

If it doesn't work: Test C

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
# Part 2: Progressive Rebuild & Test Procedure

If servo test works: since Hall sensor has been tested and proven to work:
-> introduce control button/potentiometer into sketch first before integrating sensor so that they can more easily be isolated.

---
## Reference & Calculation values:
Checked data sheet for servo speed and calculated max RPM and angle covered in one step:
- 0.1s / 60° -> 0.6s / 360°
- 60s / 0.6s = 100 RPM
- 0.1s / 60° and minimum delay of 20ms -> 100ms / 20 ms = 5 -> maximum 60 / 5 = 12° movement between delays -> use to define step increment 

---
## ✅ Servo Code Test + 'Spill Over' Angle Test

1. Run [servoTest1](./Tests/servoTest1.ino) with 5° increments
2. If test run ok: edit servo.move so the steps are 10°
3. Add button:
	- Run [servoTestButton](./Tests/servoTestButton.ino) : simple on/off, fixed speed (10° increment). Button ON -> servo.move function runs. Speed can be manually slowed down by pressing and releasing button in intervals
4. Run [servoTest_Spillover](./Tests/servoTest_Spillover.ino)
5. Possible outcomes:
	- Spill over ignored -> max step now 12°
	- Spill over transformed into backwards movement -> same
	- Spill over causes unexpected/undesired behaviour -> change speed regulation to delay-based instead of angle-based

Result: spill over ignored: anything below 0° acts like 0°, same for 180°


---
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

---
## Rebuild Sketch, Part 2:
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
## Add-On:
- Introduce RGB LED for visual RPM range display:
-  3 phases (based on potentiometer reading):
	- Phase 1: 100 - 400
	- Phase 2: 401 - 700
	- Phase 3: 701 - 1000
 * Green -> (Turquoise) -> Blue -> (Purple) -> Red
 * Phase 1: 
	 * 255 Green, 0 Blue
	 * -> 128 Green, 127 Blue
 * Phase 2: 
	 * 127 Green, 128 Blue
	 * -> 0 Green, 255 Blue ; 255 Blue, 0 Red
	 * -> 127 Blue, 128 Blue, 127 Red
 * Phase 3:
	 * 127 Blue - 128 Red
	 * -> 0 Blue - 255 Red
- Add hysteresis to prevent flickering?
