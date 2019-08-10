#include "RobotInfo.h"

/**
 * コンストラクタ
 */
RobotInfo::RobotInfo(ev3api::Clock &clock,
                     ev3api::ColorSensor &color_sensor,
                     ev3api::GyroSensor &gyro_sensor,
                     ev3api::SonarSensor &sonar_sensor,
                     ev3api::Motor &wheel_L,
                     ev3api::Motor &wheel_R,
                     ev3api::Motor &tail_motor,
                     LineMonitor *line_monitor,
                     Odometer *odometer,
                     Section *section)
    : m_clock(clock),
      m_color_sensor(color_sensor),
      m_gyro_sensor(gyro_sensor),
      m_sonar_sensor(sonar_sensor),
      m_wheel_L(wheel_L),
      m_wheel_R(wheel_R),
      m_tail_motor(tail_motor),
      m_line_monitor(line_monitor),
      m_odometer(odometer),
      m_section(section)
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
float pre_time;
void RobotInfo::update()
{
    m_odometer->measure();

    m_section_num = m_section->getSectionNum();

    m_runTime = (float)m_clock.now() / 1000.0;
    m_battery = ev3_battery_voltage_mV() / 1000.0;
    m_brightness = m_color_sensor.getBrightness();
    m_pitch_vel = m_gyro_sensor.getAnglerVelocity();
    // m_pitch_pos = m_gyro_sensor.getAngle();
    m_wheel_pos[L] = m_wheel_L.getCount();
    m_wheel_pos[R] = m_wheel_R.getCount();
    m_wheel_vel[L] = m_odometer->getWheelVelocity(Odometer::L);
    m_wheel_vel[R] = m_odometer->getWheelVelocity(Odometer::R);
    m_tail_pos = m_tail_motor.getCount();
    m_brightness_gap = m_line_monitor->getGap();
    m_robot_pos[X] = m_odometer->getRobotPose(Odometer::X);
    m_robot_pos[Y] = m_odometer->getRobotPose(Odometer::Y);
    m_robot_pos[YAW] = m_odometer->getRobotPose(Odometer::YAW);
    m_robot_dis = m_odometer->getRobotDistance();

    m_robot_liner_velocity = m_odometer->getRobotLinerVelocity();
    // m_robot_angular_velocity = m_odometer->getRobotAngularVelocity();
    m_sonar_distance = m_sonar_sensor.getDistance();
}

int RobotInfo::getCourse()
{
    return 0;
}

int RobotInfo::getSectionNum()
{
    return m_section_num;
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
 * バッテリー電圧を取得する
 * @return バッテリー電圧[V]
 */
float RobotInfo::getBatteryVoltage()
{
    return m_battery;
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
 * ホイール速度を取得
 * @parm ホイール番号（L:0, R:1）
 * @return ホイール角速度[deg/s]
 */
int RobotInfo::getWheelVelocity(int wheel)
{
    return m_wheel_vel[wheel];
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
int RobotInfo::getBrightnessGap()
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

/**
 * ロボットの並進速度を取得
 * @return ロボットの並進速度[m/s]
 */
int RobotInfo::getRobotLinerVelocity()
{
    return m_robot_liner_velocity;
}

/**
 * ロボットの角速度を取得
 * @return ロボットの角速度[deg/s]
 */
int RobotInfo::getRobotAngularVelocity()
{
    return m_robot_angular_velocity;
}

float RobotInfo::getSonarDistance()
{
    return m_sonar_distance;
}
