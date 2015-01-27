
#include <QSettings>
#include <QStringList>
#include <QMessageBox>
#include "currency.h"

//*********************************************************************************************************************
// Class cCurrency
//*********************************************************************************************************************
cCurrency::cCurrency(const QString &p_qsCurrencyString, currType p_ctCurrencyType, int p_nVat)
{
    m_nValue        = 0;
    m_nValueLeft    = 0;
    m_nValueRight   = 0;
    m_ctCurrType    = p_ctCurrencyType;
    m_nVatValue     = p_nVat;
    m_bIsNegative   = false;

    QSettings obPrefFile( "belenus.ini", QSettings::IniFormat );

    m_qsCurrencyShort               = obPrefFile.value( QString::fromAscii( "Currency/Short" ), "Ft." ).toString();
    m_qsCurrencyLong                = obPrefFile.value( QString::fromAscii( "Currency/Long" ), "Forint" ).toString();
    m_qsCurrencySeparator           = obPrefFile.value( QString::fromAscii( "Currency/Separator" ), "," ).toString();
    m_qsCurrencyDecimalSeparator    = obPrefFile.value( QString::fromAscii( "Currency/Decimal" ), "." ).toString();

    _init( p_qsCurrencyString, p_ctCurrencyType, p_nVat );
}
//*********************************************************************************************************************
// Class cCurrency
//*********************************************************************************************************************
cCurrency::cCurrency(int p_nCurrencyValue, currType p_ctCurrencyType, int p_nVat)
{
    m_nValue        = 0;
    m_nValueLeft    = 0;
    m_nValueRight   = 0;
    m_ctCurrType    = p_ctCurrencyType;
    m_nVatValue     = p_nVat;

    QSettings obPrefFile( "belenus.ini", QSettings::IniFormat );

    m_qsCurrencyShort               = obPrefFile.value( QString::fromAscii( "Currency/Short" ), "Ft." ).toString();
    m_qsCurrencyLong                = obPrefFile.value( QString::fromAscii( "Currency/Long" ), "Forint" ).toString();
    m_qsCurrencySeparator           = obPrefFile.value( QString::fromAscii( "Currency/Separator" ), "," ).toString();
    m_qsCurrencyDecimalSeparator    = obPrefFile.value( QString::fromAscii( "Currency/Decimal" ), "." ).toString();

    QString qsCurrency = QString::number(p_nCurrencyValue);

    qsCurrency.insert( qsCurrency.length()-2, _currencyDecimalSeparator() );

    _init( qsCurrency, p_ctCurrencyType, p_nVat );
}
//====================================================================================
void cCurrency::_init(const QString &p_qsCurrencyString, currType p_ctCurrencyType, int /*p_nVat*/)
{
    QString qsPureCurrency = p_qsCurrencyString;

    // Remove currency string
    if( qsPureCurrency.contains( _currencyLong() ) )
    {
        qsPureCurrency = qsPureCurrency.remove( _currencyLong() );
    }
    else if( qsPureCurrency.contains( _currencyShort() ) )
    {
        qsPureCurrency = qsPureCurrency.remove( _currencyShort() );
    }

    // Remove currency tousand separator
    if( p_qsCurrencyString.contains( _currencySeparator() ) )
    {
        qsPureCurrency = qsPureCurrency.remove( _currencySeparator() );
    }

    // Remove spaces
    qsPureCurrency = qsPureCurrency.remove( " " );

    m_bIsNegative = false;
    // Remove the minus sign
    if( qsPureCurrency.left(1).compare("-") == 0 )
    {
        m_bIsNegative = true;
        qsPureCurrency = qsPureCurrency.remove( "-" );
    }

    // Get value before and after decimal separator
    if( qsPureCurrency.contains( _currencyDecimalSeparator() ) )
    {
        QStringList qslCurr = qsPureCurrency.split( _currencyDecimalSeparator() );
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

    return QString( "%1%2" ).arg(m_bIsNegative?"-":"").arg( m_nValue );
}
//====================================================================================
QString cCurrency::currencyString(currType p_ctCurrencyType)
{
    currencyValue( p_ctCurrencyType );

    QString qsRet = "";

    if( m_nValueRight > 0 )
        qsRet = QString( "%1%2%3%4" ).arg(m_bIsNegative?"-":"").arg(m_nValueLeft).arg(_currencyDecimalSeparator()).arg(m_nValueRight);
    else
        qsRet = QString( "%1%2" ).arg(m_bIsNegative?"-":"").arg(m_nValueLeft);

    return qsRet;
}
//====================================================================================
QString cCurrency::currencyStringSeparator(currType p_ctCurrencyType)
{
    currencyValue( p_ctCurrencyType );

    QString qsRet = "";

    if( m_nValueRight > 0 )
        qsRet = QString( "%1%2%3%4" ).arg(m_bIsNegative?"-":"").arg(_separatedValue(m_nValueLeft)).arg(_currencyDecimalSeparator()).arg(m_nValueRight);
    else
        qsRet = QString( "%1%2" ).arg(m_bIsNegative?"-":"").arg(_separatedValue(m_nValueLeft));

    return qsRet;
}
//====================================================================================
QString cCurrency::currencyFullStringShort( currType p_ctCurrencyType )
{
    return QString( "%1 %2" ).arg(currencyStringSeparator(p_ctCurrencyType)).arg(_currencyShort());
}
//====================================================================================
QString cCurrency::currencyFullStringLong( currType p_ctCurrencyType )
{
    return QString( "%1 %2" ).arg(currencyStringSeparator(p_ctCurrencyType)).arg(_currencyLong());
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
            qsRet.insert( 0, _currencySeparator() );
            qsValue.truncate( qsValue.length()-3 );
        }
    }
    qsRet.insert( 0, qsValue );

    return qsRet;
}
//====================================================================================
QString cCurrency::_currencyDecimalSeparator()
{
    return m_qsCurrencyDecimalSeparator;
}
//====================================================================================
QString cCurrency::_currencySeparator()
{
    return m_qsCurrencySeparator;
}
//====================================================================================
QString cCurrency::_currencyLong()
{
    return m_qsCurrencyLong;
}
//====================================================================================
QString cCurrency::_currencyShort()
{
    return m_qsCurrencyShort;
}
