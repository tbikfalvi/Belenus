#ifndef COMMUNICATION_DEMO_H
#define COMMUNICATION_DEMO_H

#include <string>

#include "communication.h"

using namespace std;

class CS_Communication_Demo : public CS_Communication
{
public:
    CS_Communication_Demo();
    ~CS_Communication_Demo();

    int getCountAvailablePorts();
    int getComPort( int p_inIndex );

    void init( int p_nPort = 0 );
    bool isHardwareConnected( void );
    bool checkHardwarePanel( int p_nPanelIndex );
    void closeCommunication( void );

    void setTestMode( bool );

    string getCustomCaption( void );
    int getPanelCount( void );

    void setApplicationModuleCount( int nCount );
    bool setHardwareModuleCount( int nCount );
    int getHardwareModuleCount();

    void setRelayOn( const int nRelayCount );
    void setRelayOff( const int nRelayCount );
    bool getRelayStatus( const int nRelayCount );

    void setCurrentCommand( const int p_nIndex, const int p_nCurrentCommand );
    void setCounter(const int p_nIndex, const int p_nCounter , bool p_bUpdateTimer = false );
    bool setMainActionTime(const int p_nIndex, const int p_nTime, unsigned char *p_byStatus, bool p_bSend = false );
    bool isHardwareMovedNextStatus( const int p_nIndex );
    void setHardwareMovedNextStatus( const int p_nIndex );
    bool isHardwareStopped( const int p_nIndex );
    void continueStoppedDevice( const int p_nIndex );

    bool HW_SetModuleAddress();

    void HW_Kezel();

    bool isCommunicationStopped();

    void ModuleTurnOn();
    void ModuleTurnOff();

    void EnableModulIRQ();
    void DisableModulIRQ();
};

#endif // COMMUNICATION_DEMO_H
