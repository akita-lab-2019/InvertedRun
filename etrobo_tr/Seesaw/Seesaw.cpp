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
        setTailAngle(70);
        setAllWheelSpeed(90);
        clock.sleep(160);
        resetAllWheel();
        clock.sleep(3000);

        // 姿勢上げ
        setTailSpeed(50);
        wheel_L.setPWM(-35);
        wheel_R.setPWM(-35);
        clock.sleep(100);

        setTailAngle(95);
        resetAllWheel();
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

            setTailSpeed(60);
            setAllWheelSpeed(-55);
            clock.sleep(220);

            setTailAngle(115);
            resetAllWheel();
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
            clock.sleep(3000);

            // 前進
            setAllWheelSpeed(10);
            clock.sleep(1000);

            // 停止
            resetAllWheel();
            clock.sleep(1000);

            ev3_speaker_play_tone(262, 100);
            m_sequence_num++;
        }
        break;

    // 昇段
    case 4:
        setAllWheelSpeed(-10);

        if (wheel_L.getCount() < -10)
        {
            m_sequence_num++;
        }
        break;

    // 昇段
    case 5:
        setTailSpeed(35);
        setAllWheelSpeed(-11);

        if (wheel_L.getCount() < -20)
        {
            m_sequence_num++;
        }
        break;

    // 昇段
    case 6:
        setTailSpeed(-100);
        setAllWheelSpeed(-11);

        if (getWheelDeg(0) < -32)
        {
            m_sequence_num++;
        }
        break;

    // 昇段
    case 7:
        setAllWheelSpeed(98);
        setTailAngle(90);

        if (getRobotDistance() > 0.2)
        {
            resetAllWheel();
            clock.sleep(3000);

            // 姿勢を上げる
            setTailSpeed(50);
            setAllWheelSpeed(-35);
            clock.sleep(110);

            setTailAngle(108);
            resetAllWheel();
            clock.sleep(1000);
            m_sequence_num++;
        }
        break;

    // シーソー動作1まで前進
    case 8:
        lineRun(0, 4, 1, 25);
        if (abs(gyro_sensor.getAnglerVelocity()) > 40)
        {
            resetAllWheel();
            m_sequence_num++;
        }
        break;

    // シーソー動作1
    case 9:
        setTailSpeed(-58);
        setAllWheelSpeed(39);
        if (getRobotDistance() > 0.02)
        {
            m_sequence_num++;
        }
        break;

    // シーソー動作1
    case 10:
        setTailAngle(55);
        setAllWheelSpeed(39);
        if (getRobotDistance() > 0.10)
        {
            resetAllWheel();
            clock.sleep(3000);
            m_sequence_num++;
        }
        break;

    // シーソー動作2まで後進
    case 11:
        lineRun(0, -4, 1, 6);
        if (abs(gyro_sensor.getAnglerVelocity()) > 40)
        {
            resetAllWheel();
            m_sequence_num++;
        }
        break;

    // シーソー動作2
    case 12:
        setTailSpeed(10);
        setAllWheelSpeed(-37);
        if (getRobotDistance() < -0.02)
        {
            resetAllWheel();
            m_sequence_num++;
        }
        break;

    // シーソー動作2
    case 13:
        setTailAngle(108);
        setAllWheelSpeed(-37);
        if (getRobotDistance() < -0.10)
        {
            resetAllWheel();
            clock.sleep(3000);
            m_sequence_num++;
        }
        break;

    // シーソー動作3まで前進
    case 14:
        lineRun(0, 4, 1, 25);
        if (abs(gyro_sensor.getAnglerVelocity()) > 40)
        {
            resetAllWheel();
            m_sequence_num++;
        }
        break;

    // シーソー動作3
    case 15:
        setTailSpeed(-58);
        setAllWheelSpeed(37);
        if (getRobotDistance() > 0.02)
        {
            m_sequence_num++;
        }
        break;

    // シーソー動作3
    case 16:
        setTailAngle(55);
        setAllWheelSpeed(39);
        if (getRobotDistance() > 0.10)
        {
            resetAllWheel();
            clock.sleep(3000);
            m_sequence_num++;
        }
        break;

    // 前進
    case 17:
        setAllWheelSpeed(30);
        if (getRobotDistance() > 0.35)
        {
            resetAllWheel();
            clock.sleep(3000);
            setTailSpeed(100);
            setAllWheelSpeed(-70);
            clock.sleep(210);

            setTailAngle(95);
            resetAllWheel();
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
        resetAllWheel();
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

void Seesaw::setTailSpeed(int speed)
{
    if (speed > 0)
    {
        m_tail->setAngle(1000);
        m_tail->setMaxSpeed(abs(speed));
    }
    else
    {
        m_tail->setAngle(-1000);
        m_tail->setMaxSpeed(abs(speed));
    }
}

void Seesaw::setTailAngle(int angle)
{
    m_tail->setAngle(angle);
    m_tail->setMaxSpeed(100);
}

void Seesaw::setAllWheelSpeed(int speed)
{
    wheel_L.setPWM(speed);
    wheel_R.setPWM(speed);
}

void Seesaw::resetAllWheel()
{
    wheel_L.reset();
    wheel_R.reset();
}
