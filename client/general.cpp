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
//====================================================================================
cCurrency::cCurrency(const QString &p_qsCurrencyString, currType p_ctCurrencyType, int p_nVat)
{
    m_ctCurrType    = p_ctCurrencyType;
    m_nVatValue     = p_nVat;

    QString qsPureCurrency;

    // Remove currency string
    if( p_qsCurrencyString.contains( g_poPrefs->getCurrencyLong() ) )
    {
        qsPureCurrency = p_qsCurrencyString.remove( g_poPrefs->getCurrencyLong() );
    }
    else if( p_qsCurrencyString.contains( g_poPrefs->getCurrencyShort() ) )
    {
        qsPureCurrency = p_qsCurrencyString.remove( g_poPrefs->getCurrencyShort() );
    }
    // Remove currency tousan separator
    if( p_qsCurrencyString.contains( g_poPrefs->getCurrencySeparator() ) )
    {
        qsPureCurrency = qsPureCurrency.remove( g_poPrefs->getCurrencySeparator() );
    }
    // Remove spaces
    qsPureCurrency = qsPureCurrency.remove( " " );

    // Get value before and after decimal separator
    if( qsPureCurrency.contains( g_poPrefs->getCurrencyDecimalSeparator() ) )
    {
        QStringList qslCurr = qsPureCurrency.split( g_poPrefs->getCurrencyDecimalSeparator() );

        m_nValueLeft    = qslCurr.at(0).toInt();
        m_nValueRight   = qslCurr.at(1).toInt();
    }
    else
    {
        m_nValueLeft    = qsPureCurrency.toInt();
        m_nValueRight   = 0;
    }

    // Calculate full currency value (original *100)
    m_nValue = m_nValueLeft * 100 + m_nValueRight;
}
//====================================================================================
cCurrency::~cCurrency()
{

}
//====================================================================================
