#include <QApplication>
#include <QTranslator>
#include <QDir>
#include <QObject>
#include <QSettings>
#include <QMessageBox>

#include "mainwindow.h"
#include "../language/language.h"

QApplication            *apMainApp;
cLanguage                g_obLanguage;
//QTranslator     *poTransBackup;
//QTranslator     *poTransQT;

int main(int argc, char *argv[])
{
    apMainApp = new QApplication(argc, argv);

    QString qsCurrentPath = QDir::currentPath().replace( "\\", "/" );

    QSettings   obPrefFile( QString( "%1/belenus.ini" ).arg( qsCurrentPath ), QSettings::IniFormat );
    QString     qsLang = obPrefFile.value( QString::fromAscii( "Lang" ), "en" ).toString();

    g_obLanguage.getLanguages();
    g_obLanguage.init( apMainApp, "dbbackup", "_" );

    MainWindow::teAction    teAction    = MainWindow::ACT_BACKUP;
    QString                 qsFileName  = "";

    if( argc > 1 && strcmp(argv[1],"-restore") == 0 )
    {
        teAction = MainWindow::ACT_RESTORE;
    }
    else if( argc > 1 && strcmp(argv[1],"-execute") == 0 )
    {
        teAction = MainWindow::ACT_EXECUTE;
        qsFileName = QString( argv[2] );
    }

    MainWindow *wndMain = new MainWindow( 0, teAction, qsFileName );

    wndMain->show();

    return apMainApp->exec();
}
