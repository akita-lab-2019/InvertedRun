#include "LogManager.h"

extern ev3api::Clock clock;
extern ev3api::ColorSensor color_sensor;
extern ev3api::SonarSensor sonar_sensor;

/**
 * コンストラクタ
 */
LogManager::LogManager(Recorder *recorder, BluetoothManager *bt, Section *section)
    : m_recorder(recorder),
      m_bt(bt),
      m_section(section)
{
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
    sprintf(m_data_str[i++], "%d", m_section->getSectionNum());
    sprintf(m_data_str[i++], "%.2f", (float)clock.now() / 1000);
    sprintf(m_data_str[i++], "%.2f", (float)ev3_battery_voltage_mV() / 1000);
    sprintf(m_data_str[i++], "%d", color_sensor.getBrightness());
    sprintf(m_data_str[i++], "%.1f", getBrightnessGap());
    sprintf(m_data_str[i++], "%.2f", getRobotPose(0));
    sprintf(m_data_str[i++], "%.2f", getRobotPose(1));
    sprintf(m_data_str[i++], "%.1f", getRobotPose(2));
    sprintf(m_data_str[i++], "%.2f", getRobotDistance());
    sprintf(m_data_str[i++], "%.2f", (float)sonar_sensor.getDistance() / 1000);
}

void LogManager::sendToBT()
{
    char file_str[1024];
    sprintf(file_str, "%s", m_data_str[0]);
    for (int i = 1; i < 17; i++)
    {
        sprintf(file_str, "%s\t%s", file_str, m_data_str[i]);
    }
    m_bt->send(file_str);
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
