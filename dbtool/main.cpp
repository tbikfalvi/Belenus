#include <QtGui/QApplication>
#include <QTranslator>
#include "cdlgmain.h"

//====================================================================================

#include "belenusdbtool.h"

//====================================================================================

cQTLogger            g_obLogger;
cQTMySQLConnection  *g_poDB;

using namespace std;

//====================================================================================

int main(int argc, char *argv[])
{
    QApplication apMainApp(argc, argv);

    QTranslator *poTrans = new QTranslator();
    QString     qsTransFile = "BelenusDBTool_hu.qm";
    poTrans->load( qsTransFile );
    apMainApp.installTranslator( poTrans );

    cDlgMain w;
    w.show();

    return apMainApp.exec();
}
