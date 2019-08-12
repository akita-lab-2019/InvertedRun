#include "TailController.h"

/**
 * コンストラクタ
 * @param tail_motor 尻尾モータ
 */
TailController::TailController(ev3api::Motor &tail_motor, PID *pid)
    : m_tail_motor(tail_motor), m_pid(pid)
{
}

/**
 * 初期化する
 */
void TailController::init()
{
    m_tail_motor.reset();
    float tail_pid_gain[3] = {2.5, 0, 0};
    m_pid->init(tail_pid_gain);
}

/**
 * 尻尾モータを制御する
 * @param target_angle 尻尾の目標角度
 * @param max_speed 尻尾モータの最高速度
 */
void TailController::control(int target_angle, int max_speed)
{
    int pwm = m_pid->calculate(target_angle, m_tail_motor.getCount());
    // signed int pwm = (signed int)(target_angle - m_tail_motor.getCount());

    pwm = (pwm > max_speed) ? max_speed : pwm;
    pwm = (pwm < -max_speed) ? -max_speed : pwm;

    if (pwm == 0)
    {
        m_tail_motor.stop();
    }
    else
    {
        m_tail_motor.setPWM(pwm);
    }
}
