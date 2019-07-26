#include "LineTracer.h"

/**
 * コンストラクタ
 * @param lineMonitor    ライン判定
 * @param invertedWalker 倒立走行
 */
LineTracer::LineTracer(LineMonitor *line_monitor,
                       InvertedWalker *inverted_walker,
                       PID *pid,
                       ParmAdministrator *parm,
                       Odometer *odometer)
    : m_line_monitor(line_monitor),
      m_inverted_walker(inverted_walker),
      m_pid(pid),
      m_parm(parm),
      m_odometer(odometer),
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

    if (scenario_num % 2)
    {
        ev3_led_set_color(LED_ORANGE);
    }
    else
    {
        ev3_led_set_color(LED_GREEN);
    }

    switch (scenario_num)
    {
    case 0:
        sectionRun(80);
        if (m_odometer->getRobotDistance() > 0.68)
        {
            scenario_num++;
            m_pid->init(m_parm->trace_pid[1][0], m_parm->trace_pid[1][1], m_parm->trace_pid[1][2]);
        }
        break;

    case 1:
        sectionRun(20);
        if (m_odometer->getRobotDistance() > 1.30)
        {
            scenario_num++;
            m_pid->init(m_parm->trace_pid[0][0], m_parm->trace_pid[0][1], m_parm->trace_pid[0][2]);
        }
        break;

    case 2:
        sectionRun(80);
        if (m_odometer->getRobotDistance() > 1.81)
        {
            scenario_num++;
            m_pid->init(m_parm->trace_pid[1][0], m_parm->trace_pid[1][1], m_parm->trace_pid[1][2]);
        }
        break;

    case 3:
        sectionRun(20);
        if (m_odometer->getRobotDistance() > 3.40)
        {
            scenario_num++;
            m_pid->init(m_parm->trace_pid[1][0], m_parm->trace_pid[1][1], m_parm->trace_pid[1][2]);
        }
        break;

    case 4:
        sectionRun(20);
        if (m_odometer->getRobotDistance() > 4.69)
        {
            scenario_num++;
            m_pid->init(m_parm->trace_pid[0][0], m_parm->trace_pid[0][1], m_parm->trace_pid[0][2]);
        }
        break;

    case 5:
        sectionRun(80);
        if (m_odometer->getRobotDistance() > 4.87)
        {
            scenario_num++;
            m_pid->init(m_parm->trace_pid[1][0], m_parm->trace_pid[1][1], m_parm->trace_pid[1][2]);
        }
        break;

    case 6:
        sectionRun(20);
        if (m_odometer->getRobotDistance() > 5.64)
        {
            scenario_num++;
            m_pid->init(m_parm->trace_pid[0][0], m_parm->trace_pid[0][1], m_parm->trace_pid[0][2]);
        }
        break;

    case 7:
        sectionRun(80);
        if (m_odometer->getRobotDistance() > 6.51)
        {
            scenario_num++;
            m_pid->init(m_parm->trace_pid[1][0], m_parm->trace_pid[1][1], m_parm->trace_pid[1][2]);
        }
        break;

    case 8:
        sectionRun(20);
        if (m_odometer->getRobotDistance() > 7.65)
        {
            scenario_num++;
            m_pid->init(m_parm->trace_pid[1][0], m_parm->trace_pid[1][1], m_parm->trace_pid[1][2]);
        }
        break;

    case 9:
        sectionRun(20);
        if (m_odometer->getRobotDistance() > 9.03)
        {
            scenario_num++;
            m_pid->init(m_parm->trace_pid[0][0], m_parm->trace_pid[0][1], m_parm->trace_pid[0][2]);
        }
        break;

    default:
        sectionRun(80);
        break;
    }
}

/**
 * ライントレースする
 */
void LineTracer::sectionRun(int forward_v)
{
    // 旋回量を計算する
    float direction = m_pid->calculate(0, m_line_monitor->getGap());

    // 速度指令をセット
    m_inverted_walker->setCommand(m_parm->forward_v, direction);

    // 倒立走行を行う
    m_inverted_walker->run();
}
