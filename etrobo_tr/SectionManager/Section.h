/**
 *******************************************************************************
 ** 区間データを提供する
 *******************************************************************************
 **/

#ifndef EV3_SECTION_H_
#define EV3_SECTION_H_

#include <Clock.h>
#include <ColorSensor.h>
#include <GyroSensor.h>
#include <Motor.h>

#include "ParmAdministrator.h"
#include "Odometer.h"

class Section
{
public:
    Section(ParmAdministrator *parm);
    void update(int num);
    float getDistance();
    float getForward();
    float getCurvature();
    void getPidParm(float pid_parm[3]);
    float getColorTarget();
    float getTailAngle();
    bool isInverted();

private:
    ParmAdministrator *m_parm;

    float m_distance;     // 区間長さ[m]
    float m_forward;      // 前進指令速度[%]
    float m_curvature;    // 曲率[m^-1]
    float m_pid_parm[3];  // PIDパラメータ
    float m_color_target; // カラーセンサの目標値
    float m_tail_angle;   // 尻尾の角度[deg]
    bool m_is_inverted;   // 倒立走行の有無

    int m_is_curve_list[12] = {
        0,
        1,
        0,
        1,
        1,
        0,
        1,
        0,
        1,
        1,
        0,
        0};

    float m_distance_list[12] = {
        0.68,
        1.30,
        1.81,
        3.40,
        4.69,
        4.87,
        5.64,
        6.51,
        7.65,
        9.03,
        9.60,
        10.0};
};

#endif