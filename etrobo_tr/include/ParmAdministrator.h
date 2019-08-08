#ifndef EV3_PARMADMINISTRATOR_H_
#define EV3_PARMADMINISTRATOR_H_

#include <stdio.h>
#include <stdlib.h>

class ParmAdministrator
{
public:
    explicit ParmAdministrator();
    void readParm();

    char all_data_str[20][256];
    float forward_v[2];
    float color_sensor_targrt;
    float trace_pid[2][3];

private:
    float m_parm_data[20];
};

#endif // EV3_PARMADMINISTRATOR_H_
