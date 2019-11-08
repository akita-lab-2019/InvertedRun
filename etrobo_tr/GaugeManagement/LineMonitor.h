#ifndef EV3_LINEMONITOR_H_
#define EV3_LINEMONITOR_H_

#include "ev3api.h"
#include "ColorSensor.h"

void initLineMonitor();
void processLineMonitor();
void setBrightnessTarget(float target);
float getBrightnessGap();

#endif