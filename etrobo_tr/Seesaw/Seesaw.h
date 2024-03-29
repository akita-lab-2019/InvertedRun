/**
 *******************************************************************************
 ** 走行体の情報を取集，管理，提供する
 *******************************************************************************
 **/

#ifndef EV3_Seesaw_H_
#define EV3_Seesaw_H_

#include <Clock.h>
#include <GyroSensor.h>
#include "GuageManager.h"
#include "LineTracer.h"
#include "TailController.h"

class Seesaw
{
public:
    Seesaw(LineTracer *line_tracer,
           TailController *tail);

    void init();
    void run();

private:
    LineTracer *m_line_tracer;
    TailController *m_tail;

    void update();
    void lineRun(bool is_inverted, int forward, int pid_index, int target);
    void downBody();
    void upBody();
    void landing();
    void setTailSpeed(int speed);
    void setTailAngle(int angle);
    void setAllWheelSpeed(int speed);
    void resetAllWheel();

    // 現在のシーソーシーケンス番号
    int m_sequence_num = 0;

    // 着地した時の走行距離
    float m_seesaw_start_dis = 0;

    // 走行のPIDパラメータ
    float m_run_pid_param[7][3] = {{0.180, 0.000, 0.000},
                                   {0.100, 0.000, 0.000},
                                   {0.000, 0.000, 0.000},
                                   {0.100, 0.000, 0.000},
                                   {0.100, 0.000, 0.000},
                                   {0.100, 0.000, 0.000},
                                   {0.100, 0.000, 0.000}};
};

#endif
