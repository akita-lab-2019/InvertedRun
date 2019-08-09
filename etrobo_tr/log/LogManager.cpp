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
    int i = 0;
    sprintf(m_data_str[i++], "%d", m_robot_info->getCourse());
    sprintf(m_data_str[i++], "%d", m_robot_info->getSectionNum());
    sprintf(m_data_str[i++], "%f", m_robot_info->getRunTime());
    sprintf(m_data_str[i++], "%f", m_robot_info->getBatteryVoltage());
    sprintf(m_data_str[i++], "%d", m_robot_info->getBrightness());
    sprintf(m_data_str[i++], "%f", m_robot_info->getBrightnessGap());
    sprintf(m_data_str[i++], "%f", m_robot_info->getWheelVelocity(RobotInfo::L));
    sprintf(m_data_str[i++], "%f", m_robot_info->getWheelVelocity(RobotInfo::R));
    sprintf(m_data_str[i++], "%f", m_robot_info->getRobotLinerVelocity());
    sprintf(m_data_str[i++], "%f", m_robot_info->getRobotAngularVelocity());
    sprintf(m_data_str[i++], "%f", m_robot_info->getRobotPos(RobotInfo::X));
    sprintf(m_data_str[i++], "%f", m_robot_info->getRobotPos(RobotInfo::Y));
    sprintf(m_data_str[i++], "%f", m_robot_info->getRobotPos(RobotInfo::YAW));
    sprintf(m_data_str[i++], "%f", m_robot_info->getRobotDis());
    sprintf(m_data_str[i++], "%d", m_robot_info->getPitchPos());
    sprintf(m_data_str[i++], "%d", m_robot_info->getPitchVel());
    sprintf(m_data_str[i++], "%f", m_robot_info->getSonarDistance());
}

void LogManager::sendToBT()
{
    char file_str[1024];
    sprintf(file_str, "%s", m_data_str[0]);
    for (int i = 1; i < 17; i++)
    {
        sprintf(file_str, "%s\t%s", file_str, m_data_str[i]);
    }
    fprintf(m_bt, "%s\r\n", file_str);
}

void LogManager::sendToSD()
{
    char file_str[1024];
    sprintf(file_str, "%s", m_data_str[0]);
    for (int i = 1; i < 17; i++)
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
