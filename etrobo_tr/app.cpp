#include "app.h"
#include "LineTracer.h"
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
static Balancer *g_balancer;
static BalancingWalker *g_balancing_walker;
static LineTracer *g_line_tracer;
static Measurer *g_measurer;

static int bt_cmd = 0;
static FILE *bt = NULL;

// EV3システム生成
static void user_system_create()
{
    // オブジェクトの作成
    g_balancer = new Balancer();
    g_measurer = new Measurer(g_wheel_L,
                              g_wheel_R);

    g_balancing_walker = new BalancingWalker(g_gyro_sensor,
                                             g_wheel_L,
                                             g_wheel_R,
                                             g_balancer,
                                             g_measurer);

    g_line_monitor = new LineMonitor(g_color_sensor);
    g_line_tracer = new LineTracer(g_line_monitor, g_balancing_walker);

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
    delete g_balancing_walker;
    delete g_balancer;
}

//トレース実行タイミング
void ev3_cyc_tracer(intptr_t exinf)
{
    act_tsk(TRACER_TASK);
}

void main_task(intptr_t unused)
{
    // センサやモータの初期化
    user_system_create();

    // BlueTooth周りの初期化
    bt = ev3_serial_open_file(EV3_SERIAL_BT);
    assert(bt != NULL);
    act_tsk(BT_TASK);

    // 初期化完了通知
    ev3_led_set_color(LED_ORANGE);

    // スタート待機
    while (1)
    {
        // 尻尾の角度を85degに維持
        tail_control(85);

        if (bt_cmd == 1)
            break;

        if (g_touch_sesor.isPressed())
            break;

        tslp_tsk(10);
    }

    unsigned long loop_start_time = g_clock.now();
    while (g_clock.now() < loop_start_time + 30)
    {
        tail_control(0);
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
        tail_control(0);       /* 完全停止用角度に制御 */
        g_measurer->measure(); // 計測
        g_line_tracer->run();  // 倒立走行
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
