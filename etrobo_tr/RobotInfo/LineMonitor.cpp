#include "LineMonitor.h"

LineMonitor::LineMonitor(ev3api::ColorSensor &colorSensor, ParmAdministrator *parm) : m_color_sensor(colorSensor), m_target(parm->color_sensor_targrt[1])
{
    m_target = parm->color_sensor_targrt[1];
}

/**
 * エッジ部分からのずれ（白と黒の中間値をエッジ部分の色と仮定）
 * @return エッジ部分からのずれ
 */
float LineMonitor::getGap(float target)
{
    float sensor_val = target - (0.9 * m_pre_sensor_val + 0.1 * m_color_sensor.getBrightness());
    m_pre_sensor_val = m_color_sensor.getBrightness();

    return sensor_val;
}
