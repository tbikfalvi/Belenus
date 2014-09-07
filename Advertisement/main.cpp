
#include <QApplication>
#include <QTranslator>
#include <QMessageBox>

#include "advertisementwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if( argc < 2 )
    {
        QMessageBox::warning( 0, QObject::tr("Warning"),
                              QObject::tr("Unable to start advertisement window.\n"
                                          "The identification number is not defined.") );
        return 1;
    }

    advertisementwindow w( 0, atoi(argv[1]) );
    w.show();

    return a.exec();
}
