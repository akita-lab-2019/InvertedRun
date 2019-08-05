/**
 *******************************************************************************
 ** 走行のログを管理する
 ** ログを取得し，Bluetooth，SD，ディスプレイに各情報を渡す
 *******************************************************************************
 **/

#ifndef EV3_LOG_MANAGER_H_
#define EV3_LOG_MANAGER_H_

#include "Recorder.h"
#include <Clock.h>
#include "LineMonitor.h"
#include "GyroSensor.h"
#include "ColorSensor.h"
#include "RobotInfo.h"
#include "PID.h"
#include "Motor.h"
#include "Odometer.h"
#include "TailController.h"
#include "InvertedWalker.h"
#include "stdlib.h"

class LogManager
{
public:
    LogManager(FILE *bt, Recorder *recorder, RobotInfo *robot_info);

    void init();
    void update();

private:
    char m_data_str[32][32];
    FILE *m_bt;
    Recorder *m_recorder;
    RobotInfo *m_robot_info;

    void readData();
    void sendToBT();
    void sendToSD();
    void sendToDisplay();
};

#endif // EV3_LOG_MANAGER_H_
