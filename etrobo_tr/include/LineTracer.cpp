#include "LineTracer.h"

/**
 * コンストラクタ
 * @param lineMonitor    ライン判定
 * @param invertedWalker 倒立走行
 */
LineTracer::LineTracer(RobotInfo *robot_info,
                       Section *section,
                       TailWalker *tail_walker,
                       PID *pid,
                       Balancer *balancer,
                       ev3api::Motor &wheel_L,
                       ev3api::Motor &wheel_R)
    : m_robot_info(robot_info),
      m_section(section),
      m_tail_walker(tail_walker),
      m_pid(pid),
      m_balancer(balancer),
      m_wheel_L(wheel_L),
      m_wheel_R(wheel_R),
      m_is_initialized(false)
{
}

void LineTracer::init()
{
    // モータエンコーダをリセットする
    m_wheel_L.reset();
    m_wheel_R.reset();

    // 倒立振子制御初期化
    int offset = m_robot_info->getPitchVel();
    m_balancer->init(offset);

    update();
}

/**
 * 初期化する
 */
void LineTracer::update()
{
    m_is_inverted = m_section->isInverted();
    m_forward = m_section->getForward();
    m_curvature = m_section->getCurvature();
    m_section->getPidParm(m_pid_parm);
    m_color_target = m_section->getColorTarget();
    m_pid->init(m_pid_parm[0], m_pid_parm[1], m_pid_parm[2]);
}

/**
 * ライントレースする
 */
void LineTracer::run()
{
    if (m_is_initialized == false)
    {
        init();
        m_is_initialized = true;
    }

    // float direction = m_pid->calculate(0, m_robot_info->getBrightnessGap());
    float direction = -1.2 * m_curvature;
    if (m_is_inverted)
    {
        invertedRun(m_forward, direction);
    }
    else
    {
        tailRun(m_forward, direction);
    }
}

/**
 * ライントレースする
 */
void LineTracer::invertedRun(int forward_v, int turn_v)
{
    // 倒立走行に必要な各種データを取得
    int angle = m_robot_info->getPitchVel();                   // ジャイロセンサ値
    int wheel_cnt_R = m_robot_info->getWheelPos(RobotInfo::L); // 右モータ回転角度
    int wheel_cnt_L = m_robot_info->getWheelPos(RobotInfo::R); // 左モータ回転角度
    int battery = m_robot_info->getBatteryVoltage() * 1000;

    // 並進と旋回の指令値，各種データ与えてホイールの指令値を算出させる
    m_balancer->setCommand(forward_v, turn_v);
    m_balancer->update(angle, wheel_cnt_R, wheel_cnt_L, battery);

    // 左右モータに回転を指示する
    m_wheel_L.setPWM(m_balancer->getPwmLeft());
    m_wheel_R.setPWM(m_balancer->getPwmRight());

    // ブレーキは解除
    m_wheel_L.setBrake(false);
    m_wheel_R.setBrake(false);
}

/**
 * ライントレースする
 */
void LineTracer::tailRun(int forward_v, int turn_v)
{
    m_tail_walker->setCommand(forward_v, turn_v);
    m_tail_walker->update();

    // 左右モータに回転を指示する
    m_wheel_L.setPWM(m_tail_walker->getPwmLeft());
    m_wheel_R.setPWM(m_tail_walker->getPwmRight());

    // ブレーキは解除
    m_wheel_L.setBrake(false);
    m_wheel_R.setBrake(false);
}
