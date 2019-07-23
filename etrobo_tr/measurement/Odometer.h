#ifndef EV3_UNIT_ODOMETER_H_
#define EV3_UNIT_ODOMETER_H_

#include "ev3api.h"
#include "Motor.h"
#include "stdlib.h"
#include "math.h"

// 定義
class Odometer
{
public:
    Odometer(ev3api::Motor &wheel_L, ev3api::Motor &wheel_R);

    void init();
    void measure();
    float getRobotPoseX();
    float getRobotPoseY();
    float getRobotDistance();
    float getRobotAngle();

private:
    void calculate();

    const float TO_RAD = 3.141592 / 180;
    const float TO_DEG = 180 / 3.141592;
    const float WHEEL_R = 0.049;

    ev3api::Motor &m_wheel_L;
    ev3api::Motor &m_wheel_R;

    // 車輪の回転角度[rad]
    float m_wheel_rad_L; // 左車輪
    float m_wheel_rad_R; // 右車輪

    // ロボットの位置
    float m_robot_x = 0;
    float m_robot_y = 0;
    float m_pre_robot_x = 0;
    float m_pre_robot_y = 0;
    float m_pre_robot_dis = 0; // 走行距離[m]
    float m_robot_dis = 0;     // 走行距離[m]
    float m_robot_rad = 0;     // 角度[rad]
};

#endif // EV3_UNIT_ODOMETER_H_
