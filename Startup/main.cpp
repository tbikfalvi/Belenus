
#include <QApplication>
#include <QTranslator>
#include <QDir>

#include "mainwindow.h"

QTranslator     *poTransStartup;
QTranslator     *poTransQT;
QApplication    *apMainApp;

int main(int argc, char *argv[])
{
    apMainApp = new QApplication(argc, argv);

    poTransStartup = new QTranslator();
    poTransQT = new QTranslator();

    poTransStartup->load( QString("%1\\lang\\startup_en.qm").arg(QDir::currentPath()) );
    poTransQT->load( QString("%1\\lang\\qt_en.qm").arg(QDir::currentPath()) );

    apMainApp->installTranslator( poTransStartup );
    apMainApp->installTranslator( poTransQT );

    MainWindow w;
    w.show();

    return apMainApp->exec();
}
