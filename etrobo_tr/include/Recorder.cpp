#include "Recorder.h"

static FILE *m_log_file = NULL;

/**
 * コンストラクタ
 * @param lineMonitor    ライン判定
 * @param invertedWalker 倒立走行
 */
Recorder::Recorder(ev3api::Clock &clock,
                   ev3api::ColorSensor &color_sensor,
                   LineMonitor *line_monitor,
                   InvertedWalker *inverted_walker,
                   PID *pid,
                   ParmAdministrator *parm,
                   Odometer *odometer,
                   ev3api::Motor &wheel_L,
                   ev3api::Motor &wheel_R)
    : m_clock(clock),
      m_color_sensor(color_sensor),
      m_line_monitor(line_monitor),
      m_inverted_walker(inverted_walker),
      m_pid(pid),
      m_parm(parm),
      m_odometer(odometer),
      m_wheel_L(wheel_L),
      m_wheel_R(wheel_R)
{
}

/**
 * 初期化する
 */
void Recorder::init()
{
    m_log_file = fopen("log.csv", "a");
    fprintf(m_log_file, "\r\n-color_sensor_targrt:%f PID:%f;%f;%f-\r\n",
            m_parm->color_sensor_targrt,
            m_parm->trace_pid[0][0],
            m_parm->trace_pid[0][1],
            m_parm->trace_pid[0][2]);
    fprintf(m_log_file, "Time[s], Battery[V], ColorSensor, PoseX[m], PoseY[m], Distance[m], Angle[deg]\r\n");
    fclose(m_log_file);
}

/**
 * ファイル名を取得する
 */
int Recorder::getFilename()
{
    return 0;
}

/**
 * ヘッダをSDカードに書き込む
 */
void Recorder::writeHeader()
{
}

/**
 * SDカードに記録する
 */
void Recorder::record()
{
    // SDカード内に保存
    m_log_file = fopen("log.csv", "a");
    fprintf(m_log_file, "%f, %f, %d, %f, %f, %f, %f\r\n",
            m_clock.now() / 1000.0,
            (float)ev3_battery_voltage_mV() / 1000,
            m_color_sensor.getBrightness(),
            m_odometer->getRobotPoseX(),
            m_odometer->getRobotPoseY(),
            m_odometer->getRobotDistance(),
            m_odometer->getRobotAngle() * 180 / 3.14);
    fclose(m_log_file);
}