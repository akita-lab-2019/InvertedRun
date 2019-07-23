#include "Odometer.h"

/**
 * コンストラクタ
 * @param wheel_L 左モータ
 * @param wheel_L 右モータ
 */
Odometer::Odometer(ev3api::Motor &wheel_L, ev3api::Motor &wheel_R)
    : m_wheel_L(wheel_L), m_wheel_R(wheel_R)
{
}

/**
 * 初期化する
 */
void Odometer::init()
{
    m_robot_x = 0;
    m_robot_y = 0;
    m_robot_dis = 0; // 走行距離[m]
    m_robot_rad = 0; // 角度[rad]
}

/**
 * 計測する
 */
void Odometer::measure()
{
    m_wheel_rad_L = m_wheel_L.getCount() * TO_RAD;
    m_wheel_rad_R = m_wheel_R.getCount() * TO_RAD;

    calculate();
}

/**
 * オドメトリを計算する
 */
void Odometer::calculate()
{
    float wheel_dis_l = m_wheel_rad_L * WHEEL_R;
    float wheel_dis_r = m_wheel_rad_R * WHEEL_R;

    m_robot_dis = (wheel_dis_r + wheel_dis_l) / 2;
    m_robot_rad = (wheel_dis_r - wheel_dis_l) / 0.177;

    float dl = m_robot_dis - m_pre_robot_dis;
    m_robot_x = m_pre_robot_x + dl * cos(m_robot_rad);
    m_robot_y = m_pre_robot_y + dl * sin(m_robot_rad);

    m_pre_robot_dis = m_robot_dis;
    m_pre_robot_x = m_robot_x;
    m_pre_robot_y = m_robot_y;
}

/**
 * ロボットのx位置を取得
 * @return ロボットのx位置[m]
 */
float Odometer::getRobotPoseX()
{
    return m_robot_x;
}

/**
 * ロボットのy位置を取得
 * @return ロボットのx位置[m]
 */
float Odometer::getRobotPoseY()
{
    return m_robot_y;
}

/**
 * ロボットの走行距離を取得
 * @return ロボットの走行距離[m]
 */
float Odometer::getRobotDistance()
{
    return m_robot_dis;
}

/**
 * ロボットのyaw角位置を取得
 * @return ロボットのyaw角位置[rad]
 */
float Odometer::getRobotAngle()
{
    return m_robot_rad;
}
