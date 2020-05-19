#include <QApplication>
#include <QTranslator>
#include <QDir>
#include <QObject>
#include <QSettings>
#include <QMessageBox>

#include "mainwindow.h"
#include "../framework/qtlogger.h"
#include "../framework/qtframework.h"
#include "../framework/logger/FileWriter.h"
#include "../language/language.h"

#define APPLICATION_VERSION_NUMBER  "1.9.0.0"

QApplication            *apMainApp;
cQTLogger                g_obLogger;
FileWriter               g_obLogFileWriter("dbbackup_%1.log");
cQTMySQLConnection      *g_poDB;
cLanguage                g_obLanguage;

int main(int argc, char *argv[])
{
    apMainApp = new QApplication(argc, argv);

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

    MainWindow *wndMain = new MainWindow( 0, APPLICATION_VERSION_NUMBER, teAction, qsFileName );

    wndMain->show();

    return apMainApp->exec();
}
