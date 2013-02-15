#include <QtGui/QApplication>
#include <QTranslator>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator *poTrans = new QTranslator();
    poTrans->load( "BelenusActivator_hu.qm" );
    a.installTranslator( poTrans );

    a.setWindowIcon( QIcon(":/icons/BelenusActivator.ico") );

    MainWindow w;
    w.show();

    return a.exec();
}
