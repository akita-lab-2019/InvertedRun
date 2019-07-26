#include "LineTracer.h"

/**
 * コンストラクタ
 * @param lineMonitor    ライン判定
 * @param invertedWalker 倒立走行
 */
LineTracer::LineTracer(LineMonitor *line_monitor,
                       InvertedWalker *inverted_walker,
                       PID *pid,
                       ParmAdministrator *parm)
    : m_line_monitor(line_monitor),
      m_inverted_walker(inverted_walker),
      m_pid(pid),
      m_parm(parm),
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

    // 速度指令をセット
    m_inverted_walker->setCommand(80, direction);

    // 倒立走行を行う
    m_inverted_walker->run();
}
