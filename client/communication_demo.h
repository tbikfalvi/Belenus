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
    void closeCommunication( void );

    void SetTestMode( bool );

    string getCustomCaption( void );
    int getPanelCount( void );

    void SetApplicationModuleCount( int nCount );
    bool SetHardwareModuleCount( int nCount );
    int GetHardwareModuleCount();

    void HW_Kezel();
};

#endif // COMMUNICATION_DEMO_H
