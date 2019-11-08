#include "SectionTracer.h"

/**
 * コンストラクタ
 * @param lineMonitor    ライン判定
 * @param invertedWalker 倒立走行
 */
SectionTracer::SectionTracer(Section *section, LineTracer *line_tracer)
    : m_section(section),
      m_line_tracer(line_tracer),
      m_is_initialized(false)
{
}

/**
 * ライントレースする
 */
bool b_flag = 0;
bool SectionTracer::run(int start_section_num)
{
    if (m_is_initialized == false)
    {
        int m_section_num = start_section_num;
        // float distance_list[16] = {0, 0.60, 1.25, 1.70, 2.45, 3.30, 4.00, 4.60, 4.70, 5.55, 6.45, 7.00, 7.05, 7.60, 8.95, 9.25};
        // m_robot_info->setOdomOffset(-distance_list[start_section_num]);
        m_section->update(m_section_num);
        m_line_tracer->update();
        m_line_tracer->setIsInverted(1);
        m_is_initialized = true;
    }

    if (getRobotDistance() > m_section->getDistance())
    {
        m_section_num++;
        if (m_section_num < 15)
        {
            m_section->update(m_section_num);
            m_line_tracer->update();
            m_line_tracer->setIsInverted(1);
            ev3_speaker_play_tone((int)(261.63 * pow(M_E, m_section_num * 0.1)), 100);
        }
        else
        {
            return true;
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

    return false;
}
