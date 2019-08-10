
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
InvertedWalker::InvertedWalker(RobotInfo *robot_info,
                               ev3api::Motor &wheel_L,
                               ev3api::Motor &wheel_R,
                               Balancer *balancer)
    : m_robot_info(robot_info),
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
    int angle = m_robot_info->getPitchVel();                   // ジャイロセンサ値
    int wheel_cnt_R = m_robot_info->getWheelPos(RobotInfo::L); // 右モータ回転角度
    int wheel_cnt_L = m_robot_info->getWheelPos(RobotInfo::R); // 左モータ回転角度
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

//*****************************************************************************
// 関数名 : backlash_cancel
// 引数 : lpwm (左モーターPWM値 ※前回の出力値)
//        rpwm (右モーターPWM値 ※前回の出力値)
//        lenc (左モーターエンコーダー値)
//        renc (右モーターエンコーダー値)
// 返り値 : なし
// 概要 : 直近のPWM値に応じてエンコーダー値にバックラッシュ分の値を追加します。
//*****************************************************************************
void backlash_cancel(signed char lpwm, signed char rpwm, int32_t *lenc, int32_t *renc)
{
    const int BACKLASHHALF = 4; // バックラッシュの半分[deg]

    if (lpwm < 0)
        *lenc += BACKLASHHALF;
    else if (lpwm > 0)
        *lenc -= BACKLASHHALF;

    if (rpwm < 0)
        *renc += BACKLASHHALF;
    else if (rpwm > 0)
        *renc -= BACKLASHHALF;
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
    int offset = m_robot_info->getPitchVel();
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