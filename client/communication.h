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

    virtual int getCountAvailablePorts() = 0;
    virtual int getComPort( int p_inIndex ) = 0;

    virtual void init( int p_nPort = 0 ) = 0;
    virtual bool isHardwareConnected( void ) = 0;
    virtual bool checkHardwarePanel( int p_nPanelIndex ) = 0;
    virtual void closeCommunication( void ) = 0;

    virtual void setTestMode( bool bMode ) = 0;

    virtual string getCustomCaption( void ) = 0;
    virtual int getPanelCount( void ) = 0;

    virtual void setApplicationModuleCount( int nCount ) = 0;
    virtual bool setHardwareModuleCount( int nCount ) = 0;
    virtual int getHardwareModuleCount( void ) = 0;

    virtual void HW_Kezel( void ) = 0;
//-------------------------------------------------------------------
protected:
    string m_stCustomCaption;
    int    m_inPanelCount;
};

#endif // LAYER_COMMUNICATION_H
