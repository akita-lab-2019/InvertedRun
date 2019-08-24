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
#include "BluetoothManager.h"
#include "StartManager.h"

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
static BluetoothManager *g_bt;
static StartManager *g_start_manager;

/**
 * システムの初期化処理
 */
static void initSystem()
{
    g_bt = new BluetoothManager();
    g_start_manager = new StartManager(g_bt, g_touch_sensor, g_clock);

    // パラメータの管理
    g_parm_administrator = new ParmAdministrator();
    g_parm_administrator->readParm();

    g_section = new Section(g_parm_administrator);

    // 走行体情報
    g_pid_trace = new PID();
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
                                 g_section,
                                 g_pid_trace);

    // 記録
    g_recorder = new Recorder(g_parm_administrator);
    g_log_manager = new LogManager(g_recorder,
                                   g_bt,
                                   g_robot_info);

    // 尻尾制御
    g_pid_tail = new PID();
    g_tail_controller = new TailController(g_tail_motor, g_pid_tail);

    // 走行制御
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

    // 尻尾の角位置をリセット
    g_tail_motor.setPWM(-100);
    tslp_tsk(700);
    g_tail_motor.reset();

    // ジャイロのオフセット
    g_robot_info->setGyroOffset(0);

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

    // スタート待機
    g_start_manager->waitForStart();
    g_robot_info->setCourse(0);
    if (g_bt->getStartSignal() == BluetoothManager::START_R)
    {
        g_robot_info->setCourse(1);
    }

    ev3_speaker_play_tone(NOTE_E4, 100);

    // 尻尾を少し前に
    g_tail_controller->setAngle(108);
    g_tail_controller->setMaxSpeed(50);
    g_clock.sleep(200);

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
    g_robot_info->update();
    ext_tsk();
}

/**
 * ライントレースタスク
 */
bool is_goal = false;
void tracer_task(intptr_t exinf)
{
    if (is_goal == false)
    {
        // 未完走
        g_tail_controller->setAngle(75);
        g_tail_controller->setMaxSpeed(40);

        is_goal = g_section_tracer->run();
    }
    else
    {
        // 完走
        doLookup();
    }

    if (g_bt->getStartSignal() == BluetoothManager::STOP || (g_touch_sensor.isPressed() && g_robot_info->getRunTime() > 0.5))
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

            g_clock.sleep(4);
        }
    }

    ext_tsk();
}

void upBody()
{
    g_tail_controller->setAngle(90);
    g_tail_controller->setMaxSpeed(100);
}

void downBody()
{
    g_tail_controller->setAngle(70);
    g_tail_controller->setMaxSpeed(100);
}

float tail_walk_pid_param[2][3] = {{0.3, 0.0, 0.0}, {0.5, 0.0, 0.0}};
void lineRun(int forward, int pid_index, int target)
{
    g_line_tracer->setIsInverted(false);
    g_line_tracer->setForward(forward);
    g_line_tracer->setCurvature(0);
    g_line_tracer->setPidParm(tail_walk_pid_param[pid_index]);
    g_line_tracer->setColorTarget(target);
    g_line_tracer->run();
}

int lookup_sequence_num = 0;
void doLookup()
{

    switch (lookup_sequence_num)
    {
    case 0:
        // 尻尾をつく
        ev3_speaker_play_tone(262, 100);
        landing();

        ev3_speaker_play_tone(262, 100);
        g_clock.sleep(1000);

        ev3_speaker_play_tone(262, 100);
        upBody();

        g_clock.sleep(1000);

        lookup_sequence_num++;
        break;

    case 1:
        // 高姿勢で前進
        lineRun(15, 0, 15);

        if (g_robot_info->getSonarDistance() < 0.05)
        {
            g_clock.sleep(10);
            if (g_robot_info->getSonarDistance() < 0.05)
            {
                lookup_sequence_num++;
            }
        }
        break;

    case 2:
        // 倒す
        ev3_speaker_play_tone(262, 100);
        downBody();

        g_wheel_R.reset();
        g_wheel_L.reset();
        g_clock.sleep(1000);
        ev3_speaker_play_tone(262, 100);
        lookup_sequence_num++;

    case 3:
        // 低い姿勢で前進
        lineRun(15, 1, 5);

        if (g_robot_info->getRobotDis() > 0.15)
        {
            ev3_speaker_play_tone(262, 100);
            lookup_sequence_num++;
        }
        break;

    case 4:
        // 低い姿勢で前進
        lineRun(-15, 1, 5);

        if (g_robot_info->getRobotDis() < -0.2)
        {
            ev3_speaker_play_tone(262, 100);
            lookup_sequence_num++;
        }
        break;

    case 5:
        // 低い姿勢で前進
        lineRun(15, 1, 5);

        if (g_robot_info->getRobotDis() > 0.15)
        {
            ev3_speaker_play_tone(262, 100);
            lookup_sequence_num++;
        }
        break;

    case 6:
        // 低い姿勢で前進
        lineRun(-15, 1, 5);

        if (g_robot_info->getRobotDis() < -0.2)
        {
            ev3_speaker_play_tone(262, 100);
            lookup_sequence_num++;
        }
        break;

    case 7:
        // 低い姿勢で前進
        lineRun(15, 1, 5);

        if (g_robot_info->getRobotDis() > 0.15)
        {
            ev3_speaker_play_tone(262, 100);
            lookup_sequence_num++;
        }
        break;

    case 8:
        // 高姿勢で前進
        lineRun(15, 0, 15);

        if (g_robot_info->getRobotDis() > 0.45)
        {
            ev3_speaker_play_tone(262, 100);
            g_wheel_R.reset();
            g_wheel_L.reset();
            lookup_sequence_num++;
        }
        break;

    case 9:
        g_wheel_R.reset();
        g_wheel_L.reset();
        break;

        // case 4:
        //     // 旋回
        //     g_wheel_R.setPWM(30);
        //     g_wheel_L.setPWM(-30);

        //     if (g_robot_info->getRobotPos(2) > 180)
        //     {
        //         ev3_speaker_play_tone(262, 100);
        //         lookup_sequence_num++;
        //     }
        //     break;

        // case 5:
        //     // 低い姿勢で前進
        //     g_robot_info->setCourse(1);
        //     lineRun(8, 1, 5);

        //     if (g_robot_info->getRobotDis() > 0.45)
        //     {
        //         ev3_speaker_play_tone(262, 100);
        //         lookup_sequence_num++;
        //     }
        //     break;

        // case 6:
        //     // 旋回
        //     g_wheel_R.setPWM(30);
        //     g_wheel_L.setPWM(-30);

        //     if (g_robot_info->getRobotPos(2) > 360)
        //     {
        //         ev3_speaker_play_tone(262, 100);
        //         lookup_sequence_num++;
        //     }
        //     break;

        // case 7:
        //     // 低い姿勢で前進
        //     g_robot_info->setCourse(0);
        //     lineRun(8, 1, 5);

        //     if (g_robot_info->getRobotDis() > 0.45 + 0.30)
        //     {
        //         ev3_speaker_play_tone(262, 100);
        //         lookup_sequence_num++;
        //     }
        //     break;

        // case 8:
        //     // 旋回
        //     g_wheel_R.setPWM(30);
        //     g_wheel_L.setPWM(-30);

        //     if (g_robot_info->getRobotPos(2) > 540)
        //     {
        //         ev3_speaker_play_tone(262, 100);
        //         lookup_sequence_num++;
        //     }
        //     break;

        // case 9:
        //     // 低い姿勢で前進
        //     g_robot_info->setCourse(1);
        //     lineRun(8, 1, 5);

        //     if (g_robot_info->getRobotDis() > 0.45 + 0.60)
        //     {
        //         ev3_speaker_play_tone(262, 100);
        //         lookup_sequence_num++;
        //     }
        //     break;

        // case 10:
        //     // 旋回
        //     g_wheel_R.setPWM(30);
        //     g_wheel_L.setPWM(-30);

        //     if (g_robot_info->getRobotPos(2) > 540 + 180)
        //     {
        //         ev3_speaker_play_tone(262, 100);
        //         lookup_sequence_num++;
        //     }
        //     break;

        // case 11:
        //     // 低い姿勢で前進
        //     g_robot_info->setCourse(0);
        //     lineRun(8, 1, 5);

        //     if (g_robot_info->getRobotDis() > 0.45 + 0.90)
        //     {
        //         ev3_speaker_play_tone(262, 100);
        //         lookup_sequence_num++;
        //     }
        //     break;

        // case 12:
        //     // 高姿勢で前進
        //     lineRun(8, 0, 15);

        //     if (g_robot_info->getRobotDis() > 0.45 + 1.20)
        //     {
        //         ev3_speaker_play_tone(262, 100);
        //         g_wheel_R.reset();
        //         g_wheel_L.reset();
        //         lookup_sequence_num++;
        //     }
        //     break;

        // case 13:
        //     g_wheel_R.reset();
        //     g_wheel_L.reset();
        //     break;

    default:
        break;
    }
}

void landing()
{
    g_tail_controller->setAngle(75);
    g_tail_controller->setMaxSpeed(40);
    g_wheel_L.setPWM(90);
    g_wheel_R.setPWM(90);
    g_clock.sleep(150);

    g_wheel_R.reset();
    g_wheel_L.reset();

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
