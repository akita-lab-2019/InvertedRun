#include "Section.h"

/**
 * コンストラクタ
 */
Section::Section()
{
    update(0);
}

/**
 * セクションを更新する
 * @parm セクション番号
 */
void Section::update(int num)
{
    // それぞれの値を更新する
    m_section_num = num;
    m_run_method = m_run_method_list[num];
    m_distance = m_distance_list[num];
    m_forward = m_forward_list[num];
    m_curvature = m_curvature_list[num];
    m_pid_parm[0] = m_trace_pid_list[num][0];
    m_pid_parm[1] = m_trace_pid_list[num][1];
    m_pid_parm[2] = m_trace_pid_list[num][2];
}

/**
 * 現在の区間総数を取得する
 * @return 区間総数
 */
int Section::getSectionSumnum()
{
    return m_section_sumnum;
}

/**
 * 現在の区間番号を取得する
 * @return 区間番号
 */
int Section::getSectionNum()
{
    return m_section_num;
}

/**
 * 現在の区間総数を取得する
 * @return 区間総数
 */
int Section::getRunMethod()
{
    return m_run_method;
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
 * 曲率旋回量を取得する
 * @return 曲率旋回量
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
 * 輝度目標値を取得する
 * @parm 輝度目標値
 */
float Section::getColorTarget()
{
    return m_color_target;
}

/**
 * 区間開始距離[m]を取得する
 * @return 区間長さ[m]
 */
float Section::getStartDistance()
{
    if (m_section_num == 0)
        return 0;

    return m_distance_list[m_section_num - 1];
}

/**
 * 区間終了距離[m]を取得する
 * @return 区間長さ[m]
 */
float Section::getEndDistance()
{
    return m_distance_list[m_section_num];
}