#include "LineTracer.h"

extern ev3api::GyroSensor gyro_sensor;
extern ev3api::Motor wheel_L;
extern ev3api::Motor wheel_R;

PID pid_color;
PID pid_angle;

/**
 * コンストラクタ
 * @param lineMonitor    ライン判定
 * @param invertedWalker 倒立走行
 */
LineTracer::LineTracer(Section *section,
                       TailWalker *tail_walker,
                       Balancer *balancer)
    : m_section(section),
      m_tail_walker(tail_walker),
      m_balancer(balancer),
      m_is_initialized(false)
{
}

/**
 * 更新する
 */
void LineTracer::init(int run_course)
{
    m_run_course = run_course;

    // モータエンコーダをリセットする
    wheel_L.reset();
    wheel_R.reset();

    // 倒立振子制御初期化
    m_balancer->init(0);

    // 輝度目標値を設定
    m_color_target = m_section->getColorTarget();
    setBrightnessTarget(m_color_target);

    update();
}

/**
 * 初期化する
 */
void LineTracer::update()
{
    m_forward = m_section->getForward();
    m_curvature = m_section->getCurvature();
    m_section->getPidParm(m_pid_parm);

    pid_color.init(m_pid_parm);
    pid_angle.init(m_pid_parm);
}

/**
 * ライントレースする
 */
void LineTracer::run()
{
    switch (m_section->getRunMethod())
    {
    case 0: // 通常ライントレース
        m_turn = pid_color.calculate(0, getBrightnessGap());
        m_turn += -1.0 * m_curvature; //曲率旋回量を加算
        break;

    case 1: // 角度追従
        m_turn = -pid_angle.calculate(m_section->target_angle_list[(int)((getRobotDistance() - m_section->getStartDistance()) * 100)], getRobotPose(2));
        break;

    default:
        break;
    }

    // コースの識別
    if (m_run_course == 1)
        m_turn *= -1;

    if (m_forward < 0)
        m_turn *= -1;

    // 倒立走行と尻尾走行を振り分ける
    if (m_is_inverted)
    {
        invertedRun(m_forward, m_turn);
    }
    else
    {
        tailRun(m_forward, m_turn);
    }

    // 左右モータに回転を指示する
    wheel_L.setPWM(m_pwm[L]);
    wheel_R.setPWM(m_pwm[R]);

    // ブレーキは解除
    wheel_L.setBrake(false);
    wheel_R.setBrake(false);
}

/**
 * 倒立走行
 */
void LineTracer::invertedRun(int forward_v, int turn_v)
{
    // 倒立走行に必要な各種データを取得
    int angle = gyro_sensor.getAnglerVelocity(); // ジャイロセンサ値
    int wheel_cnt_R = getWheelDeg(0);            // 右モータ回転角度
    int wheel_cnt_L = getWheelDeg(1);            // 左モータ回転角度
    int battery = ev3_battery_voltage_mV();

    // 並進と旋回の指令値，各種データ与えてホイールの指令値を算出させる
    m_balancer->setCommand(forward_v, turn_v);
    m_balancer->update(angle, wheel_cnt_R, wheel_cnt_L, battery);
    m_pwm[L] = m_balancer->getPwmLeft();
    m_pwm[R] = m_balancer->getPwmRight();
}

/**
 * 尻尾走行
 */
void LineTracer::tailRun(int forward_v, int turn_v)
{
    // 並進と旋回の指令値を与えてホイールの指令値を算出させる
    m_tail_walker->setCommand(forward_v, turn_v);
    m_tail_walker->update();
    m_pwm[L] = m_tail_walker->getPwmLeft();
    m_pwm[R] = m_tail_walker->getPwmRight();
}

void LineTracer::setIsInverted(bool is_inverted)
{
    m_is_inverted = is_inverted;
}
void LineTracer::setForward(float forward)
{
    m_forward = forward;
}
void LineTracer::setCurvature(float curvature)
{
    m_curvature = curvature;
}

void LineTracer::setPidParm(float pid_param[3])
{
    m_pid_parm[0] = pid_param[0];
    m_pid_parm[1] = pid_param[1];
    m_pid_parm[2] = pid_param[2];

    pid_color.init(m_pid_parm);
    pid_angle.init(m_pid_parm);
}

void LineTracer::setColorTarget(float target)
{
    m_color_target = target;
    setBrightnessTarget(m_color_target);
}
