#include "LineTracer.h"

/**
 * コンストラクタ
 * @param lineMonitor    ライン判定
 * @param invertedWalker 倒立走行
 */
LineTracer::LineTracer(RobotInfo *robot_info,
                       Section *section,
                       InvertedWalker *inverted_walker,
                       TailWalker *tail_walker,
                       PID *pid)
    : m_robot_info(robot_info),
      m_section(section),
      m_inverted_walker(inverted_walker),
      m_tail_walker(tail_walker),
      m_pid(pid),
      m_is_initialized(false)
{
}

/**
 * 初期化する
 */
void LineTracer::update()
{
    // m_is_inverted = 1;
    // m_forward = 20;
    // m_curvature = 0;
    // m_pid_parm[0] = 0.6;
    // m_pid_parm[1] = 0.045;
    // m_pid_parm[2] = 0.065;
    // m_color_target = 25;
    m_is_inverted = m_section->isInverted();
    m_forward = m_section->getForward();
    m_curvature = m_section->getCurvature();
    m_section->getPidParm(m_pid_parm);
    m_color_target = m_section->getColorTarget();
    m_pid->init(m_pid_parm[0], m_pid_parm[1], m_pid_parm[2]);
}

/**
 * ライントレースする
 */
void LineTracer::run()
{
    if (m_is_initialized == false)
    {
        m_inverted_walker->init();
        m_pid->init(m_pid_parm[0], m_pid_parm[1], m_pid_parm[2]);
        m_is_initialized = true;
    }

    float direction = m_pid->calculate(0, m_robot_info->getBrightnessGap());
    // direction += -1.2 * m_curvature;
    if (m_is_inverted)
    {
        invertedRun(m_forward, direction);
    }
    else
    {
        tailRun(m_forward, direction);
    }
}

/**
 * ライントレースする
 */
void LineTracer::invertedRun(int forward_v, int turn_v)
{
    // 速度指令をセット
    m_inverted_walker->setCommand(forward_v, turn_v);

    // 倒立走行を行う
    m_inverted_walker->run();
}

/**
 * ライントレースする
 */
void LineTracer::tailRun(int forward_v, int turn_v)
{
    // 速度指令をセット
    m_tail_walker->setCommand(forward_v, turn_v);

    // 尻尾走行を行う
    m_tail_walker->run();
}
