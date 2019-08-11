//====================================================================================
//
// Belenus Kliens alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : language.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
// Nyelvek kezelesere szolgalo osztaly
//====================================================================================

#include <QDir>
#include <QMessageBox>

//====================================================================================

#include "language.h"

//====================================================================================
cLanguage::cLanguage()
//====================================================================================
{
    obProcessDoc            = new QDomDocument( "StarterProcess" );
    m_poApTr                = new QTranslator();
    m_poQtTr                = new QTranslator();
    m_bIsLanguageLoaded     = false;
    m_qsApplicationPrefix   = "";
    m_qsLang                = "";
    m_qsLangSeparator       = "";
    m_nErrorCode            = 0;
}
//====================================================================================
cLanguage::~cLanguage()
//------------------------------------------------------------------------------------
{
    delete obProcessDoc;
    delete m_poApTr;
    delete m_poQtTr;
}
//====================================================================================
void cLanguage::init(QApplication *p_poApplication, const QString &p_qsAppPrefix, const QString &p_qsLangSeparator)
//------------------------------------------------------------------------------------
{
    m_poMainApplication     = p_poApplication;
    m_qsApplicationPrefix   = p_qsAppPrefix;
    m_qsLangSeparator       = p_qsLangSeparator;

    setApplicationLanguage( m_qsLang.left(2) );

    m_bIsLanguageLoaded = true;
}
//====================================================================================
void cLanguage::reloadLanguage( const QString &p_qsLang )
//------------------------------------------------------------------------------------
{
    if( p_qsLang.length() != 0 )
    {
        m_qsLang = p_qsLang;
    }
    setApplicationLanguage( m_qsLang.left(2) );
}
//====================================================================================
QStringList cLanguage::getLanguages()
//------------------------------------------------------------------------------------
{
    QString         qsCurrentPath = QDir::currentPath().replace( "\\", "/" );
    QFile           qfFile( QString( "%1/lang/languages.inf" ).arg( qsCurrentPath ) );
    QStringList     qslRet;

    if( !qfFile.exists() )
    {
        m_nErrorCode = ERR_LANG_FILE_MISSING;
        return qslRet;
    }

    if( !qfFile.open(QIODevice::ReadOnly) )
    {
        m_nErrorCode = ERR_UNABLE_TO_READ_LANG_FILE;
        return qslRet;
    }

    QString      qsErrorMsg  = "";
    int          inErrorLine = 0;

    qfFile.seek( 0 );
    if( !obProcessDoc->setContent( &qfFile, &qsErrorMsg, &inErrorLine ) )
    {
        m_nErrorCode = ERR_PARSING_FILE_FAILED;
        qfFile.close();
        return qslRet;
    }
    qfFile.close();

    QDomElement     docRoot     = obProcessDoc->documentElement();
    QDomNodeList    obLanguage  = docRoot.elementsByTagName( "language" );;

    for( int i=0; i<obLanguage.count(); i++ )
    {
        qslRet << QString( "%1|%2" )
                         .arg( obLanguage.at(i).toElement().attribute("shortname") )
                         .arg( obLanguage.at(i).toElement().attribute("name") );

        if( obLanguage.at(i).toElement().attribute("current","no").compare( "yes" ) == 0 )
        {
            m_qsLang = obLanguage.at(i).toElement().attribute("shortname");
        }
    }

    m_nErrorCode = 0;
    return qslRet;
}
//====================================================================================
void cLanguage::saveCurrentLanguage(QString p_qsLang)
//------------------------------------------------------------------------------------
{

}
//====================================================================================
int cLanguage::setLanguageCombo(QComboBox *p_cmbLang)
//------------------------------------------------------------------------------------
{
    int inLangIdx = 0;

    if( m_bIsLanguageLoaded )
    {
        m_cmbLang = p_cmbLang;

        QStringList qslLanguages = getLanguages();
        QStringList obLangCodes;

        for( int nLang=0; nLang<qslLanguages.count(); nLang++ )
        {
            obLangCodes << QString( "%1 (%2)" ).arg( qslLanguages.at(nLang).split("|").at(0) )
                                               .arg( qslLanguages.at(nLang).split("|").at(1) );
            if( qslLanguages.at(nLang).split("|").at(0).compare( m_qsLang ) == 0 )
            {
                inLangIdx = nLang;
            }
        }

        m_cmbLang->addItems( obLangCodes );
        m_cmbLang->setCurrentIndex( inLangIdx );
    }

    return inLangIdx;
}
//====================================================================================
void cLanguage::setApplicationLanguage( const QString &p_qsLang )
//------------------------------------------------------------------------------------
{
    if( m_bIsLanguageLoaded )
    {
        m_poMainApplication->removeTranslator( m_poApTr );
        m_poMainApplication->removeTranslator( m_poQtTr );
    }

    QString qsLangAp = QString("lang/%1%2%3.qm").arg( m_qsApplicationPrefix ).arg( m_qsLangSeparator ).arg( p_qsLang );
    QString qsLangQT = QString("lang/qt_%1.qm").arg( p_qsLang );

    m_poApTr->load( qsLangAp );
    m_poQtTr->load( qsLangQT );

    m_poMainApplication->installTranslator( m_poApTr );
    m_poMainApplication->installTranslator( m_poQtTr );
}
//====================================================================================
