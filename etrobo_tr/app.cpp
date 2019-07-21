#include "app.h"
#include "InvertedWalker.h"
#include "LineTracer.h"
#include "TailController.h"
#include <Clock.h>
#include <TouchSensor.h>
#include "./measurement/Measurer.h"

// デストラクタ問題の回避
// https://github.com/ETrobocon/etroboEV3/wiki/problem_and_coping
void *__dso_handle = 0;

// using宣言
using ev3api::Clock;
using ev3api::ColorSensor;
using ev3api::GyroSensor;
using ev3api::Motor;
using ev3api::TouchSensor;

// オブジェクトを静的に確保
Clock g_clock;
ColorSensor g_color_sensor(PORT_3);
GyroSensor g_gyro_sensor(PORT_4);
Motor g_wheel_L(PORT_C);
Motor g_wheel_R(PORT_B);
Motor g_tail_motor(PORT_A);
TouchSensor g_touch_sesor(PORT_1);

// オブジェクトの定義
static LineMonitor *g_line_monitor;
static TailController *g_tail_controller;
static Balancer *g_balancer;
static InvertedWalker *g_inverted_walker;
static LineTracer *g_line_tracer;
static Measurer *g_measurer;

static int bt_cmd = 0;

// EV3システム生成
static void user_system_create()
{
    // オブジェクトの作成
    g_balancer = new Balancer();
    g_measurer = new Measurer(g_wheel_L, g_wheel_R);
    g_line_monitor = new LineMonitor(g_color_sensor);
    g_inverted_walker = new InvertedWalker(g_gyro_sensor,
                                           g_wheel_L,
                                           g_wheel_R,
                                           g_balancer);
    g_line_tracer = new LineTracer(g_line_monitor, g_inverted_walker);
    g_tail_controller = new TailController(g_tail_motor);

    // 初期化完了通知
    ev3_led_set_color(LED_ORANGE);
}

// EV3システム破棄
static void user_system_destroy()
{
    g_wheel_L.reset();
    g_wheel_R.reset();

    delete g_line_tracer;
    delete g_line_monitor;
    delete g_balancer;
}

//トレース実行タイミング
void ev3_cyc_tracer(intptr_t exinf)
{
    act_tsk(TRACER_TASK);
}

void main_task(intptr_t unused)
{
    // 初期化処理
    user_system_create();
    act_tsk(BT_TASK);

    // スタート待機
    while (1)
    {
        // 尻尾の角度を維持
        g_tail_controller->control(80, 50);

        if (bt_cmd == 1)
            break;

        if (g_touch_sesor.isPressed())
            break;

        g_clock.sleep(10);
    }

    // 尻尾が少し前に出るまで待機
    while (g_tail_motor.getCount() < 86)
    {
        g_tail_controller->control(86, 3);
    }

    // 一定時間その場で倒立をする
    unsigned long loop_start_time = g_clock.now();
    while (g_clock.now() < loop_start_time + 2000)
    {
        g_tail_controller->control(40, 20);
        g_inverted_walker->setCommand(0, 0);
        g_inverted_walker->run();
        g_clock.sleep(4);
    }

    // 周期ハンドラ開始
    ev3_sta_cyc(EV3_CYC_TRACER);

    // バックボタンが押されるまで待つ
    slp_tsk();

    // 周期ハンドラ停止
    ev3_stp_cyc(EV3_CYC_TRACER);

    // 終了処理
    user_system_destroy();

    ext_tsk();
}

// ライントレースタスク
void tracer_task(intptr_t exinf)
{
    if (ev3_button_is_pressed(BACK_BUTTON))
    {
        wup_tsk(MAIN_TASK); // バックボタン押下
    }
    else
    {
        g_tail_controller->control(40, 20); /* 完全停止用角度に制御 */
        g_measurer->measure();              // 計測
        g_line_tracer->run();               // 倒立走行
    }

    ext_tsk();
}

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
