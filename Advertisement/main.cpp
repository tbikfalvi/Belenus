
#include <QApplication>
#include <QTranslator>
#include <QMessageBox>

#include "advertisementwindow.h"

cQTLogger g_obLogger;

int main(int argc, char *argv[])
{
    if( argc < 2 )
    {
        QMessageBox::warning( 0, QObject::tr("Warning"),
                              QObject::tr("Unable to start advertisement window.\n"
                                          "The identification number is not defined.") );
        return 0;
    }

    QApplication appMain(argc, argv);
/*
    g_poDB     = new cQTMySQLConnection;

    g_poPrefs  = new cPreferences( QString::fromAscii( "./belenus.ini" ) );
    g_poPrefs->setDBAccess( "localhost", "belenus", "belenus", "belenus" );

    QString qsLangBl = QString("lang/advertisement_%1.qm").arg( g_poPrefs->getLang() );
    QString qsLangQT = QString("lang/qt_%1.qm").arg( g_poPrefs->getLang() );

    QTranslator *m_poAdvTr = new QTranslator();
    QTranslator *m_poQtTr = new QTranslator();

    m_poAdvTr->load( qsLangBl );
    m_poQtTr->load( qsLangQT );

    appMain.installTranslator( m_poAdvTr );
    appMain.installTranslator( m_poQtTr );

    g_poDB->open();
    g_poPrefs->loadDBSettings();
*/
    cDlgAdvertisementWindow wndAdv;
    wndAdv.show();

    return appMain.exec();
}
