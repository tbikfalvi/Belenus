//====================================================================================
//
// Belenus Kliens alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : general.h
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
// Altalanos, tobb resz altal hasznalt fuggvenyek, process-ek
//====================================================================================

#ifndef GENERAL_H
#define GENERAL_H

//====================================================================================

#include <QString>

//====================================================================================
class cGeneral
{
    //Q_OBJECT

public:
    cGeneral();
    ~cGeneral();

    QString     convertCurrency( int p_nCurrencyValue, QString p_qsCurrency );

};
//====================================================================================

#endif // GENERAL_H
