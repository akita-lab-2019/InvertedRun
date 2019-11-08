/**
 *******************************************************************************
 ** 走行距離等を計算し，データを提供する
 *******************************************************************************
 **/

#ifndef EV3_ODOMETER_H_
#define EV3_ODOMETER_H_

#include "ev3api.h"
#include "Motor.h"
#include "math.h"

void initOdometer();
void processOdometer();

float getRobotPose(int axis); // ロボットの位置[m, deg]
float getRobotDistance();     // ロボットの累計走行距離[m]
float getWheelDeg(int wheel); // ホイールの角位置[deg]

enum
{
    L,
    R
};

enum
{
    X,
    Y,
    YAW
};

#endif
