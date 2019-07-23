#include "LineMonitor.h"

LineMonitor::LineMonitor(const ev3api::ColorSensor &colorSensor) : m_color_sensor(colorSensor)
{
}

/**
 * エッジ部分からのずれ（白と黒の中間値をエッジ部分の色と仮定）
 * @return エッジ部分からのずれ
 */
float LineMonitor::getGap() const
{
    return m_target - m_color_sensor.getBrightness();
}
