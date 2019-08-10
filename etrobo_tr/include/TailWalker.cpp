
#include "TailWalker.h"

/**
 * コンストラクタ
 * @param leftWheel  左モータ
 * @param rightWheel 右モータ
 */
TailWalker::TailWalker(ev3api::Motor &wheel_L,
                       ev3api::Motor &wheel_R)
    : m_wheel_L(wheel_L),
      m_wheel_R(wheel_R)
{
}

/**
 * バランス走行する
 */
void TailWalker::run()
{
    // 左右モータに回転を指示する
    int wheel_v_L = m_forward + K * m_turn;
    int wheel_v_R = m_forward - K * m_turn;

    if (wheel_v_L > 100)
    {
        wheel_v_L = 100;
    }
    else if (wheel_v_L < -100)
    {
        wheel_v_L = -100;
    }

    if (wheel_v_R > 100)
    {
        wheel_v_R = 100;
    }
    else if (wheel_v_R < -100)
    {
        wheel_v_R = -100;
    }

    m_wheel_L.setPWM(wheel_v_L);
    m_wheel_R.setPWM(wheel_v_R);

    // ブレーキは解除
    m_wheel_L.setBrake(false);
    m_wheel_R.setBrake(false);
}

/**
 * バランス走行に必要なものをリセットする
 */
void TailWalker::init()
{
}

/**
 * PWM値を設定する
 * @param forward 前進値
 * @param turn    旋回値
 */
void TailWalker::setCommand(int forward, int turn)
{
    m_forward = forward;
    if (m_forward > 100)
    {
        m_forward = 100;
    }
    else if (m_forward < -100)
    {
        m_forward = -100;
    }

    m_turn = turn;
    if (m_turn > 100)
    {
        m_turn = 100;
    }
    else if (m_turn < -100)
    {
        m_turn = -100;
    }
}
