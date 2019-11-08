/**
 *******************************************************************************
 ** 走行のログを管理する
 ** ログを取得し，Bluetooth，SD，ディスプレイに各情報を渡す
 *******************************************************************************
 **/

#ifndef EV3_LOG_MANAGER_H_
#define EV3_LOG_MANAGER_H_

#include "ev3api.h"
#include <Clock.h>
#include <ColorSensor.h>
#include <SonarSensor.h>

#include "Recorder.h"
#include "BluetoothManager.h"
#include "Section.h"

#include "Odometer.h"
#include "LineMonitor.h"

class LogManager
{
public:
    LogManager(Recorder *recorder, BluetoothManager *bt, Section *section);

    void init();
    void update();

private:
    char m_data_str[32][32];
    // FILE *m_bt;
    Recorder *m_recorder;
    BluetoothManager *m_bt;
    Section *m_section;

    void readData();
    void sendToBT();
    void sendToSD();
    void sendToDisplay();
};

#endif // EV3_LOG_MANAGER_H_
