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
#include <QSystemTrayIcon>

//====================================================================================
class cGeneral
{
    //Q_OBJECT

public:

    enum msgBoxType
    {
        MSG_INFORMATION = 0,
        MSG_WARNING,
        MSG_ATTENTION,
        MSG_ERROR,
        MSG_QUESTION
    };

    cGeneral();
    ~cGeneral();

    void             setApplication( QApplication *p_poApplication );
    void             setApplicationLanguage( const QString &p_qsLang );
    int              customMsgBox( QWidget *parent, msgBoxType msgtype, QString buttonstext, QString msg, QString details = "" );
//    QString          convertCurrency( int p_nCurrencyValue, QString p_qsCurrency );
    void             backupDatabase( QWidget *parent );
    bool             isSystemAdmin();
    bool             isExtendedAdmin();
    bool             isExtendedOrSystemAdmin();
    void             showPatientCardInformation( QString p_qsBarcode, int p_nCloseSeconds = 0 );
    void             showPatientLastVisitInformation(QString p_qsBarcode, int p_nCloseSeconds = 0 );
    QString          getPatientCardInformationString( QString p_qsBarcode );
    QStringList      getPatientCardUnusedUnits( unsigned int p_uiCardId );
    bool             isShoppingCartHasItems();
    bool             isAppicationRunning( QString p_qsAppName );
    void             initSysTrayIcon();
    void             setSysTrayIconParent( QObject *parent = 0 );
    void             showTrayInfo( QString p_qsMessage );
    void             showTrayWarning( QString p_qsMessage );
    void             showTrayError( QString p_qsMessage );
    bool             isArchiveOnDifferentPath();
    void             setWindowMainWidget( QWidget *poWindow );
    void             setWindowSecondaryWidget( QWidget *poWindow );

    QSystemTrayIcon *m_stIcon;

private:
    QApplication    *m_poMainApplication;
    QTranslator     *m_poBlTr;
    QTranslator     *m_poQtTr;
    bool             m_bIsLanguageLoaded;
    QWidget         *m_poWindowMain;
    QWidget         *m_poWindowSecondary;
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
    bool        m_bIsNegative;

    void        _init( const QString &p_qsCurrencyString = "", currType p_ctCurrencyType = CURR_GROSS, int p_nVat = 0 );
    QString     _separatedValue( int p_nValue );
};
//====================================================================================

#endif // GENERAL_H
