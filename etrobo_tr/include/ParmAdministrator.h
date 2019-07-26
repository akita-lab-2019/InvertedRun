#ifndef EV3_PARMADMINISTRATOR_H_
#define EV3_PARMADMINISTRATOR_H_

#include <stdio.h>
#include <stdlib.h>

class ParmAdministrator
{
public:
    explicit ParmAdministrator();
    void readParm();
    float forward_v;
    float turn_v;
    float color_sensor_targrt;
    float trace_pid[3];

private:
    float m_parm_data[20];
};

#endif // EV3_PARMADMINISTRATOR_H_
