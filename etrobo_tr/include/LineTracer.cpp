#include "LineTracer.h"

/**
 * コンストラクタ
 * @param lineMonitor    ライン判定
 * @param invertedWalker 倒立走行
 */
LineTracer::LineTracer(LineMonitor *line_monitor,
                       InvertedWalker *inverted_walker,
                       PID *pid)
    : m_line_monitor(line_monitor),
      m_inverted_walker(inverted_walker),
      m_pid(pid),
      m_is_initialized(false)
{
}

/**
 * ライントレースする
 */
void LineTracer::run()
{
    if (m_is_initialized == false)
    {
        m_inverted_walker->init();
        m_is_initialized = true;
    }

    // 旋回量を計算する
    float direction = m_pid->calculate(0, m_line_monitor->getGap());
    // float direction = 0.6 * m_line_monitor->getGap();

    // 速度指令をセット
    m_inverted_walker->setCommand(60, direction);
    // m_inverted_walker->setCommand(0, 0);

    // 倒立走行を行う
    m_inverted_walker->run();
}
