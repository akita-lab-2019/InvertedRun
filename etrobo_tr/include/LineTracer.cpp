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

    int is_curve[11] = {
        0,
        1,
        0,
        1,
        1,
        0,
        1,
        0,
        1,
        1,
        0,
        0};

    float segment_dis[11] = {
        0.68,
        1.30,
        1.81,
        3.40,
        4.69,
        4.87,
        5.64,
        6.51,
        7.65,
        9.03,
        9.60};

    if (scenario_num == 11)
    {
        sectionRun(0);
    }
    else
    {
        sectionRun(m_parm->forward_v[is_curve[scenario_num]]);
        if (m_odometer->getRobotDistance() > segment_dis[scenario_num])
        {
            ev3_speaker_play_tone(NOTE_C4 + scenario_num * 100, 500);
            scenario_num++;
            m_pid->init(m_parm->trace_pid[is_curve[scenario_num]][0],
                        m_parm->trace_pid[is_curve[scenario_num]][1],
                        m_parm->trace_pid[is_curve[scenario_num]][2]);
        }
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
    m_inverted_walker->setCommand(forward_v, direction);

    // 倒立走行を行う
    m_inverted_walker->run();
}
