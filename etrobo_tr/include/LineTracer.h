
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
#include "TailWalker.h"
#include "ParmAdministrator.h"
#include "stdlib.h"

class LineTracer
{
public:
    LineTracer(RobotInfo *robot_info,
               InvertedWalker *inverted_walker,
               TailWalker *tail_walker,
               PID *pid);

    void run();

private:
    RobotInfo *m_robot_info;
    InvertedWalker *m_inverted_walker;
    TailWalker *m_tail_walker;
    PID *m_pid;

    bool m_is_initialized;
    void invertedRun(int forward_v, int turn_v);
    void tailRun(int forward_v, int turn_v);
};

#endif // EV3_APP_LINETRACER_H_
