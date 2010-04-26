#include "communication_demo.h"

CS_Communication_Demo::CS_Communication_Demo() : CS_Communication()
{
    m_stCustomCaption = "DEMO";
    m_inPanelCount    = 9;
}

CS_Communication_Demo::~CS_Communication_Demo()
{
}

void CS_Communication_Demo::init( int )
{
}

bool CS_Communication_Demo::isHardwareConnected( void )
{
    return true;
}
void CS_Communication_Demo::closeCommunication( void )
{
}

string CS_Communication_Demo::getCustomCaption( void )
{
    return m_stCustomCaption;
}

int CS_Communication_Demo::getPanelCount( void )
{
    return m_inPanelCount;
}
void CS_Communication_Demo::SetApplicationModuleCount( int )
{
}
bool CS_Communication_Demo::SetHardwareModuleCount( int )
{
    return true;
}
int CS_Communication_Demo::GetHardwareModuleCount()
{
    return m_inPanelCount;
}
void CS_Communication_Demo::HW_Kezel()
{
}
