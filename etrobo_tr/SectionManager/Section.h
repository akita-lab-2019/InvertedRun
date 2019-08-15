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
    int getSectionNum();
    float getDistance();
    float getForward();
    float getCurvature();
    void getPidParm(float pid_parm[3]);
    float getColorTarget();
    float getTailAngle();
    bool isInverted();

private:
    ParmAdministrator *m_parm;

    int m_section_num = 0; // 区間番号
    float m_distance;      // 区間長さ[m]
    float m_forward;       // 前進指令速度[%]
    float m_curvature;     // 曲率[m^-1]
    float m_pid_parm[3];   // PIDパラメータ
    float m_color_target;  // カラーセンサの目標値
    float m_tail_angle;    // 尻尾の角度[deg]
    bool m_is_inverted;    // 倒立走行の有無

    int m_is_curve_list[18] = {
        0, 2, 0, 1, 4, 4, 3, 0, 1, 0, 2, 0, 3, 3, 0, 0, 0, 0};

    float m_curvature_list[18] = {
        0, 14.5, 0, 13.0, 18.0, 22.0, -16.5, -14.5, 0, -10, 0, -12.5, 0, -14.5, 0, 14.5, 0, 0};

    float m_distance_list[18] = {
        0.60, 1.25, 1.70, 2.45, 3.30, 4.00, 4.60, 4.70, 5.55, 6.45, 7.00, 7.10, 7.45, 8.85, 9.20, 9.50, 9.80, 10.00};

    // float m_distance_list[18] = {
    //     0.68, 1.3, 1.80, 2.57, 2.65, 3.4, 4.09, 4.69, 4.87, 5.64, 6.51, 7.17, 7.32, 7.5, 7.61, 9.03, 9.4, 10.07};
};

#endif