#include <QApplication>
#include <QTranslator>
#include <QDir>
#include <QObject>
#include <QSettings>
#include <QMessageBox>

#include "mainwindow.h"

QApplication    *apMainApp;
QTranslator     *poTransBackup;
QTranslator     *poTransQT;

int main(int argc, char *argv[])
{
    apMainApp = new QApplication(argc, argv);

    QSettings   obPrefFile( "belenus.ini", QSettings::IniFormat );
    QString     qsLang = obPrefFile.value( QString::fromAscii( "Lang" ), "hu" ).toString();

    poTransBackup = new QTranslator();
    poTransQT = new QTranslator();

    poTransBackup->load( QString("%1\\lang\\dbbackup_%2.qm").arg(QDir::currentPath()).arg(qsLang) );
    poTransQT->load( QString("%1\\lang\\qt_%2.qm").arg(QDir::currentPath()).arg(qsLang) );

    apMainApp->installTranslator( poTransBackup );
    apMainApp->installTranslator( poTransQT );

    MainWindow wndMain;

    wndMain.show();

    return apMainApp->exec();
}
