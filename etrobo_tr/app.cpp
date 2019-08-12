#include "app.h"
#include "TailWalker.h"
#include "LineTracer.h"
#include "ParmAdministrator.h"
#include "LogManager.h"
#include "TailController.h"
#include "Recorder.h"
#include "PID.h"
#include "RobotInfo.h"
#include "Section.h"
#include "SectionTracer.h"
#include <Clock.h>
#include <SonarSensor.h>
#include <TouchSensor.h>
#include "Odometer.h"

// デストラクタ問題の回避
// https://github.com/ETrobocon/etroboEV3/wiki/problem_and_coping
void *__dso_handle = 0;

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
static RobotInfo *g_robot_info;
static Section *g_section;
static SectionTracer *g_section_tracer;
static ParmAdministrator *g_parm_administrator;
static LogManager *g_log_manager;
static Recorder *g_recorder;
static LineMonitor *g_line_monitor;
static TailController *g_tail_controller;
static Balancer *g_balancer;
static TailWalker *g_tail_walker;
static LineTracer *g_line_tracer;
static Odometer *g_odometer;
static PID *g_pid_tail;
static PID *g_pid_trace;

static int g_bt_cmd = 0;
int g_sonar_distance = 0;

static FILE *bt = NULL;

/**
 * システムの初期化処理
 */
static void
initSystem()
{
    // パラメータの管理
    g_parm_administrator = new ParmAdministrator();
    g_parm_administrator->readParm();

    g_section = new Section(g_parm_administrator);

    // 走行体情報
    g_odometer = new Odometer(g_wheel_L, g_wheel_R);
    g_line_monitor = new LineMonitor(g_color_sensor, g_parm_administrator);
    g_robot_info = new RobotInfo(g_clock,
                                 g_color_sensor,
                                 g_gyro_sensor,
                                 g_sonar_sensor,
                                 g_wheel_L,
                                 g_wheel_R,
                                 g_tail_motor,
                                 g_line_monitor,
                                 g_odometer,
                                 g_section);

    // 記録
    bt = ev3_serial_open_file(EV3_SERIAL_BT);
    g_recorder = new Recorder(g_parm_administrator);
    g_log_manager = new LogManager(bt, g_recorder, g_robot_info);

    // 尻尾制御
    g_pid_tail = new PID();
    g_tail_controller = new TailController(g_tail_motor, g_pid_tail);

    // 走行制御
    g_pid_trace = new PID();
    g_balancer = new Balancer();
    g_tail_walker = new TailWalker();
    g_line_tracer = new LineTracer(g_robot_info,
                                   g_section,
                                   g_tail_walker,
                                   g_pid_trace,
                                   g_balancer,
                                   g_wheel_L,
                                   g_wheel_R);
    g_section_tracer = new SectionTracer(g_robot_info, g_section, g_line_tracer, g_parm_administrator);

    g_log_manager->init();
    g_tail_controller->init();

    // タスクの開始
    ev3_sta_cyc(INFO_TASK);
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

    // 尻尾の角位置をリセット
    g_tail_motor.setPWM(-100);
    tslp_tsk(700);
    g_tail_motor.reset();

    g_robot_info->setGyroOffset(0.5);

    // スタート待機
    while (1)
    {
        g_robot_info->update();

        // 尻尾の角度を維持
        g_tail_controller->control(98, 50);

        // BlueToothスタート
        if (g_bt_cmd == 1 || g_bt_cmd == 2)
            break;

        // タッチセンサスタート
        if (g_touch_sensor.isPressed())
            break;

        g_clock.sleep(4);
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
    ev3_speaker_play_tone(NOTE_E4, 100);

    // 尻尾を少し前に
    for (int i = 0; i < 150 / 4; i++)
    {
        g_robot_info->update();
        g_tail_controller->control(105, 50);
        g_clock.sleep(4);
    }

    // 周期ハンドラ開始
    ev3_sta_cyc(TRACER_TASK);

    slp_tsk();                // バックボタンが押されるまで待つ
    ev3_stp_cyc(INFO_TASK);   // 周期ハンドラ停止
    ev3_stp_cyc(TRACER_TASK); // 周期ハンドラ停止
    ev3_stp_cyc(LOG_TASK);    // 周期ハンドラ停止
    ev3_stp_cyc(BT_RCV_TASK); // 周期ハンドラ停止
    destroySystem();          // 終了処理
    ext_tsk();
}

/**
 * 測定タスク
 */
void info_task(intptr_t exinf)
{
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
                g_tail_controller->control(75, 40);
                g_wheel_L.setPWM(80);
                g_wheel_R.setPWM(80);
            }
        }
        flag = 1;
        g_tail_controller->control(75, 40);
        g_wheel_L.reset();
        g_wheel_R.reset();
    }
    else
    {
        g_tail_controller->control(0, 20); // 完全停止用角度に制御
        g_robot_info->update();
        g_section_tracer->run(); // 倒立走行
    }

    ext_tsk();
}

/**
 * ログタスク
 */
void log_task(intptr_t exinf)
{
    g_log_manager->update();
    tslp_tsk(20);
}

/**
 * BlueTooth受信タスク
 */
void bt_recieve_task(intptr_t exinf)
{
    uint8_t c = fgetc(bt);
    switch (c)
    {
    case 'l':
        g_bt_cmd = 1;
        break;
    case 'r':
        g_bt_cmd = 2;
        break;
    case 's':
        g_bt_cmd = 3;
        break;
    default:
        break;
    }

    tslp_tsk(10);
}
