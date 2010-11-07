//====================================================================================
//
// Belenus Kliens alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : general.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
// Altalanos, tobb resz altal hasznalt fuggvenyek, process-ek
//====================================================================================

#include "general.h"
#include "belenus.h"

//====================================================================================
cGeneral::cGeneral()
//====================================================================================
{

}
//====================================================================================
cGeneral::~cGeneral()
//====================================================================================
{

}
//====================================================================================
QString cGeneral::convertCurrency( int p_nCurrencyValue, QString p_qsCurrency )
//====================================================================================
{
    QString qsValue = QString::number( p_nCurrencyValue );
    QString qsRet = "";

    if( qsValue.length() > 3 )
    {
        while( qsValue.length() > 3 )
        {
            qsRet.insert( 0, qsValue.right(3) );
            qsRet.insert( 0, g_poPrefs->getCurrencySeparator() );
            qsValue.truncate( qsValue.length()-3 );
        }
    }
    qsRet.insert( 0, qsValue );
    qsRet += " " + p_qsCurrency;

    return qsRet;
}
//====================================================================================
