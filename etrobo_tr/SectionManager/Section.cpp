#include "Section.h"

/**
 * コンストラクタ
 */
Section::Section(ParmAdministrator *parm)
    : m_parm(parm)
{
}

/**
 * セクションを更新する
 * @parm セクション番号
 */
void Section::update(int num)
{
    m_distance = m_distance_list[num];
    m_forward = m_parm->forward_v[m_is_curve_list[num]];
    m_curvature = m_curvature_list[num];
    m_pid_parm[0] = m_parm->trace_pid[m_is_curve_list[num]][0];
    m_pid_parm[1] = m_parm->trace_pid[m_is_curve_list[num]][1];
    m_pid_parm[2] = m_parm->trace_pid[m_is_curve_list[num]][2];
    m_color_target = m_parm->color_sensor_targrt;
    m_tail_angle = 0;
    m_is_inverted = 1;
}

/**
 * 区間長さ[m]を取得する
 * @return 区間長さ[m]
 */
float Section::getDistance()
{
    return m_distance;
}

/**
 * 前進指令速度[%]を取得する
 * @return 前進指令速度[%]
 */
float Section::getForward()
{
    return m_forward;
}

/**
 * 曲率[m^-1]を取得する
 * @return 曲率[m^-1]
 */
float Section::getCurvature()
{
    return m_curvature;
}

/**
 * PIDパラメータを取得する（結果は引数に代入される）
 * @parm PIDパラメータの結果
 */
void Section::getPidParm(float pid_parm[3])
{
    for (int i = 0; i < 3; i++)
        pid_parm[i] = m_pid_parm[i];
}

/**
 * カラーセンサの目標値を取得する
 * @return カラーセンサの目標値
 */
float Section::getColorTarget()
{
    return m_color_target;
}

/**
 * 尻尾の角度[deg]を取得する
 * @return 尻尾の角度[deg]
 */
float Section::getTailAngle()
{
    return m_tail_angle;
}

/**
 * 倒立走行の有無を取得する
 * @return 倒立走行の有無
 */
bool Section::isInverted()
{
    return m_is_inverted;
}
