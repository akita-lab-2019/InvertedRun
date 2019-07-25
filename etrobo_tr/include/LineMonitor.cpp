#include "LineMonitor.h"

LineMonitor::LineMonitor(ev3api::ColorSensor &colorSensor, ParmAdministrator *parm) : m_color_sensor(colorSensor), m_target(parm->color_sensor_targrt)
{
    m_target = parm->color_sensor_targrt;
}

/**
 * エッジ部分からのずれ（白と黒の中間値をエッジ部分の色と仮定）
 * @return エッジ部分からのずれ
 */
float LineMonitor::getGap() const
{
    return m_target - m_color_sensor.getBrightness();
}
