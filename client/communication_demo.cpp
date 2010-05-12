#include "communication_demo.h"

CS_Communication_Demo::CS_Communication_Demo() : CS_Communication()
{
    m_stCustomCaption = "DEMO";
    m_inPanelCount    = 9;
}

CS_Communication_Demo::~CS_Communication_Demo()
{
}

int CS_Communication_Demo::getCountAvailablePorts()
{
   return 0;
}

int CS_Communication_Demo::getComPort( int )
{
      return -1;
}

void CS_Communication_Demo::init( int )
{
}

bool CS_Communication_Demo::isHardwareConnected( void )
{
    return true;
}

bool CS_Communication_Demo::checkHardwarePanel( int )
{
    return true;
}

void CS_Communication_Demo::closeCommunication( void )
{
}

void CS_Communication_Demo::setTestMode( bool )
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
void CS_Communication_Demo::setApplicationModuleCount( int )
{
}
bool CS_Communication_Demo::setHardwareModuleCount( int )
{
    return true;
}
int CS_Communication_Demo::getHardwareModuleCount()
{
    return m_inPanelCount;
}
void CS_Communication_Demo::setRelayOn( const int )
{
}
void CS_Communication_Demo::setRelayOff( const int )
{
}
bool CS_Communication_Demo::getRelayStatus( const int )
{
    return true;
}
void CS_Communication_Demo::HW_Kezel()
{
}
