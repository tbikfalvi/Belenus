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
    char    cPrevStatus;
    char    cCurrStatus;
    bool    bInfraModul;
    bool    bJumpNextStatus;
    int     nTimeStatusCounter;
    int     nTimeStatusMain;
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

    int getCountAvailablePorts();
    int getComPort( int p_inIndex );

    void init( int p_nPort = 0 );
    bool isHardwareConnected( void );
    bool checkHardwarePanel( int p_nPanelIndex );
    void closeCommunication( void );

    void setTestMode( bool bMode );

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
    bool setMainActionTime( const int p_nIndex, const int p_nTime, BYTE *p_byStatus, bool p_bSend = false );
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

private:
    vector<typ_panel_data>   pPanel;
    vector<typ_LED_Modul>    pModul;
    vector<int>              pComPorts;
    unsigned char            bySerial_Error;
    unsigned char            byHwWdtCounter;
    unsigned char            byLedModulKikapcsTimer;
    unsigned char            byLedModulOlvasasiHiba;
    unsigned char            byLedModulUjraindulas;
    HANDLE                   m_hPort;
    DWORD                    BaudRate;
    DWORD                    Parity;
    DWORD                    m_dwBaudRate;
    DWORD                    m_dwParity;
    WORD                     wRelay_mem;
    BOOL                     bEnableIRQ_Msg;
    char                     chModulMessage;
    bool                     m_bPortOpened;
    bool                     bTest;
    bool                     bSendToModulPower_ON;
    bool                     bSendToModulPower_OFF;
    int                      PortNumber;
    int                      nHWModuleCount;         // Hardware-ben a kezelendo panel-ek szama,
    WORD                     m_wRelay;
    bool                     m_bCommunicationStopped;

    void GetAvailableCommPorts();

    bool SP_InitCommunication( int p_inPort=0, DWORD p_dwBaudRate=0, DWORD p_dwParity=0 );
    bool SP_Open( bool bSync=true );
    void SP_Close();

    void HW_ModulInit();
    void HW_EmptyBuffer();

    bool HW_ReadMessage( char *chMessage, int *nHossz, int nTimeOut );
    bool HW_SendModulMessage( char *chMessage, int nSendHossz, unsigned char byCim, BYTE *byStatus );
    bool HW_SendRelayMessage( char *chMessage, int nSendHossz, unsigned char *byData=NULL );
    BOOL HW_ReadEEProm( unsigned char byStartAddress, char *chMessage );
    BOOL HW_WriteEEProm( unsigned char byStartAddress, char *chMessage );

    unsigned char GetHWModuleStatus( unsigned char byCim );

    bool SP_ReadMessage( char *Message, int *nLength );
    bool SP_SendMessage( char *Message, int nLength );

    bool Receive( unsigned int bytesNumber,char *bytesArray, unsigned long *readNumber, OVERLAPPED *overLapped=NULL );
    bool Send(unsigned int bytesNumber,char *bytesArray);

    bool ShowLastWindowsError(void);
};

#endif // COMMUNICATION_SERIAL_H
