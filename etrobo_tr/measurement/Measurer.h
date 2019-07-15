#ifndef EV3_UNIT_DISTANCEMEASURMENT_H_
#define EV3_UNIT_DISTANCEMEASURMENT_H_

#include "ev3api.h"
#include "Motor.h"
#include "stdlib.h"

// 定義
class Measurer
{
public:
    Measurer(ev3api::Motor &leftWheel,
             ev3api::Motor &rightWheel);

    void measure();
    void calOdom();
    float getRobotDistance();
    float getRobotAngle();

private:
    const float TO_RAD = 3.141592 / 180;
    const float TO_DEG = 180 / 3.141592;

    const float WHEEL_R = 0.049;

    ev3api::Motor &m_wheel_l;
    ev3api::Motor &m_wheel_r;

    // 車輪の回転角度[rad]
    float wheel_rad_l; // 左車輪
    float wheel_rad_r; // 右車輪

    // ロボットの位置
    float robot_dis; // 走行距離[m]
    float robot_rad; // 角度[rad]
};

#endif // EV3_UNIT_DISTANCEMEASURMENT_H_
