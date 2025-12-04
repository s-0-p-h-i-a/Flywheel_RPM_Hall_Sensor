#include <stdbool.h>
#include <Arduino.h>

#ifndef HALL_LIB_H
#define HALL_LIB_H

extern const int HALL_PIN;

extern bool hallON;

extern unsigned long currentSensorRead;
extern unsigned long lastSensorRead;

extern bool secondPass;

void initHall(void);
void checkReadState(void);

#endif
