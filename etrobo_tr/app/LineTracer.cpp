#include "LineTracer.h"

/**
 * コンストラクタ
 * @param gyro_sensor  ジャイロセンサ
 * @param left_wheel   左モータ
 * @param right_wheel  右モータ
 * @param balancer     バランサ
 * @param line_monitor ラインモニタ
 * @param measurer     オドメトリ計算機
 */
LineTracer::LineTracer(ev3api::GyroSensor &gyro_sensor,
                       ev3api::Motor &left_wheel,
                       ev3api::Motor &right_wheel,
                       Balancer *balancer,
                       LineMonitor *line_monitor,
                       Measurer *measurer)
    : m_gyro_sensor(gyro_sensor),
      m_wheel_L(left_wheel),
      m_wheel_R(right_wheel),
      m_balancer(balancer),
      m_line_monitor(line_monitor),
      m_measurer(measurer)
{
}

/**
 * ライントレースする
 */
void LineTracer::run()
{
    // 初回の呼び出し時のみ，初期化処理を実行
    if (m_is_initialized == false)
    {
        LineTracer::init();
        m_is_initialized = true;
    }

    // 倒立走行に必要な各種データを取得
    int16_t angle = m_gyro_sensor.getAnglerVelocity(); // ジャイロセンサ値
    int wheel_cnt_R = m_wheel_R.getCount();            // 右モータ回転角度
    int wheel_cnt_L = m_wheel_L.getCount();            // 左モータ回転角度
    int battery = ev3_battery_voltage_mV();

    // 旋回指令量を計算
    float direction = 0.6 * m_line_monitor->getGap();

    // 速度指令を送る
    // m_balancer->setCommand(60, (int)direction);
    m_balancer->setCommand(0, 0);

    m_balancer->update(angle, wheel_cnt_R, wheel_cnt_L, battery);

    // 左右モータに回転を指示する
    m_wheel_L.setPWM(m_balancer->getPwmLeft());
    m_wheel_R.setPWM(m_balancer->getPwmRight());

    m_wheel_L.setBrake(false);
    m_wheel_R.setBrake(false);
}

/**
 * バランス走行に必要なものをリセットする
 */
void LineTracer::init()
{
    int offset = m_gyro_sensor.getAnglerVelocity(); // ジャイロセンサ値

    // モータエンコーダをリセットする
    m_wheel_L.reset();
    m_wheel_R.reset();

    // 倒立振子制御初期化
    m_balancer->init(offset);
}
