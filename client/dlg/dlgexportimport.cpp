
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include <QDateTime>

#include "dlgexportimport.h"
#include "dlgprogress.h"

cDlgExportImport::cDlgExportImport( QWidget *p_poParent, teProcessType p_teExportImport ) : QDialog( p_poParent )
{
    setupUi( this );

    pbExit->setIcon( QIcon("./resources/40x40_exit.png") );
    pbDir->setIcon( QIcon("./resources/40x40_directory.png") );
    pbExecute->setIcon( QIcon("./resources/40x40_database_sync.png") );

    pbPrev->setEnabled( false );
    pbNext->setEnabled( false );
    pbExecute->setEnabled( false );

    pageWizard->setCurrentIndex( 0 );

    m_dlgProgress = new cDlgProgress( this );

    m_teExportImport = p_teExportImport;

    m_qsDir  = QDir::currentPath();
    m_qsFile = "";

    m_qsDir.replace( '/', '\\' );
    if( m_qsDir.right(1).compare("\\") == 0 )
    {
        m_qsDir = m_qsDir.left(m_qsDir.length()-1);
    }

    if( m_teExportImport == PT_EXPORT )
    {
        lblTitle_1->setText( tr("Data export") );
        lblTitle_2->setText( tr("Select destination directory") );

        rbPatientCard->setEnabled( false );
    }
    else
    {
        lblTitle_1->setText( tr("Data import") );
        lblTitle_2->setText( tr("Select file to be imported") );
    }
}

cDlgExportImport::~cDlgExportImport()
{
    delete m_dlgProgress;
}

void cDlgExportImport::on_pbPrev_clicked()
{
    if( pageWizard->currentIndex() > 0 )
    {
        pageWizard->setCurrentIndex( pageWizard->currentIndex()-1 );
    }
    _processPage( false );
}

void cDlgExportImport::on_pbNext_clicked()
{
    if( pageWizard->currentIndex() < 2 )
    {
        pageWizard->setCurrentIndex( pageWizard->currentIndex()+1 );
    }
    _processPage();
}

void cDlgExportImport::on_pbExecute_clicked()
{
    if( m_teExportImport == PT_EXPORT )
    {
        switch( m_teData )
        {
            case DD_GUEST:
                _ExportGuests();
                break;
            case DD_PCARD:
                _ExportPatientCards();
                break;
            case DD_PROD:
                _ExportProducts();
                break;
        }
    }
    else
    {
        switch( m_teData )
        {
            case DD_GUEST:
                _ImportGuests();
                break;
            case DD_PCARD:
                _ImportPatientCards();
                break;
            case DD_PROD:
                _ImportProducts();
                break;
        }
    }
}

void cDlgExportImport::on_pbExit_clicked()
{
    close();
}

void cDlgExportImport::on_rbGuest_clicked()
{
    pbNext->setEnabled( true );
    m_teData = DD_GUEST;
    m_qsFile = "";
    lblComment->setText( "" );
}

void cDlgExportImport::on_rbPatientCard_clicked()
{
    pbNext->setEnabled( true );
    m_teData = DD_PCARD;
    m_qsFile = "";
    lblComment->setText( "" );
    if( m_teExportImport == PT_IMPORT )
    {
        lblComment->setText( tr("Please note that only the barcode of the patientcards will be imported.\n"
                                "Duplicated barcodes or barcodes with invalid length will be skipped.") );
    }
}

void cDlgExportImport::on_rbProduct_clicked()
{
    pbNext->setEnabled( true );
    m_teData = DD_PROD;
    m_qsFile = "";
    lblComment->setText( "" );
    if( m_teExportImport == PT_IMPORT )
    {
        lblComment->setText( tr("Products with duplicated barcodes will be skipped.") );
    }
}

void cDlgExportImport::_processPage( bool p_bMoveForward )
{
    switch( pageWizard->currentIndex() )
    {
        case 1:
        {
            if( p_bMoveForward )
            {
                if( m_teExportImport == PT_EXPORT )
                {
                    switch( m_teData )
                    {
                        case DD_GUEST:
                            m_qsFile = QString( "belenus_export_guest_%1.txt" ).arg( QDateTime::currentDateTime().toString("yyyyMMddhhmmss") );
                            break;
                        case DD_PCARD:
                            m_qsFile = QString( "belenus_export_patientcard_%1.txt" ).arg( QDateTime::currentDateTime().toString("yyyyMMddhhmmss") );
                            break;
                        case DD_PROD:
                            m_qsFile = QString( "belenus_export_product_%1.txt" ).arg( QDateTime::currentDateTime().toString("yyyyMMddhhmmss") );
                            break;
                    }
                }
                QString qsTarget = QString("%1\\%2").arg( m_qsDir ).arg( m_qsFile );
                ledTarget->setText( qsTarget );
                ledTarget->setToolTip( qsTarget );
            }
            pbPrev->setEnabled( true );
            pbNext->setEnabled( m_qsDir.length() > 0 && m_qsFile.length() > 0 );
            pbExecute->setEnabled( false );
            break;
        }
        case 2:
        {
            if( m_teExportImport == PT_EXPORT )
            {
                switch( m_teData )
                {
                    case DD_GUEST:
                        teInfo->setText( tr("Exporting guest data to the following file:\n\n"
                                            "%1").arg(ledTarget->text()) );
                        break;
                    case DD_PCARD:
                        teInfo->setText( tr("Exporting patientcard data to the following file:\n\n"
                                            "%1").arg(ledTarget->text()) );
                        break;
                    case DD_PROD:
                        teInfo->setText( tr("Exporting product data to the following file:\n\n"
                                            "%1").arg(ledTarget->text()) );
                        break;
                }
            }
            else
            {
                switch( m_teData )
                {
                    case DD_GUEST:
                        teInfo->setText( tr("Importing guest data from the following file:\n\n"
                                            "%1").arg(ledTarget->text()) );
                        break;
                    case DD_PCARD:
                        teInfo->setText( tr("Importing patientcard barcodes from the following file:\n\n"
                                            "%1").arg(ledTarget->text()) );
                        break;
                    case DD_PROD:
                        teInfo->setText( tr("Importing product data from the following file:\n\n"
                                            "%1").arg(ledTarget->text()) );
                        break;
                }
            }
            pbPrev->setEnabled( true );
            pbNext->setEnabled( false );
            pbExecute->setEnabled( true );
            break;
        }
        case 0:
        default:
        {
            pbPrev->setEnabled( false );
            if( rbGuest->isChecked() || rbPatientCard->isChecked() || rbProduct->isChecked() )
            {
                pbNext->setEnabled( true );
            }
            else
            {
                pbNext->setEnabled( false );
            }
            pbExecute->setEnabled( false );
            break;
        }
    }
}

void cDlgExportImport::on_pbDir_clicked()
{
    if( m_teExportImport == PT_EXPORT )
    {
        QString qsDir = QFileDialog::getExistingDirectory( this, tr("Select target directory"),
                                                           m_qsDir,
                                                           QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

        if( qsDir.length() > 0 )
        {
            m_qsDir = qsDir.replace( '/', '\\' );
            if( m_qsDir.right(1).compare("\\") == 0 )
            {
                m_qsDir = m_qsDir.left(m_qsDir.length()-1);
            }
        }
    }
    else
    {
        QFileDialog dlgFileOpen( this );

        dlgFileOpen.setDirectory( m_qsDir );
        dlgFileOpen.setFileMode( QFileDialog::ExistingFile );
        dlgFileOpen.setOptions( QFileDialog::DontResolveSymlinks );
        dlgFileOpen.setViewMode( QFileDialog::Detail );

        if( dlgFileOpen.exec() )
        {
            m_qsDir  = dlgFileOpen.directory().absolutePath();
            m_qsDir.replace( '/', '\\' );
            if( m_qsDir.right(1).compare("\\") == 0 )
            {
                m_qsDir = m_qsDir.left(m_qsDir.length()-1);
            }
            m_qsFile = dlgFileOpen.selectedFiles().at(0).right( dlgFileOpen.selectedFiles().at(0).length()-m_qsDir.length()-1 );
        }
    }

    _processPage();
}

void cDlgExportImport::_ExportGuests()
{
    m_dlgProgress->show();

    QStringList  qslLines;
    QSqlQuery   *poQuery = g_poDB->executeQTQuery( QString( "SELECT name, email, mobile, address FROM patients WHERE active=1 ORDER BY name" ) );

    while( poQuery->next() )
    {
        QString qsLine = "";

        qsLine.append( poQuery->value( 0 ).toString() );
        qsLine.append( "\t" );
        qsLine.append( poQuery->value( 1 ).toString() );
        qsLine.append( "\t" );
        qsLine.append( poQuery->value( 2 ).toString() );
        qsLine.append( "\t" );
        qsLine.append( poQuery->value( 3 ).toString() );
        qsLine.append( "\n" );

        qslLines << qsLine;
    }

    QFile   file( ledTarget->text() );

    file.open( QIODevice::WriteOnly );

    file.write( "#Guest name\tE-Mail\tPhone\tAddress\n" );

    for( int i=0; i<qslLines.count(); i++ )
    {
        file.write( qslLines.at(i).toStdString().c_str() );
    }
    file.close();

    m_dlgProgress->hide();

    pbExecute->setEnabled( false );

    QMessageBox::information( this, tr("Information"),
                              tr("Export process has been finished.") );
}

void cDlgExportImport::_ExportPatientCards()
{
    /* nincs ertelme */
}

void cDlgExportImport::_ExportProducts()
{
}

void cDlgExportImport::_ImportGuests()
{
}

void cDlgExportImport::_ImportPatientCards()
{
}

void cDlgExportImport::_ImportProducts()
{
}

