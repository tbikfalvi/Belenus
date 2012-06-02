#include <QtGui/QApplication>
#include <QTranslator>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator *poTrans = new QTranslator();
    poTrans->load( "dbtool_hu.qm" );
    a.installTranslator( poTrans );

    a.setWindowIcon( QIcon(":/icons/DBTool.ico") );

    MainWindow w;
    w.show();

    return a.exec();
}
