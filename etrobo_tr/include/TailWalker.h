
#ifndef EV3_UNIT_TAILWALKER_H_
#define EV3_UNIT_TAILWALKER_H_

#include "RobotInfo.h"
#include "GyroSensor.h"
#include "Motor.h"
#include "BalancerCpp.h"
#include "stdlib.h"

class TailWalker
{
public:
    TailWalker(ev3api::Motor &wheel_L,
               ev3api::Motor &wheel_R);

    void init();
    void run();
    void setCommand(int forward, int turn);

private:
    ev3api::Motor &m_wheel_L;
    ev3api::Motor &m_wheel_R;

    int m_forward;
    int m_turn;

    float K = 1;
};

#endif // EV3_UNIT_TAILWALKER_H_