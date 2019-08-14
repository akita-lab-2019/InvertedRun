#ifndef EV3_START_MANAGER_H_
#define EV3_START_MANAGER_H_

#include "RobotInfo.h"
#include "GyroSensor.h"
#include "Motor.h"
#include "BalancerCpp.h"
#include "stdlib.h"
#include <TouchSensor.h>
#include "BluetoothManager.h"

class StartManager
{
public:
    StartManager(BluetoothManager bt,
                 ev3api::TouchSensor touch_sensor);

    void init();
    void waitForStart();
    void start(int forward, int turn);
    int8_t getPwmRight();
    int8_t getPwmLeft();

private:
    BluetoothManager m_bt;
    ev3api::TouchSensor m_touch_sensor;

    int cancelBacklash(int8_t pwm, int enc);

    int m_forward;
    int m_turn;
    int8_t m_right_pwm;
    int8_t m_left_pwm;

    float K = 1;
};

#endif