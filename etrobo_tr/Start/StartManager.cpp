#include "StartManager.h"

extern ev3api::Clock clock;
extern ev3api::TouchSensor touch_sensor;

/**
 * コンストラクタ
 * @param leftWheel  左モータ
 * @param rightWheel 右モータ
 */
StartManager::StartManager(BluetoothManager *bt)
    : m_bt(bt)
{
}

void StartManager::init()
{
}

void StartManager::waitForStart()
{
    while (1)
    {
        // BlueToothスタート
        if (m_bt->getStartSignal() == BluetoothManager::START_L)
            break;

        if (m_bt->getStartSignal() == BluetoothManager::START_R)
            break;

        // タッチセンサスタート
        if (touch_sensor.isPressed())
            break;

        clock.sleep(4);
        clock.reset();
    }
}

void StartManager::start()
{
}
