
#ifndef EV3_SECTION_TRACER_H_
#define EV3_SECTION_TRACER_H_

#include <Clock.h>
#include "RobotInfo.h"
#include "LineMonitor.h"
#include "GyroSensor.h"
#include "PID.h"
#include "Motor.h"
#include "Odometer.h"
#include "Section.h"
#include "LineTracer.h"
#include "TailController.h"
#include "InvertedWalker.h"
#include "TailWalker.h"
#include "ParmAdministrator.h"
#include "stdlib.h"

class SectionTracer
{
public:
    SectionTracer(RobotInfo *robot_info,
                  Section *section,
                  LineTracer *line_tracer,
                  ParmAdministrator *parm);

    void run();

private:
    RobotInfo *m_robot_info;
    Section *m_section;
    LineTracer *m_line_tracer;
    ParmAdministrator *m_parm;

    bool m_is_initialized;
    int m_section_num = 0;
};

#endif // EV3_SECTION_TRACER_H_
