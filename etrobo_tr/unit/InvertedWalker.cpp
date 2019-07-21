
#include "InvertedWalker.h"

// 定数宣言
const int InvertedWalker::LOW = 30;    // 低速
const int InvertedWalker::NORMAL = 50; // 通常
const int InvertedWalker::HIGH = 70;   // 高速

/**
 * コンストラクタ
 * @param gyroSensor ジャイロセンサ
 * @param leftWheel  左モータ
 * @param rightWheel 右モータ
 * @param balancer   バランサ
 */
InvertedWalker::InvertedWalker(ev3api::GyroSensor &gyro_sensor,
                               ev3api::Motor &wheel_L,
                               ev3api::Motor &wheel_R,
                               Balancer *balancer)
    : m_gyro_sensor(gyro_sensor),
      m_wheel_L(wheel_L),
      m_wheel_R(wheel_R),
      m_balancer(balancer),
      m_forward(NORMAL),
      m_turn(LOW)
{
}

/**
 * バランス走行する
 */
void InvertedWalker::run()
{
    // 倒立走行に必要な各種データを取得
    int angle = m_gyro_sensor.getAnglerVelocity(); // ジャイロセンサ値
    int wheel_cnt_R = m_wheel_R.getCount();        // 右モータ回転角度
    int wheel_cnt_L = m_wheel_L.getCount();        // 左モータ回転角度
    int battery = ev3_battery_voltage_mV();

    // 並進と旋回の指令値，各種データ与えてホイールの指令値を算出させる
    m_balancer->setCommand(m_forward, m_turn);
    m_balancer->update(angle, wheel_cnt_R, wheel_cnt_L, battery);

    // 左右モータに回転を指示する
    m_wheel_L.setPWM(m_balancer->getPwmLeft());
    m_wheel_R.setPWM(m_balancer->getPwmRight());

    // ブレーキは解除
    m_wheel_L.setBrake(false);
    m_wheel_R.setBrake(false);
}

/**
 * バランス走行に必要なものをリセットする
 */
void InvertedWalker::init()
{
    // モータエンコーダをリセットする
    m_wheel_L.reset();
    m_wheel_R.reset();

    // 倒立振子制御初期化
    int offset = m_gyro_sensor.getAnglerVelocity();
    m_balancer->init(offset);
}

/**
 * PWM値を設定する
 * @param forward 前進値
 * @param turn    旋回値
 */
void InvertedWalker::setCommand(int forward, int turn)
{
    m_forward = forward;
    m_turn = turn;
}