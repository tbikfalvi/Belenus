//---------------------------------------------------------------------------

#ifndef ClassSerialCommH
#define ClassSerialCommH

#include <windows.h>
#include <vector>

using namespace std;

//---------------------------------------------------------------------------
class CS_SerialComm
{
   public:

      CS_SerialComm();
      ~CS_SerialComm();

      int SP_GetCountAvailablePorts();
      int SP_GetComPort( int p_inIndex );
      bool SP_IsComPortOpened( int *p_inPortNumber );

      bool SP_InitCommunication( int p_inPort=0, DWORD p_dwBaudRate=0, DWORD p_dwParity=0 );
      bool SP_Open( bool bSync=true );
      bool SP_SendMessage( char *Message, int nLength );
      bool SP_ReadMessage( char *Message, int *nLength );
      void SP_Close();

   private:

      vector<int>    pComPorts;

      HANDLE         m_hPort;
      HANDLE         hComm;
      int            PortNumber;
      DWORD          BaudRate;
      DWORD          Parity;
      OVERLAPPED     Olpw;		   // adási struktúra
      OVERLAPPED     Olpr;		   // vételi struktúra
      char           Puffer[512]; //pufferek
      DWORD          m_dwBaudRate;
      DWORD          m_dwParity;
      bool           m_bPortOpened;

   	bool Send (unsigned int bytesNumber,char *bytesArray);
      bool Receive ( unsigned int bytesNumber,char *bytesArray, unsigned long *readNumber, OVERLAPPED *overLapped=NULL );
   	bool ShowLastWindowsError(void);

      void GetAvailableCommPorts();
};
//---------------------------------------------------------------------------
#endif
