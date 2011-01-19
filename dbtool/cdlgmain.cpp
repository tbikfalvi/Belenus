//====================================================================================
//
// Belenus DB Tool alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : cdlgmain.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================

#include <QMessageBox>
#include <QFile>
#include <QDate>
#include <QDir>

//====================================================================================

#include <iostream>
#include <stdio.h>

//====================================================================================

#include "../framework/qtframework.h"
#include "cdlgmain.h"
#include "ui_cdlgmain.h"

//====================================================================================
//
//====================================================================================
cDlgMain::cDlgMain(QWidget *parent) : QDialog(parent)
{
    setupUi(this);

    g_poDB                  = new cQTMySQLConnection;

    m_qsAppPath             = QDir::currentPath();
    m_bDatabaseConnected    = false;

    tabWidget->setCurrentIndex( 0 );

    setWindowIcon( QIcon("./device.png") );

    // SQL Connection tab
    ledDatabase->setText( "belenusconvert" );
    ledUser->setText( "root" );
    ledPassword->setText( "" );
    rbBelenusConvert->setChecked( true );
    rbBelenus->setChecked( false );
    pbConnect->setIcon( QIcon("./check.png") );
    pbConnect->setEnabled( true );
    pbDisconnect->setIcon( QIcon("./check.png") );
    pbDisconnect->setEnabled( false );

    // Import settings tab
    ledImportPath->setText( m_qsAppPath );
    ledExportPath->setText( m_qsAppPath );
    pbCheckFiles->setIcon( QIcon("./check.png") );
    pbCheckFiles->setEnabled( true );

    // Import tab
    pbClearDatabase->setIcon( QIcon("./check.png") );
    pbClearDatabase->setEnabled( false );
    pbImportPatientCardTypes->setIcon( QIcon("./patientcards.png") );
    pbImportPatientCardTypes->setEnabled( false );
    pbImportPatientCards->setIcon( QIcon("./patientcard.png") );
    pbImportPatientCards->setEnabled( false );
    pbImportPatientCardUsages->setIcon( QIcon("./patientcard.png") );
    pbImportPatientCardUsages->setEnabled( false );
    chkImportToDb->setEnabled( false );
    pbImportUsers->setIcon( QIcon("./user.png") );
    pbImportUsers->setEnabled( false );

    // Patientcard tab
    pbImportFromPCUse->setIcon( QIcon("./patientcards.png") );
    pbImportFromPCUse->setEnabled( false );
    pbNext->setIcon( QIcon("./patientcard.png") );
    pbNext->setEnabled( false );
    pbSaveNext->setIcon( QIcon("./patientcard.png") );
    pbSaveNext->setEnabled( false );

    pbExit->setIcon( QIcon("./exit.png") );
}
//====================================================================================
//
//====================================================================================
cDlgMain::~cDlgMain()
{
    if( g_poDB ) delete g_poDB;
}
//====================================================================================
void cDlgMain::on_rbBelenusConvert_clicked()
//====================================================================================
{
    ledDatabase->setText( "belenusconvert" );
    ledUser->setText( "root" );
    ledPassword->setText( "" );
}
//====================================================================================
void cDlgMain::on_rbBelenus_clicked()
//====================================================================================
{
    ledDatabase->setText( "belenus" );
    ledUser->setText( "belenus" );
    ledPassword->setText( "" );
}
//====================================================================================
void cDlgMain::on_pbConnect_clicked()
//====================================================================================
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
        if( ledDatabase->text().compare( "belenusconvert" ) == 0 )
        {
            pbClearDatabase->setEnabled( true );
            pbImportFromPCUse->setEnabled( true );
            cmbType->clear();
            poQuery = g_poDB->executeQTQuery( QString("SELECT nId, strNev FROM berlettipus") );
            cmbType->addItem( tr("<Not selected>"), 0 );
            while( poQuery->next() )
            {
                cmbType->addItem( poQuery->value(1).toString(), poQuery->value(0).toInt() );
            }
            poQuery = NULL;
        }
        else
        {
            pbClearDatabase->setEnabled( false );
            pbImportFromPCUse->setEnabled( false );
        }

        m_bDatabaseConnected = true;

        tabWidget->setCurrentIndex( 1 );
    }
    catch( cSevException &e )
    {
        listLog->addItem( e.what() );
        g_obLogger(e.severity()) << e.what() << EOM;
    }
}
//====================================================================================
void cDlgMain::on_pbDisconnect_clicked()
//====================================================================================
{
    g_poDB->close();

    listLog->addItem( tr("Database connection closed.") );

    pbConnect->setEnabled( true );
    pbDisconnect->setEnabled( false );

    m_bDatabaseConnected = false;
}
//====================================================================================
void cDlgMain::on_pbCheckFiles_clicked()
//====================================================================================
{
    listLog->clear();

    m_qsDATPath = ledImportPath->text();
#ifdef __WIN32__
    m_qsDATPath = m_qsDATPath.replace( "/", "\\" );
#endif

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
#ifdef __WIN32__
    m_qsSQLPath = m_qsSQLPath.replace( "/", "\\" );
#endif

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
    bool    bIsUOK      = checkFile( "srfsv.dat" );
    bool    bIsOk       = false;

    if( !bIsPCTOK && !bIsPCOK && !bIsPCHOK && !bIsUOK )
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

    if( bIsUOK )
        pbImportUsers->setEnabled( true );
    else
        pbImportUsers->setEnabled( false );

    chkImportToDb->setEnabled( m_bDatabaseConnected );

    tabWidget->setCurrentIndex( 2 );
}
//====================================================================================
void cDlgMain::on_pbClearDatabase_clicked()
//====================================================================================
{
    try
    {
        g_poDB->executeQTQuery( QString( "TRUNCATE TABLE berlet" ) );
        listLog->addItem( tr("Table 'berlet' cleared.") );
        g_poDB->executeQTQuery( QString( "TRUNCATE TABLE berlethasznalat" ) );
        listLog->addItem( tr("Table 'berlethasznalat' cleared.") );
        g_poDB->executeQTQuery( QString( "TRUNCATE TABLE berlettipus" ) );
        listLog->addItem( tr("Table 'berlettipus' cleared.") );
    }
    catch( cSevException &e )
    {
        listLog->addItem( e.what() );
        g_obLogger(e.severity()) << e.what() << EOM;
    }
}
//====================================================================================
void cDlgMain::on_pbImportPatientCardTypes_clicked()
//====================================================================================
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

    setCursor( Qt::WaitCursor);

    m_qsPatientCardTypes.clear();

    FILE           *file = NULL;
    char           strTemp[10];
    unsigned int   nCount = 0;

#ifdef __WIN32__
    m_qsFullName = m_qsDATPath + (!m_qsDATPath.right(1).compare("\\")?QString(""):QString("\\")) + QString( "brlttpsfsv.dat" );
#else
    m_qsFullName = m_qsDATPath + (!m_qsDATPath.right(1).compare("/")?QString(""):QString("/")) + QString( "brlttpsfsv.dat" );
#endif

    file = fopen( m_qsFullName.toStdString().c_str(), "rb" );
    if( file != NULL )
    {
        memset( strTemp, 0, 10 );
        fread( strTemp, 10, 1, file );
        nCount = 0;
        fread( &nCount, 4, 1, file );
        listLog->addItem( tr("Count of patientcard types to be imported: %1").arg(nCount) );
        if( nCount > 0 )
        {
            typ_berlettipus stTemp;
            for( unsigned int i=0; i<nCount; i++ )
            {
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
                if( rbSensolite->isChecked() )
                    fread( &stTemp.nEgysegIdo, 4, 1, file );
                else
                    stTemp.nEgysegIdo = 0;

                DeCode( stTemp.strNev, 50 );
                QDate   qdFrom( stTemp.nErvTolEv, stTemp.nErvTolHo, stTemp.nErvTolNap );
                QDate   qdTo( stTemp.nErvIgEv, stTemp.nErvIgHo, stTemp.nErvIgNap );
                QString qsNev = QString( stTemp.strNev );
                qsNev = qsNev.replace( QString("\""), QString("\\\"") );

                m_qsQuery = "";
                m_qsQuery += QString( "INSERT INTO `berlettipus` (`nId`, `nAr`, `nEgyseg`, `strNev`, `nErvTolEv`, `nErvTolHo`, `nErvTolNap`, `nErvIgEv`, `nErvIgHo`, `nErvIgNap`, `nErvNapok`, `bSzolariumHaszn`, `nEgysegIdo`) VALUES" );
                m_qsQuery += QString( " ( " );
                m_qsQuery += QString( "\'%1\', " ).arg(stTemp.nID);
                m_qsQuery += QString( "\'%1\', " ).arg(stTemp.nAr);
                m_qsQuery += QString( "\'%1\', " ).arg(stTemp.nEgyseg);
                m_qsQuery += QString( "\'%1\', " ).arg(stTemp.strNev);
                m_qsQuery += QString( "\'%1\', " ).arg(stTemp.nErvTolEv);
                m_qsQuery += QString( "\'%1\', " ).arg(stTemp.nErvTolHo);
                m_qsQuery += QString( "\'%1\', " ).arg(stTemp.nErvTolNap);
                m_qsQuery += QString( "\'%1\', " ).arg(stTemp.nErvIgHo);
                m_qsQuery += QString( "\'%1\', " ).arg(stTemp.nErvIgEv);
                m_qsQuery += QString( "\'%1\', " ).arg(stTemp.nErvIgNap);
                m_qsQuery += QString( "\'%1\', " ).arg(stTemp.nErvNapok);
                m_qsQuery += QString( "\'%1\', " ).arg(stTemp.bSzolariumHaszn);
                m_qsQuery += QString( "\'%1\' ) " ).arg(stTemp.nEgysegIdo);

                if( chkImportToDb->isChecked() )
                {
                    try
                    {
                        poQuery = g_poDB->executeQTQuery( QString("SELECT * from berlettipus WHERE nID=%1").arg(stTemp.nID) );
                        if( !poQuery->first() )
                        {
                            g_poDB->executeQTQuery( m_qsQuery );
                        }
                        if( poQuery ) delete poQuery;
                        poQuery = NULL;
                    }
                    catch( cSevException &e )
                    {
                        listLog->addItem( e.what() );
                        g_obLogger(e.severity()) << e.what() << EOM;
                    }
                }

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
        }
        fclose( file );
        listLog->addItem( tr("Importing patientcard types finished.") );
    }
    else
    {
        listLog->addItem( tr( "Error occured during opening brlttpsfsv.dat file." ) );
    }

    setCursor( Qt::ArrowCursor);
}
//====================================================================================
void cDlgMain::on_pbImportPatientCards_clicked()
//====================================================================================
{
    m_qsPatientCards.clear();

    FILE           *file = NULL;
    unsigned int   nCount = 0;

#ifdef __WIN32__
    m_qsFullName = m_qsDATPath + (!m_qsDATPath.right(1).compare("\\")?QString(""):QString("\\")) + QString( "brltfsv.dat" );
#else
    m_qsFullName = m_qsDATPath + (!m_qsDATPath.right(1).compare("/")?QString(""):QString("/")) + QString( "brltfsv.dat" );
#endif

    setCursor( Qt::WaitCursor);

    file = fopen( m_qsFullName.toStdString().c_str(), "rb" );
    if( file != NULL )
    {
        memset( m_strPatiencardVersion, 0, 10 );
        fread( m_strPatiencardVersion, 10, 1, file );
        ledVersionPCDAT->setText( QString::fromStdString(m_strPatiencardVersion) );

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
                DeCode( stTemp.strVonalkod, 20 );
                DeCode( stTemp.strMegjegyzes, 50 );

                QDate   qdDate( stTemp.nErvEv, stTemp.nErvHo, stTemp.nErvNap );
                QString qsMegjegyzes = QString( stTemp.strMegjegyzes );
                qsMegjegyzes = qsMegjegyzes.replace( QString("\""), QString("\\\"") );

                m_qsQuery = "";
                m_qsQuery += QString( "INSERT INTO `berlet` (`strVonalkod`, `strMegjegyzes`, `nBerletTipus`, `nEgyseg`, `nErvEv`, `nErvHo`, `nErvNap`, `nPin`) VALUES" );
                m_qsQuery += QString( " ( " );
                m_qsQuery += QString( "\'%1\', " ).arg(stTemp.strVonalkod);
                m_qsQuery += QString( "\'%1\', " ).arg(stTemp.strMegjegyzes);
                m_qsQuery += QString( "\'%1\', " ).arg(stTemp.nBerletTipus);
                m_qsQuery += QString( "\'%1\', " ).arg(stTemp.nEgyseg);
                m_qsQuery += QString( "\'%1\', " ).arg(stTemp.nErvEv);
                m_qsQuery += QString( "\'%1\', " ).arg(stTemp.nErvHo);
                m_qsQuery += QString( "\'%1\', " ).arg(stTemp.nErvNap);
                m_qsQuery += QString( "\'%1\' ) " ).arg(stTemp.nPin);

                if( chkImportToDb->isChecked() )
                {
                    try
                    {
                        poQuery = g_poDB->executeQTQuery( QString("SELECT * from berlet WHERE strVonalkod=\"%1\"").arg(stTemp.strVonalkod) );
                        if( !poQuery->first() )
                        {
                            g_poDB->executeQTQuery( m_qsQuery );
                        }
                        if( poQuery ) delete poQuery;
                        poQuery = NULL;
                    }
                    catch( cSevException &e )
                    {
                        listLog->addItem( e.what() );
                        g_obLogger(e.severity()) << e.what() << EOM;
                    }
                }

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

    setCursor( Qt::ArrowCursor);
}
//====================================================================================
void cDlgMain::on_pbImportPatientCardUsages_clicked()
//====================================================================================
{
    m_qsPatientCardUse.clear();

    FILE           *file = NULL;
    char           strTemp[10];
    unsigned int   nCount = 0;
    QString         qsQuery;

#ifdef __WIN32__
    m_qsFullName = m_qsDATPath + (!m_qsDATPath.right(1).compare("\\")?QString(""):QString("\\")) + QString( "brlthsznltfsv.dat" );
#else
    m_qsFullName = m_qsDATPath + (!m_qsDATPath.right(1).compare("/")?QString(""):QString("/")) + QString( "brlthsznltfsv.dat" );
#endif

    setCursor( Qt::WaitCursor);

    file = fopen( m_qsFullName.toStdString().c_str(), "rb" );
    if( file != NULL )
    {
        memset( strTemp, 0, 10 );
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
                DeCode( stTemp.strVonalkod, 20 );

                QDate       tmpDate( stTemp.nEv, stTemp.nHo, stTemp.nNap );
                QTime       tmpTime( stTemp.nOra, stTemp.nPerc, 0, 0 );
                QDateTime   qdtDate( tmpDate, tmpTime );

                m_qsQuery = "";
                m_qsQuery += QString( "INSERT INTO `berlethasznalat` (`id`, `strVonalkod`, `nEv`, `nHo`, `nNap`, `nOra`, `nPerc`, `nEgyseg`) VALUES" );
                m_qsQuery += QString( " ( NULL, " );
                m_qsQuery += QString( "\'%1\', " ).arg(stTemp.strVonalkod);
                m_qsQuery += QString( "\'%1\', " ).arg(stTemp.nEv);
                m_qsQuery += QString( "\'%1\', " ).arg(stTemp.nHo);
                m_qsQuery += QString( "\'%1\', " ).arg(stTemp.nNap);
                m_qsQuery += QString( "\'%1\', " ).arg(stTemp.nOra);
                m_qsQuery += QString( "\'%1\', " ).arg(stTemp.nPerc);
                m_qsQuery += QString( "\'%1\' ) " ).arg(stTemp.nEgyseg);

                if( chkImportToDb->isChecked() )
                {
                    try
                    {
                        poQuery = g_poDB->executeQTQuery( QString("SELECT * from berlethasznalat WHERE strVonalkod=\"%1\" AND nEv=%2 AND nHo=%3 AND nNap=%4 AND nOra=%5 AND nPerc=%6" ).arg(stTemp.strVonalkod).arg(stTemp.nEv).arg(stTemp.nHo).arg(stTemp.nNap).arg(stTemp.nOra).arg(stTemp.nPerc) );
                        if( !poQuery->first() )
                        {
                            g_poDB->executeQTQuery( m_qsQuery );
                        }
                        if( poQuery ) delete poQuery;
                        poQuery = NULL;
                    }
                    catch( cSevException &e )
                    {
                        listLog->addItem( e.what() );
                        g_obLogger(e.severity()) << e.what() << EOM;
                    }
                }

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

    setCursor( Qt::ArrowCursor);
}
//====================================================================================
void cDlgMain::on_pbImportUsers_clicked()
//====================================================================================
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
        memset( strTemp, 0, 10 );
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
                DeCode( stTemp.strAzonosito, 20 );
                DeCode( stTemp.strLoginNev, 20 );
                DeCode( stTemp.strNevCsalad, 100 );
                DeCode( stTemp.strJelszo, 20 );
                DeCode( stTemp.strMegjegyzes, 1000 );

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
//====================================================================================
void cDlgMain::on_pbImportFromPCUse_clicked()
//====================================================================================
{
    try
    {
        poQuery = g_poDB->executeQTQuery( QString("SELECT strVonalkod, nEv, nHo, nNap, SUM(nEgyseg) AS nEgyseg FROM berlethasznalat GROUP BY strVonalkod") );
        rbNextPCUse->setChecked( true );
        if( getNextNewPatientCard() )
        {
            pbNext->setEnabled( true );
            pbSaveNext->setEnabled( true );
        }
        else
        {
            pbNext->setEnabled( false );
            pbSaveNext->setEnabled( false );
        }
    }
    catch( cSevException &e )
    {
        listLog->addItem( e.what() );
        g_obLogger(e.severity()) << e.what() << EOM;
        poQuery = NULL;
        pbSaveNext->setEnabled( false );
    }
}
//====================================================================================
void cDlgMain::on_pbUpdate_clicked()
//====================================================================================
{
    try
    {
        poQuery = g_poDB->executeQTQuery( QString("SELECT * from berlet WHERE strMegjegyzes=\"\" ORDER BY strVonalkod") );
        rbNextPatientCard->setChecked( true );
        if( getNextNewPatientCard() )
        {
            pbNext->setEnabled( true );
            pbSaveNext->setEnabled( true );
        }
        else
        {
            pbNext->setEnabled( false );
            pbSaveNext->setEnabled( false );
        }
    }
    catch( cSevException &e )
    {
        listLog->addItem( e.what() );
        g_obLogger(e.severity()) << e.what() << EOM;
        poQuery = NULL;
        pbSaveNext->setEnabled( false );
    }
}
//====================================================================================
void cDlgMain::on_pbUpdateUsage_clicked()
//====================================================================================
{
    try
    {
        poQuery = g_poDB->executeQTQuery( QString("SELECT * from berlet ORDER BY strVonalkod") );
        rbNextPatientCard->setChecked( true );
        if( getNextNewPatientCard() )
        {
            pbNext->setEnabled( true );
            pbSaveNext->setEnabled( true );
            if( QMessageBox::question( this, tr("Question"),
                                       tr("Do you want to start patientcard usage update based on berlethasznalat table?"),
                                       QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes ) == QMessageBox::Yes )
            {
                while( pbSaveNext->isEnabled() )
                {
                    on_pbSaveNext_clicked();
                    if( m_bSaveAborted )
                    {
                        break;
                    }
                }
            }
        }
        else
        {
            pbNext->setEnabled( false );
            pbSaveNext->setEnabled( false );
        }
    }
    catch( cSevException &e )
    {
        listLog->addItem( e.what() );
        g_obLogger(e.severity()) << e.what() << EOM;
        poQuery = NULL;
        pbSaveNext->setEnabled( false );
    }
}
//====================================================================================
void cDlgMain::on_cmbType_currentIndexChanged(int)
//====================================================================================
{
    if( cmbType->currentIndex() )
    {
        QSqlQuery *poPCTQuery = g_poDB->executeQTQuery( QString("SELECT nEgyseg FROM berlettipus WHERE nId=%1").arg(cmbType->itemData(cmbType->currentIndex()).toInt()) );
        poPCTQuery->first();
        ledUnits->setText( poPCTQuery->value(0).toString() );
        int inUnitsLeft = ledUnits->text().toInt()-ledUnitsUsed->text().toInt();
        ledUnitsLeft->setText( QString::number(inUnitsLeft) );
    }
    else
    {
        ledUnits->setText( "" );
        ledUnitsLeft->setText( "" );
    }
}
//====================================================================================
void cDlgMain::on_pbNext_clicked()
//====================================================================================
{
    if( getNextNewPatientCard() )
    {
        pbNext->setEnabled( true );
        pbSaveNext->setEnabled( true );
    }
    else
    {
        pbNext->setEnabled( false );
        pbSaveNext->setEnabled( false );
    }
}
//====================================================================================
void cDlgMain::on_pbSaveNext_clicked()
//====================================================================================
{
    m_bSaveAborted = true;
    if( ledBarcode->text() == "" )
    {
        QMessageBox::warning( this, tr("Warning"),
                              tr("Barcode of the patientcard can not be empty.\n"
                                 "Please define a valid barcode."));
        ledBarcode->setFocus();
        m_bSaveAborted = false;
        return;
    }
    else if( rbNextPCUse->isChecked() )
    {
        QSqlQuery *poQBerlet = g_poDB->executeQTQuery( QString("SELECT * FROM berlet WHERE strVonalkod=\"%1\"").arg(ledBarcode->text()) );

        if( poQBerlet->first() )
        {
            QMessageBox::warning( this, tr("Warning"),
                                  tr("This barcode already saved in database.\n"
                                     "Please jump to the next unsaved patientcard or define another valid barcode."));
            ledBarcode->setFocus();
            m_bSaveAborted = false;
            return;
        }
    }
    if( cmbType->itemData( cmbType->currentIndex() ) == 0 )
    {
        QMessageBox::warning( this, tr("Warning"),
                              tr("The patientcard can not be saved with no patientcard type selected.\n"
                                 "Please select a valid patientcard type."));
        cmbType->setFocus();
        m_bSaveAborted = false;
        return;
    }
    if( ledUnitsLeft->text() == "" ||
        ledUnitsLeft->text().toInt() < 0 )
    {
        ledUnitsLeft->setText( "0" );
    }

    m_qsQuery = "";
    if( rbNextPCUse->isChecked() )
    {
        m_qsQuery += QString( "INSERT INTO " );
    }
    else if( rbNextPatientCard->isChecked() )
    {
        m_qsQuery += QString( "UPDATE " );
    }
    m_qsQuery += QString( "berlet SET " );

    if( rbNextPCUse->isChecked() )
        m_qsQuery += QString( "strVonalkod = \'%1\', " ).arg(ledBarcode->text());

    m_qsQuery += QString( "strMegjegyzes = \'%1\', " ).arg(ledComment->text());
    m_qsQuery += QString( "nBerletTipus = \'%1\', " ).arg(cmbType->itemData(cmbType->currentIndex()).toInt());
    m_qsQuery += QString( "nEgyseg = \'%1\', " ).arg(ledUnitsLeft->text().toInt());
    m_qsQuery += QString( "nErvEv = \'%1\', " ).arg(deValid->date().year());
    m_qsQuery += QString( "nErvHo = \'%1\', " ).arg(deValid->date().month());
    m_qsQuery += QString( "nErvNap = \'%1\', " ).arg(deValid->date().day());
    m_qsQuery += QString( "nPin = \'%1\' " ).arg(0);
    if( rbNextPatientCard->isChecked() )
    {
        m_qsQuery += QString( " WHERE strVonalkod=\"%1\" " ).arg( ledBarcode->text() );
    }
    //listLog->addItem(m_qsQuery);

    if( rbNextPCUse->isChecked() )
    {
        try
        {
            QSqlQuery *poQBerlet = g_poDB->executeQTQuery( QString("SELECT * from berlet WHERE strVonalkod=\"%1\"").arg(ledBarcode->text()) );
            if( !poQBerlet->first() )
            {
                g_poDB->executeQTQuery( m_qsQuery );
            }
            if( poQBerlet ) delete poQBerlet;
        }
        catch( cSevException &e )
        {
            listLog->addItem( e.what() );
            g_obLogger(e.severity()) << e.what() << EOM;
        }
    }
    else if( rbNextPatientCard->isChecked() )
    {
        try
        {
            g_poDB->executeQTQuery( m_qsQuery );
        }
        catch( cSevException &e )
        {
            listLog->addItem( e.what() );
            g_obLogger(e.severity()) << e.what() << EOM;
            m_bSaveAborted = false;
        }
    }

    on_pbNext_clicked();
    ledComment->setFocus();
}
//====================================================================================
void cDlgMain::on_pbExportToSQL_clicked()
//====================================================================================
{
    createPCTFile();
    createPCFile();
}
//====================================================================================
void cDlgMain::on_pbExportToPCDat_clicked()
//====================================================================================
{
    ledBarcode->setText( "" );
    ledComment->setText( "" );
    cmbType->setCurrentIndex( 0 );
    ledUnitsUsed->setText( "" );
    ledFirstUse->setText( "" );
    deValid->setDate( QDate(2000,1,1) );

    pbNext->setEnabled( false );
    pbSaveNext->setEnabled( false );

    if( ledVersionPCDAT->text().length() != 9 )
    {
        QMessageBox::warning( this, tr("Warning"),
                              tr("The version number of newly created patientcard file is invalid."));
        return;
    }

    try
    {
        memset( m_strPatiencardVersion, 0, 10 );
        strncpy( m_strPatiencardVersion, ledVersionPCDAT->text().toStdString().c_str(), 9 );

        poQuery = g_poDB->executeQTQuery( QString("SELECT COUNT(strVonalkod) FROM berlet") );
        poQuery->first();

        int     inBerletCount = poQuery->value(0).toInt();
        FILE    *file = NULL;

#ifdef __WIN32__
        m_qsFullName = m_qsSQLPath + (!m_qsSQLPath.right(1).compare("\\")?QString(""):QString("\\")) + QString( "brltfsv_new.dat" );
#else
        m_qsFullName = m_qsSQLPath + (!m_qsSQLPath.right(1).compare("/")?QString(""):QString("/")) + QString( "brltfsv_new.dat" );
#endif

        file = fopen( m_qsFullName.toStdString().c_str(), "wb" );
        fwrite( m_strPatiencardVersion, 10, 1, file );
        fwrite( &inBerletCount, 4, 1, file );
        poQuery = g_poDB->executeQTQuery( QString("SELECT * FROM berlet ORDER BY strVonalkod") );

        typ_berlet   stTemp;

        while( poQuery->next() )
        {
            strcpy( stTemp.strVonalkod, poQuery->value(0).toString().toStdString().c_str() );
            strcpy( stTemp.strMegjegyzes, poQuery->value(1).toString().toStdString().c_str() );
            stTemp.nBerletTipus = poQuery->value(2).toInt();
            stTemp.nEgyseg = poQuery->value(3).toInt();
            stTemp.nErvEv = poQuery->value(4).toInt();
            stTemp.nErvHo = poQuery->value(5).toInt();;
            stTemp.nErvNap = poQuery->value(6).toInt();
            stTemp.nPin = poQuery->value(7).toInt();
            EnCode( stTemp.strVonalkod, 20 );
            EnCode( stTemp.strMegjegyzes, 50 );

            //fwrite( &stTemp, 94, 1, file );
            fwrite( stTemp.strVonalkod, 20, 1, file );
            fwrite( stTemp.strMegjegyzes, 50, 1, file );
            fwrite( &stTemp.nBerletTipus, 4, 1, file );
            fwrite( &stTemp.nEgyseg, 4, 1, file );
            fwrite( &stTemp.nErvEv, 4, 1, file );
            fwrite( &stTemp.nErvHo, 4, 1, file );
            fwrite( &stTemp.nErvNap, 4, 1, file );
            fwrite( &stTemp.nPin, 4, 1, file );
        }
        fclose( file );
    }
    catch( cSevException &e )
    {
        listLog->addItem( e.what() );
        g_obLogger(e.severity()) << e.what() << EOM;
    }
}
//====================================================================================
void cDlgMain::on_pbExit_clicked()
//====================================================================================
{
    if( m_bDatabaseConnected )
    {
        on_pbDisconnect_clicked();
    }

    close();
}
//====================================================================================
void cDlgMain::EnCode( char *str, int size )
//====================================================================================
{
   for(int i=0;i<size;i++)
   {
      str[i] ^= 11;
   }
}
//====================================================================================
void cDlgMain::DeCode( char *str, int size )
//====================================================================================
{
   for(int i=0;i<size;i++)
   {
      str[i] ^= 11;
   }
}
//====================================================================================
unsigned int cDlgMain::patientCardId( QString /*p_qsBarcode*/ )
//====================================================================================
{
    return 0;
}
//====================================================================================
bool cDlgMain::checkFile( QString p_qsFileName )
//====================================================================================
{
    QFile   *obQFTemp = NULL;
    bool     bRet = false;

#ifdef __WIN32__
    m_qsFullName = m_qsDATPath + (!m_qsDATPath.right(1).compare("\\")?QString(""):QString("\\")) + p_qsFileName;
#else
    m_qsFullName = m_qsDATPath + (!m_qsDATPath.right(1).compare("/")?QString(""):QString("/")) + p_qsFileName;
#endif
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
//====================================================================================
//
//====================================================================================
bool cDlgMain::createPCTFile()
{
    FILE    *file = NULL;
    bool    bRet = true;

#ifdef __WIN32__
    m_qsFullName = m_qsSQLPath + (!m_qsSQLPath.right(1).compare("\\")?QString(""):QString("\\")) + QString( "patientcardtypes.sql" );
#else
    m_qsFullName = m_qsSQLPath + (!m_qsSQLPath.right(1).compare("/")?QString(""):QString("/")) + QString( "patientcardtypes.sql" );
#endif

    file = fopen( m_qsFullName.toStdString().c_str(), "wt" );
    fputs( m_qsPatientCardTypes.toStdString().c_str(), file );
    fclose( file );

    return bRet;
}
//====================================================================================
//
//====================================================================================
bool cDlgMain::createPCFile()
{
    FILE    *file = NULL;
    bool    bRet = true;

#ifdef __WIN32__
    m_qsFullName = m_qsSQLPath + (!m_qsSQLPath.right(1).compare("\\")?QString(""):QString("\\")) + QString( "patientcards.sql" );
#else
    m_qsFullName = m_qsSQLPath + (!m_qsSQLPath.right(1).compare("/")?QString(""):QString("/")) + QString( "patientcards.sql" );
#endif

    file = fopen( m_qsFullName.toStdString().c_str(), "wt" );
    fputs( m_qsPatientCards.toStdString().c_str(), file );
    fclose( file );

    return bRet;
}
//====================================================================================
bool cDlgMain::getNextNewPatientCard()
//====================================================================================
{
    bool bRet = false;

    ledBarcode->setText( "" );
    ledComment->setText( "" );
    cmbType->setCurrentIndex( 0 );
    ledUnitsUsed->setText( "" );
    ledFirstUse->setText( "" );
    deValid->setDate( QDate(2000,1,1) );

    if( !poQuery->next() )
        return bRet;

    if( rbNextPCUse->isChecked() )
    {
        QSqlQuery *poQBerlet = g_poDB->executeQTQuery( QString("SELECT * FROM berlet WHERE strVonalkod=\"%1\"").arg(poQuery->value( 0 ).toString()) );

        if( !poQBerlet->first() )
        {
            ledBarcode->setText( poQuery->value(0).toString() );
            ledUnitsUsed->setText( poQuery->value(4).toString() );
            ledFirstUse->setText( QDate(poQuery->value(1).toInt(),poQuery->value(2).toInt(),poQuery->value(3).toInt()).toString("yyyy/MM/dd") );
            deValid->setDate( QDate::fromString(ledFirstUse->text(),"yyyy/MM/dd").addDays(365) );
            bRet = true;
        }
        else
        {
            bRet = getNextNewPatientCard();
        }
        if( poQBerlet ) delete poQBerlet;
    }
    else if( rbNextPatientCard->isChecked() )
    {
        QSqlQuery *poQPCUse = g_poDB->executeQTQuery( QString("SELECT SUM(nEgyseg) AS nEgyseg FROM berlethasznalat WHERE strVonalkod='%1'").arg(poQuery->value(0).toString()) );

        poQPCUse->first();
        ledBarcode->setText( poQuery->value(0).toString() );
        ledComment->setText( poQuery->value(1).toString() );
        if( poQPCUse->value(0).toInt() > 0 )
            ledUnitsUsed->setText( QString::number(poQPCUse->value(0).toInt()) );
        else
            ledUnitsUsed->setText( "0" );
        for( int i=0; i<cmbType->count(); i++ )
        {
            if( cmbType->itemData(i).toInt() == poQuery->value(2).toInt() )
            {
                cmbType->setCurrentIndex(i);
                break;
            }
        }
        if( poQuery->value(3).toInt() > 0 )
            ledUnitsLeft->setText( poQuery->value(3).toString() );
        deValid->setDate( QDate(poQuery->value(4).toInt(),poQuery->value(5).toInt(),poQuery->value(6).toInt()) );
        bRet = true;
        if( poQPCUse ) delete poQPCUse;
    }

    return bRet;
}
//====================================================================================
