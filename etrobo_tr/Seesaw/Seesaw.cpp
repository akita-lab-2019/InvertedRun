#include "Seesaw.h"

extern ev3api::Clock clock;
extern ev3api::GyroSensor gyro_sensor;
extern ev3api::Motor wheel_L;
extern ev3api::Motor wheel_R;

/**
 * コンストラクタ
 */
Seesaw::Seesaw(LineTracer *line_tracer, TailController *tail)
    : m_line_tracer(line_tracer), m_tail(tail)
{
}

/**
 * 初期化する
 */
void Seesaw::init()
{
}

/**
 * 情報を更新する
 */
void Seesaw::update()
{
}

/**
 * 走行を行う
 */
long start_time = 0;
void Seesaw::run()
{
    float seesaw_dis = getRobotDistance() - m_seesaw_start_dis;

    switch (m_sequence_num)
    {
    // ゴール通知
    case 0:
        m_seesaw_start_dis = getRobotDistance();
        m_line_tracer->setPidParm(m_run_pid_param[0]);
        m_sequence_num++;
        break;

    // ライントレース
    case 1:
        lineRun(1, 80, 0, 35);
        if (seesaw_dis > 0.40)
        {
            ev3_led_set_color(LED_ORANGE);
            ev3_speaker_play_tone(262, 100);
            start_time = clock.now();
            m_sequence_num++;
        }
        break;

    // 着地と姿勢上げ
    case 2:
        // 着地
        m_tail->setAngle(70);
        m_tail->setMaxSpeed(40);
        wheel_L.setPWM(90);
        wheel_R.setPWM(90);
        clock.sleep(160);

        wheel_L.reset();
        wheel_R.reset();
        clock.sleep(3000);

        // 姿勢上げ
        m_tail->setAngle(1000);
        m_tail->setMaxSpeed(50);
        wheel_L.setPWM(-35);
        wheel_R.setPWM(-35);
        clock.sleep(100);

        m_tail->setAngle(95);
        m_tail->setMaxSpeed(90);
        wheel_L.reset();
        wheel_R.reset();
        clock.sleep(1000);

        m_sequence_num++;
        break;

    // 前進して車輪を乗り上げる
    case 3:
        // 前進
        lineRun(0, 6, 1, 15);

        // 車輪を乗り上げ後
        if (getRobotDistance() > 0.30)
        {
            ev3_speaker_play_tone(262, 100);

            // // 左右にじたばた
            // for (int i = 0; i < 3; i++)
            // {
            //     wheel_L.setPWM(8);
            //     wheel_R.reset();
            //     clock.sleep(800);
            //     wheel_L.reset();
            //     wheel_R.setPWM(8);
            //     clock.sleep(800);
            // }

            m_tail->setAngle(1000);
            m_tail->setMaxSpeed(60);
            wheel_L.setPWM(-55);
            wheel_R.setPWM(-55);
            clock.sleep(220);

            m_tail->setAngle(115);
            m_tail->setMaxSpeed(90);
            wheel_L.reset();
            wheel_R.reset();
            clock.sleep(1000);

            // 左右にじたばた
            for (int i = 0; i < 2; i++)
            {
                wheel_L.setPWM(8);
                wheel_R.reset();
                clock.sleep(800);
                wheel_L.reset();
                wheel_R.setPWM(8);
                clock.sleep(800);
            }

            m_tail->setAngle(115);
            clock.sleep(3000);

            // 前進
            m_tail->setAngle(115);
            m_tail->setMaxSpeed(100);
            wheel_L.setPWM(10);
            wheel_R.setPWM(10);
            clock.sleep(1000);

            // 停止
            wheel_L.reset();
            wheel_R.reset();
            clock.sleep(1000);

            ev3_speaker_play_tone(262, 100);
            m_sequence_num++;
        }
        break;

    // 昇段
    case 4:
        wheel_L.setPWM(-10);
        wheel_R.setPWM(-10);

        if (wheel_L.getCount() < -10)
        {
            m_sequence_num++;
        }
        break;

    // 昇段
    case 5:
        m_tail->setAngle(1000);
        m_tail->setMaxSpeed(35);
        wheel_L.setPWM(-11);
        wheel_R.setPWM(-11);

        if (wheel_L.getCount() < -20)
        {
            m_sequence_num++;
        }
        break;

    // 昇段
    case 6:
        m_tail->setAngle(-1000);
        m_tail->setMaxSpeed(100);
        wheel_L.setPWM(-11);
        wheel_R.setPWM(-11);

        if (getWheelDeg(0) < -32)
        {
            m_sequence_num++;
        }
        break;

    // 昇段
    case 7:
        wheel_L.setPWM(95);
        wheel_R.setPWM(95);
        m_tail->setAngle(92);
        m_tail->setMaxSpeed(100);

        if (getRobotDistance() > 0.2)
        {
            wheel_L.reset();
            wheel_R.reset();
            clock.sleep(3000);

            m_tail->setAngle(1000);
            m_tail->setMaxSpeed(50);
            wheel_L.setPWM(-35);
            wheel_R.setPWM(-35);
            clock.sleep(110);

            m_tail->setAngle(108);
            m_tail->setMaxSpeed(90);
            wheel_L.reset();
            wheel_R.reset();
            clock.sleep(1000);
            m_sequence_num++;
        }
        break;

    // シーソー動作まで前進
    case 8:
        lineRun(0, 4, 1, 25);
        if (abs(gyro_sensor.getAnglerVelocity()) > 40)
        {
            wheel_L.reset();
            wheel_R.reset();
            m_sequence_num++;
        }
        break;

    // シーソー動作
    case 9:
        m_tail->setAngle(-1000);
        m_tail->setMaxSpeed(58);
        wheel_L.setPWM(39);
        wheel_R.setPWM(39);
        if (getRobotDistance() > 0.02)
        {
            m_sequence_num++;
        }
        break;

    // シーソー動作
    case 10:
        m_tail->setAngle(55);
        m_tail->setMaxSpeed(40);
        wheel_L.setPWM(35);
        wheel_R.setPWM(35);
        if (getRobotDistance() > 0.10)
        {
            wheel_L.reset();
            wheel_R.reset();
            clock.sleep(1500);
            m_sequence_num++;
        }
        break;

    // シーソー動作まで後進
    case 11:
        lineRun(0, -4, 1, 6);
        if (abs(gyro_sensor.getAnglerVelocity()) > 40)
        {
            wheel_L.reset();
            wheel_R.reset();
            m_sequence_num++;
        }
        break;

    // シーソー動作
    case 12:
        m_tail->setAngle(1000);
        m_tail->setMaxSpeed(10);
        wheel_L.setPWM(-37);
        wheel_R.setPWM(-37);
        if (getRobotDistance() < -0.02)
        {
            wheel_L.reset();
            wheel_R.reset();
            m_sequence_num++;
        }
        break;

    // シーソー動作
    case 13:
        m_tail->setAngle(108);
        m_tail->setMaxSpeed(40);
        wheel_L.setPWM(-35);
        wheel_R.setPWM(-35);
        if (getRobotDistance() < -0.10)
        {
            wheel_L.reset();
            wheel_R.reset();
            clock.sleep(1500);
            m_sequence_num++;
        }
        break;

    // シーソー動作まで前進
    case 14:
        lineRun(0, 4, 1, 25);
        if (abs(gyro_sensor.getAnglerVelocity()) > 40)
        {
            wheel_L.reset();
            wheel_R.reset();
            m_sequence_num++;
        }
        break;

    // シーソー動作
    case 15:
        m_tail->setAngle(-1000);
        m_tail->setMaxSpeed(58);
        wheel_L.setPWM(37);
        wheel_R.setPWM(37);
        if (getRobotDistance() > 0.02)
        {
            m_sequence_num++;
        }
        break;

    // シーソー動作
    case 16:
        m_tail->setAngle(55);
        m_tail->setMaxSpeed(40);
        wheel_L.setPWM(35);
        wheel_R.setPWM(35);
        if (getRobotDistance() > 0.10)
        {
            wheel_L.reset();
            wheel_R.reset();
            clock.sleep(3000);
            m_sequence_num++;
        }
        break;

    // 前進
    case 17:
        wheel_L.setPWM(30);
        wheel_R.setPWM(30);
        if (getRobotDistance() > 0.35)
        {
            wheel_L.reset();
            wheel_R.reset();
            clock.sleep(3000);
            m_tail->setAngle(1000);
            m_tail->setMaxSpeed(100);
            wheel_L.setPWM(-70);
            wheel_R.setPWM(-70);
            clock.sleep(210);

            m_tail->setAngle(95);
            m_tail->setMaxSpeed(90);
            wheel_L.reset();
            wheel_R.reset();
            clock.sleep(1000);
            wheel_L.setPWM(-5);
            wheel_R.setPWM(5);
            clock.sleep(500);
            m_sequence_num++;
        }
        break;

    // // 前進
    case 18:
        lineRun(0, 8, 1, 15);

        if (getRobotDistance() > 0.40)
        {
            ev3_speaker_play_tone(262, 1000);
            m_sequence_num++;
        }
        break;

    // 停止
    case 19:
        wheel_L.reset();
        wheel_R.reset();
        break;

    default:
        break;
    }
}

void Seesaw::upBody()
{
    m_tail->setAngle(90);
    m_tail->setMaxSpeed(100);
}

void Seesaw::downBody()
{
    m_tail->setAngle(40);
    m_tail->setMaxSpeed(100);
}

/**
 * ライントレースする
 *
 */
void Seesaw::lineRun(bool is_inverted, int forward, int pid_index, int target)
{
    m_line_tracer->setIsInverted(is_inverted);
    m_line_tracer->setForward(forward);
    m_line_tracer->setCurvature(0);
    m_line_tracer->setColorTarget(target);
    m_line_tracer->run();
}

void Seesaw::landing()
{
}
