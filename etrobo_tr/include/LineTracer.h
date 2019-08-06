
#ifndef EV3_APP_LINETRACER_H_
#define EV3_APP_LINETRACER_H_

#include <Clock.h>
#include "RobotInfo.h"
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
    LineTracer(RobotInfo *robot_info,
               InvertedWalker *inverted_walker,
               PID *pid,
               ParmAdministrator *parm,
               TailController *tail_controller,
               ev3api::Motor &wheel_L,
               ev3api::Motor &wheel_R);

    void run();

private:
    RobotInfo *m_robot_info;
    InvertedWalker *m_inverted_walker;
    PID *m_pid;
    ParmAdministrator *m_parm;
    TailController *m_tail_controller;
    ev3api::Motor &m_wheel_L;
    ev3api::Motor &m_wheel_R;
    bool m_is_initialized;
    int scenario_num = 0;
    void sectionRun(int forward_v);
};

#endif // EV3_APP_LINETRACER_H_
