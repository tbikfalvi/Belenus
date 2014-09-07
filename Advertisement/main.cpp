
#include <QApplication>
#include <QTranslator>
#include <QMessageBox>
#include <QSettings>

#include "advertisementwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSettings   obPrefFile( "belenus.ini", QSettings::IniFormat );
    QString     qsLang = obPrefFile.value( QString::fromAscii( "Lang" ), "en" ).toString();

    QTranslator *poTransAdv = new QTranslator();
    QTranslator *poTransQT = new QTranslator();

    poTransAdv->load( QString("%1\\lang\\advertisement_%2.qm").arg( QDir::currentPath() ).arg( qsLang ) );
    poTransQT->load( QString("%1\\lang\\qt_%2.qm").arg( QDir::currentPath() ).arg( qsLang ) );

    a.installTranslator( poTransAdv );
    a.installTranslator( poTransQT );

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
