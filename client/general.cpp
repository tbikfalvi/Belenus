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
    cTracer obTrace( "cCurrency::cCurrency(QString,currType,int)" );

    m_nValue        = 0;
    m_nValueLeft    = 0;
    m_nValueRight   = 0;
    m_ctCurrType    = p_ctCurrencyType;
    m_nVatValue     = p_nVat;

    _init( p_qsCurrencyString, p_ctCurrencyType, p_nVat );
}
//*********************************************************************************************************************
// Class cCurrency
//*********************************************************************************************************************
cCurrency::cCurrency(int p_nCurrencyValue, currType p_ctCurrencyType, int p_nVat)
{
    cTracer obTrace( "cCurrency::cCurrency(int,currType,int)" );

    m_nValue        = 0;
    m_nValueLeft    = 0;
    m_nValueRight   = 0;
    m_ctCurrType    = p_ctCurrencyType;
    m_nVatValue     = p_nVat;

    QString qsCurrency = QString::number(p_nCurrencyValue);

    qsCurrency.insert( qsCurrency.length()-2, g_poPrefs->getCurrencyDecimalSeparator() );

    _init( qsCurrency, p_ctCurrencyType, p_nVat );

    g_obLogger(cSeverity::INFO) << "m_nValue " << m_nValue << EOM;
    g_obLogger(cSeverity::INFO) << "m_nValueNet " << m_nValueNet << EOM;
    g_obLogger(cSeverity::INFO) << "m_nValueGross " << m_nValueGross << EOM;
}
//====================================================================================
void cCurrency::_init(const QString &p_qsCurrencyString, currType p_ctCurrencyType, int p_nVat)
{
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

    // Remove currency tousand separator
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
        QString     qsRight = qslCurr.at(1);

        m_nValueLeft    = qslCurr.at(0).toInt();
        m_nValueRight   = qsRight.append("00").left(2).toInt();
    }
    else
    {
        m_nValueLeft    = qsPureCurrency.toInt();
        m_nValueRight   = 0;
    }

    // Calculate full currency value (original *100)
    m_nValue = m_nValueLeft * 100 + m_nValueRight;

    if( m_nVatValue > 0 )
    {
        if( p_ctCurrencyType == CURR_NET )
        {
            m_nValueNet   = m_nValue;
            m_nValueGross = m_nValue + (m_nValue * m_nVatValue) / 100;
        }
        else
        {
            m_nValueNet   = m_nValue  * 100 / (100 + m_nVatValue);
            m_nValueGross = m_nValue;
        }
    }
    else
    {
        m_nValueNet   = m_nValue;
        m_nValueGross = m_nValue;
    }

    g_obLogger(cSeverity::INFO) << "net/gross " << QString("%1/%2").arg(m_nValueNet).arg(m_nValueGross) << EOM;
}
//====================================================================================
cCurrency::~cCurrency()
{

}
//====================================================================================
QString cCurrency::currencyValue(currType p_ctCurrencyType)
{
    if( p_ctCurrencyType == CURR_GROSS )
    {
        m_nValue = m_nValueGross;
    }
    else
    {
        m_nValue = m_nValueNet;
    }

    m_nValueLeft  = m_nValue / 100;
    m_nValueRight = m_nValue % 100;

    g_obLogger(cSeverity::DEBUG) << "L/R " << QString("%1/%2").arg(m_nValueLeft).arg(m_nValueRight) << EOM;

    return QString( "%1" ).arg( m_nValue );
}
//====================================================================================
QString cCurrency::currencyString(currType p_ctCurrencyType)
{
    currencyValue( p_ctCurrencyType );

    QString qsRet = "";

    if( m_nValueRight > 0 )
        qsRet = QString( "%1%2%3" ).arg(m_nValueLeft).arg(g_poPrefs->getCurrencyDecimalSeparator()).arg(m_nValueRight);
    else
        qsRet = QString( "%1" ).arg(m_nValueLeft);

    return qsRet;
}
//====================================================================================
QString cCurrency::currencyStringSeparator(currType p_ctCurrencyType)
{
    currencyValue( p_ctCurrencyType );

    QString qsRet = "";

    if( m_nValueRight > 0 )
        qsRet = QString( "%1%2%3" ).arg(_separatedValue(m_nValueLeft)).arg(g_poPrefs->getCurrencyDecimalSeparator()).arg(m_nValueRight);
    else
        qsRet = QString( "%1" ).arg(_separatedValue(m_nValueLeft));

    return qsRet;
}
//====================================================================================
QString cCurrency::currencyFullStringShort( currType p_ctCurrencyType )
{
    currencyValue( p_ctCurrencyType );

    return QString( "%1%2%3 %4" ).arg(_separatedValue(m_nValueLeft)).arg(g_poPrefs->getCurrencyDecimalSeparator()).arg(m_nValueRight).arg(g_poPrefs->getCurrencyShort());
}
//====================================================================================
QString cCurrency::currencyFullStringLong( currType p_ctCurrencyType )
{
    currencyValue( p_ctCurrencyType );

    return QString( "%1%2%3 %4" ).arg(_separatedValue(m_nValueLeft)).arg(g_poPrefs->getCurrencyDecimalSeparator()).arg(m_nValueRight).arg(g_poPrefs->getCurrencyLong());
}
//====================================================================================
QString cCurrency::_separatedValue(int p_nValue)
{
    QString qsValue = QString::number( p_nValue );
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

    return qsRet;
}
//====================================================================================
