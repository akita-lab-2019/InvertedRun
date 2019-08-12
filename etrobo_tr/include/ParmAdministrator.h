#ifndef EV3_PARMADMINISTRATOR_H_
#define EV3_PARMADMINISTRATOR_H_

#include <stdio.h>
#include <stdlib.h>

class ParmAdministrator
{
public:
    explicit ParmAdministrator();
    void readParm();

    float color_sensor_targrt[2]; // [0]:尻尾走行の場合 [1]:倒立走行の場合
    float trace_pid[5][3];
    float curvature[5];
    float forward[18];

private:
    float m_parm_data[50];
};

#endif // EV3_PARMADMINISTRATOR_H_
