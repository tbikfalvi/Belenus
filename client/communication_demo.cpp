#include "communication_demo.h"

CS_Communication_Demo::CS_Communication_Demo() : CS_Communication()
{
    m_stCustomCaption = "DEMO";
    m_inPanelCount    = 9;
}

CS_Communication_Demo::~CS_Communication_Demo()
{
}

bool CS_Communication_Demo::IsHardwareConnected( void )
{
    return true;
}

string CS_Communication_Demo::getCustomCaption( void )
{
    return m_stCustomCaption;
}

int CS_Communication_Demo::getPanelCount( void )
{
    return m_inPanelCount;
}
