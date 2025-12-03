#include "flywheel_lib.h"
#include "hall_lib.h"
#include <stdbool.h>
#include <Arduino.h>

#ifndef RPMLIB_H
#define RPMLIB_H

extern int thisRPM;
extern int lastRPM;

int getTimeWindow();
int getRPM();

#endif