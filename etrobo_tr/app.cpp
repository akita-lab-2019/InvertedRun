#include "app.h"
#include "InvertedWalker.h"
#include "LineTracer.h"
#include "ParmAdministrator.h"
#include "TailController.h"
#include "PID.h"
#include <Clock.h>
#include <TouchSensor.h>
#include "Odometer.h"

// デストラクタ問題の回避
// https://github.com/ETrobocon/etroboEV3/wiki/problem_and_coping
void *__dso_handle = 0;

static FILE *bt = NULL;

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
static ParmAdministrator *g_parm_administrator;
static LineMonitor *g_line_monitor;
static TailController *g_tail_controller;
static Balancer *g_balancer;
static InvertedWalker *g_inverted_walker;
static LineTracer *g_line_tracer;
static Odometer *g_odometer;
static PID *g_pid_tail;
static PID *g_pid_trace;

static int g_bt_cmd = 0;

/**
 * システムの初期化処理
 */
static void
initSystem()
{
    // オブジェクトの作成
    g_parm_administrator = new ParmAdministrator();
    g_parm_administrator->readParm();

    g_pid_tail = new PID(2.5, 0, 0);
    g_pid_trace = new PID(g_parm_administrator->trace_pid[0][0],
                          g_parm_administrator->trace_pid[0][1],
                          g_parm_administrator->trace_pid[0][2]);
    g_balancer = new Balancer();
    g_odometer = new Odometer(g_wheel_L, g_wheel_R);
    g_line_monitor = new LineMonitor(g_color_sensor, g_parm_administrator);
    g_inverted_walker = new InvertedWalker(g_gyro_sensor,
                                           g_wheel_L,
                                           g_wheel_R,
                                           g_balancer);
    g_line_tracer = new LineTracer(g_line_monitor, g_inverted_walker, g_pid_trace, g_parm_administrator, g_odometer);
    g_tail_controller = new TailController(g_tail_motor, g_pid_tail);

    bt = ev3_serial_open_file(EV3_SERIAL_BT);

    // SDカードへの保存の初期化
    init();

    ev3_sta_cyc(LOG_TASK);
    ev3_sta_cyc(BT_RCV_TASK);

    // 初期化完了通知
    ev3_led_set_color(LED_ORANGE);
}

/**
 * システムの破棄処理
 */
static void destroySystem()
{
    g_wheel_L.reset();
    g_wheel_R.reset();
    g_tail_motor.reset();

    delete g_line_monitor;
    delete g_tail_controller;
    delete g_balancer;
    delete g_inverted_walker;
    delete g_line_tracer;
    delete g_odometer;
    delete g_pid_tail;
    delete g_pid_trace;
}

/**
 * メインタスク
 */
void main_task(intptr_t unused)
{
    // 初期化処理
    initSystem();

    // スタート待機
    while (1)
    {
        // 尻尾の角度を維持
        g_tail_controller->control(85, 50);

        // BlueToothスタート
        if (g_bt_cmd == 1)
            break;

        // タッチセンサスタート
        if (g_touch_sesor.isPressed())
            break;

        g_clock.sleep(10);
        g_clock.reset();
    }

    // 周期ハンドラ開始
    ev3_sta_cyc(TRACER_TASK);

    slp_tsk();                // バックボタンが押されるまで待つ
    ev3_stp_cyc(TRACER_TASK); // 周期ハンドラ停止
    ev3_stp_cyc(LOG_TASK);    // 周期ハンドラ停止
    ev3_stp_cyc(BT_RCV_TASK); // 周期ハンドラ停止
    destroySystem();          // 終了処理
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
        g_tail_controller->control(0, 60); // 完全停止用角度に制御
        g_odometer->measure();             // 計測
        g_line_tracer->run();              // 倒立走行
    }

    ext_tsk();
}

/**
 * ログタスク
 */
void log_task(intptr_t exinf)
{
    // Bluetoothで送信
    fprintf(bt, "%f\t%f\t%f\t%f\t%f \r\n",
            (float)ev3_battery_voltage_mV() / 1000,
            g_odometer->getRobotPoseX(),
            g_odometer->getRobotPoseY(),
            g_odometer->getRobotDistance(),
            g_odometer->getRobotAngle() * 180 / 3.14);

    // SDカード内に保存
    record();

    // LCD表示
    char str[10][32];
    for (int i = 0; i < 10; i++)
    {
        str[i][0] = '\0';
    }

    int i = 0;
    sprintf(str[i++], "time:    %f[sec]", g_clock.now() / 1000.0);
    sprintf(str[i++], "battery: %f[V]", ev3_battery_voltage_mV() / 1000.0);
    sprintf(str[i++], "color_s: %d", g_color_sensor.getBrightness());
    sprintf(str[i++], "pose_x:  %f[m]", g_odometer->getRobotPoseX());
    sprintf(str[i++], "pose_y:  %f[m]", g_odometer->getRobotPoseY());
    sprintf(str[i++], "angle:   %f[deg]", g_odometer->getRobotAngle() * 180 / 3.14);
    sprintf(str[i++], "%f", g_parm_administrator->color_sensor_targrt);

    for (int i = 0; i < 7; i++)
    {
        ev3_lcd_draw_string(str[i], 5, 5 + 10 * i);
    }

    tslp_tsk(10);
}

/**
 * BlueTooth受信タスク
 */
void bt_recieve_task(intptr_t exinf)
{
    uint8_t c = fgetc(bt);
    switch (c)
    {
    case '1':
        g_bt_cmd = 1;
        break;
    default:
        break;
    }
}

static FILE *log_file = NULL;
void init()
{
    log_file = fopen("log.csv", "a");
    fprintf(log_file, "\r\n-color_sensor_targrt:%f PID:%f;%f;%f-\r\n",
            g_parm_administrator->color_sensor_targrt,
            g_parm_administrator->trace_pid[0][0],
            g_parm_administrator->trace_pid[0][1],
            g_parm_administrator->trace_pid[0][2]);
    fprintf(log_file, "Time[s], Battery[V], ColorSensor, PoseX[m], PoseY[m], Distance[m], Angle[deg]\r\n");
    fclose(log_file);
}

void record()
{
    // SDカード内に保存
    log_file = fopen("log.csv", "a");
    fprintf(log_file, "%f, %f, %d, %f, %f, %f, %f \r\n",
            g_clock.now() / 1000.0,
            (float)ev3_battery_voltage_mV() / 1000,
            g_color_sensor.getBrightness(),
            g_odometer->getRobotPoseX(),
            g_odometer->getRobotPoseY(),
            g_odometer->getRobotDistance(),
            g_odometer->getRobotAngle() * 180 / 3.14);
    fclose(log_file);
}
