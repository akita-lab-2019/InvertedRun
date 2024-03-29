#ifndef EV3_APP_TAILCONTROLLER_H_
#define EV3_APP_TAILCONTROLLER_H_

#include "ev3api.h"
#include "LineMonitor.h"
#include "GyroSensor.h"
#include "Motor.h"
#include "PID.h"
#include "BalancerCpp.h"
#include "stdlib.h"

class TailController
{
public:
    TailController(PID *pid);

    void init();
    void setAngle(int target_angle);
    void setMaxSpeed(int max_speed);
    void setTailSpeed(int tail_speed);
    void control();

private:
    PID *m_pid;

    int m_target_angle = 0;
    int m_max_speed = 0;
};

#endif // EV3_APP_TAILCONTROLLER_H_
