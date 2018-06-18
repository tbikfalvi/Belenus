//====================================================================================
//
// Belenus Kliens alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : language.h
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
// Nyelvek kezelesere szolgalo osztaly
//====================================================================================

#ifndef LANGUAGE_H
#define LANGUAGE_H

//====================================================================================

#include <QApplication>
#include <QTranslator>
#include <QString>
#include <QFile>
#include <QDomDocument>
#include <QComboBox>

//====================================================================================

#define ERR_LANG_FILE_MISSING           1
#define ERR_UNABLE_TO_READ_LANG_FILE    2
#define ERR_PARSING_FILE_FAILED         3

//====================================================================================
class cLanguage
{
    //Q_OBJECT

public:

    cLanguage();
    ~cLanguage();

    void             init(QApplication *p_poApplication,
                          const QString &p_qsAppPrefix = "",
                          const QString &p_qsLangSeparator = "",
                          const QString &p_qsLang = "" );
    void             reloadLanguage( const QString &p_qsLang = "" );
    QStringList      getLanguages();
    int              setLanguageCombo( QComboBox *p_cmbLang );

    int              errorCode()    { return m_nErrorCode; }

private:

    void             setApplicationLanguage( const QString &p_qsLang );

    QApplication    *m_poMainApplication;
    QTranslator     *m_poApTr;
    QTranslator     *m_poQtTr;
    QDomDocument    *obProcessDoc;
    QComboBox       *m_cmbLang;
    bool             m_bIsLanguageLoaded;
    QString          m_qsApplicationPrefix;
    QString          m_qsLang;
    QString          m_qsLangSeparator;
    int              m_nErrorCode;
};
//====================================================================================

#endif // GENERAL_H
