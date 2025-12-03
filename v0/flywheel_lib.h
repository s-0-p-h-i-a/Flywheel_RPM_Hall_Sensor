#include <stdbool.h>
#include <Arduino.h>

#ifndef FLYWHEEL_LIB_H
#define FLYWHEEL_LIB_H

extern const int ANGLE_ZERO;
extern const int ANGLE_180;
extern const int BUTTON_PIN;

extern int flywheelAngle;
extern int speed;

void initFlywheel(void);

void spinFlywheel(void);

void setSpeed(void);

void checkSpeed(void);

#endif
