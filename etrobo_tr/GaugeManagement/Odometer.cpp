#include "Odometer.h"

extern ev3api::Motor wheel_L;
extern ev3api::Motor wheel_R;

const float TO_RAD = 3.141592 / 180;
const float TO_DEG = 180 / 3.141592;
const float WHEEL_RADIUS = 0.049;
const float WHEEL_DIST = 0.177;

float m_wheel_deg[2] = {0};      // ホイール角度[deg]
float m_robot_pose[3] = {0};     // ロボット位置[m],[deg]
float m_pre_robot_pose[3] = {0}; // 前回のロボット位置[m],[deg]
float m_robot_dis = 0;           // ロボット走行距離[m]
float m_pre_robot_dis = 0;       // 前回のロボット走行距離[m]

/**
 * 初期化する
 */
void initOdometer()
{
    m_robot_pose[X] = 0;
    m_robot_pose[Y] = 0;
    m_robot_pose[YAW] = 0;
    m_robot_dis = 0;
}

/**
 * 計測する
 */
void processOdometer()
{
    // ホイールの回転角度を算出する
    m_wheel_deg[L] = wheel_L.getCount();
    m_wheel_deg[R] = wheel_R.getCount();

    // ホイールの累計走行距離を計算
    float wheel_dis[2];
    wheel_dis[L] = m_wheel_deg[L] * TO_RAD * WHEEL_RADIUS;
    wheel_dis[R] = m_wheel_deg[R] * TO_RAD * WHEEL_RADIUS;

    // ロボットの累計走行距離を計算
    m_robot_dis = (wheel_dis[R] + wheel_dis[L]) / 2;
    m_robot_pose[YAW] = (wheel_dis[R] - wheel_dis[L]) / WHEEL_DIST * TO_DEG;

    // ロボットの座標を計算
    float dl = m_robot_dis - m_pre_robot_dis;
    m_robot_pose[X] = m_pre_robot_pose[X] + dl * cos(m_robot_pose[YAW] * TO_RAD);
    m_robot_pose[Y] = m_pre_robot_pose[Y] + dl * sin(m_robot_pose[YAW] * TO_RAD);

    // 次のループにデータを渡す
    m_pre_robot_dis = m_robot_dis;
    for (int i = 0; i < 3; i++)
        m_pre_robot_pose[i] = m_robot_pose[i];
}

/**
 * ロボットの位置を取得する
 * @parm 軸番号（X:0, Y:1, YAW:2）
 * @return ロボットの位置（X:[m], Y:[m], YAW[deg]）
 */
float getRobotPose(int axis)
{
    return m_robot_pose[axis];
}

/**
 * ロボットの走行距離を取得
 * @return ロボットの走行距離[m]
 */
float getRobotDistance()
{
    return m_robot_dis;
}

/**
 * ホイール角度を取得
 * @parm ホイール番号（L:0, R:1）
 * @return ホイール角度[deg]
 */
float getWheelDeg(int wheel)
{
    return m_wheel_deg[wheel];
}