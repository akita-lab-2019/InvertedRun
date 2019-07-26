
#ifndef EV3_APP_LINETRACER_H_
#define EV3_APP_LINETRACER_H_

#include "LineMonitor.h"
#include "GyroSensor.h"
#include "PID.h"
#include "Motor.h"
#include "InvertedWalker.h"
#include "ParmAdministrator.h"
#include "stdlib.h"

class LineTracer
{
public:
    LineTracer(LineMonitor *line_monitor,
               InvertedWalker *inverted_walker,
               PID *pid,
               ParmAdministrator *parm);

    void run();

private:
    LineMonitor *m_line_monitor;
    InvertedWalker *m_inverted_walker;
    PID *m_pid;
    ParmAdministrator *m_parm;
    bool m_is_initialized;
};

#endif // EV3_APP_LINETRACER_H_
