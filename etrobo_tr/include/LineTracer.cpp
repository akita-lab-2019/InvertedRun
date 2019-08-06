#include "LineTracer.h"

/**
 * コンストラクタ
 * @param lineMonitor    ライン判定
 * @param invertedWalker 倒立走行
 */
LineTracer::LineTracer(RobotInfo *robot_info,
                       InvertedWalker *inverted_walker,
                       TailWalker *tail_walker,
                       PID *pid)
    : m_robot_info(robot_info),
      m_inverted_walker(inverted_walker),
      m_tail_walker(tail_walker),
      m_pid(pid),
      m_is_initialized(false)
{
}

/**
 * ライントレースする
 */
bool b_flag = 0;
void LineTracer::run()
{
    if (m_is_initialized == false)
    {
        m_inverted_walker->init();
        m_pid->init(0.6, 0.045, 0.065);
        m_is_initialized = true;
    }

    float direction = m_pid->calculate(0, m_robot_info->getBrightnessGap());
    tailRun(20, direction);

    // if (scenario_num == 11)
    // {
    //     m_tail_controller->control(65, 40);
    // }
    // else
    // {
    //     if (scenario_num == 0 && b_flag == 0)
    //     {
    //         b_flag = 1;
    //         m_pid->init(m_parm->trace_pid[is_curve[scenario_num]][0],
    //                     m_parm->trace_pid[is_curve[scenario_num]][1],
    //                     m_parm->trace_pid[is_curve[scenario_num]][2]);
    //     }
    //     sectionRun(m_parm->forward_v[is_curve[scenario_num]]);
    //     if (m_robot_info->getRobotDis() > segment_dis[scenario_num])
    //     {
    //         ev3_speaker_play_tone(NOTE_C4 + scenario_num * 100, 100);
    //         scenario_num++;
    //         m_pid->init(m_parm->trace_pid[is_curve[scenario_num]][0],
    //                     m_parm->trace_pid[is_curve[scenario_num]][1],
    //                     m_parm->trace_pid[is_curve[scenario_num]][2]);
    //     }
    //     m_tail_controller->control(0, 60);

    //     if (scenario_num == 11)
    //     {
    //         if (m_robot_info->getRobotDis() > 9.8)
    //         {
    //             scenario_num++;
    //         }
    //         sectionRun(20);
    //     }

    //     if (scenario_num == 12)
    //     {
    //         long start_time = m_robot_info->getRunTime();
    //         while (m_robot_info->getRunTime() - start_time < 200)
    //         {
    //             m_tail_controller->control(65, 40);
    //             m_wheel_L.setPWM(80);
    //             m_wheel_R.setPWM(80);
    //         }
    //         m_tail_controller->control(65, 40);
    //         m_wheel_L.reset();
    //         m_wheel_R.reset();
    //     }
    // }
}

/**
 * ライントレースする
 */
void LineTracer::invertedRun(int forward_v, int turn_v)
{
    // 速度指令をセット
    m_inverted_walker->setCommand(forward_v, turn_v);

    // 倒立走行を行う
    m_inverted_walker->run();
}

/**
 * ライントレースする
 */
void LineTracer::tailRun(int forward_v, int turn_v)
{
    // 速度指令をセット
    m_tail_walker->setCommand(forward_v, turn_v);

    // 尻尾走行を行う
    m_tail_walker->run();
}
