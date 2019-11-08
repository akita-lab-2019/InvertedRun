#ifndef EV3_START_MANAGER_H_
#define EV3_START_MANAGER_H_

#include "ev3api.h"
#include <Clock.h>

#include "GuageManager.h"
#include "GyroSensor.h"
#include "Motor.h"
#include "BalancerCpp.h"
#include "stdlib.h"
#include <TouchSensor.h>
#include "BluetoothManager.h"

class StartManager
{
public:
    StartManager(BluetoothManager *bt);

    void init();
    void waitForStart();
    void start();

private:
    BluetoothManager *m_bt;
};

#endif