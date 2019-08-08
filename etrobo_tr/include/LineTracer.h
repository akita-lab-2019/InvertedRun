
#ifndef EV3_APP_LINETRACER_H_
#define EV3_APP_LINETRACER_H_

#include <Clock.h>
#include "RobotInfo.h"
#include "LineMonitor.h"
#include "GyroSensor.h"
#include "PID.h"
#include "Motor.h"
#include "Section.h"
#include "Odometer.h"
#include "TailController.h"
#include "InvertedWalker.h"
#include "TailWalker.h"
#include "ParmAdministrator.h"
#include "stdlib.h"

class LineTracer
{
public:
    LineTracer(RobotInfo *robot_info,
               Section *section,
               InvertedWalker *inverted_walker,
               TailWalker *tail_walker,
               PID *pid);

    void update();
    void run();

private:
    RobotInfo *m_robot_info;
    Section *m_section;
    InvertedWalker *m_inverted_walker;
    TailWalker *m_tail_walker;
    PID *m_pid;

    void invertedRun(int forward_v, int turn_v);
    void tailRun(int forward_v, int turn_v);

    bool m_is_initialized;
    float m_forward;      // 前進指令速度[%]
    float m_curvature;    // 曲率[m^-1]
    float m_pid_parm[3];  // PIDパラメータ
    float m_color_target; // カラーセンサの目標値
    bool m_is_inverted;   // 倒立走行の有無
};

#endif // EV3_APP_LINETRACER_H_
