#include <QtGui/QApplication>
#include "cdlgmain.h"

//====================================================================================

#include "../framework/qtframework.h"

//====================================================================================

cQTLogger            g_obLogger;
cQTMySQLConnection  *g_poDB;

using namespace std;

//====================================================================================

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    cDlgMain w;
    w.show();

    return a.exec();
}
