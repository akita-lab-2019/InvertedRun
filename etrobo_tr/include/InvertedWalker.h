
#ifndef EV3_UNIT_INVERTEDWALKER_H_
#define EV3_UNIT_INVERTEDWALKER_H_

#include "RobotInfo.h"
#include "GyroSensor.h"
#include "Motor.h"
#include "BalancerCpp.h"
#include "stdlib.h"

class InvertedWalker
{
public:
    // クラスの定数
    static const int LOW;
    static const int NORMAL;
    static const int HIGH;

    InvertedWalker(RobotInfo *robot_info,
                   ev3api::Motor &wheel_L,
                   ev3api::Motor &wheel_R,
                   Balancer *balancer);

    void init();
    void run();
    void setCommand(int forward, int turn);

private:
    RobotInfo *m_robot_info;
    ev3api::Motor &m_wheel_L;
    ev3api::Motor &m_wheel_R;
    Balancer *m_balancer;
    int m_forward;
    int m_turn;
};

#endif // EV3_UNIT_INVERTEDWALKER_H_