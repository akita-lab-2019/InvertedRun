#include "LineMonitor.h"

extern ev3api::ColorSensor color_sensor;

float m_brightness_target = 35;

void initLineMonitor()
{
}

void processLineMonitor()
{
}

void setBrightnessTarget(float target)
{
    m_brightness_target = target;
}

/**
 * 輝度偏差を取得する
 * @return 輝度偏差
 */
float getBrightnessGap()
{
    return m_brightness_target - color_sensor.getBrightness();
}
