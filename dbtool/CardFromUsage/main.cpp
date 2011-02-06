#include <QtGui/QApplication>
#include "kiwisunberlet.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setWindowIcon( QIcon(":/icons/KiwiSunBerlet.ico") );

    KiwiSunBerlet w;
    w.show();

    return a.exec();
}
