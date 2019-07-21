
#ifndef EV3_UNIT_INVERTEDWALKER_H_
#define EV3_UNIT_INVERTEDWALKER_H_

#include "GyroSensor.h"
#include "Motor.h"
#include "BalancerCpp.h"
#include "./../measurement/Measurer.h"
#include "stdlib.h"

class InvertedWalker
{
public:
    // クラスの定数
    static const int LOW;
    static const int NORMAL;
    static const int HIGH;

    InvertedWalker(ev3api::GyroSensor &gyro_sensor,
                   ev3api::Motor &wheel_L,
                   ev3api::Motor &wheel_R,
                   Balancer *balancer);

    void init();
    void run();
    void setCommand(int forward, int turn);

private:
    ev3api::GyroSensor &m_gyro_sensor;
    ev3api::Motor &m_wheel_L;
    ev3api::Motor &m_wheel_R;
    Balancer *m_balancer;
    Measurer *m_measurer;
    int m_forward;
    int m_turn;
};

#endif // EV3_UNIT_INVERTEDWALKER_H_