#ifndef EV3_APP_PID_H_
#define EV3_APP_PID_H_

#include "LineMonitor.h"
#include "GyroSensor.h"
#include "Motor.h"
#include "BalancerCpp.h"
#include "stdlib.h"

class PID
{
public:
    PID(float Kp, float Ki, float Kd);

    void init(float Kp, float Ki, float Kd);
    float calculate(float target_val, float now_val);

private:
    float m_Kp = 0;
    float m_Ki = 0;
    float m_Kd = 0;

    float integral = 0;
    float diff[2] = {0}; // 0: 現在, 1: 過去
};

#endif // EV3_APP_PID_H_
