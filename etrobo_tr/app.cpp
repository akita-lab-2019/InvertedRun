/******************************************************************************
 *  app.cpp (for LEGO Mindstorms EV3)
 *  Created on: 2015/01/25
 *  Implementation of the Task main_task
 *  Author: Kazuhiro.Kawachi
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#include "app.h"
#include "LineTracer.h"
#include <Clock.h>
#include <TouchSensor.h>
#include "./measurement/Measurer.h"

void tail_control(signed int angle);

// デストラクタ問題の回避
// https://github.com/ETrobocon/etroboEV3/wiki/problem_and_coping
void *__dso_handle = 0;

// using宣言
using ev3api::Clock;
using ev3api::ColorSensor;
using ev3api::GyroSensor;
using ev3api::Motor;
using ev3api::TouchSensor;

// Device objects
// オブジェクトを静的に確保する
Clock g_clock;
ColorSensor gColorSensor(PORT_3);
GyroSensor gGyroSensor(PORT_4);
Motor gLeftWheel(PORT_C);
Motor gRightWheel(PORT_B);
Motor g_tail_motor(PORT_A);
TouchSensor g_touch_sesor(PORT_1);

// オブジェクトの定義
static LineMonitor *gLineMonitor;
static Balancer *gBalancer;
static BalancingWalker *gBalancingWalker;
static LineTracer *gLineTracer;
static Measurer *gMeasurer;

static int bt_cmd = 0;  /* Bluetoothコマンド 1:リモートスタート */
static FILE *bt = NULL; /* Bluetoothファイルハンドル */

/**
 * EV3システム生成
 */
static void user_system_create()
{
    // オブジェクトの作成
    gBalancer = new Balancer();
    gMeasurer = new Measurer(gLeftWheel,
                             gRightWheel);
    gBalancingWalker = new BalancingWalker(gGyroSensor,
                                           gLeftWheel,
                                           gRightWheel,
                                           gBalancer,
                                           gMeasurer);
    gLineMonitor = new LineMonitor(gColorSensor);
    gLineTracer = new LineTracer(gLineMonitor, gBalancingWalker);

    // 初期化完了通知
    ev3_led_set_color(LED_ORANGE);
}

/**
 * EV3システム破棄
 */
static void user_system_destroy()
{
    gLeftWheel.reset();
    gRightWheel.reset();

    delete gLineTracer;
    delete gLineMonitor;
    delete gBalancingWalker;
    delete gBalancer;
}

/**
 * トレース実行タイミング
 */
void ev3_cyc_tracer(intptr_t exinf)
{
    act_tsk(TRACER_TASK);
}

/**
 * メインタスク
 */
void main_task(intptr_t unused)
{
    user_system_create(); // センサやモータの初期化処理

    /* Open Bluetooth file */
    bt = ev3_serial_open_file(EV3_SERIAL_BT);
    assert(bt != NULL);

    /* Bluetooth通信タスクの起動 */
    act_tsk(BT_TASK);

    ev3_led_set_color(LED_ORANGE); /* 初期化完了通知 */

    /* スタート待機 */
    while (1)
    {
        tail_control(85); /* 完全停止用角度に制御 */

        if (bt_cmd == 1)
        {
            break; /* リモートスタート */
        }

        if (g_touch_sesor.isPressed())
        {
            break; /* タッチセンサが押された */
        }

        tslp_tsk(10); /* 10msecウェイト */
    }

    unsigned long loop_start_time = g_clock.now();
    while (g_clock.now() < loop_start_time + 30)
    {
        tail_control(0); /* 完全停止用角度に制御 */
    }

    // 周期ハンドラ開始
    ev3_sta_cyc(EV3_CYC_TRACER);

    slp_tsk(); // バックボタンが押されるまで待つ

    // 周期ハンドラ停止
    ev3_stp_cyc(EV3_CYC_TRACER);

    user_system_destroy(); // 終了処理

    ext_tsk();
}

/**
 * ライントレースタスク
 */
void tracer_task(intptr_t exinf)
{
    if (ev3_button_is_pressed(BACK_BUTTON))
    {
        wup_tsk(MAIN_TASK); // バックボタン押下
    }
    else
    {
        tail_control(45);     /* 完全停止用角度に制御 */
        gMeasurer->measure(); // 計測
        gLineTracer->run();   // 倒立走行
    }

    ext_tsk();
}

//*****************************************************************************
// 関数名 : bt_task
// 引数 : unused
// 返り値 : なし
// 概要 : Bluetooth通信によるリモートスタート。 Tera Termなどのターミナルソフトから、
//       ASCIIコードで1を送信すると、リモートスタートする。
//*****************************************************************************
void bt_task(intptr_t unused)
{
    while (1)
    {
        static FILE *bt = NULL;
        bt = ev3_serial_open_file(EV3_SERIAL_BT);

        fprintf(bt, "Hello\r\n");

        tslp_tsk(10);
        uint8_t c = fgetc(bt); /* 受信 */
        switch (c)
        {
        case '1':
            bt_cmd = 1;
            break;
        default:
            break;
        }
        fputc(c, bt); /* エコーバック */
    }
}

//*****************************************************************************
// 関数名 : tail_control
// 引数 : angle (モーター目標角度[度])
// 返り値 : 無し
// 概要 : 走行体完全停止用モーターの角度制御
//*****************************************************************************
void tail_control(signed int angle)
{
    signed int pwm = (signed int)((angle - g_tail_motor.getCount()) * 2.5); /* 比例制御 */
    /* PWM出力飽和処理 */
    if (pwm > 60)
    {
        pwm = 60;
    }
    else if (pwm < -60)
    {
        pwm = -60;
    }

    if (pwm == 0)
    {
        g_tail_motor.stop();
    }
    else
    {
        g_tail_motor.setPWM(pwm);
    }
}
