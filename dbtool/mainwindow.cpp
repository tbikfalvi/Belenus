
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QVector>
#include <QCloseEvent>
#include <QTextStream>
#include <QtSql/QSqlQuery>
#include <QCryptographicHash>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_qdExpCurrentDir   = QDir::currentPath();
    m_nProgramType      = DBTool::KiwiSun;

    ui->rbProgramKiwiSun->setChecked( true );
    ui->ledPathDB->setText( m_qdExpCurrentDir.path() );

    connect( ui->rbProgramKiwiSun, SIGNAL(toggled(bool)), this, SLOT(slotProgramSelected()) );
    //connect( ui->rbProgramSensolite, SIGNAL(toggled(bool)), this, SLOT(slotProgramSelected()) );

    ui->tabWidget->setCurrentIndex( 0 );

    m_poDB = NULL;

    m_dlgProgress = new cDlgProgress( this );

    slotProgramSelected();
}

MainWindow::~MainWindow()
{
    delete m_dlgProgress;
    delete ui;
}

void MainWindow::slotProgramSelected()
{
    if( ui->rbProgramKiwiSun->isChecked() )
    {
        m_nProgramType      = DBTool::KiwiSun;
        ui->ledPExportS2->setEnabled( true );
    }
    /*else if( ui->rbProgramSensolite->isChecked() )
    {
        m_nProgramType      = DBTool::Sensolite;
        ui->ledPExportS2->setEnabled( false );
    }*/
}

void MainWindow::closeEvent( QCloseEvent *p_poEvent )
{
    if( QMessageBox::question( this, tr("Question"),
                               tr("Are you sure you want to close the application?"),
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
    {
        p_poEvent->accept();
    }
    else
    {
        p_poEvent->ignore();
    }
}

void MainWindow::on_rbProgramKiwiSun_clicked()
{
    ui->imgLogo->setPixmap( QPixmap(":/imgLogo/KiwiSun.png") );
}

/*void MainWindow::on_rbProgramSensolite_clicked()
{
    ui->imgLogo->setPixmap( QPixmap(":/imgLogo/Sensolite.png") );
}*/

void MainWindow::on_pbExpSelectDir_clicked()
{
    QString qsDirectory = QFileDialog::getExistingDirectory(this, tr("Select Directory"), m_qdExpCurrentDir.absolutePath() );

    if (!qsDirectory.isEmpty())
    {
        m_qdExpCurrentDir = QDir( qsDirectory );
        ui->ledPathDB->setText( qsDirectory );

        m_qsPCTFileName = QString( "%1/brlttpsfsv.dat" ).arg( ui->ledPathDB->text() ).replace( "/", "\\" );
        m_qsPCFileName  = QString( "%1/brltfsv.dat" ).arg( ui->ledPathDB->text() ).replace( "/", "\\" );
        m_qsPTFileName  = QString( "%1/trmktpsfsv.dat" ).arg( ui->ledPathDB->text() ).replace( "/", "\\" );
        m_qsPFileName   = QString( "%1/trmkfsv.dat" ).arg( ui->ledPathDB->text() ).replace( "/", "\\" );
        m_qsPAFileName  = QString( "%1/trmktpssgfsv.dat" ).arg( ui->ledPathDB->text() ).replace( "/", "\\" );
        m_qsUFileName   = QString( "%1/srfsv.dat" ).arg( ui->ledPathDB->text() ).replace( "/", "\\" );

        ui->listLog->addItem( tr("Filenames of patientcard and patientcard types data:") );
        ui->listLog->addItem( m_qsPCTFileName );
        ui->listLog->addItem( m_qsPCFileName );
        ui->listLog->addItem( m_qsPTFileName );
        ui->listLog->addItem( m_qsPFileName );
        ui->listLog->addItem( m_qsUFileName );
    }
}

void MainWindow::on_pbImportDB_clicked()
{
    _loadPatientCardTypes();
    _loadPatientCards();
    _loadProductTypes();
    _loadProducts();
    _loadProductAssign();
    _loadUsers();
}

//====================================================================================
void MainWindow::_loadPatientCardTypes()
//====================================================================================
{
    FILE           *file = NULL;
    unsigned int   nCount = 0;

    setCursor( Qt::WaitCursor);

    m_qvPatientCardTypes.clear();

    file = fopen( m_qsPCTFileName.toStdString().c_str(), "rb" );
    if( file != NULL )
    {
        memset( m_strPatiencardTypeVersion, 0, 10 );
        fread( m_strPatiencardTypeVersion, 10, 1, file );
        nCount = 0;
        fread( &nCount, 4, 1, file );
        ui->listLog->addItem( tr("Count of patientcard types to be imported: %1").arg(nCount) );
        if( nCount > 0 )
        {
            typ_berlettipus stTemp;
            for( unsigned int i=0; i<nCount; i++ )
            {
                fread( &stTemp.nID, 4, 1, file );
                stTemp.nNewID = 0;
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
                if( m_nProgramType == DBTool::Sensolite )
                {
                    fread( &stTemp.nEgysegIdo, 4, 1, file );
                }
                else
                {
                    stTemp.nEgysegIdo = 0;
                }

                _DeCode( stTemp.strNev, 50 );

                m_qvPatientCardTypes.append( stTemp );
            }
        }
        fclose( file );
        ui->listLog->addItem( tr("Importing %1 patientcard types finished.").arg(m_qvPatientCardTypes.size()) );
    }
    else
    {
        ui->listLog->addItem( tr( "Error occured during opening brlttpsfsv.dat file." ) );
    }

    setCursor( Qt::ArrowCursor);
}

//====================================================================================
void MainWindow::_loadPatientCards()
//====================================================================================
{
    FILE           *file = NULL;
    unsigned int    nCount = 0;
    char            m_strPatiencardVersion[10];

    setCursor( Qt::WaitCursor);

    m_qvPatientCards.clear();

    file = fopen( m_qsPCFileName.toStdString().c_str(), "rb" );
    if( file != NULL )
    {
        memset( m_strPatiencardVersion, 0, 10 );
        fread( m_strPatiencardVersion, 10, 1, file );
        //ledVersionPCDAT->setText( QString::fromStdString(m_strPatiencardVersion) );

        nCount = 0;
        fread( &nCount, 4, 1, file );
        ui->listLog->addItem( tr("Count of patientcards to be imported: %1").arg(nCount) );
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
                _DeCode( stTemp.strVonalkod, 20 );
                _DeCode( stTemp.strMegjegyzes, 50 );

                m_qvPatientCards.append( stTemp );
            }
        }
        fclose( file );
        ui->listLog->addItem( tr("Importing %1 patientcards finished.").arg(m_qvPatientCards.size()) );
    }
    else
    {
        ui->listLog->addItem( tr( "Error occured during opening brltfsv.dat file." ) );
    }

    setCursor( Qt::ArrowCursor);
}
//====================================================================================
void MainWindow::_loadProductTypes()
//====================================================================================
{
    FILE           *file = NULL;
    unsigned int    nCount = 0;
    char            m_strVersion[10];

    setCursor( Qt::WaitCursor);

    m_qvProductTypes.clear();

    file = fopen( m_qsPTFileName.toStdString().c_str(), "rb" );
    if( file != NULL )
    {
        memset( m_strVersion, 0, 10 );
        fread( m_strVersion, 10, 1, file );

        nCount = 0;
        fread( &nCount, 4, 1, file );
        ui->listLog->addItem( tr("Count of product types to be imported: %1").arg(nCount) );
        if( nCount > 0 )
        {
            typ_termektipus stTemp;
            for( unsigned int i=0; i<nCount; i++ )
            {
                fread( &stTemp.nID, 4, 1, file );
                stTemp.nNewID = 0;
                fread( stTemp.strNev, 100, 1, file );
                _DeCode( stTemp.strNev, 100 );

                //ui->listLog->addItem( QString( "[%1] \'%2\'" ).arg(stTemp.nID).arg(stTemp.strNev) );

                m_qvProductTypes.append( stTemp );
            }
        }

        fclose( file );
        ui->listLog->addItem( tr("Importing %1 product types finished.").arg(m_qvProductTypes.size()) );
    }
    else
    {
        ui->listLog->addItem( tr( "Error occured during opening trmktpsfsv.dat file." ) );
    }

    setCursor( Qt::ArrowCursor);
}
//====================================================================================
void MainWindow::_loadProducts()
//====================================================================================
{
    FILE           *file = NULL;
    unsigned int    nCount = 0;
    char            m_strVersion[10];

    setCursor( Qt::WaitCursor);

    m_qvProducts.clear();

    file = fopen( m_qsPFileName.toStdString().c_str(), "rb" );
    if( file != NULL )
    {
        memset( m_strVersion, 0, 10 );
        fread( m_strVersion, 10, 1, file );

        nCount = 0;
        fread( &nCount, 4, 1, file );
        ui->listLog->addItem( tr("Count of products to be imported: %1").arg(nCount) );
        if( nCount > 0 )
        {
            typ_termek stTemp;
            for( unsigned int i=0; i<nCount; i++ )
            {
                fread( &stTemp.nID, 4, 1, file );
                stTemp.nNewID = 0;
                fread( stTemp.strVonalkod, 20, 1, file );
                fread( stTemp.strNev, 100, 1, file );
                fread( &stTemp.nAr, 4, 1, file );
                fread( &stTemp.nDarab, 4, 1, file );
                fread( &stTemp.nArBeszerzes, 4, 1, file );

                _DeCode( stTemp.strVonalkod, 20 );
                _DeCode( stTemp.strNev, 100 );

                //ui->listLog->addItem( QString( "[%1] \'%2\' \'%3\' [%4] [%5] [%6]" ).arg(stTemp.nID).arg(stTemp.strVonalkod).arg(stTemp.strNev).arg(stTemp.nAr).arg(stTemp.nDarab).arg(stTemp.nArBeszerzes) );

                m_qvProducts.append( stTemp );
            }
        }

        fclose( file );
        ui->listLog->addItem( tr("Importing %1 products finished.").arg(m_qvProducts.size()) );
    }
    else
    {
        ui->listLog->addItem( tr( "Error occured during opening trmkfsv.dat file." ) );
    }

    setCursor( Qt::ArrowCursor);
}
//====================================================================================
void MainWindow::_loadProductAssign()
//====================================================================================
{
    FILE           *file = NULL;
    unsigned int    nCount = 0;
    char            m_strVersion[10];

    setCursor( Qt::WaitCursor);

    m_qvProductAssigns.clear();

    file = fopen( m_qsPAFileName.toStdString().c_str(), "rb" );
    if( file != NULL )
    {
        memset( m_strVersion, 0, 10 );
        fread( m_strVersion, 10, 1, file );

        nCount = 0;
        fread( &nCount, 4, 1, file );
        ui->listLog->addItem( tr("Count of product assigns to be imported: %1").arg(nCount) );
        if( nCount > 0 )
        {
            typ_termektipusassign stTemp;
            for( unsigned int i=0; i<nCount; i++ )
            {
                fread( &stTemp.nTermekID, 4, 1, file );
                fread( &stTemp.nTTipusID, 4, 1, file );

                //ui->listLog->addItem( QString( "[%1] [%2]" ).arg(stTemp.nTermekID).arg(stTemp.nTTipusID) );

                m_qvProductAssigns.append( stTemp );
            }
        }

        fclose( file );
        ui->listLog->addItem( tr("Importing %1 product assigns finished.").arg(m_qvProductAssigns.size()) );
    }
    else
    {
        ui->listLog->addItem( tr( "Error occured during opening trmktpssgfsv.dat file." ) );
    }

    setCursor( Qt::ArrowCursor);
}
//====================================================================================
void MainWindow::_loadUsers()
//====================================================================================
{
    FILE           *file = NULL;
    unsigned int    nCount = 0;
    char            m_strVersion[10];

    setCursor( Qt::WaitCursor);

    m_qvUsers.clear();

    file = fopen( m_qsUFileName.toStdString().c_str(), "rb" );
    if( file != NULL )
    {
        memset( m_strVersion, 0, 10 );
        fread( m_strVersion, 10, 1, file );

        nCount = 0;
        fread( &nCount, 4, 1, file );
        ui->listLog->addItem( tr("Count of users to be imported: %1").arg(nCount) );
        if( nCount > 0 )
        {
            typ_user stTemp;
            for( unsigned int i=0; i<nCount; i++ )
            {
                fread( &stTemp.nID, 4, 1, file );
                fread( stTemp.strAzonosito, 20, 1, file );
                fread( stTemp.strLoginNev, 20, 1, file );
                fread( stTemp.strNevCsalad, 100, 1, file );
                fread( stTemp.strJelszo, 20, 1, file );
                fread( stTemp.strMegjegyzes, 1000, 1, file );
                fread( &stTemp.nUserLevel, 4, 1, file );

                _DeCode( stTemp.strAzonosito, 20 );
                _DeCode( stTemp.strLoginNev, 20 );
                _DeCode( stTemp.strNevCsalad, 100 );
                _DeCode( stTemp.strJelszo, 20 );
                _DeCode( stTemp.strMegjegyzes, 1000 );

                ui->listLog->addItem( QString( "\'%1\' \'%2\' \'%3\' \'%4\' \'%5\' [%6]" ).arg(stTemp.strAzonosito).arg(stTemp.strLoginNev).arg(stTemp.strNevCsalad).arg(stTemp.strJelszo).arg(stTemp.strMegjegyzes).arg(stTemp.nUserLevel) );

                m_qvUsers.append( stTemp );
            }
        }

        fclose( file );
        ui->listLog->addItem( tr("Importing %1 users finished.").arg(m_qvUsers.size()) );
    }
    else
    {
        ui->listLog->addItem( tr( "Error occured during opening srfsv.dat file." ) );
    }

    setCursor( Qt::ArrowCursor);
}
//====================================================================================
void MainWindow::_EnCode( char *str, int size )
//====================================================================================
{
   for(int i=0;i<size;i++)
   {
      str[i] ^= 11;
   }
}
//====================================================================================
void MainWindow::_DeCode( char *str, int size )
//====================================================================================
{
   for(int i=0;i<size;i++)
   {
      str[i] ^= 11;
   }
}
//====================================================================================
void MainWindow::_loadPatientCardTypeImport()
{/*
    QFile file( ui->ledImportFileName->text() );

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);

    if( !in.atEnd() )
    {
        QString qsLine = in.readLine();

        if( qsLine.compare("#Ar,Egyseg,Nev,TolEv,TolHo,TolNap,IgEv,IgHo,IgNap,Napok,Hasznalat,Egysegido") )
        {
            QMessageBox::warning( this, tr("Attention"),
                                  tr("The selected file is not a valid import file.\n"
                                     "Please select a valid file and restart the process.") );
            return;
        }
    }

    int inPCTAppend = 0;

    while( !in.atEnd() )
    {
        QString     qsLine = in.readLine();
        QStringList qslRecord = qsLine.split( QChar(',') );

        if( qslRecord.count() == 12 )
        {
            typ_berlettipus stTemp;

            stTemp.nID              = _getPatientCardTypeId();
            stTemp.nAr              = qslRecord.at(0).toInt();
            stTemp.nEgyseg          = qslRecord.at(1).toInt();
            strcpy( stTemp.strNev, qslRecord.at(2).toStdString().c_str() );
            stTemp.nErvTolEv        = qslRecord.at(3).toInt();
            stTemp.nErvTolHo        = qslRecord.at(4).toInt();
            stTemp.nErvTolNap       = qslRecord.at(5).toInt();
            stTemp.nErvIgEv         = qslRecord.at(6).toInt();
            stTemp.nErvIgHo         = qslRecord.at(7).toInt();
            stTemp.nErvIgNap        = qslRecord.at(8).toInt();
            stTemp.nErvNapok        = qslRecord.at(9).toInt();
            stTemp.bSzolariumHaszn  = qslRecord.at(10).toInt();
            stTemp.nEgysegIdo       = qslRecord.at(11).toInt();

            m_qvPatientCardTypes.append( stTemp );
            inPCTAppend++;
        }
    }

    ui->listLog->addItem( tr("Importing %1 patientcards finished.").arg(inPCTAppend) );
*/}

/*void MainWindow::on_pbSelectImportFile_clicked()
{
    QString qsFileName = QFileDialog::getOpenFileName(this, tr("Open import file"), "", tr("Import Files (*.imp)"));

    if( qsFileName.length() )
        ui->ledImportFileName->setText( qsFileName );
}*/

/*void MainWindow::on_pbImportPCTText_clicked()
{
    if( ui->ledImportFileName->text().length() < 1 )
    {
        QMessageBox::warning( this, tr("Attention"),
                              tr("No import file selected."));
        return;
    }

    _loadPatientCardTypeImport();
}*/

int MainWindow::_getPatientCardTypeId()
{
    int nRet = 0;

    for( int i=0; i<m_qvPatientCardTypes.size(); i++ )
    {
        if( m_qvPatientCardTypes.at(i).nID > nRet )
            nRet = m_qvPatientCardTypes.at(i).nID;
    }

    return ++nRet;
}


/*void MainWindow::on_pbExportPCTDat_clicked()
{
    FILE    *file = NULL;

    int      nBerletTypeCount = m_qvPatientCardTypes.size();

    setCursor( Qt::WaitCursor);

    file = fopen( "brlttpsfsv_new.dat", "wb" );
    if( file != NULL )
    {
        fwrite( m_strPatiencardTypeVersion, 10, 1, file );
        fwrite( &nBerletTypeCount, 4, 1, file );

        typ_berlettipus   stTemp;

        for( int i=0; i<nBerletTypeCount; i++ )
        {
            stTemp = m_qvPatientCardTypes.at(i);

            _EnCode( stTemp.strNev, 50 );

            fwrite( &stTemp.nID, 4, 1, file );
            fwrite( &stTemp.nAr, 4, 1, file );
            fwrite( &stTemp.nEgyseg, 4, 1, file );
            fwrite( stTemp.strNev, 50, 1, file );
            fwrite( &stTemp.nErvTolEv, 4, 1, file );
            fwrite( &stTemp.nErvTolHo, 4, 1, file );
            fwrite( &stTemp.nErvTolNap, 4, 1, file );
            fwrite( &stTemp.nErvIgEv, 4, 1, file );
            fwrite( &stTemp.nErvIgHo, 4, 1, file );
            fwrite( &stTemp.nErvIgNap, 4, 1, file );
            fwrite( &stTemp.nErvNapok, 4, 1, file );
            fwrite( &stTemp.bSzolariumHaszn, 1, 1, file );
            if( m_nProgramType == DBTool::Sensolite )
            {
                fwrite( &stTemp.nEgysegIdo, 4, 1, file );
            }
        }
        fclose( file );
        ui->listLog->addItem( tr( "'brlttpsfsv_new.dat' file created with %2 patientcard types" ).arg(nBerletTypeCount) );
    }
    else
    {
        ui->listLog->addItem( tr( "Error occured during opening brlttpsfsv_new.dat file." ) );
    }

    setCursor( Qt::ArrowCursor);
}*/

void MainWindow::on_pbPExportConnect_clicked()
{
    m_poDB = new QSqlDatabase( QSqlDatabase::addDatabase( "QMYSQL" ) );

    m_poDB->setHostName( "localhost" );
    m_poDB->setDatabaseName( "belenus" );
    m_poDB->setUserName( "belenus" );
    m_poDB->setPassword( "belenus" );

    if( m_poDB->open() )
    {
        ui->listLog->addItem( tr("Successfully connected to Belenus database") );
        ui->pbExportProcess->setEnabled( true );

        QString qsSQLCommand = QString( "SELECT * FROM licences" );

        QSqlQuery qsQuery = m_poDB->exec( qsSQLCommand );

        qsQuery.last();
        m_nLicenceId = qsQuery.value(0).toInt();

        ui->listLog->addItem( tr("License code: %1 [%2]").arg(qsQuery.value(1).toString()).arg(m_nLicenceId) );
    }
}

void MainWindow::on_pbExportProcess_clicked()
{
    if( m_poDB->isOpen() )
    {
        m_dlgProgress->showProgress();

        if( ui->chkPExportPCT->isChecked() )
        {
            _exportToBelenusPatientCardTypes();
        }

        if( ui->chkPExportPC->isChecked() )
        {
            _exportToBelenusPatientCards();
        }

        if( ui->chkPExportPT->isChecked() )
        {
            _exportToBelenusProductTypes();
        }

        if( ui->chkPExportP->isChecked() )
        {
            _exportToBelenusProducts();
        }

        if( ui->chkPExportUser->isChecked() )
        {
            _exportToBelenusUsers();
        }

        m_poDB->close();
        m_dlgProgress->hideProgress();
        ui->listLog->addItem( tr("Connection to Belenus database closed") );
    }
    ui->pbExportProcess->setEnabled( false );
    if( m_poDB != NULL ) delete m_poDB;

    m_poDB = NULL;
}

int MainWindow::_getTimeLeft(int p_nBerletTipus, int p_nEgyseg )
{
    int nRet = 0;

    int nBerletTypeCount = m_qvPatientCardTypes.size();

    for( int i=0; i<nBerletTypeCount; i++ )
    {
        if( m_qvPatientCardTypes.at(i).nID == p_nBerletTipus )
        {
            if( m_nProgramType == DBTool::Sensolite )
            {
                nRet = m_qvPatientCardTypes.at(i).nEgysegIdo * p_nEgyseg * 60;
            }
            else
            {
                nRet = ui->ledPExportS2->text().toInt() * p_nEgyseg * 60;
            }
            break;
        }
    }

    return nRet;
}

void MainWindow::_exportToBelenusPatientCardTypes()
{
    int nBerletTypeCount = m_qvPatientCardTypes.size();

    for( int i=0; i<nBerletTypeCount; i++ )
    {
        QString qsSQLCommand = QString( "INSERT INTO `patientCardTypes` (`patientCardTypeId`, `licenceId`, `name`, `price`, `vatpercent`, `units`, `validDateFrom`, `validDateTo`, `validDays`, `unitTime`, `active`, `archive`) VALUES ( " );

        qsSQLCommand += QString( "%1, " ).arg( m_qvPatientCardTypes.at(i).nID+1 );
        qsSQLCommand += QString( "%1, " ).arg( m_nLicenceId );
        qsSQLCommand += QString( "'%1', " ).arg( m_qvPatientCardTypes.at(i).strNev );
        qsSQLCommand += QString( "%1, " ).arg( m_qvPatientCardTypes.at(i).nAr );
        qsSQLCommand += QString( "0, " );
        qsSQLCommand += QString( "%1, " ).arg( m_qvPatientCardTypes.at(i).nEgyseg );
        qsSQLCommand += QString( "'%1-%2-%3', " ).arg( m_qvPatientCardTypes.at(i).nErvTolEv ).arg( m_qvPatientCardTypes.at(i).nErvTolHo ).arg( m_qvPatientCardTypes.at(i).nErvTolNap );
        qsSQLCommand += QString( "'%1-%2-%3', " ).arg( m_qvPatientCardTypes.at(i).nErvIgEv ).arg( m_qvPatientCardTypes.at(i).nErvIgHo ).arg( m_qvPatientCardTypes.at(i).nErvIgNap );
        qsSQLCommand += QString( "%1, " ).arg( m_qvPatientCardTypes.at(i).nErvNapok );
        if( m_nProgramType == DBTool::Sensolite )
        {
            qsSQLCommand += QString( "%1, " ).arg( m_qvPatientCardTypes.at(i).nEgysegIdo );
        }
        else
        {
            qsSQLCommand += QString( "%1, " ).arg( ui->ledPExportS2->text().toInt() );
        }
        qsSQLCommand += QString( "1, 'ARC');" );

        QSqlQuery query = m_poDB->exec( qsSQLCommand );
        m_qvPatientCardTypes[i].nNewID = query.lastInsertId().toInt();

        qsSQLCommand = QString( "INSERT INTO `patientcardtypeenabled` (`licenceId`, `patientCardTypeId`, `start`, `stop`, `modified`, `archive`) VALUES ( " );
        qsSQLCommand += QString( "%1, " ).arg( m_nLicenceId );
        qsSQLCommand += QString( "%1, " ).arg( m_qvPatientCardTypes.at(i).nID+1 );
        qsSQLCommand += QString( "'00:00:00', " );
        qsSQLCommand += QString( "'23:59:00', " );
        qsSQLCommand += QString( "0, 'ARC');" );
//QMessageBox::information(this,"",qsSQLCommand);
        m_poDB->exec( qsSQLCommand );
    }
}

void MainWindow::_exportToBelenusPatientCards()
{
    int nBerletCount = m_qvPatientCards.size();

    for( int i=0; i<nBerletCount; i++ )
    {
        QString qsSQLCommand = QString( "INSERT INTO `patientCards` ( `licenceId`, `patientCardTypeId`, `patientId`, `barcode`, `comment`, `units`, `timeLeft`, `validDateFrom`, `validDateTo`, `pincode`, `active`, `archive`) VALUES ( " );

        qsSQLCommand += QString( "%1, " ).arg( m_nLicenceId );
        qsSQLCommand += QString( "%1, " ).arg( _getPatientCardTypeNewId( m_qvPatientCards.at(i).nBerletTipus ) );
        qsSQLCommand += QString( "0, " );
        qsSQLCommand += QString( "'%1', " ).arg( m_qvPatientCards.at(i).strVonalkod );
        qsSQLCommand += QString( "'%1', " ).arg( m_qvPatientCards.at(i).strMegjegyzes );
        qsSQLCommand += QString( "%1, " ).arg( m_qvPatientCards.at(i).nEgyseg );
        qsSQLCommand += QString( "%1, " ).arg( _getTimeLeft(m_qvPatientCards.at(i).nBerletTipus, m_qvPatientCards.at(i).nEgyseg) );
        qsSQLCommand += QString( "'%1-%2-%3', " ).arg( m_qvPatientCards.at(i).nErvEv-1 ).arg( m_qvPatientCards.at(i).nErvHo ).arg( m_qvPatientCards.at(i).nErvNap );
        qsSQLCommand += QString( "'%1-%2-%3', " ).arg( m_qvPatientCards.at(i).nErvEv ).arg( m_qvPatientCards.at(i).nErvHo ).arg( m_qvPatientCards.at(i).nErvNap );
        qsSQLCommand += QString( "NULL, 1, 'ARC' );" );

        m_poDB->exec( qsSQLCommand );
    }
}

void MainWindow::_exportToBelenusProductTypes()
{
    int nProductTypeCount = m_qvProductTypes.size();

    for( int i=0; i<nProductTypeCount; i++ )
    {
        QString qsSQLCommand = QString( "INSERT INTO `productTypes` (`licenceId`, `name`, `active`, `archive`) VALUES ( " );

        qsSQLCommand += QString( "%1, " ).arg( m_nLicenceId );
        qsSQLCommand += QString( "'%1', " ).arg( m_qvProductTypes.at(i).strNev );
        qsSQLCommand += QString( "1, 'ARC' );" );

        QSqlQuery query = m_poDB->exec( qsSQLCommand );
        m_qvProductTypes[i].nNewID = query.lastInsertId().toInt();
    }
}

void MainWindow::_exportToBelenusProducts()
{
    int nProductCount = m_qvProducts.size();

    for( int i=0; i<nProductCount; i++ )
    {
        QString qsSQLCommand = QString( "INSERT INTO `products` (`licenceId`, `name`, `barcode`, `netPriceBuy`, `vatpercentBuy`, `netPriceSell`, `vatpercentSell`, `productCount`, `modified`, `active`, `archive`) VALUES ( " );

        qsSQLCommand += QString( "%1, " ).arg( m_nLicenceId );
        qsSQLCommand += QString( "'%1', " ).arg( m_qvProducts.at(i).strNev );
        qsSQLCommand += QString( "'%1', " ).arg( m_qvProducts.at(i).strVonalkod );
        qsSQLCommand += QString( "%1, " ).arg( m_qvProducts.at(i).nArBeszerzes );
        qsSQLCommand += QString( "0, " );
        qsSQLCommand += QString( "%1, " ).arg( m_qvProducts.at(i).nAr );
        qsSQLCommand += QString( "0, " );
        qsSQLCommand += QString( "%1, " ).arg( m_qvProducts.at(i).nDarab );
        qsSQLCommand += QString( "0, 1, 'ARC');" );

        QSqlQuery query = m_poDB->exec( qsSQLCommand );
        m_qvProducts[i].nNewID = query.lastInsertId().toInt();
    }

    int nProductAssign = m_qvProductAssigns.size();

    for( int i=0; i<nProductAssign; i++ )
    {
        QString qsSQLCommand = QString( "INSERT INTO  `connectproductwithtype` (`productTypeId` ,`productId` ,`licenceId`) VALUES ( " );

        qsSQLCommand += QString( "%1, " ).arg( _getProductTypeNewId(m_qvProductAssigns.at(i).nTTipusID) );
        qsSQLCommand += QString( "%1, " ).arg( _getProductNewId(m_qvProductAssigns.at(i).nTermekID) );
        qsSQLCommand += QString( "%1 ); " ).arg( m_nLicenceId );

        //ui->listLog->addItem( qsSQLCommand );

        m_poDB->exec( qsSQLCommand );
    }
}

void MainWindow::_exportToBelenusUsers()
{
    int nUSerCount = m_qvUsers.size();

    for( int i=0; i<nUSerCount; i++ )
    {
        QString qsPassword;

        if( QString(m_qvUsers.at(i).strJelszo).length() == 0 )
        {
            qsPassword = "da39a3ee5e6b4b0d3255bfef95601890afd80709";
        }
        else
        {
            QByteArray  obPwdHash = QCryptographicHash::hash( QString( m_qvUsers.at(i).strJelszo ).toAscii(), QCryptographicHash::Sha1 );
            qsPassword = QString( obPwdHash.toHex() );
        }
        QString qsSQLCommand = QString( "INSERT INTO `users` (`licenceId`, `name`, `realName`, `password`, `accgroup`, `active`, `comment`, `archive`) VALUES ( " );

        qsSQLCommand += QString( "%1, " ).arg( m_nLicenceId );
        qsSQLCommand += QString( "'%1', " ).arg( m_qvUsers.at(i).strLoginNev );
        qsSQLCommand += QString( "'%1', " ).arg( m_qvUsers.at(i).strNevCsalad );
        qsSQLCommand += QString( "'%1', 1, 1, " ).arg( qsPassword );
        qsSQLCommand += QString( "'%1', " ).arg( m_qvUsers.at(i).strMegjegyzes );
        qsSQLCommand += QString( "'ARC');" );

        //ui->listLog->addItem( qsSQLCommand );

        m_poDB->exec( qsSQLCommand );
    }
}

int MainWindow::_getPatientCardTypeNewId( int p_nID )
{
    int nRet = 0;
    int nCount = m_qvPatientCardTypes.size();

    for( int i=0; i<nCount; i++ )
    {
        if( m_qvPatientCardTypes.at(i).nID == p_nID )
        {
            nRet = m_qvPatientCardTypes.at(i).nNewID;
            break;
        }
    }
    return nRet;
}

int MainWindow::_getProductTypeNewId( int p_nID )
{
    int nRet = 0;
    int nCount = m_qvProductTypes.size();

    for( int i=0; i<nCount; i++ )
    {
        if( m_qvProductTypes.at(i).nID == p_nID )
        {
            nRet = m_qvProductTypes.at(i).nNewID;
            break;
        }
    }
    return nRet;
}

int MainWindow::_getProductNewId( int p_nID )
{
    int nRet = 0;
    int nCount = m_qvProducts.size();

    for( int i=0; i<nCount; i++ )
    {
        if( m_qvProducts.at(i).nID == p_nID )
        {
            nRet = m_qvProducts.at(i).nNewID;
            break;
        }
    }
    return nRet;
}

