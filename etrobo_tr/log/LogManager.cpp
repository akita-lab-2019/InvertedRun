#include "LogManager.h"

/**
 * コンストラクタ
 */
LogManager::LogManager(FILE *bt, Recorder *recorder, RobotInfo *robot_info)
{
    m_bt = bt;
    m_recorder = recorder;
    m_robot_info = robot_info;
}

/**
 * 初期化する
 */
void LogManager::init()
{
    // SDカードへの保存の初期化
    m_recorder->init();
}

/**
 * 情報を更新する
 */
void LogManager::update()
{
    // データを取得
    readData();

    // Bluetoothで送信
    sendToBT();

    // SDカード内に保存
    sendToSD();

    // LCD表示
    sendToDisplay();
}

/**
 * コンストラクタ
 */
void LogManager::readData()
{
    // データを抽出して文字配列に格納
    // int i = 0;
    // sprintf(m_data_str[i++], "%f", g_clock.now() / 1000.0);
    // sprintf(m_data_str[i++], "%f", ev3_battery_voltage_mV() / 1000.0);
    // sprintf(m_data_str[i++], "%d", g_color_sensor.getBrightness());
    // sprintf(m_data_str[i++], "%f", g_odometer->getRobotPoseX());
    // sprintf(m_data_str[i++], "%f", g_odometer->getRobotPoseY());
    // sprintf(m_data_str[i++], "%f", g_odometer->getRobotAngle() * 180 / 3.14);
    // sprintf(m_data_str[i++], "%f", g_odometer->getRobotDistance());
    // sprintf(m_data_str[i++], "%d", g_sonar_sensor.getDistance());

    int i = 0;
    sprintf(m_data_str[i++], "%f", m_robot_info->getRunTime());
    sprintf(m_data_str[i++], "%f", 10.0);
    sprintf(m_data_str[i++], "%d", m_robot_info->getBrightness());
    sprintf(m_data_str[i++], "%f", m_robot_info->getRobotPos(0));
    sprintf(m_data_str[i++], "%f", m_robot_info->getRobotPos(1));
    sprintf(m_data_str[i++], "%f", m_robot_info->getRobotPos(2));
    sprintf(m_data_str[i++], "%f", m_robot_info->getRobotDis());
    sprintf(m_data_str[i++], "%f", 10.0);
}

void LogManager::sendToBT()
{
    char file_str[255];
    sprintf(file_str, "%s", m_data_str[0]);
    for (int i = 1; i < 7; i++)
    {
        sprintf(file_str, "%s\t%s", file_str, m_data_str[i]);
    }
    fprintf(m_bt, "%s\r\n", file_str);
}

void LogManager::sendToSD()
{
    char file_str[255];
    sprintf(file_str, "%s", m_data_str[0]);
    for (int i = 1; i < 7; i++)
    {
        sprintf(file_str, "%s, %s", file_str, m_data_str[i]);
    }
    m_recorder->record(file_str);
}

void LogManager::sendToDisplay()
{
    char lcd_str[32][32];
    char lcd_caption_str[32][32] = {
        "time:    ",
        "battery: ",
        "color_s: ",
        "pose_x:  ",
        "pose_y:  ",
        "angle:   ",
        "dis:     ",
        "sonar:   ",
    };
    for (int i = 0; i < 8; i++)
    {
        sprintf(lcd_str[i], "%s%s", lcd_caption_str[i], m_data_str[i]);
        ev3_lcd_draw_string(lcd_str[i], 5, 5 + 10 * i);
    }
}
