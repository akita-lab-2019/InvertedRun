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
    // int i = 0;
    color_sensor_targrt[0] = 30;
    color_sensor_targrt[1] = 30;

    trace_pid[0][0] = 0.400;
    trace_pid[0][1] = 0;
    trace_pid[0][2] = 0.031;

    trace_pid[1][0] = 0.492;
    trace_pid[1][1] = 0;
    trace_pid[1][2] = 0.038;

    trace_pid[2][0] = 0.500;
    trace_pid[2][1] = 0;
    trace_pid[2][2] = 0.039;

    trace_pid[3][0] = 0.600;
    trace_pid[3][1] = 0;
    trace_pid[3][2] = 0.040;

    curvature[0] = 0;
    curvature[1] = 10;
    curvature[2] = 11;
    curvature[3] = 15;

    forward[0] = 110;
    forward[1] = 120;
    forward[2] = 120;
    forward[3] = 98;
    forward[4] = 78;
    forward[5] = 75;
    forward[6] = 80;
    forward[7] = 85;
    forward[8] = 90;
    forward[9] = 95;
    forward[10] = 90;
    forward[11] = 85;
    forward[12] = 75;
    forward[13] = 70;
    forward[14] = 120;
    forward[15] = 110;
    forward[16] = 110;
    forward[17] = 110;
}