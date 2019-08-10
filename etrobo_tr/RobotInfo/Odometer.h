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

    float getRobotPose(int axis);
    float getRobotDistance();
    float getRobotLinerVelocity();
    float getRobotAngularVelocity();
    float getWheelVelocity(int wheel);
    float getWheelPose(int wheel);

    enum
    {
        L,
        R
    };

    enum
    {
        X,
        Y,
        YAW
    };

private:
    void calculate();

    const float TO_RAD = 3.141592 / 180;
    const float TO_DEG = 180 / 3.141592;
    const float WHEEL_RADIUS = 0.049;
    const float WHEEL_DIST = 0.177;

    ev3api::Motor &m_wheel_L;
    ev3api::Motor &m_wheel_R;

    float m_wheel_deg[2] = {0};      // ホイール角位置[deg]
    float m_pre_wheel_deg[2] = {0};  // 前回のホイール角位置[deg]
    float m_wheel_deg_v[2] = {0};    // ホイール角速度[deg/s]
    float m_robot_pose[3] = {0};     // ロボット位置[m],[deg]
    float m_pre_robot_pose[3] = {0}; // 前回のロボット位置[m],[deg]
    float m_robot_dis = 0;           // ロボット走行距離[m]
    float m_pre_robot_dis = 0;       // 前回のロボット走行距離[m]
    float m_robot_liner_v = 0;       // ロボットの速度[m/s]
    float m_robot_angular_v = 0;     // ロボットの角速度[deg/s]
};

#endif // EV3_UNIT_ODOMETER_H_
