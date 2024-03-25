//====================================================================================

#ifndef CURRENCY_H
#define CURRENCY_H

//====================================================================================

#include <QString>

extern QString                  g_qsCurrencyShort;
extern QString                  g_qsCurrencyLong;
extern QString                  g_qsCurrencySeparator;
extern QString                  g_qsCurrencyDecimalSeparator;

//====================================================================================
class cCurrency
{
public:

    enum currType
    {
        CURR_NET = 0,
        CURR_GROSS
    };

    cCurrency( const QString &p_qsCurrencyString = "", currType p_ctCurrencyType = CURR_GROSS, int p_nVat = 0 );
    cCurrency( int p_nCurrencyValue = 0, currType p_ctCurrencyType = CURR_GROSS, int p_nVat = 0 );
    ~cCurrency();

    QString     currencyValue( currType p_ctCurrencyType = CURR_GROSS );
    QString     currencyString( currType p_ctCurrencyType = CURR_GROSS );
    QString     currencyStringSeparator( currType p_ctCurrencyType = CURR_GROSS );
    QString     currencyFullStringShort( currType p_ctCurrencyType = CURR_GROSS );
    QString     currencyFullStringLong( currType p_ctCurrencyType = CURR_GROSS );

private:

    currType    m_ctCurrType;
    int         m_nVatValue;
    int         m_nValueLeft;
    int         m_nValueRight;
    int         m_nValue;
    int         m_nValueNet;
    int         m_nValueGross;
/*    QString     m_qsCurrencyShort;
    QString     m_qsCurrencyLong;
    QString     m_qsCurrencySeparator;
    QString     m_qsCurrencyDecimalSeparator;*/
    bool        m_bIsNegative;

    void        _init( const QString &p_qsCurrencyString = "", currType p_ctCurrencyType = CURR_GROSS, int p_nVat = 0 );
    QString     _separatedValue( int p_nValue );
    QString     _currencyDecimalSeparator();
    QString     _currencySeparator();
    QString     _currencyLong();
    QString     _currencyShort();
};
//====================================================================================

#endif // GENERAL_H
