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

int main(int argc, char *argv[])
{
    apMainApp = new QApplication(argc, argv);

    g_obLanguage.getLanguages();
    g_obLanguage.init( apMainApp, "initodb", "_" );

    MainWindow *wndMain = new MainWindow( 0 );

    wndMain->show();

    return apMainApp->exec();
}
