#ifndef EV3_RECORDER_H_
#define EV3_RECORDER_H_

#include <Clock.h>
#include "LineMonitor.h"
#include "GyroSensor.h"
#include "ColorSensor.h"
#include "PID.h"
#include "Motor.h"
#include "Odometer.h"
#include "TailController.h"
#include "InvertedWalker.h"
#include "ParmAdministrator.h"
#include "stdlib.h"

class Recorder
{
public:
    Recorder(ParmAdministrator *parm);

    void init();
    int getFileNum();
    void writeHeader();
    void record(char *str);

private:
    char m_logfile_name[15];
    ParmAdministrator *m_parm;
};

#endif // EV3_RECORDER_H_
