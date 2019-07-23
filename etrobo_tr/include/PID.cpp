#include "PID.h"

/**
 * コンストラクタ
 * @param tail_motor 尻尾モータ
 */
PID::PID(float Kp, float Ki, float Kd)
    : m_Kp(Kp), m_Ki(Ki), m_Kd(Kd)
{
}

/**
 * 初期化する
 */
void PID::init(float Kp, float Ki, float Kd)
{
    m_Kp = Kp;
    m_Ki = Ki;
    m_Kd = Kd;

    integral = 0;
    diff[0] = 0;
    diff[1] = 0;
}

/**
 * 尻尾モータを制御する
 * 4ms周期で呼び出される前提
 * @param target_val 目標値
 * @param now_val    現在の値
 */
float PID::calculate(float target_val, float now_val)
{
    float dt = 0.004; // 制御周期は4ms

    diff[0] = target_val - now_val;
    integral += (diff[0] + diff[1]) * (dt / 2.0);

    float p, i, d;
    p = m_Kp * diff[0];
    i = m_Ki * integral;
    d = m_Kd * ((diff[0] - diff[1]) / dt);

    // 次回ループのために今回の値を前回の値にする
    diff[1] = diff[0];

    return p + i + d;
}