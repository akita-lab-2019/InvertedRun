
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
    TailWalker();

    void init();
    void update();
    void setCommand(int forward, int turn);
    int8_t getPwmRight();
    int8_t getPwmLeft();

private:
    int cancelBacklash(int8_t pwm, int enc);

    int m_forward;
    int m_turn;
    int8_t m_right_pwm;
    int8_t m_left_pwm;

    float K = 1;
};

#endif // EV3_UNIT_TAILWALKER_H_