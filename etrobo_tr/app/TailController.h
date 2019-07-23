#ifndef EV3_APP_TAILCONTROLLER_H_
#define EV3_APP_TAILCONTROLLER_H_

#include "LineMonitor.h"
#include "GyroSensor.h"
#include "Motor.h"
#include "PID.h"
#include "BalancerCpp.h"
#include "stdlib.h"

class TailController
{
public:
    TailController(ev3api::Motor &tail_motor, PID *pid);

    void init();
    void control(int target_angle, int max_speed);

private:
    ev3api::Motor &m_tail_motor;
    PID *m_pid;
};

#endif // EV3_APP_TAILCONTROLLER_H_
