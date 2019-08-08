#include "SectionTracer.h"

/**
 * コンストラクタ
 * @param lineMonitor    ライン判定
 * @param invertedWalker 倒立走行
 */
SectionTracer::SectionTracer(RobotInfo *robot_info,
                             Section *section,
                             LineTracer *line_tracer,
                             ParmAdministrator *parm)
    : m_robot_info(robot_info),
      m_section(section),
      m_line_tracer(line_tracer),
      m_parm(parm),
      m_is_initialized(false)
{
}

/**
 * ライントレースする
 */
bool b_flag = 0;
void SectionTracer::run()
{
    if (m_is_initialized == false)
    {
        m_section->update(0);
        m_line_tracer->update();
        m_is_initialized = true;
    }

    if (m_section_num != 17)
    {
        if (m_robot_info->getRobotDis() > m_section->getDistance())
        {
            m_section_num++;
            m_section->update(m_section_num);
            m_line_tracer->update();
            ev3_speaker_play_tone(NOTE_C4 + m_section_num * 100, 100);
        }
    }

    if (m_section_num % 2)
    {
        ev3_led_set_color(LED_ORANGE);
    }
    else
    {
        ev3_led_set_color(LED_GREEN);
    }

    m_line_tracer->run();
}
