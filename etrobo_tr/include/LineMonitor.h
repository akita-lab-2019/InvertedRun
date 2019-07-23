#ifndef EV3_UNIT_LINEMONITOR_H_
#define EV3_UNIT_LINEMONITOR_H_

#include "ColorSensor.h"

// 定義
class LineMonitor
{
public:
    explicit LineMonitor(const ev3api::ColorSensor &colorSensor);
    float getGap() const;

private:
    const ev3api::ColorSensor &m_color_sensor;

    const float m_target = 40;
};

#endif // EV3_UNIT_LINEMONITOR_H_
