/**
 *******************************************************************************
 ** 区間パラメータリストを提供する
 *******************************************************************************
 **/

#ifndef EV3_SECTION_H_
#define EV3_SECTION_H_

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
    int m_section_num = 0;       // 区間番号
    float m_distance;            // 区間長さ[m]
    float m_forward;             // 前進指令速度[%]
    float m_curvature;           // 曲率旋回量[m^-1]
    float m_pid_parm[3];         // PIDパラメータ
    float m_color_target = 30.0; // カラーセンサの目標値

    //////////////////////////LOWパラメータ////////////////////////////

    // // 前進量リスト
    // float m_forward_list[15] = {105, 100, 105, 103, 95, 98, 100, 106, 105, 110, 105, 103, 100, 97, 105};

    // // 曲率旋回量リスト
    // float m_curvature_list[4] = {0, 10, 11, 14.2};

    // // PID係数リスト
    // float m_trace_pid_list[4][3] = {{0.375, 0.000, 0.029},
    //                                 {0.440, 0.000, 0.034},
    //                                 {0.498, 0.000, 0.039},
    //                                 {0.538, 0.000, 0.041}};

    // // 区間終了距離（それぞれの区間がスタートから何mの地点で終わるのかをリストアップしている）
    // float m_distance_list[15] = {0.60, 1.25, 1.70, 2.45, 3.32, 4.00, 4.60, 4.70, 5.53, 6.45, 7.00, 7.032, 7.58, 8.95, 9.25};
    // // float m_distance_list[15] = {0.60, 1.25, 1.70, 2.45, 3.30, 4.00, 4.60, 4.70, 5.55, 6.45, 7.00, 7.05, 7.60, 8.95, 0.3};

    // // 曲率分類リスト（それぞれの区間がその曲率分類に属しているかをリストアップしている）
    // int m_is_curve_list[15] = {0, 2, 0, 1, 3, 3, 2, 0, 1, 0, 1, 0, 2, 3, 0};

    ///////////////////////////////////////////////////////////////////

    //////////////////////////HIGHパラメータ////////////////////////////

    // 前進量リスト
    float m_forward_list[15] = {110, 115, 113, 110, 105, 105, 110, 114, 116, 120, 115, 110, 110, 115, 120};

    // 曲率旋回量リスト
    float m_curvature_list[5] = {0, 10.5, 11.4, 15.6, 10};

    // PID係数リスト
    float m_trace_pid_list[5][3] = {{0.200, 0.000, 0.016},
                                    {0.400, 0.000, 0.031},
                                    {0.450, 0.000, 0.035},
                                    {0.570, 0.000, 0.044},
                                    {0.500, 0.000, 0.039}};

    // 区間終了距離（それぞれの区間がスタートから何mの地点で終わるのかをリストアップしている）
    float m_distance_list[15] = {0.60, 1.24, 1.68, 2.43, 3.32, 4.00, 4.60, 4.75, 5.56, 6.45, 7.05, 7.24, 7.56, 8.95, 9.25};
    // float m_distance_list[15] = {0.60, 1.25, 1.70, 2.45, 3.30, 4.00, 4.60, 4.70, 5.55, 6.45, 7.00, 7.05, 7.60, 8.95, 0.3};

    // 曲率分類リスト（それぞれの区間がその曲率分類に属しているかをリストアップしている）
    int m_is_curve_list[15] = {0, 2, 0, 1, 3, 3, 2, 0, 1, 0, 1, 4, 3, 3, 0};

    ///////////////////////////////////////////////////////////////////
};

#endif