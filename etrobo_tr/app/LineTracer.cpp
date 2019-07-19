/******************************************************************************
 *  LineTracer.cpp (for LEGO Mindstorms EV3)
 *  Created on: 2015/01/26
 *  Implementation of the Class LineTracer
 *  Author: Kazuhiro Kawachi
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#include "LineTracer.h"

/**
 * コンストラクタ
 * @param lineMonitor     ライン判定
 * @param balancingWalker 倒立走行
 */
LineTracer::LineTracer(const LineMonitor *lineMonitor,
                       BalancingWalker *balancingWalker)
    : mLineMonitor(lineMonitor),
      mBalancingWalker(balancingWalker),
      mIsInitialized(false)
{
}

/**
 * ライントレースする
 */
void LineTracer::run()
{
    if (mIsInitialized == false)
    {
        mBalancingWalker->init();
        mIsInitialized = true;
    }

    // 走行体の向きを計算する
    // int direction = calcDirection(isOnLine);
    float direction = 0.6 * mLineMonitor->getGap();

    mBalancingWalker->setCommand(60, (int)direction);
    // mBalancingWalker->setCommand(0, 0);

    // 倒立走行を行う
    mBalancingWalker->run();
}
