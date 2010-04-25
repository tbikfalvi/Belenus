//====================================================================================
//
// Belenus Kliens alkalmazas © Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : communication_serial.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#ifndef COMMUNICATION_SERIAL_H
#define COMMUNICATION_SERIAL_H

//====================================================================================

#include <windows.h>
#include <vector>
#include <string>

#include "communication.h"

using namespace std;

//====================================================================================

//====================================================================================
//
//------------------------------------------------------------------------------------
typedef struct _typ_panel_data
{
   char  cPrevStatus;
   char  cCurrStatus;
        bool	bInfraModul;
        bool	bJumpNextStatus;
        int	nTimeStatusCounter;
        int	nTimeStatusMain;
} typ_panel_data;
//====================================================================================
//
//------------------------------------------------------------------------------------
typedef struct _typ_LED_Modul
{
   bool  bVan;
   bool  bStop;
   bool  bSendIras;
   bool  bSendStart;
   bool  bSendEnd;
} typ_LED_Modul;

//====================================================================================
class CS_Communication_Serial : public CS_Communication
{
public:
    CS_Communication_Serial();
    ~CS_Communication_Serial();

    void init( int p_nPort = 0 );
    bool isHardwareConnected( void );
    void closeCommunication( void );

    string getCustomCaption( void );
    int getPanelCount( void );

private:
    bool                     m_bPortOpened;
    vector<int>              pComPorts;
    int                      PortNumber;
    DWORD                    BaudRate;
    DWORD                    Parity;
    DWORD                    m_dwBaudRate;
    DWORD                    m_dwParity;
    HANDLE                   m_hPort;

    bool SP_InitCommunication( int p_inPort=0, DWORD p_dwBaudRate=0, DWORD p_dwParity=0 );
    bool SP_Open( bool bSync=true );
    void SP_Close();
};

#endif // COMMUNICATION_SERIAL_H
