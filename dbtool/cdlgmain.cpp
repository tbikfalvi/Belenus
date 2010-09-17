
#include <QMessageBox>
#include <QFile>
#include <QDate>
#include <QDir>

#include <iostream>
#include <stdio.h>

#include "../framework/qtframework.h"

#include "cdlgmain.h"
#include "ui_cdlgmain.h"

cDlgMain::cDlgMain(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    setWindowIcon( QIcon("./device.png") );

    pbCheckFiles->setIcon( QIcon("./check.png") );
    pbImportPatientCardTypes->setIcon( QIcon("./patientcards.png") );
    pbImportPatientCards->setIcon( QIcon("./patientcard.png") );
    pbImportPatientCardUsages->setIcon( QIcon("./patientcard.png") );
    pbImportUsers->setIcon( QIcon("./user.png") );
    pbExit->setIcon( QIcon("./exit.png") );

    m_qsAppPath = QDir::currentPath();

    ledImportPath->setText( m_qsAppPath );
    ledExportPath->setText( m_qsAppPath );

    pbImportPatientCardTypes->setEnabled( false );
    pbImportPatientCards->setEnabled( false );
    pbImportPatientCardUsages->setEnabled( false );
    pbImportUsers->setEnabled( false );
    pbDisconnect->setEnabled( false );

    g_poDB = new cQTMySQLConnection;
}

cDlgMain::~cDlgMain()
{
    if( g_poDB ) delete g_poDB;
}

void cDlgMain::EnCode( char *str, int size )
{
   for(int i=0;i<size;i++)
   {
      str[i] ^= 11;
   }
}

void cDlgMain::DeCode( char *str, int size )
{
   for(int i=0;i<size;i++)
   {
      str[i] ^= 11;
   }
}

unsigned int cDlgMain::patientCardId( QString /*p_qsBarcode*/ )
{
    return 0;
}

bool cDlgMain::checkFile( QString p_qsFileName )
{
    QFile   *obQFTemp = NULL;
    bool     bRet = false;

    m_qsFullName = m_qsDATPath + (!m_qsDATPath.right(1).compare("\\")?QString(""):QString("\\")) + p_qsFileName;
    obQFTemp = new QFile( m_qsFullName );
    if( obQFTemp->exists() )
    {
        listLog->addItem( tr("The %1 file exists.").arg(p_qsFileName) );
        bRet = true;
    }
    else
    {
        listLog->addItem( tr("The %1.dat file not found.").arg(p_qsFileName) );
    }

    if( obQFTemp ) delete obQFTemp;

    return bRet;
}

void cDlgMain::on_pbCheckFiles_clicked()
{
    listLog->clear();

    m_qsDATPath = ledImportPath->text();
    m_qsDATPath = m_qsDATPath.replace( "/", "\\" );

    if( QDir::setCurrent( m_qsDATPath ) )
    {
        QDir::setCurrent( m_qsAppPath );
        listLog->addItem( tr( "Path of the DAT files checked." ) );
    }
    else
    {
        listLog->addItem( tr( "Invalid path defined for DAT files." ) );
        QMessageBox::warning( this, tr("Warning"),
                              tr("The defined path of the DAT files is\n"
                                 "invalid or not exists.\n"
                                 "Please check the defined path.") );
        ledImportPath->setFocus();
        return;
    }

    m_qsSQLPath = ledExportPath->text();
    m_qsSQLPath = m_qsSQLPath.replace( "/", "\\" );

    if( QDir::setCurrent( m_qsSQLPath ) )
    {
        QDir::setCurrent( m_qsAppPath );
        listLog->addItem( tr( "Path of the SQL files checked." ) );
    }
    else
    {
        listLog->addItem( tr( "Invalid path defined for SQL files." ) );
        QMessageBox::warning( this, tr("Warning"),
                              tr("The defined path of the SQL files is\n"
                                 "invalid or not exists.\n"
                                 "Please check the defined path.") );
        ledExportPath->setFocus();
        return;
    }

    bool    bIsPCTOK    = checkFile( "brlttpsfsv.dat" );
    bool    bIsPCOK     = checkFile( "brltfsv.dat");
    bool    bIsPCHOK    = checkFile( "brlthsznltfsv.dat");
    bool    bIsOk       = false;

    if( !bIsPCTOK && !bIsPCOK && bIsPCHOK )
        return;

    ledLicenceId->text().toInt( &bIsOk );
    if( !bIsOk )
    {
        QMessageBox::warning( this, tr("Warning"),
                              tr("The defined LicenceId is invalid.\n"
                                 "Please use only numerical characters.") );
        listLog->addItem( tr("LicenceId is invalid.") );
        ledLicenceId->setFocus();
        return;
    }

    if( bIsPCTOK )
        pbImportPatientCardTypes->setEnabled( true );
    else
        pbImportPatientCardTypes->setEnabled( false );

    if( bIsPCOK )
        pbImportPatientCards->setEnabled( true );
    else
        pbImportPatientCards->setEnabled( false );

    if( bIsPCHOK )
        pbImportPatientCardUsages->setEnabled( true );
    else
        pbImportPatientCardUsages->setEnabled( false );
}

void cDlgMain::on_pbExit_clicked()
{
    close();
}

void cDlgMain::on_pbImportPatientCardTypes_clicked()
{
    bool    bIsOk = false;

    ledLicenceId->text().toInt( &bIsOk );
    if( !bIsOk )
    {
        QMessageBox::warning( this, tr("Warning"),
                              tr("The defined LicenceId is invalid.\n"
                                 "Please use only numerical characters.") );
        return;
    }

    m_qsPatientCardTypes.clear();

    FILE           *file = NULL;
    char           strTemp[10];
    unsigned int   nCount = 0;

    m_qsFullName = m_qsDATPath + (!m_qsDATPath.right(1).compare("\\")?QString(""):QString("\\")) + QString( "brlttpsfsv.dat" );

    file = fopen( m_qsFullName.toStdString().c_str(), "rb" );
    if( file != NULL )
    {
        memset( strTemp, 0, sizeof(strTemp) );
        fread( strTemp, 10, 1, file );
        nCount = 0;
        fread( &nCount, 4, 1, file );
        listLog->addItem( tr("Count of patientcard types to be imported: %1").arg(nCount) );
        if( nCount > 0 )
        {
//            listLog->addItem( tr("Imported 0 record.") );
//            QListWidgetItem *obListItem = listLog->item( listLog->count()-1 );

            typ_berlettipus stTemp;
            for( unsigned int i=0; i<nCount; i++ )
            {
//                obListItem->setText( tr("Imported %1 record.").arg(i+1) );
                fread( &stTemp.nID, 4, 1, file );
                fread( &stTemp.nAr, 4, 1, file );
                fread( &stTemp.nEgyseg, 4, 1, file );
                fread( stTemp.strNev, 50, 1, file );
                fread( &stTemp.nErvTolEv, 4, 1, file );
                fread( &stTemp.nErvTolHo, 4, 1, file );
                fread( &stTemp.nErvTolNap, 4, 1, file );
                fread( &stTemp.nErvIgEv, 4, 1, file );
                fread( &stTemp.nErvIgHo, 4, 1, file );
                fread( &stTemp.nErvIgNap, 4, 1, file );
                fread( &stTemp.nErvNapok, 4, 1, file );
                fread( &stTemp.bSzolariumHaszn, 1, 1, file );
                fread( &stTemp.nEgysegIdo, 4, 1, file );

                DeCode( stTemp.strNev, sizeof(stTemp.strNev) );
                QDate   qdFrom( stTemp.nErvTolEv, stTemp.nErvTolHo, stTemp.nErvTolNap );
                QDate   qdTo( stTemp.nErvIgEv, stTemp.nErvIgHo, stTemp.nErvIgNap );
                QString qsNev = QString( stTemp.strNev );
                qsNev = qsNev.replace( QString("\""), QString("\\\"") );

                m_qsPatientCardTypes += QString( "ALTER TABLE `patientCardTypes` auto_increment=%1;\n" ).arg(stTemp.nID);
                m_qsPatientCardTypes += QString( "INSERT INTO `patientCardTypes` (`patientCardTypeId`, `licenceId`, `name`, `price`, `vatpercent`, `units`, `validDateFrom`, `validDateTo`, `validDays`, `unitTime`, `active`, `archive`) VALUES" );
                m_qsPatientCardTypes += QString( " ( " );
                m_qsPatientCardTypes += QString( "\'%1\', " ).arg(stTemp.nID);
                m_qsPatientCardTypes += QString( "\'%1\', " ).arg( ledLicenceId->text().toInt() );
                m_qsPatientCardTypes += QString( "\"%1\", " ).arg(qsNev);
                m_qsPatientCardTypes += QString( "\'%1\', " ).arg(stTemp.nAr);
                m_qsPatientCardTypes += QString( "\'0\', " );
                m_qsPatientCardTypes += QString( "\'%1\', " ).arg(stTemp.nEgyseg);
                m_qsPatientCardTypes += QString( "\"%1\", " ).arg(qdFrom.toString("yyyy-MM-dd"));
                m_qsPatientCardTypes += QString( "\"%1\", " ).arg(qdTo.toString("yyyy-MM-dd"));
                m_qsPatientCardTypes += QString( "\'%1\', " ).arg(stTemp.nErvNapok);
                m_qsPatientCardTypes += QString( "\'%1\', " ).arg(stTemp.nEgysegIdo);
                m_qsPatientCardTypes += QString( "\'1\', \"NEW\" ); \n" );
            }
//            if( obListItem ) delete obListItem;
        }
        fclose( file );
        listLog->addItem( tr("Importing patientcard types finished.") );
    }
    else
    {
        listLog->addItem( tr( "Error occured during opening brlttpsfsv.dat file." ) );
    }
}

bool cDlgMain::createPCTFile()
{
    FILE    *file = NULL;
    bool    bRet = true;

    m_qsFullName = m_qsSQLPath + (!m_qsSQLPath.right(1).compare("\\")?QString(""):QString("\\")) + QString( "patientcardtypes.sql" );

    file = fopen( m_qsFullName.toStdString().c_str(), "wt" );
    fputs( m_qsPatientCardTypes.toStdString().c_str(), file );
    fclose( file );

    return bRet;
}

void cDlgMain::on_pbImportPatientCards_clicked()
{
    m_qsPatientCards.clear();

    FILE           *file = NULL;
    char           strTemp[10];
    unsigned int   nCount = 0;
    QString         qsQuery;

    m_qsFullName = m_qsDATPath + (!m_qsDATPath.right(1).compare("\\")?QString(""):QString("\\")) + QString( "brltfsv.dat" );

    file = fopen( m_qsFullName.toStdString().c_str(), "rb" );
    if( file != NULL )
    {
        memset( strTemp, 0, sizeof(strTemp) );
        fread( strTemp, 10, 1, file );

        nCount = 0;
        fread( &nCount, 4, 1, file );
        listLog->addItem( tr("Count of patientcards to be imported: %1").arg(nCount) );
        if( nCount > 0 )
        {
            typ_berlet   stTemp;
            for( unsigned int i=0; i<nCount; i++ )
            {
                fread( stTemp.strVonalkod, 20, 1, file );
                fread( stTemp.strMegjegyzes, 50, 1, file );
                fread( &stTemp.nBerletTipus, 4, 1, file );
                fread( &stTemp.nEgyseg, 4, 1, file );
                fread( &stTemp.nErvEv, 4, 1, file );
                fread( &stTemp.nErvHo, 4, 1, file );
                fread( &stTemp.nErvNap, 4, 1, file );
                fread( &stTemp.nPin, 4, 1, file );
                DeCode( stTemp.strVonalkod, sizeof(stTemp.strVonalkod) );
                DeCode( stTemp.strMegjegyzes, sizeof(stTemp.strMegjegyzes) );

                QDate   qdDate( stTemp.nErvEv, stTemp.nErvHo, stTemp.nErvNap );
                QString qsMegjegyzes = QString( stTemp.strMegjegyzes );
                qsMegjegyzes = qsMegjegyzes.replace( QString("\""), QString("\\\"") );

                m_qsPatientCards += QString( "INSERT INTO `patientCards` (`patientCardId`, `licenceId`, `patientCardTypeId`, `patientId`, `barcode`, `comment`, `units`, `timeLeft`, `validDateFrom`, `validDateTo`, `pincode`, `active`, `archive`) VALUES" );
                m_qsPatientCards += QString( " ( " );
                m_qsPatientCards += QString( "NULL, " );
                m_qsPatientCards += QString( "\'%1\', " ).arg( ledLicenceId->text().toInt() );
                m_qsPatientCards += QString( "%1, " ).arg(stTemp.nBerletTipus);
                m_qsPatientCards += QString( "0, " );
                m_qsPatientCards += QString( "\"%1\", " ).arg(stTemp.strVonalkod);
                m_qsPatientCards += QString( "\"%1\", " ).arg(qsMegjegyzes);
                m_qsPatientCards += QString( "%1, ").arg(stTemp.nEgyseg);
                m_qsPatientCards += QString( "0, ");
                m_qsPatientCards += QString( "\"2008-01-01\", ");
                m_qsPatientCards += QString( "\"%1\", ").arg(qdDate.toString("yyyy-MM-dd"));
                m_qsPatientCards += QString( "NULL, 1, \"NEW\" ); \n" );
            }
        }
        fclose( file );
        listLog->addItem( tr("Importing patientcards finished.") );
    }
    else
    {
        listLog->addItem( tr( "Error occured during opening brltfsv.dat file." ) );
    }
}

bool cDlgMain::createPCFile()
{
    FILE    *file = NULL;
    bool    bRet = true;

    m_qsFullName = m_qsSQLPath + (!m_qsSQLPath.right(1).compare("\\")?QString(""):QString("\\")) + QString( "patientcards.sql" );

    file = fopen( m_qsFullName.toStdString().c_str(), "wt" );
    fputs( m_qsPatientCards.toStdString().c_str(), file );
    fclose( file );

    return bRet;
}

void cDlgMain::on_pbImportPatientCardUsages_clicked()
{
    m_qsPatientCardUse.clear();

    FILE           *file = NULL;
    char           strTemp[10];
    unsigned int   nCount = 0;
    QString         qsQuery;

    m_qsFullName = m_qsDATPath + (!m_qsDATPath.right(1).compare("\\")?QString(""):QString("\\")) + QString( "brlthsznltfsv.dat" );

    file = fopen( m_qsFullName.toStdString().c_str(), "rb" );
    if( file != NULL )
    {
        memset( strTemp, 0, sizeof(strTemp) );
        fread( strTemp, 10, 1, file );

        nCount = 0;
        fread( &nCount, 4, 1, file );
        listLog->addItem( tr("Count of patientcard uses to be imported: %1").arg(nCount) );
        if( nCount > 0 )
        {
            typ_berlethasznalat   stTemp;
            for( unsigned int i=0; i<nCount; i++ )
            {
                fread( stTemp.strVonalkod, 20, 1, file );
                fread( &stTemp.nEv, 4, 1, file );
                fread( &stTemp.nHo, 4, 1, file );
                fread( &stTemp.nNap, 4, 1, file );
                fread( &stTemp.nOra, 4, 1, file );
                fread( &stTemp.nPerc, 4, 1, file );
                fread( &stTemp.nEgyseg, 4, 1, file );
                DeCode( stTemp.strVonalkod, sizeof(stTemp.strVonalkod) );

                QDate       tmpDate( stTemp.nEv, stTemp.nHo, stTemp.nNap );
                QTime       tmpTime( stTemp.nOra, stTemp.nPerc, 0, 0 );
                QDateTime   qdtDate( tmpDate, tmpTime );

                m_qsPatientCardUse += QString( "INSERT INTO `patientCardHistories` (`patientCardHistoryId`, `licenceId`, `patientCardId`, `dateTimeUsed`, `units`, `time`, `active`, `archive`) VALUES" );
                m_qsPatientCardUse += QString( " ( " );
                m_qsPatientCardUse += QString( "NULL, " );
                m_qsPatientCardUse += QString( "\'%1\', " ).arg( ledLicenceId->text().toInt() );
                m_qsPatientCardUse += QString( "\"%1\", " ).arg(patientCardId(stTemp.strVonalkod));
                m_qsPatientCardUse += QString( "\"%1\", " ).arg(qdtDate.toString(("yyyy-MM-dd hh:mm:ss")));
                m_qsPatientCardUse += QString( "%1, ").arg(stTemp.nEgyseg);
                m_qsPatientCardUse += QString( "\"00:00:00\", ");
                m_qsPatientCardUse += QString( "1, \"NEW\" ); \n" );
            }
        }
        fclose( file );
        listLog->addItem( tr("Importing patientcard uses finished.") );
    }
    else
    {
        listLog->addItem( tr( "Error occured during opening brlthsznltfsv.dat file." ) );
    }
}

void cDlgMain::on_pbImportUsers_clicked()
{
    m_qsUsers.clear();

    FILE           *file = NULL;
    char           strTemp[10];
    unsigned int   nCount = 0;
    QString         qsQuery;

    m_qsFullName = m_qsDATPath + (!m_qsDATPath.right(1).compare("\\")?QString(""):QString("\\")) + QString( "srfsv.dat" );

    file = fopen( m_qsFullName.toStdString().c_str(), "rb" );
    if( file != NULL )
    {
        memset( strTemp, 0, sizeof(strTemp) );
        fread( strTemp, 10, 1, file );

        nCount = 0;
        fread( &nCount, 4, 1, file );
        listLog->addItem( tr("Count of users to be imported: %1").arg(nCount) );
        if( nCount > 0 )
        {
            typ_user   stTemp;
            for( unsigned int i=0; i<nCount; i++ )
            {
                fread( &stTemp.dID, 4, 1, file );
                fread( stTemp.strAzonosito, 20, 1, file );
                fread( stTemp.strLoginNev, 20, 1, file );
                fread( stTemp.strNevCsalad, 100, 1, file );
                fread( stTemp.strJelszo, 20, 1, file );
                fread( stTemp.strMegjegyzes, 1000, 1, file );
                fread( &stTemp.nUserLevel, 4, 1, file );
                DeCode( stTemp.strAzonosito, sizeof(stTemp.strAzonosito) );
                DeCode( stTemp.strLoginNev, sizeof(stTemp.strLoginNev) );
                DeCode( stTemp.strNevCsalad, sizeof(stTemp.strNevCsalad) );
                DeCode( stTemp.strJelszo, sizeof(stTemp.strJelszo) );
                DeCode( stTemp.strMegjegyzes, sizeof(stTemp.strMegjegyzes) );

                m_qsUsers += QString( "INSERT INTO `users` (`licenceId`, `name`, `realName`, `password`, `accgroup`, `comment`, `active`, `archive`) VALUES" );
                m_qsUsers += QString( " ( " );
                m_qsUsers += QString( "\'%1\', " ).arg( ledLicenceId->text().toInt() );
                m_qsUsers += QString( "\"%1\", " ).arg( stTemp.strLoginNev );
                m_qsUsers += QString( "\"%1\", " ).arg( stTemp.strNevCsalad );
                m_qsUsers += QString( "\"%1\", " ).arg( stTemp.strJelszo );
                m_qsUsers += QString( "\'%1\', " ).arg( (stTemp.nUserLevel==3?1:2) );
                m_qsUsers += QString( "\"%1\", " ).arg( stTemp.strMegjegyzes );
                m_qsUsers += QString( "\'1\', \"NEW\" ); \n" );
            }
        }
        fclose( file );
        listLog->addItem( tr("Importing users finished.") );
    }
    else
    {
        listLog->addItem( tr( "Error occured during opening srfsv.dat file." ) );
    }
}

void cDlgMain::on_pbExportDatabase_clicked()
{
    createPCTFile();
    createPCFile();
}

void cDlgMain::on_pbConnect_clicked()
{
    g_poDB->setHostName( "localhost" );
    g_poDB->setDatabaseName( ledDatabase->text() );
    g_poDB->setUserName( ledUser->text() );
    g_poDB->setPassword( ledPassword->text() );

    try
    {
        g_poDB->open();

        listLog->addItem( tr("Database connection established.") );

        pbConnect->setEnabled( false );
        pbDisconnect->setEnabled( true );
    }
    catch( cSevException &e )
    {
        listLog->addItem( e.what() );
        g_obLogger(e.severity()) << e.what() << EOM;
    }
}

void cDlgMain::on_pbDisconnect_clicked()
{
    g_poDB->close();

    listLog->addItem( tr("Database connection closed.") );

    pbConnect->setEnabled( true );
    pbDisconnect->setEnabled( false );
}
