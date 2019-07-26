
#ifndef EV3_APP_LINETRACER_H_
#define EV3_APP_LINETRACER_H_

#include <Clock.h>
#include "LineMonitor.h"
#include "GyroSensor.h"
#include "PID.h"
#include "Motor.h"
#include "Odometer.h"
#include "TailController.h"
#include "InvertedWalker.h"
#include "ParmAdministrator.h"
#include "stdlib.h"

class LineTracer
{
public:
    LineTracer(ev3api::Clock &clock,
               LineMonitor *line_monitor,
               InvertedWalker *inverted_walker,
               PID *pid,
               ParmAdministrator *parm,
               Odometer *odometer,
               TailController *tail_controller,
               ev3api::Motor &wheel_L,
               ev3api::Motor &wheel_R);

    void run();

private:
    ev3api::Clock &m_clock;
    LineMonitor *m_line_monitor;
    InvertedWalker *m_inverted_walker;
    PID *m_pid;
    ParmAdministrator *m_parm;
    Odometer *m_odometer;
    TailController *m_tail_controller;
    ev3api::Motor &m_wheel_L;
    ev3api::Motor &m_wheel_R;
    bool m_is_initialized;
    int scenario_num = 0;
    void sectionRun(int forward_v);
};

#endif // EV3_APP_LINETRACER_H_
