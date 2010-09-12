#include <QtGui/QApplication>
#include "cdlgmain.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    cDlgMain w;
    w.show();

    return a.exec();
}
