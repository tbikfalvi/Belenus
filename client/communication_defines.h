//====================================================================================
//
// Belenus Kliens alkalmazas © Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : communication_defines.h
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#include "windows.h"

//====================================================================================

#ifndef COMMUNICATION_DEFINES_H
#define COMMUNICATION_DEFINES_H

//====================================================================================

#define	SIZEOF_PANEL_DATA		12
#define	SIZEOF_MODUL_DATA		5

#define EXCEPTION_OPEN_COM_PORT         0
#define EXCEPTION_GET_COM_PORT_CONFIG   1
#define EXCEPTION_SET_COM_PORT_CONFIG   2
#define EXCEPTION_CLOSE_COM_PORT        3
#define EXCEPTION_READ_COM_PORT		4
#define EXCEPTION_WRITE_COM_PORT	5
#define EXCEPTION_SET_COM_TIMEOUTS	6
#define EXCEPTION_INVALID_COM_HANDLE	7
#define EXCEPTION_GET_COM_PROP		8

#define STATUS_ALAP                     0
#define STATUS_VETKOZES                 1
#define STATUS_SZAUNAZAS                2
#define STATUS_BARNULAS                 3
#define STATUS_UTOHUTES                 4
#define STATUS_VARAKOZAS                9

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

//====================================================================================

#endif // COMMUNICATION_DEFINES_H
