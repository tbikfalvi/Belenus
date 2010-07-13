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
    void setCounter( const int p_nIndex, const int p_nCounter );
    void setMainActionTime( const int p_nIndex, const int p_nTime );
    bool isHardwareMovedNextStatus( const int p_nIndex );
    void setHardwareMovedNextStatus( const int p_nIndex );
    bool isHardwareStopped( const int p_nIndex );

    void HW_Kezel();
};

#endif // COMMUNICATION_DEMO_H
