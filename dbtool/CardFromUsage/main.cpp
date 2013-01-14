#include <QtGui/QApplication>
#include <QTranslator>
#include "kiwisunberlet.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator *poTrans = new QTranslator();
//    QString     qsTransFile = "lang/" + g_poPrefs->getLangFilePrefix() + g_poPrefs->getLang() + ".qm";
    poTrans->load( "kiwisunberlet_hu.qm" );
    a.installTranslator( poTrans );

    a.setWindowIcon( QIcon(":/icons/KiwiSunBerlet.ico") );

    KiwiSunBerlet w;
    w.show();

    return a.exec();
}
