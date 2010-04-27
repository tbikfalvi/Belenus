//====================================================================================
//
// Belenus Kliens alkalmazas � Pagony Multimedia Studio Bt - 2010
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

    bySerial_Error             = 0;
    byHwWdtCounter             = 0;
    byLedModulKikapcsTimer     = 0;
    byLedModulOlvasasiHiba     = 0;
    byLedModulUjraindulas      = 0;
    m_hPort                    = NULL;
    BaudRate                   = 0;
    Parity                     = 0;
    m_dwBaudRate               = 0;
    m_dwParity                 = 0;
    wRelay_mem                 = 0;
    bEnableIRQ_Msg             = false;
    chModulMessage             = 0;
    m_bPortOpened              = false;
    bTest                      = false;
    bSendToModulPower_ON       = false;
    bSendToModulPower_OFF      = false;
    PortNumber                 = 0;
    nHWModuleCount             = 0;         // Hardware-ben a kezelendo panel-ek szama,

    GetAvailableCommPorts();
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
int CS_Communication_Serial::getCountAvailablePorts()
{
   return pComPorts.size();
}
//====================================================================================
// CS_Communication_Serial::
//------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------
int CS_Communication_Serial::getComPort( int p_inIndex )
{
   if( p_inIndex > -1 && p_inIndex < (int)pComPorts.size() )
      return pComPorts[p_inIndex];
   else
      return -1;
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
    char  chMessage[512];
    char  chSerialIn[512];
    int   nRecHossz = 0;
    bool  bRes = false;

    memset( chMessage, 0, sizeof(chMessage) );
    memset( chSerialIn, 0, sizeof(chSerialIn) );

    if( SP_ReadMessage( chSerialIn, &nRecHossz ) )
    {
       if( chSerialIn[ nRecHossz-1 ] == MODUL_IRQ )
       {
          chModulMessage = MODUL_IRQ;
       }
    }

    chMessage[ 0 ]= SEARCH_HW;
    chMessage[ 1 ]= 'H';

    SP_SendMessage( chMessage, 2 );

    bySerial_Error++;
    Sleep( 100 ); //var, hogy a PIC tudjon v�laszolni

    if( HW_ReadMessage( chSerialIn, &nRecHossz, 5  ) )
    {
       if( (chSerialIn[ nRecHossz-2 ] == 'Y') && ((unsigned char) chSerialIn[ nRecHossz-1 ] == HW_SUCCESS) )
       {
          bRes = true;
       }
       byHwWdtCounter = WDT_TIME;
    }

    return bRes;
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
void CS_Communication_Serial::SetTestMode( bool bMode )
{
    bTest = bMode;
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
//---------------------------------------------------------------------------
// SetApplicationModuleCount
//---------------------------------------------------------------------------
// A hardware altal kezelendo szolariumok szamat tartalmazo valtozo beallitasa.
// A fuggvenyt akkor kell egyszer meghivni, ha a hadrver mar mukodik, van
// vele kapcsolat. Ha a program DEMO modban van, vagy problema van a
// kapcsolattal a fuggvenyt nem szabad meghivni, vagy 0 ertekkel kell hivni.
//---------------------------------------------------------------------------
// Parameterek:
//    nCount      - kezelt szolariumok szama
//---------------------------------------------------------------------------
// Visszateresi ertek:
//    nincs
//---------------------------------------------------------------------------
void CS_Communication_Serial::SetApplicationModuleCount( int nCount )
{
   nHWModuleCount = nCount;

   if( nHWModuleCount > 0 )
   {
      for( int i=0; i< nHWModuleCount; i++ )
      {
         typ_LED_Modul  stTemp;

         memset( &stTemp, 0, SIZEOF_MODUL_DATA );
         pModul.push_back( stTemp );
      }
      for( int i=0; i< nHWModuleCount; i++ )
      {
         typ_panel_data    stTemp;

         memset( &stTemp, 0, SIZEOF_PANEL_DATA );
         pPanel.push_back( stTemp );
      }
   }
}
//---------------------------------------------------------------------------
// SetHardwareModuleCount
//---------------------------------------------------------------------------
// A hardware altal kezelt szolariumok szamanak beallitasa a hardverben.
//---------------------------------------------------------------------------
// Parameterek:
//    nCount      - kezelt szolariumok szama
//---------------------------------------------------------------------------
// Visszateresi ertek:
//    TRUE        - ha siker�lt a hardverben a szolariumok szamat
//                  beallitani
//    FALSE       - ha nem sikerult beallitani a hardverben a szolariumok
//                  szamat
//---------------------------------------------------------------------------
bool CS_Communication_Serial::SetHardwareModuleCount( int nCount )
{
   bool  bRet = false;
   char  strEEProm[100];
   char  strCount[3];

   if( nCount > 99 )
      return false;

   memset( strEEProm, 0, sizeof(strEEProm) );
   memset( strCount, 0, sizeof(strCount) );

   sprintf( strCount, "%02d", nCount );

   if( HW_ReadEEProm( 0, strEEProm ) )
   {
      memcpy( strEEProm, strCount, 2 );
      if( HW_WriteEEProm( 0, strEEProm ) )
      {
         bRet = true;
      }
   }

   return bRet;
}
//---------------------------------------------------------------------------
// GetHardwareModuleCount
//---------------------------------------------------------------------------
// A hardware altal kezelt szolariumok szamanak kiolvasasa a hardware eszkozbol.
//---------------------------------------------------------------------------
// Parameterek:
//    nincs
//---------------------------------------------------------------------------
// Visszateresi ertek:
//    -1          - ha a panelok szamat nem sikerult kiolvasni
//    nemnegativ  - a hardverben tarolt szolarium panelok szama
//---------------------------------------------------------------------------
int CS_Communication_Serial::GetHardwareModuleCount()
{
   int   nRet = -1;
   char  strEEProm[100];

   memset( strEEProm, 0, sizeof(strEEProm) );
   if( HW_ReadEEProm( 0, strEEProm ) )
   {
      strEEProm[2] = 0;
      nRet = atoi( strEEProm );
   }

   return nRet;
}
//---------------------------------------------------------------------------
// HW_Kezel
//---------------------------------------------------------------------------
// 300 sec-k�nt a foprogram h�vja meg a fuggvenyt ami lekezeli es
// tovabbkuldi a parancsokat, illetve szolgaltatja a szolariumok allapotat
//---------------------------------------------------------------------------
// Parameterek:
//    nincs
//---------------------------------------------------------------------------
// Visszateresi ertek:
//    nincs
//---------------------------------------------------------------------------
void CS_Communication_Serial::HW_Kezel()
{
   //---------------------------------------------------------------------------
   // Demo illetve helytelen parameterbeallitas eset�n nem foglalkozik a
   // hardware-al es kilep
   //---------------------------------------------------------------------------
   if( nHWModuleCount == 0 )
        return;

   int i;
   int nHossz;
   char chSerialOut[2048];
   char chSerialIn[2048];
   unsigned char byStatus;
    WORD wRelay;

   //---------------------------------------------------
   // Relay �llapotok be�ll�t�sa
   //---------------------------------------------------
    if( !bTest )
    {
        wRelay = 0;
        for( i=0; i<nHWModuleCount; i++ )
        {
            if( pPanel[i].cCurrStatus == STATUS_BARNULAS )
            {
//                MINDIG BE VANNAK KAPCSOLVA BARNULAS ALATT
//          if( !pSzolarium[i]->CsovekKikapcsolva() )
//          {
                if( !pModul[ i ].bStop )  //ha nincs relay stop
                {
                    wRelay |= rel_csovek[(nHWModuleCount-1)*12+i];
                }
//          }
                if( !pPanel[i].bInfraModul )
                {
                    wRelay |= rel_vent[(nHWModuleCount-1)*12+i];
                }
            }
            else if( pPanel[i].cCurrStatus == STATUS_SZAUNAZAS )
            {
                wRelay |= rel_vent[(nHWModuleCount-1)*12+i];
            }
            else if( pPanel[i].cCurrStatus == STATUS_UTOHUTES )
            {
                if( !pPanel[i].bInfraModul )
                {
                    wRelay |= rel_vent[(nHWModuleCount-1)*12+i];
                }
            }
        }
    }

    if( wRelay_mem != wRelay )
    {
        chSerialOut[0] = SET_RELAY;
                chSerialOut[1] = (unsigned char) wRelay;
                chSerialOut[2] = (unsigned char) (wRelay>>8);
        chSerialOut[3] = 0x5a;
        if( HW_SendRelayMessage( chSerialOut, 4, NULL ) )
        {
            wRelay_mem = wRelay;
        }
    }

   //-------------------------------------------------------
   // WDT frissit�s, ha nem volt kikuldve semmi akkor WDT
   //-------------------------------------------------------
   if( byHwWdtCounter > 0 )
    {
        byHwWdtCounter--;
    }
    if( byHwWdtCounter == 0 )
    {
        chSerialOut[0] = CLR_RELAY_TIMER;
        if( HW_SendRelayMessage( chSerialOut, 1, NULL  ) )
        {
            byHwWdtCounter = WDT_TIME;
        }
    }

   //--------------------------------------------------------------------------------------
   // Szoli LED Modul vez�rl�s
   //--------------------------------------------------------------------------------------
   //Ha j� a soros komunik�ci�
   if( bySerial_Error == 0 )
   {
      //�s fel van kapcsolva a LED t�p
      if( (byLedModulKikapcsTimer == 0) && !bTest )
      {
         //Szoli LED Modul IRQ figyel�s
         if( SP_ReadMessage( chSerialIn, &nHossz ) )
         {
              if( chSerialIn[ nHossz-1 ] == MODUL_IRQ ||
                  chModulMessage == MODUL_IRQ )
              {
                  chModulMessage = 0;
                  //mivel Modul IRQ akkor lekapcsolja az IRQ-�zenetet,
                  // m�g le nem k�rdezi a status-okat
                  DisableModulIRQ();

                  // Modul IRQ lekezeles
                  for( i=0; i<nHWModuleCount; i++ )
                  {
                      if( pModul[ i ].bVan )
                      {
                          chSerialOut[0] = SEND_1BYTE_TO_MODUL;
                          chSerialOut[1] = GET_STATUS;
                          if( HW_SendModulMessage( chSerialOut, 2, i, &byStatus ) )
                          {
                             byLedModulOlvasasiHiba = 0;

                             ///////////////////////////////////////////
                             if( byStatus == 0 )
                             {}//Standby
                             ///////////////////////////////////////////
                             else if( (byStatus & 0x03) == 0x02 ) //Stop SW
                             {
                                 if( pPanel[i].cCurrStatus == STATUS_SZAUNAZAS )
                                 {
                                     // Szauna megszak�t�sa, k�vetkez� ciklus ind�t�sa
                                     pPanel[i].bJumpNextStatus = true;
                                 }
                                 else if( pPanel[i].cCurrStatus == STATUS_BARNULAS )
                                 {
                                     pModul[ i ].bStop = true; //Relay kikapcsol�s
                                 }
                             }
                             ///////////////////////////////////////////
                             else if( (byStatus & 0x03) == 0x01 ) //Start SW
                             {
                                 if( pPanel[i].cCurrStatus == STATUS_VETKOZES )
                                 {
                                     // Vetk�z�s megszak�t�sa, k�vetkez� ciklus ind�t�sa
                                     pPanel[i].bJumpNextStatus = true;
                                 }
                                 else if( pPanel[i].cCurrStatus == STATUS_BARNULAS )
                                 {
                                     pModul[ i ].bStop = false; //Relay bekapcsol�s
                                 }
                             }

                             ///////////////////////////////////////////
                             else if( (byStatus & 0x03) == 0x03 ) // Restart, ujraindult
                             {
                                 pModul[ i ].bSendIras = true;
                                 if( (pPanel[i].cCurrStatus == STATUS_BARNULAS) ||
                                     (pPanel[i].cCurrStatus == STATUS_SZAUNAZAS) )
                                 {
                                     pModul[ i ].bSendStart = true;
                                 }
                             }
                          }
                          else//if( pHardware->bSendMessageLEDModul_ReceiveStatus( chSerialOut, 2, i, &byStatus ) )
                          {
                              //nincs v�lasz
                              byLedModulOlvasasiHiba++;
                          }//if( bSendMessageLEDModul_ReceiveStatus( chSerialOut, 2, i, &byStatus ) )
                      }//if( pModul[ i ].bVan )
                  }//for
              }//if( chSerialIn[ nHossz-1 ] == MODUL_IRQ )
              else if( chSerialIn[ nHossz-1 ] == START_MESSAGE )
              {
                  //ujraindult a Relay panel, mindent ki kell k�ldeni
                  wRelay_mem = 0;
              }
         }// nincs LED Modul IRQ

         ////////////////////////////////////////////////////////////////////////
         //Szoli LED Modul vez�rl�s

         //Elen�rzi van-e k�ldenival� adat
         bool bVanKuldeniValoAdat = false;
         for( i=0; i<nHWModuleCount; i++ )
         {
             if( pModul[ i ].bVan )
             {
                 if( pPanel[i].cPrevStatus == STATUS_ALAP && pPanel[i].cCurrStatus == STATUS_VETKOZES )  //vetk�z�s
                 {
                    pModul[ i ].bSendIras = true;
                 }
                 if( pPanel[i].cPrevStatus == STATUS_VETKOZES && pPanel[i].cCurrStatus == STATUS_ALAP )  //esc
                 {
                    pModul[ i ].bSendIras = true;
                 }
                 if( pPanel[i].cPrevStatus == STATUS_VETKOZES && pPanel[i].cCurrStatus == STATUS_SZAUNAZAS )
                 {
                    pModul[ i ].bSendIras = true;
                    pModul[ i ].bSendStart = true;
                 }
                 if( pPanel[i].cPrevStatus == STATUS_VETKOZES && pPanel[i].cCurrStatus == STATUS_BARNULAS )
                 {
                    pModul[ i ].bSendStart = true;
                 }
                 if( pPanel[i].cPrevStatus == STATUS_SZAUNAZAS && pPanel[i].cCurrStatus == STATUS_VARAKOZAS )
                 {
                    pModul[ i ].bSendIras = true;
                    pModul[ i ].bSendStart = true;
                 }
                 if( pPanel[i].cPrevStatus == STATUS_VARAKOZAS && pPanel[i].cCurrStatus == STATUS_BARNULAS )
                 {
                    pModul[ i ].bSendIras = true;
                    pModul[ i ].bSendStart = true;
                 }
                 if( pPanel[i].cPrevStatus == STATUS_BARNULAS  && pPanel[i].cCurrStatus == STATUS_UTOHUTES )
                 {
                    pModul[ i ].bSendEnd = true;
                    pModul[ i ].bStop = false;
                 }
                 pPanel[i].cPrevStatus = pPanel[i].cCurrStatus;
             }
         }

         bVanKuldeniValoAdat = false;
         for( i=0; i<nHWModuleCount; i++ )
         {
             if( pModul[ i ].bVan &&
                 ( pModul[ i ].bSendIras  ||
                   pModul[ i ].bSendStart ||
                   pModul[ i ].bSendEnd    ) )
             {
                 bVanKuldeniValoAdat = true;
                 break;
             }
         }

         //Adatok kik�ld�se a moduloknak
         if( bVanKuldeniValoAdat )
         {
             // Letiltlja az IRQ-t, hogy ne zavarja meg a komunik�ci�t
             DisableModulIRQ();

             bool bHiba = false;
             int nIdo;
             //Kikuldi az adatokat
             for( i=0; i<nHWModuleCount; i++ )
             {
                 if( pModul[ i ].bVan )
                 {
                     /////////////////////////////////////////
                     if( pModul[ i ].bSendIras )
                     {
                         chSerialOut[0] = SEND_3BYTE_TO_MODUL;
                         chSerialOut[1] = SEND_TIME;
                         nIdo = 0;
                         if( pPanel[i].cPrevStatus == STATUS_VETKOZES )
                             nIdo = pPanel[i].nTimeStatusMain;
                         else if( pPanel[i].cPrevStatus == STATUS_BARNULAS )
                             nIdo = pPanel[i].nTimeStatusCounter;
                         else if( pPanel[i].cPrevStatus == STATUS_SZAUNAZAS )
                             nIdo = pPanel[i].nTimeStatusCounter;
                         else if( pPanel[i].cPrevStatus == STATUS_VARAKOZAS )
                             nIdo = pPanel[i].nTimeStatusCounter;

                         chSerialOut[2] = (char)(nIdo/60);
                         chSerialOut[3] = (char)(nIdo%60);

                         if( HW_SendModulMessage( chSerialOut, 4, i, &byStatus ) )
                         {
                             pModul[ i ].bSendIras = false;
                         }
                         else
                            bHiba = true;
                     }
                     /////////////////////////////////////////
                     if( pModul[ i ].bSendStart )
                     {
                         chSerialOut[0] = SEND_1BYTE_TO_MODUL;
                         chSerialOut[1] = SEND_START;
                         if( HW_SendModulMessage( chSerialOut, 2, i, &byStatus ) )
                         {
                             pModul[ i ].bSendStart = false;
                         }
                         else
                            bHiba = true;
                     }
                     /////////////////////////////////////////
                     if( pModul[ i ].bSendEnd )
                     {
                         chSerialOut[0] = SEND_1BYTE_TO_MODUL;
                         chSerialOut[1] = SEND_END;
                         if( HW_SendModulMessage( chSerialOut, 2, i, &byStatus ) )
                         {
                             pModul[ i ].bSendEnd = false;
                         }
                         else
                            bHiba = true;
                     }
                 }
             }
             if( bHiba )
                 byLedModulOlvasasiHiba++;
         }

         // Enged�lyezi az IRQ-t, ha le volt tiltva
         EnableModulIRQ();
      }//if( byLedModulKikapcsTimer == 0 )


      // LED t�pegys�g lekapcsol�s/felkapcsol�s hiba eset�n pr�b�lkozik �raind�t�ssal
      if( byLedModulOlvasasiHiba > LED_MODUL_OLVASASI_HIBA )
      {
          // LED modul tap lekapcsolas
          if( byLedModulKikapcsTimer == 0 )
          {
              byLedModulKikapcsTimer = LED_MODUL_KIKAPCS_IDO_x300ms;
              bSendToModulPower_OFF = true;
          }
          else
          {
              byLedModulKikapcsTimer--;

              // 0.9s-al kor�bban felkapcsolja, hogy magukhoz t�rjenek
              if( byLedModulKikapcsTimer == 3 )
                  bSendToModulPower_ON = true;

              if( byLedModulKikapcsTimer == 0 )
              {
                  // LED modul tap visszakapcsol�s ha m�g nem �rte el a max.sz�mot
                  if( byLedModulUjraindulas > 0 )
                  {
                      byLedModulOlvasasiHiba = 0;
                      byLedModulUjraindulas --;
                      bSendToModulPower_ON = false;
                  }
                  else
                  {
/*------------------------------------------------------------------------------------/

    __TO_BE_SOLVED__

                      //Tam�s felugrik egy figyelmeztet� ablak
                      //LED Modul komunik�ci�s HIBA!
                      if( !bErrorMessageDisplayed )
                      {
                         bErrorMessageDisplayed = true;
                         if( Application->MessageBox( FormLanguage->LangStr(STR_MSG_LEDMODUL_COMMERR).c_str(),
                                                      FormLanguage->LangStr(STR_MSGBOX_EXCALMATION).c_str(),
                                                      MB_ICONEXCLAMATION ) == IDOK )
                         {
                            bErrorMessageDisplayed = false;
                         }
                      }
/------------------------------------------------------------------------------------*/
                  }
              }
          }
      }


      if( bSendToModulPower_ON )
      {
          chSerialOut[0] = MODUL_POWER_ON;
          if( HW_SendRelayMessage( chSerialOut, 1  ) )
          {
              bSendToModulPower_ON = false;
              HW_ModulInit();
              EnableModulIRQ();
          }
      }

      if( bSendToModulPower_OFF )
      {
          chSerialOut[0] = MODUL_POWER_OFF;
          if( HW_SendRelayMessage( chSerialOut, 1  ) )
              bSendToModulPower_OFF = false;
      }
   }// if( bySerial_Error == 0 )
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
void CS_Communication_Serial::GetAvailableCommPorts()
{
   char  channel[10];

   for(int i=0; i<32; i++)
   {
      HANDLE hcom;
      sprintf( channel, "COM%d", i+1 );

      if( (hcom = CreateFile( channel,
                              GENERIC_READ|GENERIC_WRITE,
                              0,
                              NULL,
                              OPEN_EXISTING,
                              FILE_ATTRIBUTE_NORMAL,
                              NULL )) != INVALID_HANDLE_VALUE )
      {
         CloseHandle( hcom );
         pComPorts.push_back(i+1);
      }
   }
}
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
//---------------------------------------------------------------------------
// CS_Communication_Serial::HW_ModulInit
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void CS_Communication_Serial::HW_ModulInit()
{
   for(int i=0; i<nHWModuleCount; i++ )
   {
      pModul[i].bVan = false;

      int j = 4;

      do
      {
         GetHWModuleStatus( i );
         j--;
      }while( j>0 && !pModul[i].bVan );
   }
}
//====================================================================================
// CS_Communication_Serial::
//------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------
void CS_Communication_Serial::HW_EmptyBuffer()
{
   char     chSerialIn[2048];
   int      nRecHossz;

   if( SP_ReadMessage( chSerialIn, &nRecHossz ) )
   {
      if( chSerialIn[ nRecHossz-1 ] == MODUL_IRQ )
      {
         chModulMessage = MODUL_IRQ;
      }
   }
}
//---------------------------------------------------------------------------
// HW_ReadMessage - bReadSerialMessageTimOut
//---------------------------------------------------------------------------
// Soros port olvasasa adott timeout figyelembe vetelevel. Ha a megadott
// timeout alatt nem erkezik valasz, hibaval ter vissza.
//---------------------------------------------------------------------------
// Parameterek:
//    chMessage   OUT   uzenet buffer, ebbe olvassa be a fuggveny az uzenetet
//    nHossz      OUT   a kiolvasott uzenet hossza byte-okban
//    nTimeOut    IN    varakozasi idointervallumok (15 ezredmasodperc) szama
//---------------------------------------------------------------------------
// Visszateresi ertek:
//    bRet     TRUE, ha a kiolvasas sikeres volt
//             FALSE, ha a kiolvasas sikertelen volt
//---------------------------------------------------------------------------
bool CS_Communication_Serial::HW_ReadMessage( char *chMessage, int *nHossz, int nTimeOut )
{
   bool bRet = false;

   *nHossz = 0;
   do
   {
      if( SP_ReadMessage( chMessage, nHossz )  )
      {
         if( (*nHossz > 0) && ((unsigned char) chMessage[ (*nHossz)-1 ] != 0xff) )
         {
            bRet = true;
         }
         else
         {
            Sleep(15);
         }
      }
   }while( !bRet && (nTimeOut-- > 0) );

   Sleep( 2 ); //sz�ks�ges a hardware miatt

   if( chMessage[ (*nHossz)-1 ] == START_MESSAGE )
   {
      //ujraindult a Relay panel, mindent ki kell k�ldeni
      wRelay_mem = 0;
      bRet = false;
   }

   return bRet;
}
//---------------------------------------------------------------------------
// HW_SendModulMessage - bSendMessageLEDModul_ReceiveStatus
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Parameterek:
//        nincs
//---------------------------------------------------------------------------
// Visszateresi ertek:
//        nincs
//---------------------------------------------------------------------------
//bool CS_Communication_Serial::HW_SendModulMessage( char *chMessage, int nSendHossz, BYTE byCim, BYTE *byStatus )
bool CS_Communication_Serial::HW_SendModulMessage( char *chMessage, int nSendHossz, unsigned char byCim, unsigned char *byStatus )
{
   char     chSerialIn[2048];
   int      nRecHossz;
   bool     bRet = false;
   unsigned char     byTimeOut;

   //buffer �r�t�s
   HW_EmptyBuffer();

   byCim++;
   chMessage[ 1 ] += byCim<<4;

   SP_SendMessage( chMessage, nSendHossz );
   *byStatus = 0;

   //2 byte-os parancsn�l 35 ms szukseges (60)
   //4  byte-os parancsn�l 70 ms szukseges (105)
   if( nSendHossz > 2 )
   {
      byTimeOut = 11;
   }
   else
   {
      byTimeOut = 6;
   }

   if( HW_ReadMessage( chSerialIn, &nRecHossz, byTimeOut  ) )
   {
      *byStatus = (unsigned char) chSerialIn[ nRecHossz-1 ];
      byHwWdtCounter = WDT_TIME;
      bySerial_Error = 0; // sikeres v�lasz
      if( (*byStatus>>4) == byCim )  //v�lasz ellen�rz�s
      {
         *byStatus &= 0x0b; //irq 0 stop start
         bRet = true;
      }
      else
      {
         bRet = false;
      }
   }
   else
   {
      bRet = false;
   }

   //Hib�s v�lasz
   if( chSerialIn[ nRecHossz-1 ] == START_MESSAGE )
   {
      //ujraindult a Relay panel, mindent ki kell k�ldeni
      wRelay_mem = 0;
   }

   return bRet;
}
//---------------------------------------------------------------------------
// HW_SendRelayMessage - bySendMessageRelay_ReceiveData
//                     - bSendMessageRelay_Receive
//                     - bSendMessageRelay_ReceiveSuccess
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Parameterek:
//---------------------------------------------------------------------------
// Visszateresi ertek:
//---------------------------------------------------------------------------
//bool CS_Communication_Serial::HW_SendRelayMessage( char *chMessage, int nSendHossz, BYTE *byData=NULL )
bool CS_Communication_Serial::HW_SendRelayMessage( char *chMessage, int nSendHossz, unsigned char *byData )
{
   bool  bRet = false;
   char  chSerialIn[2048];
   int   nRecHossz;

   //buffer �r�t�s
   HW_EmptyBuffer();

   SP_SendMessage( chMessage, nSendHossz );  //Serial Send
   bySerial_Error++;

   if( HW_ReadMessage( chSerialIn, &nRecHossz, 5 ) )
   {
      byHwWdtCounter = WDT_TIME;
      bySerial_Error = 0; // sikeres v�lasz
      if( byData != NULL )
      {
         byData[0] = chSerialIn[ nRecHossz-1 ];
      }
      bRet = true;
   }

   if( chSerialIn[ nRecHossz-1 ] == START_MESSAGE )
   {
      //ujraindult a Relay panel, mindent ki kell k�ldeni
      wRelay_mem = 0;
   }

   return bRet;
}
//---------------------------------------------------------------------------
// HW_ReadEEProm - bReedEEprom
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
// Parameterek:
//    nincs
//---------------------------------------------------------------------------
// Visszateresi ertek:
//    nincs
//---------------------------------------------------------------------------
//BOOL CS_Communication_Serial::HW_ReadEEProm( BYTE byStartAddress, char *chMessage )
BOOL CS_Communication_Serial::HW_ReadEEProm( unsigned char byStartAddress, char *chMessage )
{
   unsigned char byAddress = byStartAddress;
   unsigned char i = 0;
   char chSerialOut[2048];

   do
   {
      unsigned char byData;
      chSerialOut[0] = READ_EEPROM;
      chSerialOut[1] = byAddress++;
      if( HW_SendRelayMessage( chSerialOut, 2, &byData ) )
      {
         chMessage[i] = byData;
      }
      else
      {
         return false;
      }
   }while( (chMessage[i++] != 0) && (i < 100) );

   //0 a v�g�re
   chMessage[i] = 0;

   return TRUE;
}
//---------------------------------------------------------------------------
// HW_WriteEEProm - bWriteEEprom
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
// Parameterek:
//    nincs
//---------------------------------------------------------------------------
// Visszateresi ertek:
//    nincs
//---------------------------------------------------------------------------
//BOOL CS_Communication_Serial::HW_WriteEEProm( BYTE byStartAddress, char *chMessage )
BOOL CS_Communication_Serial::HW_WriteEEProm( unsigned char byStartAddress, char *chMessage )
{
   BOOL bRet = false;
   unsigned char byAddress = byStartAddress;
   unsigned char i = 0;
   char chSerialOut[2048];

   while( (chMessage[i] != 0) && (i < 100) && !bRet )
   {
      chSerialOut[0] = WRITE_EEPROM;
      chSerialOut[1] = byAddress++;
      chSerialOut[2] = chMessage[ i++ ];
      Sleep( 15 );
      if( HW_SendRelayMessage( chSerialOut, 3, NULL ) )
      {
         bRet = true;
      }
   }

   //0 a v�g�re
   chSerialOut[0] = WRITE_EEPROM;
   chSerialOut[1] = byAddress;
   chSerialOut[2] = 0;
   Sleep( 15 );
   if( HW_SendRelayMessage( chSerialOut, 3, NULL ) )
   {
      bRet = true;
   }

   return bRet;
}
//---------------------------------------------------------------------------
// GetHWModuleStatus - byLedModulKeres
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
// Parameterek:
//    nincs
//---------------------------------------------------------------------------
// Visszateresi ertek:
//    nincs
//---------------------------------------------------------------------------
//BYTE CS_Communication_Serial::GetHWModuleStatus( BYTE byCim )
unsigned char CS_Communication_Serial::GetHWModuleStatus( unsigned char byCim )
{
   if( byCim > nHWModuleCount-1 )
      return 0xff;

   unsigned char byStatus;
   char chMessage[2048];

   pModul[byCim].bVan = false;
   pModul[byCim].bSendIras = false;
   pModul[byCim].bSendStart = false;
   pModul[byCim].bSendEnd = false;

   chMessage[0] = SEND_1BYTE_TO_MODUL;
   chMessage[1] = GET_STATUS;
   if( HW_SendModulMessage( chMessage, 2, byCim, &byStatus ) )
   {
      pModul[byCim].bVan = true;
      return byStatus;
   }
   else
   {
      return 0xff;
   }
}
//---------------------------------------------------------------------------
// EnableModulIRQ
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Parameterek:
//---------------------------------------------------------------------------
// Visszateresi ertek:
//---------------------------------------------------------------------------
void CS_Communication_Serial::EnableModulIRQ()
{
    char chSerialOut[2048];
        unsigned char byTemp;

    if( !bEnableIRQ_Msg )
   {
        do
        {
            chSerialOut[0] = MODUL_ENABLE;
            if( HW_SendRelayMessage( chSerialOut, 1, NULL ) )
                bEnableIRQ_Msg = true;

        }while( !bEnableIRQ_Msg && byTemp++<10 );
    }
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Parameterek:
//---------------------------------------------------------------------------
// Visszateresi ertek:
//---------------------------------------------------------------------------
void CS_Communication_Serial::DisableModulIRQ()
{
    char chSerialOut[2048];
        unsigned char byTemp;

   if( bEnableIRQ_Msg )
   {
        do
        {
            chSerialOut[0] = MODUL_DISABLE;
            if( HW_SendRelayMessage( chSerialOut, 1, NULL ) )
                bEnableIRQ_Msg = false;

        }while( bEnableIRQ_Msg && byTemp++<10 );
   }
}
//====================================================================================
// CS_Communication_Serial::
//------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------
bool CS_Communication_Serial::SP_ReadMessage( char *Message, int *nLength )
{
   bool bRet = false;

   unsigned long readNumber;

   bRet = Receive ( 512, Message, &readNumber );
   *nLength = readNumber;

   return bRet;
}
//====================================================================================
// CS_Communication_Serial::
//------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------
bool CS_Communication_Serial::SP_SendMessage( char *Message, int nLength )
{
   bool bRet = false;

   bRet = Send( nLength, Message );

   return bRet;
}
//====================================================================================
// CS_Communication_Serial::
//------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------
bool CS_Communication_Serial::Receive ( unsigned int    bytesNumber,
                         char *            bytesArray,
                                                 unsigned long*    readNumber, OVERLAPPED *overLapped )
{

        if (!m_hPort) {

                return false;
        }

        if ( ! ReadFile ( m_hPort,           // port handle
                                         bytesArray,      // read buffer
                                         bytesNumber,     // bytes to read
                                         readNumber,      // read bytes
                                         overLapped ) )         // overlapped infos
        {

                ShowLastWindowsError();
                return false;
        } else if (!(*readNumber)) {

                return true;
        }

        return true;
}
//====================================================================================
// CS_Communication_Serial::
//------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------
bool CS_Communication_Serial::Send ( unsigned int bytesNumber, char *bytesArray )
{
   unsigned long     sentNumber;
   OVERLAPPED olWrite = {0};
   COMSTAT ComStat;
   DWORD lpErrors;
   DWORD dwRes;            // result of function WaitForSingleObject
   DWORD Timeout = 500; // WaitForSingleObject Timeout in ms, it can be INFINITE
   bool wRes;            // result of the write method

   if (!m_hPort)
   {
      return false;
   }

   // Create this write operation's OVERLAPPED structure's hEvent.
   olWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

   if (olWrite.hEvent == NULL)
   {
      // error creating overlapped event handle
      return false;
   }

   if( !WriteFile ( m_hPort,           // port handle
                 bytesArray,      // read buffer
                 bytesNumber,     // bytes to read
                 &sentNumber,      // sent bytes
                 &olWrite ) )         // overlapped infos
   {
      if (GetLastError() != ERROR_IO_PENDING)
      {
            // WriteFile failed, but isn't delayed. Report error and abort.
            if (!CancelIo(m_hPort))
            {}

            if (!ClearCommError(m_hPort, &lpErrors , &ComStat))
            {}

            wRes = false;
      }
      else
      {
            // Write is pending.

            // The function returns if the interval elapses, even if the
            // object's state is nonsignaled. If Timeout is zero,
            // the function tests the object's state and returns immediately.
            dwRes = WaitForSingleObject(olWrite.hEvent, Timeout);
            switch(dwRes)
            {
                  // OVERLAPPED structure's event has been signaled.
                  case WAIT_OBJECT_0:
                  {
                        //If the last param is TRUE, the function does not return until the operation has been completed.
                        if (!GetOverlappedResult(m_hPort, &olWrite, &sentNumber, TRUE))
                        {
                              // Write fails
                              if (!CancelIo(m_hPort))
                              {}
                              if (!ClearCommError(m_hPort, &lpErrors , &ComStat))
                              {}
                              wRes = false;
                        }
                        else
                        {
                              // Write operation completed successfully.
                              wRes = true;
                        }
                        break;
                  }
                  //     The time-out interval elapsed, and the OVERLAPPED structure's event has NOT been signaled.
                  case WAIT_TIMEOUT:
                  {
                        if (!CancelIo(m_hPort))
                        {}

                        if (!ClearCommError(m_hPort, &lpErrors , &ComStat))
                        {}
                        wRes = false;
                        break;
                  }
                  default:
                  {
                        // An error has occurred in WaitForSingleObject.
                        // This usually indicates a problem with the
                        // OVERLAPPED structure's event handle.
                        if (!CancelIo(m_hPort))
                        {}
                        if (!ClearCommError(m_hPort, &lpErrors , &ComStat))
                        {}
                        wRes = false;
                        break;
                  }
            }
      }
      return wRes;
   }


   else if ( sentNumber != bytesNumber )
   {
      if (!CancelIo(m_hPort))
      {}
      if (!ClearCommError(m_hPort, &lpErrors , &ComStat))
      {}
      ShowLastWindowsError();
      return false;
   }

   return true;
}
//====================================================================================
// CS_Communication_Serial::
//------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------
bool CS_Communication_Serial::ShowLastWindowsError(void)
{
        LPVOID         lpMsgBuf=NULL;

        if (!FormatMessage(
                    FORMAT_MESSAGE_ALLOCATE_BUFFER |
                    FORMAT_MESSAGE_FROM_SYSTEM |
                    FORMAT_MESSAGE_IGNORE_INSERTS,
                    NULL,
                    ::GetLastError(),
                    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
                    (LPTSTR) &lpMsgBuf,
                    0,
                    NULL ))
        {

                return false;
        } else {

        }

        // Free the buffer.
        if (lpMsgBuf) LocalFree( lpMsgBuf );

        return true;
}
