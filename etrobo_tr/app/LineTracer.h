#ifndef EV3_APP_LINETRACER_H_
#define EV3_APP_LINETRACER_H_

#include "LineMonitor.h"
#include "GyroSensor.h"
#include "Motor.h"
#include "BalancerCpp.h"
#include "./../measurement/Measurer.h"
#include "stdlib.h"

class LineTracer
{
public:
    LineTracer(ev3api::GyroSensor &gyro_sensor,
               ev3api::Motor &left_wheel,
               ev3api::Motor &right_wheel,
               Balancer *balancer,
               LineMonitor *line_monitor,
               Measurer *measurer);

    void init();
    void setCommand(int forward, int turn);
    void run();

private:
    ev3api::GyroSensor &m_gyro_sensor;
    ev3api::Motor &m_wheel_L;
    ev3api::Motor &m_wheel_R;
    Balancer *m_balancer;
    LineMonitor *m_line_monitor;
    Measurer *m_measurer;

    int m_forward;
    int m_turn;
    bool m_is_initialized;

    const int LOW = 30;
    const int NORMAL = 50;
    const int HIGH = 70;
};

#endif // EV3_APP_LINETRACER_H_
