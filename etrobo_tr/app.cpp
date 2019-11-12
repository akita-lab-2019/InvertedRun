#include "app.h"
#include "TailWalker.h"
#include "LineTracer.h"
#include "LogManager.h"
#include "TailController.h"
#include "Recorder.h"
#include "PID.h"
#include "Section.h"
#include "SectionTracer.h"
#include <Clock.h>
#include <SonarSensor.h>
#include <TouchSensor.h>
#include "Odometer.h"
#include "BluetoothManager.h"
#include "StartManager.h"
#include "Seesaw.h"
#include "Lookup.h"

// デストラクタ問題の回避
// https://github.com/ETrobocon/etroboEV3/wiki/problem_and_coping
void *__dso_handle = 0;

int g_run_course = 0;

// using宣言
using ev3api::Clock;
using ev3api::ColorSensor;
using ev3api::GyroSensor;
using ev3api::Motor;
using ev3api::SonarSensor;
using ev3api::TouchSensor;

// オブジェクトを静的に確保
Clock clock;
TouchSensor touch_sensor(PORT_1);
SonarSensor sonar_sensor(PORT_2);
ColorSensor color_sensor(PORT_3);
GyroSensor gyro_sensor(PORT_4);
Motor tail_motor(PORT_A);
Motor wheel_L(PORT_C);
Motor wheel_R(PORT_B);

// オブジェクトの定義
static Section *g_section;
static SectionTracer *g_section_tracer;
static LogManager *g_log_manager;
static Recorder *g_recorder;
static TailController *g_tail_controller;
static Balancer *g_balancer;
static TailWalker *g_tail_walker;
static LineTracer *g_line_tracer;
static PID *g_pid_tail;
static BluetoothManager *g_bt;
static StartManager *g_start_manager;
// static Seesaw *g_seesaw;
// static Lookup *g_lookup;

/**
 * システムの初期化処理
 */
static void initSystem()
{
    g_bt = new BluetoothManager();
    g_start_manager = new StartManager(g_bt);

    g_section = new Section();

    // 記録
    g_recorder = new Recorder();
    g_log_manager = new LogManager(g_recorder,
                                   g_bt,
                                   g_section);

    // 尻尾制御
    g_pid_tail = new PID();
    g_tail_controller = new TailController(g_pid_tail);

    // 走行制御
    g_balancer = new Balancer();
    g_tail_walker = new TailWalker();
    g_line_tracer = new LineTracer(g_section,
                                   g_tail_walker,
                                   g_balancer);

    g_section_tracer = new SectionTracer(g_section, g_line_tracer);

    // g_seesaw = new Seesaw(
    //     g_line_tracer,
    //     g_tail_controller);

    // g_lookup = new Lookup(
    //     g_line_tracer,
    //     g_tail_controller);

    g_log_manager->init();
    g_tail_controller->init();

    initOdometer();

    // 尻尾の角位置をリセット
    tail_motor.setPWM(-100);
    tslp_tsk(700);
    tail_motor.reset();

    // 尻尾の角度を維持
    g_tail_controller->setAngle(98);
    g_tail_controller->setMaxSpeed(50);

    // タスクの開始
    ev3_sta_cyc(INFO_TASK);
    ev3_sta_cyc(TAIL_TASK);
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
    wheel_L.reset();
    wheel_R.reset();
    tail_motor.reset();

    delete g_tail_controller;
    delete g_balancer;
    delete g_line_tracer;
    delete g_pid_tail;
}

/**
 * メインタスク
 */
void main_task(intptr_t unused)
{
    // 初期化処理1
    initSystem();

    // スタート待機
    g_start_manager->waitForStart();
    // setCourse(0);
    if (g_bt->getStartSignal() == BluetoothManager::START_R)
    {
        g_run_course = 1;
        // setCourse(1);
    }

    ev3_speaker_play_tone(NOTE_E4, 100);

    // 尻尾を少し前に
    g_tail_controller->setAngle(108);
    g_tail_controller->setMaxSpeed(50);
    clock.sleep(200);

    // 周期ハンドラ開始
    ev3_sta_cyc(TRACER_TASK);

    // 後始末
    slp_tsk();                // バックボタンが押されるまで待つ
    ev3_stp_cyc(INFO_TASK);   // 周期ハンドラ停止
    ev3_stp_cyc(TRACER_TASK); // 周期ハンドラ停止
    ev3_stp_cyc(TAIL_TASK);   // 周期ハンドラ停止
    ev3_stp_cyc(LOG_TASK);    // 周期ハンドラ停止
    ev3_stp_cyc(BT_RCV_TASK); // 周期ハンドラ停止
    destroySystem();          // 終了処理
    ext_tsk();
}

/**
 * 尻尾制御タスク
 */
void tail_task(intptr_t exinf)
{
    g_tail_controller->control();
    ext_tsk();
}

/**
 * 測定タスク
 */
void info_task(intptr_t exinf)
{
    // g_robot_info->update();
    ext_tsk();
}

/**
 * ライントレースタスク
 */
bool is_goal = false;
void tracer_task(intptr_t exinf)
{
    processOdometer();

    if (is_goal == false)
    {
        // 未完走
        g_tail_controller->setAngle(70);
        g_tail_controller->setMaxSpeed(40);

        is_goal = g_section_tracer->run(g_run_course, g_bt->getStartSectionNum());
    }
    else
    {
        // // 完走
        // if (g_run_course == 0)
        // {
        //     // Lコース（シーソー）
        //     g_seesaw->run();
        // }
        // else
        // {
        //     // Rコース（ルックアップ）
        //     g_lookup->run();
        // }
    }

    if (g_bt->getStartSignal() == BluetoothManager::STOP || (touch_sensor.isPressed() && clock.now() > 500))
    {
        // 停止信号を受信
        landing();

        while (true)
        {
            if (ev3_button_is_pressed(BACK_BUTTON))
            {
                // バックボタン押下
                wup_tsk(MAIN_TASK); // タスクの起床
            }

            clock.sleep(4);
        }
    }

    ext_tsk();
}

void landing()
{
    g_tail_controller->setAngle(75);
    g_tail_controller->setMaxSpeed(40);
    wheel_L.setPWM(90);
    wheel_R.setPWM(90);
    clock.sleep(150);

    wheel_R.reset();
    wheel_L.reset();

    // g_wheel_L.setPWM(0);
    // g_wheel_R.setPWM(0);
    // g_wheel_L.setBrake(true);
    // g_wheel_R.setBrake(true);
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
    g_bt->receive();
    tslp_tsk(10);
}
