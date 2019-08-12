#include "ParmAdministrator.h"

ParmAdministrator::ParmAdministrator()
{
}

/**
 * SDカードのルートにある設定ファイルから各パラメータを読みだす
 */
void ParmAdministrator::readParm()
{
    FILE *fp;
    char str_data[32];

    fp = fopen("param.txt", "r");

    if (fp != NULL)
    {
        for (int i = 0; i < 40; i++)
        {
            fgets(str_data, 30, fp);
            for (int j = 0; j < 20; j++)
            {
                str_data[j] = ' ';
            }
            m_parm_data[i] = atof(str_data);
        }
    }
    fclose(fp); // ファイルを閉じる

    // 各変数に代入
    int i = 0;
    color_sensor_targrt[0] = 25;
    color_sensor_targrt[1] = 25;
    trace_pid[0][0] = 0.60;
    trace_pid[0][1] = 0.045;
    trace_pid[0][2] = 0.065;
    trace_pid[1][0] = 0.60;
    trace_pid[1][1] = 0.045;
    trace_pid[1][2] = 0.065;
    trace_pid[2][0] = 0.60;
    trace_pid[2][1] = 0.045;
    trace_pid[2][2] = 0.065;
    trace_pid[3][0] = 0.60;
    trace_pid[3][1] = 0.045;
    trace_pid[3][2] = 0.065;
    trace_pid[4][0] = 1.200;
    trace_pid[4][1] = 0.200;
    trace_pid[4][2] = 0.200;
    curvature[0] = 0;
    curvature[1] = 0;
    curvature[2] = 5;
    curvature[3] = 10;
    curvature[4] = 15;
    forward[0] = 90;
    forward[1] = 120;
    forward[2] = 120;
    forward[3] = 100;
    forward[4] = 75;
    forward[5] = 75;
    forward[6] = 120;
    forward[7] = 120;
    forward[8] = 120;
    forward[9] = 120;
    forward[10] = 120;
    forward[11] = 120;
    forward[12] = 120;
    forward[13] = 120;
    forward[14] = 120;
    forward[15] = 120;
    forward[16] = 120;
    forward[17] = 120;
}