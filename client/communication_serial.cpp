#include "communication_serial.h"
#include "windows.h"

CS_Communication_Serial::CS_Communication_Serial()
{
    /* a kovetkezo ertekeket EEP-bol kell olvasni */
    m_stCustomCaption = "SERIAL";
    m_inPanelCount = 6;
}

CS_Communication_Serial::~CS_Communication_Serial()
{
}

bool CS_Communication_Serial::IsHardwareConnected( void )
{
    return false;
}

string CS_Communication_Serial::getCustomCaption( void )
{
    return m_stCustomCaption;
}

int CS_Communication_Serial::getPanelCount( void )
{
    return m_inPanelCount;
}
