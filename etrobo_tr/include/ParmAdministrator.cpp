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
    char fname[] = "parm.txt";
    char str_data[256];
    fp = fopen(fname, "r");

    if (fp != NULL)
    {
        for (int i = 0; i < 20; i++)
        {
            fgets(str_data, 256, fp);
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
    forward_v = m_parm_data[i++];
    turn_v = m_parm_data[i++];
    color_sensor_targrt = m_parm_data[i++];
    trace_pid[0] = m_parm_data[i++];
    trace_pid[1] = m_parm_data[i++];
    trace_pid[2] = m_parm_data[i++];
}
