#ifndef EV3_RECORDER_H_
#define EV3_RECORDER_H_

#include <Clock.h>
#include "LineMonitor.h"
#include "GyroSensor.h"
#include "ColorSensor.h"
#include "PID.h"
#include "Motor.h"
#include "Odometer.h"
#include "TailController.h"
#include "InvertedWalker.h"
#include "ParmAdministrator.h"
#include "stdlib.h"

class Recorder
{
public:
    Recorder(ev3api::Clock &clock,
             ev3api::ColorSensor &color_sensor,
             LineMonitor *line_monitor,
             InvertedWalker *inverted_walker,
             PID *pid,
             ParmAdministrator *parm,
             Odometer *odometer,
             ev3api::Motor &wheel_L,
             ev3api::Motor &wheel_R);

    void init();
    int getFilename();
    void writeHeader();
    void record();

private:
    ev3api::Clock &m_clock;
    ev3api::ColorSensor &m_color_sensor;
    LineMonitor *m_line_monitor;
    InvertedWalker *m_inverted_walker;
    PID *m_pid;
    ParmAdministrator *m_parm;
    Odometer *m_odometer;
    ev3api::Motor &m_wheel_L;
    ev3api::Motor &m_wheel_R;
};

#endif // EV3_RECORDER_H_
