#ifndef EV3_UNIT_LINEMONITOR_H_
#define EV3_UNIT_LINEMONITOR_H_

#include "ColorSensor.h"
#include "ParmAdministrator.h"

// 定義
class LineMonitor
{
public:
    explicit LineMonitor(ev3api::ColorSensor &colorSensor, ParmAdministrator *parm);
    float getGap();

private:
    const ev3api::ColorSensor &m_color_sensor;

    float m_target = 30;
};

#endif // EV3_UNIT_LINEMONITOR_H_
