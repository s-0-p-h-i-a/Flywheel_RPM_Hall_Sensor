#include "flywheel_lib.h"
#include "hall_lib.h"
#include "rpmlib.h"
#include <Arduino.h>

#ifndef PLOTTER_LIB_H
#define PLOTTER_LIB_H

extern const int RPM_SHIFTUP;
extern const int ANGLE_SHIFTUP;
extern const int ANGLE_AMPLIFY;
extern const int HALL_AMPLIFY;
extern const int SPEED_AMPLIFY;

void plotterDisplay(void);

#endif
