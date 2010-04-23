
//	__TO_BE_SOLVED__  <- Megoldást kell találni 

//---------------------------------------------------------------------------

#ifndef ClassHardwareH
#define ClassHardwareH

#include <windows.h>
#include <vector>

#include "ClassSerialComm.h"

//---------------------------------------------------------------------------
typedef struct _typ_panel_data
{
   char  cPrevStatus;
   char  cCurrStatus;
	bool	bInfraModul;
	bool	bJumpNextStatus;
	int	nTimeStatusCounter;
	int	nTimeStatusMain;
} typ_panel_data;
//---------------------------------------------------------------------------
#define	SIZEOF_PANEL_DATA		12
//---------------------------------------------------------------------------
typedef struct _typ_LED_Modul
{
   bool  bVan;
   bool  bStop;
   bool  bSendIras;
   bool  bSendStart;
   bool  bSendEnd;
} typ_LED_Modul;
//---------------------------------------------------------------------------
#define	SIZEOF_MODUL_DATA		5
//---------------------------------------------------------------------------
// Hardware class
//---------------------------------------------------------------------------
// A szolárium gépeket kezelő hardver (PLC) vezérlő osztálya. A szülő
// soros port kezelő osztály segítségével kommunikál a hardverrel az 
// osztály.
//---------------------------------------------------------------------------
class CS_Hardware : public CS_SerialComm
{
   //-------------------------------------------------------------------
   public:
      //----------------------------------------------------------------
      // 
      vector<typ_panel_data>    pPanel;
      
      //----------------------------------------------------------------
      // 
      CS_Hardware();
      ~CS_Hardware();

      bool IsHardwareConnected( void );
//      BYTE GetHWModuleStatus( BYTE byCim );
      unsigned char GetHWModuleStatus( unsigned char byCim );
      
      void SetApplicationModuleCount( int nCount );
      bool SetHardwareModuleCount( int nCount );
      int GetHardwareModuleCount();
      
		void HW_Kezel();
		
//      BOOL HW_ReadEEProm( BYTE byStartAddress, char *chMessage );
      BOOL HW_ReadEEProm( unsigned char byStartAddress, char *chMessage );
//      BOOL HW_WriteEEProm( BYTE byStartAddress, char *chMessage );
      BOOL HW_WriteEEProm( unsigned char byStartAddress, char *chMessage );

		void EnableModulIRQ();
		void DisableModulIRQ();
		
		void SetTestMode( bool bMode );

   //-------------------------------------------------------------------
   private:

      //----------------------------------------------------------------
      // 
      vector<typ_LED_Modul>   pModul;
      
      int   nHWModuleCount;         // Hardware-ben a kezelendo panel-ek szama, 
//      BYTE  byLedModulOlvasasiHiba;
      unsigned char  byLedModulOlvasasiHiba;
//      BYTE  byLedModulUjraindulas;
      unsigned char  byLedModulUjraindulas;
//      BYTE  byLedModulKikapcsTimer;
      unsigned char  byLedModulKikapcsTimer;

//      BYTE  byHwWdtCounter;
      unsigned char  byHwWdtCounter;
//      BYTE  bySerial_Error;
      unsigned char  bySerial_Error;
      char  chModulMessage;
      WORD  wRelay_mem;
		BOOL  bEnableIRQ_Msg;
		
		bool	bTest;
		
		bool bSendToModulPower_ON;
		bool bSendToModulPower_OFF;

      //----------------------------------------------------------------
      // 
      void HW_ModulInit();
		
      void HW_EmptyBuffer();
      
      bool HW_ReadMessage( char *chMessage, int *nHossz, int nTimeOut );

//      bool HW_SendModulMessage( char *chMessage, int nSendHossz, BYTE byCim, BYTE *byStatus );
      bool HW_SendModulMessage( char *chMessage, int nSendHossz, unsigned char byCim, BYTE *byStatus );
//      bool HW_SendRelayMessage( char *chMessage, int nSendHossz, BYTE *byData );
      bool HW_SendRelayMessage( char *chMessage, int nSendHossz, unsigned char *byData );
};
//---------------------------------------------------------------------------
#endif
