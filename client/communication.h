#ifndef LAYER_COMMUNICATION_H
#define LAYER_COMMUNICATION_H

#include <string>

using namespace std;

class CS_Communication
{
//-------------------------------------------------------------------
public:
    CS_Communication() { m_stCustomCaption = "";
                         m_inPanelCount = 0;
                       };
    ~CS_Communication() {};

    virtual bool IsHardwareConnected( void ) = 0;
    virtual string getCustomCaption( void ) = 0;
    virtual int getPanelCount( void ) = 0;

//-------------------------------------------------------------------
protected:
    string m_stCustomCaption;
    int    m_inPanelCount;
};

#endif // LAYER_COMMUNICATION_H
