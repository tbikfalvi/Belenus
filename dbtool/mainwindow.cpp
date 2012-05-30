
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QVector>
#include <QCloseEvent>
#include <QTextStream>

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
    connect( ui->rbProgramSensolite, SIGNAL(toggled(bool)), this, SLOT(slotProgramSelected()) );

    ui->tabWidget->setCurrentIndex( 0 );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotProgramSelected()
{
    if( ui->rbProgramKiwiSun->isChecked() )
    {
        m_nProgramType      = DBTool::KiwiSun;
    }
    else if( ui->rbProgramSensolite->isChecked() )
    {
        m_nProgramType      = DBTool::Sensolite;
    }
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

void MainWindow::on_rbProgramSensolite_clicked()
{
    ui->imgLogo->setPixmap( QPixmap(":/imgLogo/Sensolite.png") );
}

void MainWindow::on_pbExpSelectDir_clicked()
{
    QString qsDirectory = QFileDialog::getExistingDirectory(this, tr("Select Directory"), m_qdExpCurrentDir.absolutePath() );

    if (!qsDirectory.isEmpty())
    {
        m_qdExpCurrentDir = QDir( qsDirectory );
        ui->ledPathDB->setText( qsDirectory );

        m_qsPCTFileName = QString( "%1/brlttpsfsv.dat" ).arg( ui->ledPathDB->text() ).replace( "/", "\\" );
        m_qsPCFileName  = QString( "%1/brltfsv.dat" ).arg( ui->ledPathDB->text() ).replace( "/", "\\" );

        ui->listLog->addItem( tr("Filenames of patientcard and patientcard types data:") );
        ui->listLog->addItem( m_qsPCTFileName );
        ui->listLog->addItem( m_qsPCFileName );
    }
}

void MainWindow::on_pbImportDB_clicked()
{
    _loadPatientCardTypes();
    _loadPatientCards();
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
{
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
}

void MainWindow::on_pbSelectImportFile_clicked()
{
    QString qsFileName = QFileDialog::getOpenFileName(this, tr("Open import file"), "", tr("Import Files (*.imp)"));

    if( qsFileName.length() )
        ui->ledImportFileName->setText( qsFileName );
}

void MainWindow::on_pbImportPCTText_clicked()
{
    if( ui->ledImportFileName->text().length() < 1 )
    {
        QMessageBox::warning( this, tr("Attention"),
                              tr("No import file selected."));
        return;
    }

    _loadPatientCardTypeImport();
}

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


void MainWindow::on_pbExportPCTDat_clicked()
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
}
