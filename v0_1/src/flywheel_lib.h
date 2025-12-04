#include <Arduino.h>

#ifndef FLYWHEEL_LIB_H
#define FLYWHEEL_LIB_H

extern const int ANGLE_ZERO;
extern const int ANGLE_180;
extern const int MAX_SPEED;
extern const int MIN_SPEED;
extern const int BUTTON_PIN;
extern const int FASTER_BUTTON_PIN;
extern const int SLOWER_BUTTON_PIN;
extern const int BUTTON_DELAY;

extern unsigned int flywheelAngle;
extern unsigned int lastAngle;
extern int speed;
extern bool isIdle;

extern long unsigned int thisTime;
extern long unsigned int thisSpeedIncreaseTime;
extern long unsigned int lastSpeedIncreaseTime;
extern long unsigned int thisSpeedDecreaseTime;
extern long unsigned int lastSpeedDecreaseTime;

void initFlywheel(void);

void handleEdgeAngles(void);
void checkFlywheelState(void);
bool canAccelerate(void);
bool canDecelerate(void);
void updateSpeed(void);

void spinFlywheel(void);

void setSpeed(void);

#endif
