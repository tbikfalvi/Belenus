//====================================================================================
//
// Belenus Kliens alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : communication_rfid.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
// RFID kommunikacio kezeles
//====================================================================================

#include "belenus.h"
#include "communication_rfid.h"

//====================================================================================
cCommRFID::cCommRFID()
//------------------------------------------------------------------------------------
{
    m_hPort         = NULL;
    m_nCommPort     = 0;
    m_bPortOpened   = false;
}

//====================================================================================
cCommRFID::~cCommRFID()
//------------------------------------------------------------------------------------
{
    if( m_bPortOpened )
    {
        _portClose();
    }
}

//====================================================================================
void cCommRFID::init(int p_nCommPort)
//------------------------------------------------------------------------------------
{
    m_nCommPort = p_nCommPort;

    if( _portOpen() )
    {
        m_bPortOpened = true;
    }
}

//====================================================================================
bool cCommRFID::openRFIDConnection()
//------------------------------------------------------------------------------------
{
    return _portOpen();
}

//====================================================================================
void cCommRFID::closeRFIDConnection()
//------------------------------------------------------------------------------------
{
    _portClose();
}

//====================================================================================
bool cCommRFID::isRFIDConnected()
//------------------------------------------------------------------------------------
{
    return m_bPortOpened;
}

//====================================================================================
QString cCommRFID::readRFID()
//------------------------------------------------------------------------------------
{
    QString qsRet = "";
    char    chSerialIn[2048];
    int     nHossz = 0;

    memset( chSerialIn, 0, sizeof(chSerialIn) );

    if( _readMessage( chSerialIn, &nHossz ) )
    {
        //g_obLogger(cSeverity::DEBUG) << "[RFID] Read: '" << chSerialIn << "' length: " << nHossz << EOM;

        qsRet = QString( chSerialIn );

        if( qsRet.length() != nHossz )
        {
            // No ending zero in chSerialIn
            qsRet = "";
        }
    }
    else
    {
        closeRFIDConnection();
    }

    return qsRet;
}

//====================================================================================
bool cCommRFID::_portOpen()
//------------------------------------------------------------------------------------
{
    cTracer obTrace( "cCommRFID::portOpen" );

    if( m_nCommPort < 1 ) return false;

    char portName[20];

    memset( portName, 0, sizeof(portName) );
    sprintf( portName, "\\\\.\\COM%d", m_nCommPort );

    g_obLogger(cSeverity::DEBUG) << QString("[RFID] Open serial connection with CreateFile - port: %1").arg(portName) << EOM;
    m_hPort = CreateFile( portName,                      // port name
                          GENERIC_READ | GENERIC_WRITE,  // access mode
                          0,                             // share mode
                          NULL,                          // default security attributes
                          OPEN_EXISTING,                 // creation mode
                          FILE_ATTRIBUTE_NORMAL,         // synchronous mode
                          NULL );                        // no file model

    if ( m_hPort == INVALID_HANDLE_VALUE )
    {
        g_obLogger(cSeverity::DEBUG) << QString("[RFID] FAILED") << EOM;
        m_hPort = NULL;
        return false;
    }
    g_obLogger(cSeverity::DEBUG) << QString("[RFID] SUCCEEDED") << EOM;

    DCB dcb;

    g_obLogger(cSeverity::DEBUG) << QString("[RFID] GetCommState") << EOM;
    if ( !GetCommState( m_hPort, &dcb ) )
    {
        g_obLogger(cSeverity::DEBUG) << QString("[RFID] FAILED") << EOM;
       ::CloseHandle(m_hPort);
       return false;
    }
    g_obLogger(cSeverity::DEBUG) << QString("[RFID] SUCCEEDED") << EOM;

 // dcb.DCBlength;                         // sizeof(DCB)
    dcb.BaudRate = 9600;                   // current baud rate
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
    dcb.Parity = 0;                        // 0-4=no,odd,even,mark,space
    dcb.StopBits = 0;                      // 0,1,2 = 1, 1.5, 2
 // dcb.XonChar;                           // Tx and Rx XON character
 // dcb.XoffChar;                          // Tx and Rx XOFF character
 // dcb.ErrorChar;                         // error replacement character
 // dcb.EofChar;                           // end of input character
 // dcb.EvtChar;                           // received event character
 // dcb.wReserved1;                        // reserved; do not use

    g_obLogger(cSeverity::DEBUG) << QString("[RFID] SetCommState") << EOM;
    if ( !SetCommState( m_hPort, &dcb ) )
    {
        g_obLogger(cSeverity::DEBUG) << QString("[RFID] FAILED") << EOM;
       //throw ( EXCEPTION_SET_COM_PORT_CONFIG );
       ::CloseHandle(m_hPort);
       return false;
    }
    g_obLogger(cSeverity::DEBUG) << QString("[RFID] SUCCEEDED") << EOM;

    g_obLogger(cSeverity::DEBUG) << QString("[RFID] SetupComm") << EOM;
    SetupComm( m_hPort, 512, 512 );

    COMMTIMEOUTS   timeout;

    g_obLogger(cSeverity::DEBUG) << QString("[RFID] SetCommTimeouts") << EOM;
    memset( (char*)&timeout, 0, sizeof(COMMTIMEOUTS) );
    timeout.ReadIntervalTimeout = MAXDWORD;
    SetCommTimeouts( m_hPort, &timeout );

    m_bPortOpened = true;

    return true;
}

//====================================================================================
void cCommRFID::_portClose()
//------------------------------------------------------------------------------------
{
    m_bPortOpened = false;
    if (m_hPort != NULL && !CloseHandle(m_hPort))
    {
        //throw ( EXCEPTION_CLOSE_COM_PORT );
    }
    m_hPort = NULL;
}

//====================================================================================
bool cCommRFID::_readMessage( char *Message, int *nLength )
//------------------------------------------------------------------------------------
{
   bool bRet = false;

   unsigned long readNumber;

   bRet = _receive ( 512, Message, &readNumber );
   *nLength = readNumber;

   return bRet;
}

//====================================================================================
bool cCommRFID::_receive( unsigned int   bytesNumber,
                          char*          bytesArray,
                          unsigned long* readNumber,
                          OVERLAPPED*    overLapped )
//------------------------------------------------------------------------------------
{
    if( !m_hPort )
    {
        return false;
    }

    if( !ReadFile( m_hPort,         // port handle
                   bytesArray,      // read buffer
                   bytesNumber,     // bytes to read
                   readNumber,      // read bytes
                   overLapped ) )   // overlapped infos
    {
        //ShowLastWindowsError();
        return false;
    }
    else if( !(*readNumber) )
    {
        return true;
    }

    return true;
}

