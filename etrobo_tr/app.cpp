#include "app.h"
#include "InvertedWalker.h"
#include "LineTracer.h"
#include "ParmAdministrator.h"
#include "TailController.h"
#include "Recorder.h"
#include "PID.h"
#include <Clock.h>
#include <SonarSensor.h>
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
using ev3api::SonarSensor;
using ev3api::TouchSensor;

// オブジェクトを静的に確保
Clock g_clock;
TouchSensor g_touch_sensor(PORT_1);
SonarSensor g_sonar_sensor(PORT_2);
ColorSensor g_color_sensor(PORT_3);
GyroSensor g_gyro_sensor(PORT_4);
Motor g_tail_motor(PORT_A);
Motor g_wheel_L(PORT_C);
Motor g_wheel_R(PORT_B);

// オブジェクトの定義
static ParmAdministrator *g_parm_administrator;
static Recorder *g_recorder;
static LineMonitor *g_line_monitor;
static TailController *g_tail_controller;
static Balancer *g_balancer;
static InvertedWalker *g_inverted_walker;
static LineTracer *g_line_tracer;
static Odometer *g_odometer;
static PID *g_pid_tail;
static PID *g_pid_trace;

static int g_bt_cmd = 0;
int g_sonar_distance = 0;

/**
 * システムの初期化処理
 */
static void
initSystem()
{
    // オブジェクトの作成
    g_parm_administrator = new ParmAdministrator();
    g_parm_administrator->readParm();

    g_recorder = new Recorder(g_clock,
                              g_color_sensor,
                              g_line_monitor,
                              g_inverted_walker,
                              g_pid_trace,
                              g_parm_administrator,
                              g_odometer,
                              g_wheel_L,
                              g_wheel_R);
    ;

    g_pid_tail = new PID(2.5, 0, 0);
    g_tail_controller = new TailController(g_tail_motor, g_pid_tail);

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

    g_line_tracer = new LineTracer(g_clock,
                                   g_line_monitor,
                                   g_inverted_walker,
                                   g_pid_trace,
                                   g_parm_administrator,
                                   g_odometer,
                                   g_tail_controller,
                                   g_wheel_L,
                                   g_wheel_R);

    bt = ev3_serial_open_file(EV3_SERIAL_BT);

    // SDカードへの保存の初期化
    g_recorder->init();

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
        if (g_bt_cmd == 1 || g_bt_cmd == 2)
            break;

        // タッチセンサスタート
        if (g_touch_sensor.isPressed())
            break;

        g_clock.sleep(10);
        g_clock.reset();
    }

    if (g_bt_cmd == 2)
    {
        g_parm_administrator->trace_pid[0][0] /= -1;
        g_parm_administrator->trace_pid[0][1] /= -1;
        g_parm_administrator->trace_pid[0][2] /= -1;
        g_parm_administrator->trace_pid[1][0] /= -1;
        g_parm_administrator->trace_pid[1][1] /= -1;
        g_parm_administrator->trace_pid[1][2] /= -1;
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
bool flag = 0;
void tracer_task(intptr_t exinf)
{
    if (ev3_button_is_pressed(BACK_BUTTON))
    {
        wup_tsk(MAIN_TASK); // バックボタン押下
    }
    else if (g_bt_cmd == 3 || (g_touch_sensor.isPressed() && g_clock.now() > 500))
    {
        g_bt_cmd = 3;
        if (flag == 0)
        {
            long start_time = g_clock.now();
            while (g_clock.now() - start_time < 150)
            {
                g_tail_controller->control(65, 40);
                g_wheel_L.setPWM(80);
                g_wheel_R.setPWM(80);
            }
        }
        flag = 1;
        g_tail_controller->control(65, 40);
        g_wheel_L.reset();
        g_wheel_R.reset();
    }
    else
    {
        // g_tail_controller->control(0, 60); // 完全停止用角度に制御
        g_odometer->measure(); // 計測
        g_line_tracer->run();  // 倒立走行
    }

    ext_tsk();
}

/**
 * ログタスク
 */
void log_task(intptr_t exinf)
{
    g_sonar_distance = g_sonar_sensor.getDistance();
    // Bluetoothで送信
    // fprintf(bt, "\033[2J");
    // fprintf(bt, "\033[%d;%dH", 1, 0);
    // fprintf(bt, "time[sec]\tbatte[V]\tpose_x[m]\tpose_y[m]\tdista[m]\tangle[deg]\tsonar[cm]\tpich_p[deg]\tpitch_v[deg/sec]\r\n");
    fprintf(bt, "%f\t%f\t%d\t%f\t%f\t%f\t%f\t%d \r\n",
            g_clock.now() / 1000.0,
            (float)ev3_battery_voltage_mV() / 1000,
            g_color_sensor.getBrightness(),
            g_odometer->getRobotPoseX(),
            g_odometer->getRobotPoseY(),
            g_odometer->getRobotDistance(),
            g_odometer->getRobotAngle() * 180 / 3.14,
            g_sonar_distance);

    // SDカード内に保存
    // g_recorder->record();

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
    sprintf(str[i++], "sonar:   %d[cm]", g_sonar_distance);
    // sprintf(str[i++], "pich_p:  %d[deg]", g_gyro_sensor.getAngle());
    // sprintf(str[i++], "pich_v:  %d[deg/sec]", g_gyro_sensor.getAnglerVelocity());

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
    case '2':
        g_bt_cmd = 2;
        break;
    case '3':
        g_bt_cmd = 3;
        break;
    default:
        break;
    }
}
