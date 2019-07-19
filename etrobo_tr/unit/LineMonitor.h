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

    const float LIGHT_WHITE = 55; // 白色の光センサ値
    const float LIGHT_BLACK = 0;  // 黒色の光センサ値
};

#endif // EV3_UNIT_LINEMONITOR_H_
