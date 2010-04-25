//====================================================================================
//
// Belenus Kliens alkalmazas © Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : communication_serial.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Ballok Peter, Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#include <stdio.h>

#include "communication_serial.h"
#include "communication_defines.h"
#include "windows.h"

//====================================================================================
//====================================================================================
// PUBLIKUS FUGGVENYEK
//====================================================================================
//====================================================================================

//====================================================================================
// CS_Communication_Serial::
//------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------
CS_Communication_Serial::CS_Communication_Serial()
{
    /* a kovetkezo ertekeket EEP-bol kell olvasni */
    m_stCustomCaption = "SERIAL";
    m_inPanelCount = 6;
}
//====================================================================================
// CS_Communication_Serial::
//------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------
CS_Communication_Serial::~CS_Communication_Serial()
{
}
//====================================================================================
// CS_Communication_Serial::
//------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------
void CS_Communication_Serial::init( int p_nPort )
{
    if( p_nPort != 0 )
        SP_InitCommunication( p_nPort );
    else
        SP_InitCommunication();

    SP_Open();
}
//====================================================================================
// CS_Communication_Serial::
//------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------
bool CS_Communication_Serial::isHardwareConnected( void )
{
    return false;
}
//====================================================================================
// CS_Communication_Serial::
//------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------
void CS_Communication_Serial::closeCommunication( void )
{
    if( m_bPortOpened )
        SP_Close();
}
//====================================================================================
// CS_Communication_Serial::
//------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------
string CS_Communication_Serial::getCustomCaption( void )
{
    return m_stCustomCaption;
}
//====================================================================================
// CS_Communication_Serial::
//------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------
int CS_Communication_Serial::getPanelCount( void )
{
    return m_inPanelCount;
}

//====================================================================================
//====================================================================================
// PRIVAT FUGGVENYEK
//====================================================================================
//====================================================================================

//====================================================================================
// CS_Communication_Serial::
//------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------
bool CS_Communication_Serial::SP_InitCommunication( int p_inPort, DWORD p_dwBaudRate, DWORD p_dwParity )
{
   if( pComPorts.size() < 1 )
       return false;

   if( p_inPort )
      PortNumber = p_inPort;
   else
      PortNumber = pComPorts[0];

   if( p_dwBaudRate )
      BaudRate = p_dwBaudRate;
   else
      BaudRate = m_dwBaudRate;

   if( p_dwParity )
      Parity = p_dwParity;
   else
      Parity = m_dwParity;

   return true;
}
//====================================================================================
// CS_Communication_Serial::
//------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------
bool CS_Communication_Serial::SP_Open( bool bSync )
{
   char portName[20];

   memset( portName, 0, sizeof(portName) );
   sprintf( portName, "COM%d", PortNumber );

   if( bSync )
   {
      m_hPort = CreateFile( portName,              // port name
                            GENERIC_READ | GENERIC_WRITE,  // access mode
                            0,                             // share mode
                            NULL,                          // default security attributes
                            OPEN_EXISTING,                 // creation mode
                            FILE_ATTRIBUTE_NORMAL,         // synchronous mode
                            NULL );                        // no file model
   }
   else
   {
      m_hPort = CreateFile( portName,              // port name
                            GENERIC_READ | GENERIC_WRITE,  // access mode
                            0,                             // share mode
                            NULL,                          // default security attributes
                            OPEN_EXISTING,                 // creation mode
                            FILE_FLAG_OVERLAPPED,          // asynchronous mode
                            NULL );                        // no file model
   }

        if ( m_hPort == INVALID_HANDLE_VALUE )
   {
      m_hPort = NULL;
      return false;
   }

   DCB dcb;

   if ( !GetCommState( m_hPort, &dcb ) )
   {
      ::CloseHandle(m_hPort);
      return false;
   }

// dcb.DCBlength;                         // sizeof(DCB)
   dcb.BaudRate = BaudRate;               // current baud rate
   dcb.fBinary = 1;                       // binary mode, no EOF check
   dcb.fParity = 1;                       // enable parity checking
   dcb.fOutxCtsFlow = 0;                  // CTS output flow control
   dcb.fOutxDsrFlow = 0;                  // DSR output flow control
// dcb.fDtrControl:2;                     // DTR flow control type
// dcb.fDsrSensitivity:1;                 // DSR sensitivity
// dcb.fTXContinueOnXoff:1;               // XOFF continues Tx
   dcb.fOutX = 0;                         // XON/XOFF out flow control
   dcb.fInX = 0;                          // XON/XOFF in flow control
// dcb.fErrorChar: 1;                     // enable error replacement
// dcb.fNull: 1;                          // enable null stripping
   dcb.fRtsControl = RTS_CONTROL_DISABLE; // RTS flow control
// dcb.fAbortOnError:1;                   // abort reads/writes on error
// dcb.fDummy2:17;                        // reserved
// dcb.wReserved;                         // not currently used
// dcb.XonLim;                            // transmit XON threshold
// dcb.XoffLim;                           // transmit XOFF threshold
   dcb.ByteSize = 8;                      // number of bits/byte, 4-8
   dcb.Parity = Parity;                   // 0-4=no,odd,even,mark,space
   dcb.StopBits = 0;                      // 0,1,2 = 1, 1.5, 2
// dcb.XonChar;                           // Tx and Rx XON character
// dcb.XoffChar;                          // Tx and Rx XOFF character
// dcb.ErrorChar;                         // error replacement character
// dcb.EofChar;                           // end of input character
// dcb.EvtChar;                           // received event character
// dcb.wReserved1;                        // reserved; do not use

   if ( !SetCommState( m_hPort, &dcb ) )
   {
      //throw ( EXCEPTION_SET_COM_PORT_CONFIG );
      ::CloseHandle(m_hPort);
      return false;
   }

   SetupComm( m_hPort, 512, 512 );

   COMMTIMEOUTS   timeout;

   memset( (char*)&timeout, 0, sizeof(COMMTIMEOUTS) );
   timeout.ReadIntervalTimeout = MAXDWORD;
   SetCommTimeouts( m_hPort, &timeout );

   m_bPortOpened = true;

   return true;
}
//====================================================================================
// CS_Communication_Serial::
//------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------
void CS_Communication_Serial::SP_Close()
{
    m_bPortOpened = false;
    if (m_hPort != NULL && !CloseHandle(m_hPort))
    {
        //throw ( EXCEPTION_CLOSE_COM_PORT );
    }
    m_hPort = NULL;
}
