#include "RobotInfo.h"

/**
 * コンストラクタ
 */
RobotInfo::RobotInfo(ev3api::Clock &clock,
                     ev3api::ColorSensor &color_sensor,
                     ev3api::GyroSensor &gyro_sensor,
                     ev3api::Motor &wheel_L,
                     ev3api::Motor &wheel_R,
                     ev3api::Motor &tail_motor,
                     LineMonitor *line_monitor,
                     Odometer *odometer)
    : m_clock(clock),
      m_color_sensor(color_sensor),
      m_gyro_sensor(gyro_sensor),
      m_wheel_L(wheel_L),
      m_wheel_R(wheel_R),
      m_tail_motor(tail_motor),
      m_line_monitor(line_monitor),
      m_odometer(odometer)
{
}

/**
 * 初期化する
 */
void RobotInfo::init()
{
}

/**
 * 情報を更新する
 */
void RobotInfo::update()
{
    m_odometer->measure();

    m_runTime = m_clock.now();
    m_brightness = m_color_sensor.getBrightness();
    m_pitch_vel = m_gyro_sensor.getAnglerVelocity();
    // m_pitch_pos = m_gyro_sensor.getAngle();
    m_wheel_pos[L] = m_wheel_L.getCount();
    m_wheel_pos[R] = m_wheel_R.getCount();
    m_tail_pos = m_tail_motor.getCount();
    m_brightness_gap = m_line_monitor->getGap();
    m_robot_pos[X] = m_odometer->getRobotPoseX();
    m_robot_pos[Y] = m_odometer->getRobotPoseY();
    m_robot_pos[YAW] = m_odometer->getRobotAngle();
    m_robot_dis = m_odometer->getRobotDistance();
}

/**
 * 走行開始からの時間を取得する
 * @return 走行開始からの時間[sec]
 */
float RobotInfo::getRunTime()
{
    return m_runTime;
}

/**
 * カラーセンサからの輝度を取得する
 * @return カラーセンサから取得した輝度
 */
int RobotInfo::getBrightness()
{
    return m_brightness;
}

/**
 * pitchの角速度を取得する
 * @return pitchの角速度[deg/rad]
 */
int RobotInfo::getPitchVel()
{
    return m_pitch_vel;
}

/**
 * pitchの角位置を取得する
 * @return pitchの角位置[deg]
 */
int RobotInfo::getPitchPos()
{
    return m_pitch_pos;
}

/**
 * 車輪の角位置を取得する
 * @parm 車輪番号（左:0, 右:1）
 * @return 車輪の角位置（左[deg], 右[deg]）
 */
int RobotInfo::getWheelPos(int wheel)
{
    return m_wheel_pos[wheel];
}

/**
 * 尻尾モータの角位置を取得する
 * @return 尻尾モータの角位置[deg]
 */
float RobotInfo::getTailPos()
{
    return m_tail_pos;
}

/**
 * カラーセンサから取得した輝度と目標値との偏差を取得する
 * @return カラーセンサから取得した輝度と目標値との偏差
 */
float RobotInfo::getBrightnessGap()
{
    return m_brightness_gap;
}

/**
 * ロボットの座標を取得する
 * @parm 軸番号（X:0, Y:1, YAW:2）
 * @return ロボットの座標（x[m], y[m], yaw[deg]）
 */
float RobotInfo::getRobotPos(int axis)
{
    return m_robot_pos[axis];
}

/**
 * 走行距離を取得する
 * @return 走行距離[m]
 */
float RobotInfo::getRobotDis()
{
    return m_robot_dis;
}
