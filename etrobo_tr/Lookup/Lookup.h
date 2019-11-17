/**
 *******************************************************************************
 ** 走行体の情報を取集，管理，提供する
 *******************************************************************************
 **/

#ifndef EV3_Lookup_H_
#define EV3_Lookup_H_

#include <Clock.h>
#include <GyroSensor.h>
#include <SonarSensor.h>
#include "GuageManager.h"
#include "LineTracer.h"
#include "TailController.h"

class Lookup
{
public:
    Lookup(LineTracer *line_tracer,
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

    // 現在のシーソーシーケンス番号
    int m_sequence_num = 0;

    // 着地した時の走行距離
    float m_landing_dis = 0;

    // 走行のPIDパラメータ
    float m_run_pid_param[2][3] = {{0.400, 0.000, 0.031},
                                   {0.5, 0.0, 0.0}};
};

#endif
