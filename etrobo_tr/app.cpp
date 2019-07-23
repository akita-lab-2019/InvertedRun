#include "app.h"
#include "InvertedWalker.h"
#include "LineTracer.h"
#include "TailController.h"
#include "PID.h"
#include <Clock.h>
#include <TouchSensor.h>
#include "./measurement/Odometer.h"

// デストラクタ問題の回避
// https://github.com/ETrobocon/etroboEV3/wiki/problem_and_coping
void *__dso_handle = 0;

static FILE *bt = NULL; //ファイルポインタ

// using宣言
using ev3api::Clock;
using ev3api::ColorSensor;
using ev3api::GyroSensor;
using ev3api::Motor;
using ev3api::TouchSensor;

// オブジェクトを静的に確保
Clock g_clock;
TouchSensor g_touch_sesor(PORT_1);
ColorSensor g_color_sensor(PORT_3);
GyroSensor g_gyro_sensor(PORT_4);
Motor g_tail_motor(PORT_A);
Motor g_wheel_L(PORT_C);
Motor g_wheel_R(PORT_B);

// オブジェクトの定義
static LineMonitor *g_line_monitor;
static TailController *g_tail_controller;
static Balancer *g_balancer;
static InvertedWalker *g_inverted_walker;
static LineTracer *g_line_tracer;
static Odometer *g_odometer;
static PID *g_pid_tail;
static PID *g_pid_trace;

static int bt_cmd = 0;

// EV3システム生成
static void user_system_create()
{
    // オブジェクトの作成
    g_pid_tail = new PID(1, 0, 0);
    g_pid_trace = new PID(0.64, 0.010, 0.028);
    g_balancer = new Balancer();
    g_odometer = new Odometer(g_wheel_L, g_wheel_R);
    g_line_monitor = new LineMonitor(g_color_sensor);
    g_inverted_walker = new InvertedWalker(g_gyro_sensor,
                                           g_wheel_L,
                                           g_wheel_R,
                                           g_balancer);
    g_line_tracer = new LineTracer(g_line_monitor, g_inverted_walker, g_pid_trace);
    g_tail_controller = new TailController(g_tail_motor, g_pid_tail);

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
    bt = ev3_serial_open_file(EV3_SERIAL_BT);

    // 初期化処理
    user_system_create();

    act_tsk(BT_TASK);

    // スタート待機
    while (1)
    {
        // 尻尾の角度を維持
        g_tail_controller->control(83, 50);

        if (bt_cmd == 1)
            break;

        if (g_touch_sesor.isPressed())
            break;

        g_clock.sleep(10);
    }

    ev3_sta_cyc(BT_SEND);

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
        g_tail_controller->control(0, 20); /* 完全停止用角度に制御 */
        g_odometer->measure();             // 計測
        g_line_tracer->run();              // 倒立走行
    }

    ext_tsk();
}

void bt_task(intptr_t unused)
{
    while (1)
    {

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

void Send_value()
{
    fprintf(bt, "%f\t %f\t %f\t %f\t %f \r\n",
            (float)ev3_battery_voltage_mV() / 1000,
            g_odometer->getRobotPoseX(),
            g_odometer->getRobotPoseY(),
            g_odometer->getRobotDistance(),
            g_odometer->getRobotAngle() * 180 / 3.14);

    tslp_tsk(10);
}

void bt_send(intptr_t unused)
{
    Send_value();
}
