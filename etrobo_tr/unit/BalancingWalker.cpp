/******************************************************************************
 *  BalancingWalker.cpp (for LEGO Mindstorms EV3)
 *  Created on: 2015/01/25
 *  Implementation of the Class BalancingWalker
 *  Author: Kazuhiro.Kawachi
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#include "BalancingWalker.h"

// 定数宣言
const int BalancingWalker::LOW = 30;    // 低速
const int BalancingWalker::NORMAL = 50; // 通常
const int BalancingWalker::HIGH = 70;   // 高速

/**
 * コンストラクタ
 * @param gyroSensor ジャイロセンサ
 * @param leftWheel  左モータ
 * @param rightWheel 右モータ
 * @param balancer   バランサ
 */
BalancingWalker::BalancingWalker(const ev3api::GyroSensor &gyroSensor,
                                 ev3api::Motor &leftWheel,
                                 ev3api::Motor &rightWheel,
                                 Balancer *balancer,
                                 Measurer *measurer)
    : mGyroSensor(gyroSensor),
      mLeftWheel(leftWheel),
      mRightWheel(rightWheel),
      mBalancer(balancer),
      m_measurer(measurer),
      mForward(LOW),
      mTurn(LOW)
{
}

/**
 * バランス走行する
 */
void BalancingWalker::run()
{
    int16_t angle = mGyroSensor.getAnglerVelocity(); // ジャイロセンサ値
    int rightWheelEnc = mRightWheel.getCount();      // 右モータ回転角度
    int leftWheelEnc = mLeftWheel.getCount();        // 左モータ回転角度

    float robot_dis = m_measurer->getRobotDistance();
    float robot_rad = m_measurer->getRobotAngle();

    if (robot_rad < 3.14)
    {
        mBalancer->setCommand(0, -20);
    }
    else
    {
        mBalancer->setCommand(0, 0);
    }

    int battery = ev3_battery_voltage_mV();
    mBalancer->update(angle, rightWheelEnc, leftWheelEnc, battery);

    // 左右モータに回転を指示する
    mLeftWheel.setPWM(mBalancer->getPwmLeft());
    mRightWheel.setPWM(mBalancer->getPwmRight());

    char message[30];
    sprintf(message, "arg :%f [m]", robot_rad * 180 / 3.14);
    ev3_lcd_draw_string(message, 0, 10);

    mLeftWheel.setBrake(false);
    mRightWheel.setBrake(false);
}

/**
 * バランス走行に必要なものをリセットする
 */
void BalancingWalker::init()
{
    int offset = mGyroSensor.getAnglerVelocity(); // ジャイロセンサ値

    // モータエンコーダをリセットする
    mLeftWheel.reset();
    mRightWheel.reset();

    // 倒立振子制御初期化
    mBalancer->init(offset);
}

/**
 * PWM値を設定する
 * @param forward 前進値
 * @param turn    旋回値
 */
void BalancingWalker::setCommand(int forward, int turn)
{
    mForward = forward;
    mTurn = turn;
}
