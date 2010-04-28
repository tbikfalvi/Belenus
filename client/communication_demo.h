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

    void HW_Kezel();
};

#endif // COMMUNICATION_DEMO_H
