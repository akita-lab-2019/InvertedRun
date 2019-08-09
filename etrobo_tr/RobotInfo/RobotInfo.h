/**
 *******************************************************************************
 ** 走行体の情報を取集，管理，提供する
 *******************************************************************************
 **/

#ifndef EV3_ROBOT_INFO_H_
#define EV3_ROBOT_INFO_H_

#include <Clock.h>
#include <ColorSensor.h>
#include <GyroSensor.h>
#include <Motor.h>

#include "LineMonitor.h"
#include "Odometer.h"
#include "Section.h"

class RobotInfo
{
public:
    RobotInfo(ev3api::Clock &clock,
              ev3api::ColorSensor &color_sensor,
              ev3api::GyroSensor &gyro_sensor,
              ev3api::Motor &wheel_L,
              ev3api::Motor &wheel_R,
              ev3api::Motor &tail_motor,
              LineMonitor *line_monitor,
              Odometer *odometer,
              Section *section);

    void init();
    void update();

    int getCourse();
    int getSectionNum();
    float getBatteryVoltage();
    float getRunTime();
    int getBrightness();
    int getPitchVel();
    int getPitchPos();
    int getWheelPos(int wheel);
    float getWheelVelocity(int wheel);
    float getTailPos();
    float getBrightnessGap();
    float getRobotPos(int axis);
    float getRobotDis();
    float getRobotLinerVelocity();
    float getRobotAngularVelocity();
    float getSonarDistance();

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
    ev3api::Clock &m_clock;
    ev3api::ColorSensor &m_color_sensor;
    ev3api::GyroSensor &m_gyro_sensor;
    ev3api::Motor &m_wheel_L;
    ev3api::Motor &m_wheel_R;
    ev3api::Motor &m_tail_motor;
    LineMonitor *m_line_monitor;
    Odometer *m_odometer;
    Section *m_section;

    int m_section_num = 0;      // セクション番号
    float m_runTime = 0;        // 走行開始からの時間[sec]
    float m_battery = 0;        // バッテリー電圧[V]
    int m_brightness = 0;       // カラーセンサから取得した輝度
    int m_pitch_vel = 0;        // pitchの角速度[deg/rad]
    int m_pitch_pos = 0;        // pitchの角位置[deg]
    int m_wheel_pos[2] = {0};   // 車輪の角位置（左[deg], 右[deg]）
    float m_wheel_vel[2] = {0}; // 車輪の角速度（左[deg/s], 右[deg/s]）
    float m_tail_pos = 0;       // 尻尾モータの角位置[deg]
    float m_brightness_gap = 0; // カラーセンサから取得した輝度と目標値との偏差
    float m_robot_pos[3] = {0}; // ロボットの座標（x[m], y[m], yaw[deg]）
    float m_robot_dis = 0;      // 走行距離[m]
    float m_robot_liner_velocity;
    float m_robot_angular_velocity;
};

#endif // EV3_ROBOT_INFO_H_
