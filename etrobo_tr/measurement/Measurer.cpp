#include "Measurer.h"

/**
 * コンストラクタ
 * @param colorSensor カラーセンサ
 */
Measurer::Measurer(ev3api::Motor &leftWheel, ev3api::Motor &rightWheel) : m_wheel_l(leftWheel), m_wheel_r(rightWheel)
{
}

void Measurer::measure()
{
    wheel_rad_l = m_wheel_l.getCount() * TO_RAD;
    wheel_rad_r = m_wheel_r.getCount() * TO_RAD;

    calOdom();
}

void Measurer::calOdom()
{
    float wheel_dis_l = wheel_rad_l * WHEEL_R;
    float wheel_dis_r = wheel_rad_r * WHEEL_R;
    robot_dis = (wheel_dis_r + wheel_dis_l) / 2;
    robot_rad = (wheel_dis_r - wheel_dis_l) / 0.18;
}

float Measurer::getRobotDistance()
{
    return robot_dis;
}

float Measurer::getRobotAngle()
{
    return robot_rad;
}
