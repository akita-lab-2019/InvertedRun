/**
 *******************************************************************************
 ** 区間パラメータリストを提供する
 *******************************************************************************
 **/

#ifndef EV3_SECTION_H_
#define EV3_SECTION_H_

// 適用するパラメータを選択する
#define LOW_PARAM
//#define HIGH_PARAM
// #define PID_PARAM

class Section
{
public:
    Section();
    void update(int num);
    int getSectionNum();
    float getForward();
    float getCurvature();
    void getPidParm(float pid_parm[3]);
    float getColorTarget();
    float getDistance();

private:
    int m_section_num = 0;     // 区間番号
    float m_distance;          // 区間長さ[m]
    float m_forward;           // 前進指令速度[%]
    float m_curvature;         // 曲率旋回量[m^-^]
    float m_pid_parm[3];       // PIDパラメータ
    float m_color_target = 23; // カラーセンサの目標値

#ifdef LOW_PARAM
    //////////////////////////LOWパラメータ////////////////////////////
    // 前進量リスト
    float m_forward_list[15] = {105, 105, 105, 103, 95, 95, 98, 103, 105, 110, 106, 103, 100, 105, 110};

    // 曲率旋回量リスト
    float m_curvature_list[15] = {0, 10.7, 0, 10, 14.4, 14.2, 11, 0, 10, 0, 10, 0, 11, 14.1, 0};

    // PID係数リスト
    float m_trace_pid_list[15][3] = {{0.300, 0.000, 0.029},  // 0
                                     {0.460, 0.000, 0.050},  // 1
                                     {0.300, 0.000, 0.038},  // 2
                                     {0.460, 0.000, 0.050},  // 3"
                                     {0.587, 0.100, 0.078},  // 4"
                                     {0.550, 0.100, 0.074},  // 5"
                                     {0.485, 0.000, 0.063},  // 6
                                     {0.300, 0.000, 0.035},  // 7
                                     {0.470, 0.000, 0.069},  // 8"
                                     {0.300, 0.000, 0.029},  // 9
                                     {0.470, 0.000, 0.070},  // 10"
                                     {0.300, 0.000, 0.060},  // 11
                                     {0.485, 0.000, 0.065},  // 12
                                     {0.553, 0.100, 0.060},  // 13"
                                     {0.300, 0.000, 0.038}}; // 14

    // 区間終了距離（それぞれの区間がスタートから何mの地点で終わるのかをリストアップしている）
    float m_distance_list[15] = {0.60, 1.25, 1.70, 2.45, 3.32, 4.00, 4.60, 4.70, 5.53, 6.45, 7.00, 7.23, 7.58, 8.95, 9.25};
///////////////////////////////////////////////////////////////////
#endif

#ifdef HIGH_PARAM
    //////////////////////////HIGHパラメータ////////////////////////////
    // 前進量リスト
    float m_forward_list[15] = {110, 115, 113, 108, 105, 100, 105, 107, 113, 120, 110, 110, 110, 115, 120};

    // 曲率旋回量リスト
    float m_curvature_list[15] = {0, 11, 0, 10, 15.4, 15.3, 11.4, 0, 10.3, 0, 10.3, 10, 11.4, 11.4, 0};

    // PID係数リスト
    float m_trace_pid_list[15][3] = {{0.300, 0.000, 0.023},  // 0
                                     {0.500, 0.000, 0.039},  // 1
                                     {0.280, 0.000, 0.022},  // 2
                                     {0.410, 0.000, 0.032},  // 3
                                     {0.580, 0.000, 0.050},  // 4
                                     {0.575, 0.000, 0.052},  // 5
                                     {0.515, 0.000, 0.040},  // 6
                                     {0.300, 0.000, 0.023},  // 7
                                     {0.450, 0.000, 0.035},  // 8
                                     {0.250, 0.000, 0.019},  // 9
                                     {0.470, 0.000, 0.036},  // 10
                                     {0.480, 0.000, 0.037},  // 11
                                     {0.515, 0.000, 0.040},  // 12
                                     {0.600, 0.000, 0.050},  // 13
                                     {0.300, 0.000, 0.023}}; // 14

    // 区間終了距離（それぞれの区間がスタートから何mの地点で終わるのかをリストアップしている）
    float m_distance_list[15] = {0.60, 1.24, 1.68, 2.43, 3.33, 4.05, 4.63, 4.69, 5.56, 6.45, 7.10, 7.25, 7.53, 8.95, 9.25};
///////////////////////////////////////////////////////////////////
#endif

#ifdef PID_PARAM
    //////////////////////////PIDのみパラメータ////////////////////////////
    // 前進量リスト
    float m_forward_list[15] = {105, 105, 105, 103, 95, 95, 98, 103, 105, 110, 106, 103, 100, 105, 110};

    // 曲率旋回量リスト
    float m_curvature_list[15] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    // PID係数リスト
    float m_trace_pid_list[15][3] = {{0.400, 0.000, 0.046},  // 0
                                     {0.720, 0.000, 0.067},  // 1
                                     {0.400, 0.000, 0.063},  // 2
                                     {0.600, 0.000, 0.063},  // 3"
                                     {0.870, 0.000, 0.083},  // 4"
                                     {0.798, 0.000, 0.070},  // 5"
                                     {0.613, 0.000, 0.062},  // 6
                                     {0.400, 0.000, 0.060},  // 7
                                     {0.530, 0.000, 0.070},  // 8"
                                     {0.400, 0.000, 0.060},  // 9
                                     {0.630, 0.000, 0.065},  // 10"
                                     {0.460, 0.000, 0.058},  // 11
                                     {0.789, 0.000, 0.057},  // 12
                                     {0.820, 0.000, 0.069},  // 13"
                                     {0.400, 0.000, 0.063}}; // 14

    // 区間終了距離（それぞれの区間がスタートから何mの地点で終わるのかをリストアップしている）
    float m_distance_list[15] = {0.60, 1.25, 1.70, 2.45, 3.32, 4.00, 4.60, 4.70, 5.53, 6.45, 7.00, 7.24, 7.58, 8.95, 9.25};
///////////////////////////////////////////////////////////////////
#endif
};

#endif