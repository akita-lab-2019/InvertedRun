/******************************************************************************
 *  BalancingWalker.h (for LEGO Mindstorms EV3)
 *  Created on: 2015/01/25
 *  Definition of the Class BalancingWalker
 *  Author: Kazuhiro.Kawachi
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#ifndef EV3_UNIT_BALANCINGWALKER_H_
#define EV3_UNIT_BALANCINGWALKER_H_

#include "GyroSensor.h"
#include "Motor.h"
#include "BalancerCpp.h"
#include "./../measurement/Measurer.h"
#include "stdlib.h"

class BalancingWalker
{
public:
    // クラスの定数
    static const int LOW;
    static const int NORMAL;
    static const int HIGH;

    BalancingWalker(const ev3api::GyroSensor &gyroSensor,
                    ev3api::Motor &leftWheel,
                    ev3api::Motor &rightWheel,
                    Balancer *balancer,
                    Measurer *measurer);

    void init();
    void run();
    void setCommand(int forward, int turn);

private:
    // 値を得るだけのものはconstで定義してデータを参照
    // 出力したり，読み取り以外の操作がある場合はポインタ
    const ev3api::GyroSensor &mGyroSensor;
    ev3api::Motor &mLeftWheel;
    ev3api::Motor &mRightWheel;
    Balancer *mBalancer;
    Measurer *m_measurer;
    int mForward;
    int mTurn;
};

#endif // EV3_UNIT_BALANCINGWALKER_H_
