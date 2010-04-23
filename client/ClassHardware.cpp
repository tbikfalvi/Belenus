
//	__TO_BE_SOLVED__  <- Megoldást kell találni 

//---------------------------------------------------------------------------

#include "ClassHardware.h"

//---------------------------------------------------------------------------
#define STATUS_ALAP              0
#define STATUS_VETKOZES          1
#define STATUS_SZAUNAZAS         2
#define STATUS_BARNULAS          3
#define STATUS_UTOHUTES          4
#define STATUS_VARAKOZAS         9
//---------------------------------------------------------------------------
//Relay mask define
#define R0   0x0000

#define R1   0x0100
#define R2   0x0200
#define R3   0x0400
#define R4   0x0800
#define R5   0x1000
#define R6   0x2000

#define R7   0x0001
#define R8   0x0002
#define R9   0x0004
#define R10  0x0008
#define R11  0x0010
#define R12  0x0020
//---------------------------------------------------------------------------
#define MAX_MODUL_SZAM   15
//---------------------------------------------------------------------------
//  Modul Parancsok         ( ~I2C Kommunikacio a Modulokal )
//---------------------------------------------------------------------------
//                                        SEND                    RECEIVE
//Command                                 Byte                     Byte
#define    SEND_TIME         0x01     // 1 command + 2 data      1 Status
#define    SEND_START        0x02     // 1 command               1 Status
#define    SEND_END          0x03     // 1 command               1 Status
#define    GET_STATUS        0x04     // 1 command               1 Status
#define    GET_TIME          0x05     // 1 command               1 Status
#define    SEND_SET_ADDR     0x06     // 0xa5 0x99   3 command               1 Status
#define    SEND_UTOHUTES_END 0x07     // 1 command               1 Status
//---------------------------------------------------------------------------
//Command Byte   ---------------------------------
//               | 4bit address  |  4bit command |
//               ---------------------------------
//---------------------------------------------------------------------------
//Response  STATUS Byte
//            ----------------------------------------------------------
//            | 4bit address  | getint | Timer_On | ModStop | ModStart |
//            ----------------------------------------------------------
#define    ModStart     0
#define    ModStop      1
#define    Timer_On     2
#define    getint       3
//---------------------------------------------------------------------------
//Soros Vonal parancsok a SzRel-hez
//Direct relay commands:
//---------------------------------------------------------------------------
#define SET_RELAY         '1'//, RelH,RelL, 0x5a                   4    1 ERR


#define CLR_RELAY_TIMER   '3'//,0x09  5s-enkent torolni a PIC-ben  1    1 ERR_

#define READ_EEPROM       '4'//,ADDRESS                            2    1 Data
#define WRITE_EEPROM      '5'//,ADDRESS, DATA                      2    1 ERR


#define SEND_3BYTE_TO_MODUL '6'// Perc,  MPerc                     3    1 Status
#define SEND_1BYTE_TO_MODUL '7'//                                  1    1 Status

#define MODUL_ENABLE         0x10//                                1    1 ERR
#define MODUL_DISABLE        0x11//                                1    1 ERR

#define MODUL_POWER_ON       0x12//                                1    1 ERR
#define MODUL_POWER_OFF      0x13//                                1    1 ERR


#define SEARCH_HW         'S'//'H'                                 2    'Y'

#define  INIT_REQUEST  'R' // 2 |    'R'         |  4 db 'I' vÂ laszra vÂ r 100ms-os szunetekel

  // Ezt kovetoen a PIC -kuld 4 byte-ot (Random szamok), majd a PC visszakuldi a
  // "kiszÂ molt Â‚rtÂ‚ket" 1Byte.
  // Pic vÂ lasz SUCCESS / ERROR
  // Ha sikertelen volt a PC megvarja az 'RR' parancsot es lehet ismet probÂ lkozni.
  //
  // A "KiszÂ molt Â‚rtÂ‚ket" a kÂ”vetkezÂ‹kÂ‚pen kell kiszÂ molni:
  //   Ezeket a szamokat irja ki a PIC 4db 'I' utan:
  //   Szam1, Szam2, Szam3, Szam4
  //
  // Ezt kovetoen ki kell szamolni az eredmenyt es visszakuldeni a PC-nek.
  //                        _________________________________
  //    Szam1  bitcsere     | 5 | 7 | 4 | 6 | 3 | 1 | 0 | 2 |  pl. 0x8c -> 0x49, 0xcb -> 0x5e
  //                        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //                                     Szam2                 XOR
  //                        ---------------------------------
  //               Result =
  //                                     Szam3                 ADD
  //                        ---------------------------------
  //               Result =
  //                                     Szam4                 XOR
  //                        ---------------------------------
  //         Final Result =
  //
  //
  //
  //  pl.
  //   Szam1    Szam2    Szam3    Szam4       Final Result
  //    0x6c     0x6d     0x2c     0x74         0x67
  //    0x8b     0xa2     0xd4     0x7d         0x17
  //    0x64     0xf0     0x16     0x81         0x6f
  //

//---------------------------------------------------------------------------
//Responses:
#define  HW_ERROR         0x00//      ERR_..
#define  HW_SUCCESS       0x01//      ERR_..
#define  HW_UNKN_COMM     0x02//      ERR_..
//---------------------------------------------------------------------------
//Message
#define  MODUL_IRQ     'Q'//
#define  START_MESSAGE 'M'//
//---------------------------------------------------------------------------
#define WDT_TIME  6  //6*300ms
#define LED_MODUL_OLVASASI_HIBA       10
#define LED_MODUL_MAX_UJRAINDULAS     5
#define LED_MODUL_KIKAPCS_IDO_x300ms  10

//---------------------------------------------------------------------------
//12 Relay-os hardware
const WORD rel_csovek [] = {
 R2, R4, R6, R8, R10, R12, R0, R0,  R0, R0, R0, R0,  //1
 R2, R4, R6, R8, R10, R12, R0, R0,  R0, R0, R0, R0,  //2
 R2, R4, R6, R8, R10, R12, R0, R0,  R0, R0, R0, R0,  //3
 R2, R4, R6, R8, R10, R12, R0, R0,  R0, R0, R0, R0,  //4
 R2, R4, R6, R8, R10, R12, R0, R0,  R0, R0, R0, R0,  //5
 R2, R4, R6, R8, R10, R12, R0, R0,  R0, R0, R0, R0,  //6
 R2, R4, R6, R8, R10, R12, R9, R11, R0, R0, R0, R0,  //7
 R2, R4, R6, R8, R10, R12, R9, R11, R0, R0, R0, R0,  //8
 R2, R4, R6, R8, R10, R12, R9, R11, R7, R0, R0, R0,  //9
 R2, R4, R6, R8, R10, R12, R9, R11, R7, R5, R0, R0,  //10
 R2, R4, R6, R8, R10, R12, R9, R11, R7, R5, R3, R0,  //11
 R2, R4, R6, R8, R10, R12, R9, R11, R7, R5, R3, R1 };//12

const WORD rel_vent [] = {
 R1, R3, R5, R7, R9,  R11, R0, R0, R0, R0, R0, R0,  //1
 R1, R3, R5, R7, R9,  R11, R0, R0, R0, R0, R0, R0,  //2
 R1, R3, R5, R7, R9,  R11, R0, R0, R0, R0, R0, R0,  //3
 R1, R3, R5, R7, R9,  R11, R0, R0, R0, R0, R0, R0,  //4
 R1, R3, R5, R7, R9,  R11, R0, R0, R0, R0, R0, R0,  //5
 R1, R3, R5, R7, R9,  R11, R0, R0, R0, R0, R0, R0,  //6
 R1, R3, R5, R7, R0,  R0,  R0, R0, R0, R0, R0, R0,  //7
 R1, R3, R5, R7, R0,  R0,  R0, R0, R0, R0, R0, R0,  //8
 R1, R3, R5, R0, R0,  R0,  R0, R0, R0, R0, R0, R0,  //9
 R1, R3, R0, R0, R0,  R0,  R0, R0, R0, R0, R0, R0,  //10
 R1, R0, R0, R0, R0,  R0,  R0, R0, R0, R0, R0, R0,  //11
 R0, R0, R0, R0, R0,  R0,  R0, R0, R0, R0, R0, R0 };//12

//---------------------------------------------------------------------------
// CS_Hardware
//---------------------------------------------------------------------------
// Konstruktor fuggveny. Valtozok inicializalasa.
//---------------------------------------------------------------------------
// Parameterek:
//    nincs
//---------------------------------------------------------------------------
// Visszateresi ertek:
//    nincs
//---------------------------------------------------------------------------
CS_Hardware::CS_Hardware()
{
   nHWModuleCount 			= 0;
   
   byLedModulOlvasasiHiba 	= 0;
   byLedModulUjraindulas 	= LED_MODUL_MAX_UJRAINDULAS;
   byLedModulKikapcsTimer 	= 0;
   
   byHwWdtCounter 			= WDT_TIME;
   bySerial_Error			= 0;
   chModulMessage			= 0;
   wRelay_mem				= 0;

   bEnableIRQ_Msg			= false;
	
	bTest						= false;
}
//---------------------------------------------------------------------------
// ~CS_Hardware
//---------------------------------------------------------------------------
// Destruktor fuggveny.
//---------------------------------------------------------------------------
// Parameterek:
//    nincs
//---------------------------------------------------------------------------
// Visszateresi ertek:
//    nincs
//---------------------------------------------------------------------------
CS_Hardware::~CS_Hardware()
{
}
//---------------------------------------------------------------------------
// IsHardwareConnected -- bSearchHardware
//---------------------------------------------------------------------------
// Ellenorzi, hogy van-e csatlakoztatva hardver egyseg a szamitogephez
//---------------------------------------------------------------------------
// Parameterek:
//    nincs
//---------------------------------------------------------------------------
// Visszateresi ertek:
//    TRUE     - ha van hardver egyseg csatlakoztatva
//    FALSE    - ha nincs hardver egyseg csatlakoztatva, vagy 
//               a kapcsolatfelvetel sikertelen
//---------------------------------------------------------------------------
bool CS_Hardware::IsHardwareConnected( void )
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
   Sleep( 100 ); //var, hogy a PIC tudjon válaszolni

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
//BYTE CS_Hardware::GetHWModuleStatus( BYTE byCim )
unsigned char CS_Hardware::GetHWModuleStatus( unsigned char byCim )
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
void CS_Hardware::SetApplicationModuleCount( int nCount )
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
         typ_panel_data	stTemp;
         
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
//    TRUE        - ha sikerült a hardverben a szolariumok szamat
//                  beallitani
//    FALSE       - ha nem sikerult beallitani a hardverben a szolariumok
//                  szamat
//---------------------------------------------------------------------------
bool CS_Hardware::SetHardwareModuleCount( int nCount )
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
int CS_Hardware::GetHardwareModuleCount()
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
//BOOL CS_Hardware::HW_ReadEEProm( BYTE byStartAddress, char *chMessage )
BOOL CS_Hardware::HW_ReadEEProm( unsigned char byStartAddress, char *chMessage )
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

   //0 a végére
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
//BOOL CS_Hardware::HW_WriteEEProm( BYTE byStartAddress, char *chMessage )
BOOL CS_Hardware::HW_WriteEEProm( unsigned char byStartAddress, char *chMessage )
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

   //0 a végére
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
// SetTestMode
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Parameterek:
//		bMode			IN		Teszt mód be, illetve kikapcsolására 
//---------------------------------------------------------------------------
// Visszateresi ertek:
//		nincs
//---------------------------------------------------------------------------
void CS_Hardware::SetTestMode( bool bMode )
{
	bTest = bMode;
}
//---------------------------------------------------------------------------
// HW_EmptyBuffer
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Parameterek:
//		nincs
//---------------------------------------------------------------------------
// Visszateresi ertek:
//		nincs
//---------------------------------------------------------------------------
void CS_Hardware::HW_EmptyBuffer()
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
bool CS_Hardware::HW_ReadMessage( char *chMessage, int *nHossz, int nTimeOut )
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

   Sleep( 2 ); //szükséges a hardware miatt

   if( chMessage[ (*nHossz)-1 ] == START_MESSAGE )
   {
      //ujraindult a Relay panel, mindent ki kell küldeni
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
//		nincs
//---------------------------------------------------------------------------
// Visszateresi ertek:
//		nincs
//---------------------------------------------------------------------------
//bool CS_Hardware::HW_SendModulMessage( char *chMessage, int nSendHossz, BYTE byCim, BYTE *byStatus )
bool CS_Hardware::HW_SendModulMessage( char *chMessage, int nSendHossz, unsigned char byCim, unsigned char *byStatus )
{
   char     chSerialIn[2048];
   int      nRecHossz;
   bool     bRet = false;
   unsigned char     byTimeOut;

   //buffer ürítés
   HW_EmptyBuffer();

   byCim++;
   chMessage[ 1 ] += byCim<<4;

   SP_SendMessage( chMessage, nSendHossz );
   *byStatus = 0;

   //2 byte-os parancsnál 35 ms szukseges (60)
   //4  byte-os parancsnál 70 ms szukseges (105)
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
      bySerial_Error = 0; // sikeres válasz
      if( (*byStatus>>4) == byCim )  //válasz ellenörzés
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

   //Hibás válasz
   if( chSerialIn[ nRecHossz-1 ] == START_MESSAGE )
   {
      //ujraindult a Relay panel, mindent ki kell küldeni
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
//bool CS_Hardware::HW_SendRelayMessage( char *chMessage, int nSendHossz, BYTE *byData=NULL )
bool CS_Hardware::HW_SendRelayMessage( char *chMessage, int nSendHossz, unsigned char *byData=NULL )
{
   bool  bRet = false;
   char  chSerialIn[2048];
   int   nRecHossz;

   //buffer ürítés
   HW_EmptyBuffer();

   SP_SendMessage( chMessage, nSendHossz );  //Serial Send
   bySerial_Error++;

   if( HW_ReadMessage( chSerialIn, &nRecHossz, 5 ) )
   {
      byHwWdtCounter = WDT_TIME;
      bySerial_Error = 0; // sikeres válasz
      if( byData != NULL )
      {
         byData[0] = chSerialIn[ nRecHossz-1 ];
      }
      bRet = true;
   }

   if( chSerialIn[ nRecHossz-1 ] == START_MESSAGE )
   {
      //ujraindult a Relay panel, mindent ki kell küldeni
      wRelay_mem = 0;
   }

   return bRet;
}
//---------------------------------------------------------------------------
// HW_ModulInit
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Parameterek:
//---------------------------------------------------------------------------
// Visszateresi ertek:
//---------------------------------------------------------------------------
void CS_Hardware::HW_ModulInit()
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
//---------------------------------------------------------------------------
// EnableModulIRQ
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Parameterek:
//---------------------------------------------------------------------------
// Visszateresi ertek:
//---------------------------------------------------------------------------
void CS_Hardware::EnableModulIRQ()
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
void CS_Hardware::DisableModulIRQ()
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
//---------------------------------------------------------------------------
// HW_Kezel
//---------------------------------------------------------------------------
// 300 sec-ként a foprogram hívja meg a fuggvenyt ami lekezeli es
// tovabbkuldi a parancsokat, illetve szolgaltatja a szolariumok allapotat
//---------------------------------------------------------------------------
// Parameterek:
//    nincs
//---------------------------------------------------------------------------
// Visszateresi ertek:
//    nincs
//---------------------------------------------------------------------------
void CS_Hardware::HW_Kezel()
{
   //---------------------------------------------------------------------------
   // Demo illetve helytelen parameterbeallitas esetén nem foglalkozik a 
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
   // Relay állapotok beállítása
   //---------------------------------------------------
	if( !bTest )
	{
		wRelay = 0;
		for( i=0; i<nHWModuleCount; i++ )
		{
			if( pPanel[i].cCurrStatus == STATUS_BARNULAS )
			{
//				MINDIG BE VANNAK KAPCSOLVA BARNULAS ALATT
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
   // WDT frissités, ha nem volt kikuldve semmi akkor WDT
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
   // Szoli LED Modul vezérlés
   //--------------------------------------------------------------------------------------
   //Ha jó a soros komunikáció
   if( bySerial_Error == 0 )
   {
      //és fel van kapcsolva a LED táp
      if( (byLedModulKikapcsTimer == 0) && !bTest )
      {
         //Szoli LED Modul IRQ figyelés
         if( SP_ReadMessage( chSerialIn, &nHossz ) )
         {
              if( chSerialIn[ nHossz-1 ] == MODUL_IRQ ||
                  chModulMessage == MODUL_IRQ )
              {
                  chModulMessage = 0;
                  //mivel Modul IRQ akkor lekapcsolja az IRQ-üzenetet,
                  // míg le nem kérdezi a status-okat
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
                                     // Szauna megszakítása, következõ ciklus indítása
                                     pPanel[i].bJumpNextStatus = true;
                                 }
                                 else if( pPanel[i].cCurrStatus == STATUS_BARNULAS )
                                 {
                                     pModul[ i ].bStop = true; //Relay kikapcsolás
                                 }
                             }
                             ///////////////////////////////////////////
                             else if( (byStatus & 0x03) == 0x01 ) //Start SW
                             {
                                 if( pPanel[i].cCurrStatus == STATUS_VETKOZES )
                                 {
                                     // Vetkõzés megszakítása, következõ ciklus indítása
                                     pPanel[i].bJumpNextStatus = true;
                                 }
                                 else if( pPanel[i].cCurrStatus == STATUS_BARNULAS )
                                 {
                                     pModul[ i ].bStop = false; //Relay bekapcsolás
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
                              //nincs válasz
                              byLedModulOlvasasiHiba++;
                          }//if( bSendMessageLEDModul_ReceiveStatus( chSerialOut, 2, i, &byStatus ) )
                      }//if( pModul[ i ].bVan )
                  }//for
              }//if( chSerialIn[ nHossz-1 ] == MODUL_IRQ )
              else if( chSerialIn[ nHossz-1 ] == START_MESSAGE )
              {
                  //ujraindult a Relay panel, mindent ki kell küldeni
                  wRelay_mem = 0;
              }
         }// nincs LED Modul IRQ

         ////////////////////////////////////////////////////////////////////////
         //Szoli LED Modul vezérlés

         //Elenörzi van-e küldenivaló adat
         bool bVanKuldeniValoAdat = false;
         for( i=0; i<nHWModuleCount; i++ )
         {
             if( pModul[ i ].bVan )
             {
                 if( pPanel[i].cPrevStatus == STATUS_ALAP && pPanel[i].cCurrStatus == STATUS_VETKOZES )  //vetkõzés
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

         //Adatok kiküldése a moduloknak
         if( bVanKuldeniValoAdat )
         {
             // Letiltlja az IRQ-t, hogy ne zavarja meg a komunikációt
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

         // Engedélyezi az IRQ-t, ha le volt tiltva
         EnableModulIRQ();
      }//if( byLedModulKikapcsTimer == 0 )


      // LED tápegység lekapcsolás/felkapcsolás hiba esetén próbálkozik úraindítással
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

              // 0.9s-al korábban felkapcsolja, hogy magukhoz térjenek
              if( byLedModulKikapcsTimer == 3 )
                  bSendToModulPower_ON = true;

              if( byLedModulKikapcsTimer == 0 )
              {
                  // LED modul tap visszakapcsolás ha még nem érte el a max.számot
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

                      //Tamás felugrik egy figyelmeztetõ ablak
                      //LED Modul komunikációs HIBA!
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
//---------------------------------------------------------------------------

