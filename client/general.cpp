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
    m_poBlTr       = new QTranslator();
    m_poQtTr            = new QTranslator();
    m_bIsLanguageLoaded = false;
}
//====================================================================================
cGeneral::~cGeneral()
//====================================================================================
{
    delete m_poBlTr;
    delete m_poQtTr;
}
//====================================================================================
void cGeneral::setApplication( QApplication *p_poApplication )
//====================================================================================
{
    m_poMainApplication = p_poApplication;
}
//====================================================================================
void cGeneral::setApplicationLanguage( const QString &p_qsLang )
//====================================================================================
{
    if( m_bIsLanguageLoaded )
    {
        m_poMainApplication->removeTranslator( m_poBlTr );
        m_poMainApplication->removeTranslator( m_poQtTr );
    }

    QString qsLangBl = QString("lang/%1%2.qm").arg( g_poPrefs->getLangFilePrefix() ).arg( p_qsLang );
    QString qsLangQT = QString("lang/qt_%1.qm").arg( p_qsLang );

    m_poBlTr->load( qsLangBl );
    m_poQtTr->load( qsLangQT );

    m_poMainApplication->installTranslator( m_poBlTr );
    m_poMainApplication->installTranslator( m_poQtTr );
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
//*********************************************************************************************************************
// Class cCurrency
//*********************************************************************************************************************
cCurrency::cCurrency(const QString &p_qsCurrencyString, currType p_ctCurrencyType, int p_nVat)
{
    cTracer obTrace( "cCurrency::cCurrency" );

    m_nValue        = 0;
    m_nValueLeft    = 0;
    m_nValueRight   = 0;
    m_ctCurrType    = p_ctCurrencyType;
    m_nVatValue     = p_nVat;

    g_obLogger(cSeverity::INFO) << "fullstr: [" << p_qsCurrencyString << "]" << EOM;

    QString qsPureCurrency = p_qsCurrencyString;

    // Remove currency string
    if( qsPureCurrency.contains( g_poPrefs->getCurrencyLong() ) )
    {
        qsPureCurrency = qsPureCurrency.remove( g_poPrefs->getCurrencyLong() );
    }
    else if( qsPureCurrency.contains( g_poPrefs->getCurrencyShort() ) )
    {
        qsPureCurrency = qsPureCurrency.remove( g_poPrefs->getCurrencyShort() );
    }
    g_obLogger(cSeverity::INFO) << "1: [" << qsPureCurrency << "]" << EOM;

    // Remove currency tousand separator
    if( p_qsCurrencyString.contains( g_poPrefs->getCurrencySeparator() ) )
    {
        qsPureCurrency = qsPureCurrency.remove( g_poPrefs->getCurrencySeparator() );
    }
    g_obLogger(cSeverity::INFO) << "2: [" << qsPureCurrency << "]" << EOM;

    // Remove spaces
    qsPureCurrency = qsPureCurrency.remove( " " );
    g_obLogger(cSeverity::INFO) << "3: [" << qsPureCurrency << "]" << EOM;

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

    g_obLogger(cSeverity::INFO) << "full/left/right " << QString("%1/%2/%3").arg(m_nValue).arg(m_nValueLeft).arg(m_nValueRight) << EOM;
}
//====================================================================================
cCurrency::~cCurrency()
{

}
//====================================================================================
QString cCurrency::currencyFullString() const
{
    return QString( "%1%2%3 %4" ).arg(m_nValueLeft).arg(g_poPrefs->getCurrencyDecimalSeparator()).arg(m_nValueRight).arg(g_poPrefs->getCurrencyShort());
}
