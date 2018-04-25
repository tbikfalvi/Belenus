
#include <QApplication>
#include <QTranslator>
#include <QMessageBox>
#include <QSettings>
#include <QtSql>
#include <QProcess>

#include "advertisementwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString qsCurrentPath = QDir::currentPath().replace( "\\", "/" );

    QSettings   obPrefFile( QString( "%1/belenus.ini" ).arg( qsCurrentPath ), QSettings::IniFormat );
    QString     qsLang = obPrefFile.value( QString::fromAscii( "Lang" ), "en" ).toString();

    QTranslator *poTransAdv = new QTranslator();
    QTranslator *poTransQT = new QTranslator();

    poTransAdv->load( QString("%1\\lang\\advertisement_%2.qm").arg( QDir::currentPath() ).arg( qsLang ) );
    poTransQT->load( QString("%1\\lang\\qt_%2.qm").arg( QDir::currentPath() ).arg( qsLang ) );

    a.installTranslator( poTransAdv );
    a.installTranslator( poTransQT );

    if( argc < 2 )
    {
        QSqlDatabase       *m_poDB;

        m_poDB = new QSqlDatabase( QSqlDatabase::addDatabase( "QMYSQL" ) );

        m_poDB->setHostName( "localhost" );
        m_poDB->setDatabaseName( "belenus" );
        m_poDB->setUserName( "belenus" );
        m_poDB->setPassword( "belenus" );
        m_poDB->open();

        QSqlQuery poQuery = m_poDB->exec( QString( "SELECT advertisementId FROM advertisements WHERE active=1" ) );

        while( poQuery.next() )
        {
            QSettings   obPrefFile( "advertisement.cmd", QSettings::IniFormat );

            QString qsStatus = obPrefFile.value( QString::fromAscii( "Advertisement%1/Status" ).arg( poQuery.value(0).toUInt() ), "" ).toString();

            if( qsStatus.compare( "RUNNING" ) == 0 )
            {
                continue;
            }

            QProcess *qpAdv = new QProcess(0);

            if( !qpAdv->startDetached( QString("Advertisement.exe %1").arg( poQuery.value(0).toUInt() ) ) )
            {
                QMessageBox::warning( 0, QObject::tr("Warning"),
                                      QObject::tr("Error occured when starting process:Advertisement.exe\n\nError code: %1\n"
                                         "0 > The process failed to start.\n"
                                         "1 > The process crashed some time after starting successfully.\n"
                                         "2 > The last waitFor...() function timed out.\n"
                                         "4 > An error occurred when attempting to write to the process.\n"
                                         "3 > An error occurred when attempting to read from the process.\n"
                                         "5 > An unknown error occurred.").arg(qpAdv->error()) );
            }
            delete qpAdv;
        }
        m_poDB->close();

/*        QMessageBox::warning( 0, QObject::tr("Warning"),
                              QObject::tr("Unable to start advertisement window.\n"
                                          "The identification number is not defined.") );*/
        return 1;
    }

    advertisementwindow w( 0, atoi(argv[1]) );
    w.show();

    return a.exec();
}
