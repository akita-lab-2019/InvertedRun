
#ifndef EV3_APP_LINETRACER_H_
#define EV3_APP_LINETRACER_H_

#include "LineMonitor.h"
#include "GyroSensor.h"
#include "PID.h"
#include "Motor.h"
#include "Odometer.h"
#include "InvertedWalker.h"
#include "ParmAdministrator.h"
#include "stdlib.h"

class LineTracer
{
public:
    LineTracer(LineMonitor *line_monitor,
               InvertedWalker *inverted_walker,
               PID *pid,
               ParmAdministrator *parm,
               Odometer *odometer);

    void run();

private:
    LineMonitor *m_line_monitor;
    InvertedWalker *m_inverted_walker;
    PID *m_pid;
    ParmAdministrator *m_parm;
    Odometer *m_odometer;
    bool m_is_initialized;
    int scenario_num = 0;
    void sectionRun(int forward_v);
};

#endif // EV3_APP_LINETRACER_H_
