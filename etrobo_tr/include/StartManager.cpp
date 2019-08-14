#include "StartManager.h"

/**
 * コンストラクタ
 * @param leftWheel  左モータ
 * @param rightWheel 右モータ
 */
StartManager::StartManager()
{
}

/**
 * バランス走行に必要なものをリセットする
 */
void StartManager::init()
{
}

/**
 * バランス走行する
 */
void StartManager::update()
{
    m_left_pwm = m_forward + K * m_turn;
    m_right_pwm = m_forward - K * m_turn;

    if (m_left_pwm > 100)
    {
        m_left_pwm = 100;
    }
    else if (m_left_pwm < -100)
    {
        m_left_pwm = -100;
    }

    if (m_right_pwm > 100)
    {
        m_right_pwm = 100;
    }
    else if (m_right_pwm < -100)
    {
        m_right_pwm = -100;
    }
}
