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

    void init( int p_nPort = 0 );
    bool isHardwareConnected( void );
    void closeCommunication( void );

    string getCustomCaption( void );
    int getPanelCount( void );
};

#endif // COMMUNICATION_DEMO_H
