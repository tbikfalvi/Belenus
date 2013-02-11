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

#include <QApplication>
#include <QTranslator>
#include <QString>

//====================================================================================
class cGeneral
{
    //Q_OBJECT

public:
    cGeneral();
    ~cGeneral();

    void        setApplication( QApplication *p_poApplication );
    void        setApplicationLanguage( const QString &p_qsLang );
    QString     convertCurrency( int p_nCurrencyValue, QString p_qsCurrency );

private:
    QApplication    *m_poMainApplication;
    QTranslator     *m_poBlTr;
    QTranslator     *m_poQtTr;
    bool             m_bIsLanguageLoaded;
};
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
    ~cCurrency();

    QString     currencyFullString() const;

private:

    currType    m_ctCurrType;
    int         m_nVatValue;
    int         m_nValueLeft;
    int         m_nValueRight;
    int         m_nValue;
};
//====================================================================================

#endif // GENERAL_H
