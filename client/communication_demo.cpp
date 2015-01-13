#include "communication_demo.h"

CS_Communication_Demo::CS_Communication_Demo() : CS_Communication()
{
    m_stCustomCaption = "DEMO";
    m_inPanelCount    = 12;
}

CS_Communication_Demo::~CS_Communication_Demo()                             {}
int CS_Communication_Demo::getCountAvailablePorts()                         { return 0; }
int CS_Communication_Demo::getComPort( int )                                { return -1; }
void CS_Communication_Demo::init( int )                                     {}
bool CS_Communication_Demo::isHardwareConnected( void )                     { return false; }
bool CS_Communication_Demo::checkHardwarePanel( int )                       { return true; }
void CS_Communication_Demo::closeCommunication( void )                      {}
void CS_Communication_Demo::setTestMode( bool )                             {}
string CS_Communication_Demo::getCustomCaption( void )                      { return m_stCustomCaption; }
int CS_Communication_Demo::getPanelCount( void )                            { return m_inPanelCount; }
void CS_Communication_Demo::setApplicationModuleCount( int )                {}
bool CS_Communication_Demo::setHardwareModuleCount( int )                   { return true; }
int CS_Communication_Demo::getHardwareModuleCount()                         { return m_inPanelCount; }
void CS_Communication_Demo::setRelayOn( const int )                         {}
void CS_Communication_Demo::setRelayOff( const int )                        {}
bool CS_Communication_Demo::getRelayStatus( const int )                     { return true; }
void CS_Communication_Demo::setCurrentCommand( const int, const int )       {}
void CS_Communication_Demo::setCounter( const int, const int, bool )              {}
bool CS_Communication_Demo::setMainActionTime(const int, const int, unsigned char*, bool ) { return true; }
bool CS_Communication_Demo::isHardwareMovedNextStatus( const int )          { return false; }
void CS_Communication_Demo::setHardwareMovedNextStatus( const int )         {}
bool CS_Communication_Demo::isHardwareStopped( const int )                  { return false; }
void CS_Communication_Demo::continueStoppedDevice( const int )              {}
bool CS_Communication_Demo::HW_SetModuleAddress()                           { return false; }
void CS_Communication_Demo::HW_Kezel()                                      {}
bool CS_Communication_Demo::isCommunicationStopped()                        { return false; }
void CS_Communication_Demo::ModuleTurnOn()                                  {}
void CS_Communication_Demo::ModuleTurnOff()                                 {}
void CS_Communication_Demo::EnableModulIRQ()                                {}
void CS_Communication_Demo::DisableModulIRQ()                               {}
