//====================================================================================
//
// Belenus Kliens alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : communication_rfid.h
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
// RFID kommunikacio kezeles
//====================================================================================

#ifndef COMMUNICATION_RFID_H
#define COMMUNICATION_RFID_H

//====================================================================================

#include <QString>

//====================================================================================

#include <windows.h>

//====================================================================================

using namespace std;

//====================================================================================
class cCommRFID
{
public:

    cCommRFID();
    ~cCommRFID();

    void                     init( int p_nCommPort );
    bool                     openRFIDConnection();
    void                     closeRFIDConnection();
    bool                     isRFIDConnected();
    QString                  readRFID();

private:

    HANDLE                   m_hPort;
    int                      m_nCommPort;
    bool                     m_bPortOpened;

    bool                    _portOpen();
    void                    _portClose();
    bool                    _readMessage( char *Message, int *nLength );
    bool                    _receive( unsigned int bytesNumber, char *bytesArray, unsigned long *readNumber, OVERLAPPED *overLapped = NULL );

};

#endif
