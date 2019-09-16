
#include "BluetoothManager.h"

static FILE *bt = NULL;
/**
 * コンストラクタ
 * @param leftWheel  左モータ
 * @param rightWheel 右モータ
 */
BluetoothManager::BluetoothManager()
{
    bt = ev3_serial_open_file(EV3_SERIAL_BT);
}

/**
 * バランス走行に必要なものをリセットする
 */
void BluetoothManager::init()
{
}

void BluetoothManager::send(char *str)
{
    fprintf(bt, "%s\r\n", str);
}

void BluetoothManager::receive()
{
    m_rcv_char = fgetc(bt);

    switch (m_rcv_char)
    {
    case '0':
        m_start_section_num = 0;
        break;
    case '1':
        m_start_section_num = 1;
        break;
    case '2':
        m_start_section_num = 2;
        break;
    case '3':
        m_start_section_num = 3;
        break;
    case '4':
        m_start_section_num = 4;
        break;
    case '5':
        m_start_section_num = 5;
        break;
    case '6':
        m_start_section_num = 6;
        break;
    case '7':
        m_start_section_num = 7;
        break;
    case '8':
        m_start_section_num = 8;
        break;
    case '9':
        m_start_section_num = 9;
        break;
    case 'a':
        m_start_section_num = 10;
        break;
    case 'b':
        m_start_section_num = 11;
        break;
    case 'c':
        m_start_section_num = 12;
        break;
    case 'd':
        m_start_section_num = 13;
        break;
    case 'e':
        m_start_section_num = 14;
        break;
    case 'l':
        m_start_sig = START_L;
        break;
    case 'r':
        m_start_sig = START_R;
        break;
    case 's':
        m_start_sig = STOP;
        break;
    default:
        m_start_sig = NONE;
        m_start_section_num = 0;
        break;
    }
}

int BluetoothManager::getStartSignal()
{
    return m_start_sig;
}

int BluetoothManager::getStartSectionNum()
{
    return m_start_section_num;
}
