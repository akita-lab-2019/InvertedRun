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
                     Section *section,
                     PID *trace_pid)
    : m_clock(clock),
      m_color_sensor(color_sensor),
      m_gyro_sensor(gyro_sensor),
      m_sonar_sensor(sonar_sensor),
      m_wheel_L(wheel_L),
      m_wheel_R(wheel_R),
      m_tail_motor(tail_motor),
      m_line_monitor(line_monitor),
      m_odometer(odometer),
      m_section(section),
      m_trace_pid(trace_pid)
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
    m_battery = 0.9 * m_pre_battery + 0.1 * ev3_battery_voltage_mV() / 1000.0;
    m_brightness = m_color_sensor.getBrightness();
    m_pitch_vel = m_gyro_sensor.getAnglerVelocity();
    // m_pitch_pos = m_gyro_sensor.getAngle();
    m_wheel_pos[L] = m_wheel_L.getCount();
    m_wheel_pos[R] = m_wheel_R.getCount();
    m_tail_pos = m_tail_motor.getCount();
    m_robot_pos[X] = m_odometer->getRobotPose(Odometer::X);
    m_robot_pos[Y] = m_odometer->getRobotPose(Odometer::Y);
    m_robot_pos[YAW] = m_odometer->getRobotPose(Odometer::YAW);
    m_robot_dis = m_odometer->getRobotDistance();
    m_sonar_distance = (float)m_sonar_sensor.getDistance() / 100.0;

    m_trace_pid_team_val[P] = m_trace_pid->getTeamValue(PID::P);
    m_trace_pid_team_val[I] = m_trace_pid->getTeamValue(PID::I);
    m_trace_pid_team_val[D] = m_trace_pid->getTeamValue(PID::D);

    m_pre_battery = m_battery;
}

void RobotInfo::setCourse(int course)
{
    m_course = course;
}

int RobotInfo::getCourse()
{
    return m_course;
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
int RobotInfo::getBrightnessGap(int target)
{
    return (int)m_line_monitor->getGap(target);
    ;
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

float RobotInfo::getSonarDistance()
{
    return m_sonar_distance;
}

void RobotInfo::setForward(int forward)
{
    m_forward = forward;
}

void RobotInfo::setTurn(int turn)
{
    m_turn = turn;
}

void RobotInfo::setPWM(int pwm_l, int pwm_r)
{
    m_pwm[L] = pwm_l;
    m_pwm[R] = pwm_r;
}

int RobotInfo::getForward()
{
    return m_forward;
}

int RobotInfo::getTurn()
{
    return m_turn;
}

int RobotInfo::getPWM(int wheel)
{
    return m_pwm[wheel];
}

void RobotInfo::setGyroOffset(float offset)
{
    m_gyro_offset = offset;
}

float RobotInfo::getGyroOffset()
{
    return m_gyro_offset;
}

float RobotInfo::getTracePidTeamValue(int team)
{
    return m_trace_pid_team_val[team];
}