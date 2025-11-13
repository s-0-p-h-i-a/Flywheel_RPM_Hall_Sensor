TEST / PROOF OF CONCEPT: hall flywheel sensor emulator

13/10

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

  
- Next day, considered potential servo powering issues across the setup, narrowed down to two main probable issues:
1. Stripped usb cable: insufficient breadboard slot contact and too many broken copper wires
2. Wall adapter known to have fluctuating current: when used to charge phone, charge times vary a lot and can be very long
- Cut and stripped cable repeatedly until figuring out proper technique to strip without breaking copper wires
- Contacted electronics repair shop to ask about copper soldering to strengthen the wires: they do not do this specific type of soldering at their shop


13/11

- Clean up / rewrite main program
- Write script for test1: simple slow continuous servo motion, 1° increments
- For now, keeping main program in one file for simplicity, will refactor into multiple files once full behaviour is
implemented
- Write button test script, 10° increments
- Write potentiometer test script: level 1 continuous movement (10° increments) for any non zero values
- Level 2, map speed to pot level: 12° increments, delay range 20ms - 80ms -> 60ms interval
	-> delay = 80 - potLevel * (60/1023)
- It may be easier to round down max speed to 10° increments... will have to check how this affects RPM calc math
- TO-DO:
	- Check 12° steps math
	- Move RPM calculation to separate function
