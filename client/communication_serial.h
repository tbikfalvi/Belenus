#ifndef COMMUNICATION_SERIAL_H
#define COMMUNICATION_SERIAL_H

#include <string>

#include "communication.h"

using namespace std;

class CS_Communication_Serial : public CS_Communication
{
public:
    CS_Communication_Serial();
    ~CS_Communication_Serial();

    bool IsHardwareConnected( void );
    string getCustomCaption( void );
    int getPanelCount( void );
};

#endif // COMMUNICATION_SERIAL_H
