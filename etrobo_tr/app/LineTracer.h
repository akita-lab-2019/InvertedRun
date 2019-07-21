
#ifndef EV3_APP_LINETRACER_H_
#define EV3_APP_LINETRACER_H_

#include "LineMonitor.h"
#include "GyroSensor.h"
#include "Motor.h"
#include "InvertedWalker.h"
#include "./../measurement/Measurer.h"
#include "stdlib.h"

class LineTracer
{
public:
    LineTracer(LineMonitor *line_monitor,
               InvertedWalker *inverted_walker);

    void run();

private:
    LineMonitor *m_line_monitor;
    InvertedWalker *m_inverted_walker;
    bool m_is_initialized;
};

#endif // EV3_APP_LINETRACER_H_
