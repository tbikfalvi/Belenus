//====================================================================================
//
//====================================================================================

#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QVector>

//====================================================================================

#include <iostream>
#include <stdio.h>

//====================================================================================

#include "kiwisunberlet.h"
#include "ui_kiwisunberlet.h"

//====================================================================================
KiwiSunBerlet::KiwiSunBerlet(QWidget *parent) : QDialog(parent)
//====================================================================================
{
    setupUi(this);

    m_qdExpCurrentDir = QDir::currentPath();

    ledPathDB->setText( m_qdExpCurrentDir.path() );
    cmbExpPatientCardType->addItem( tr("<Not selected>") );
    ledExpDays->setText( "0" );

    m_qsPCFileName  = "";
    m_qsPCTFileName = "";
    m_qsPCUFileName = "";

    dateImportStart->setDate( QDate::currentDate() );

    tabPatientCardUse->setEnabled( false );
}
//====================================================================================
KiwiSunBerlet::~KiwiSunBerlet()
//====================================================================================
{
}
//====================================================================================
// TAB 1 - Expiration date changer
//====================================================================================
void KiwiSunBerlet::on_pbExpSelectDir_clicked()
//====================================================================================
{
    QString qsDirectory = QFileDialog::getExistingDirectory(this, tr("Select Directory"), m_qdExpCurrentDir.absolutePath() );

    if (!qsDirectory.isEmpty())
    {
        m_qdExpCurrentDir = QDir( qsDirectory );
        ledPathDB->setText( qsDirectory );

        m_qsPCTFileName = QString( "%1/brlttpsfsv.dat" ).arg( ledPathDB->text() ).replace( "/", "\\" );
        m_qsPCFileName  = QString( "%1/brltfsv.dat" ).arg( ledPathDB->text() ).replace( "/", "\\" );

        listLog->addItem( tr("Filenames of patientcard and patientcard types data:") );
        listLog->addItem( m_qsPCTFileName );
        listLog->addItem( m_qsPCFileName );
    }
}
//====================================================================================
void KiwiSunBerlet::on_pbExpImportDB_clicked()
//====================================================================================
{
    pbExpExport->setEnabled( false );

    _loadPatientCardTypes();

    if( m_qvPatientCardTypes.size() == 0 )
    {
        QMessageBox::warning( this, tr("Attention"),
                              tr("Unable to open data file of patientcard types OR\n"
                                 "no patientcard types found in selected data file.\n\n"
                                 "Please select the correct directory.") );
        return;
    }

    _loadPatientCards();

    if( m_qvPatientCards.size() == 0 )
    {
        QMessageBox::warning( this, tr("Attention"),
                              tr("Unable to open data file of patientcards OR\n"
                                 "no patientcards found in selected data file.\n\n"
                                 "Please select the correct directory.") );
        return;
    }

    for( int i=0; i<m_qvPatientCardTypes.size(); i++ )
    {
        cmbExpPatientCardType->addItem( QString(m_qvPatientCardTypes.at(i).strNev) );
    }

    pbExpExport->setEnabled( true );
}
//====================================================================================
void KiwiSunBerlet::on_pbExpExport_clicked()
//====================================================================================
{
    if( cmbExpPatientCardType->currentIndex() == 0 )
    {
        QMessageBox::warning( this, tr("Attention"),
                              tr("No patientcard type selected.\n"
                                 "Please select a patientcard type.") );
        return;
    }

    int    nPCTID = 0;
    int    i;

    for( i=0; i<m_qvPatientCardTypes.size(); i++ )
    {
        if( cmbExpPatientCardType->itemText( cmbExpPatientCardType->currentIndex() ).compare( QString(m_qvPatientCardTypes.at(i).strNev) ) == 0 )
        {
            nPCTID = m_qvPatientCardTypes.at(i).nID;
            break;
        }
    }

    int nCount = 0;
    for( i=0; i<m_qvPatientCards.size(); i++ )
    {
        if( m_qvPatientCards.at(i).nBerletTipus == nPCTID )
            nCount++;
    }

    if( ledExpDays->text().toInt() == 0 )
    {
        QMessageBox::warning( this, tr("Attention"),
                              tr("Number of days is invalid.\n"
                                 "Please set a valid value different than zero.") );
        return;
    }

    if( QMessageBox::question( this, tr("question"),
                               tr("Are you sure you want to modify the expiration date\n"
                                  "of (%1) patientcards related to patientcard type:\n"
                                  "%2\n"
                                  "and export them with new data?").arg(nCount).arg(cmbExpPatientCardType->itemText( cmbExpPatientCardType->currentIndex() )),
                               QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes )
    {
        nCount = 0;
        for( i=0; i<m_qvPatientCards.size(); i++ )
        {
            if( m_qvPatientCards.at(i).nBerletTipus == nPCTID )
            {
                QDate   qdExp = QDate( m_qvPatientCards.at(i).nErvEv,
                                       m_qvPatientCards.at(i).nErvHo,
                                       m_qvPatientCards.at(i).nErvNap );

                qdExp = qdExp.addDays( ledExpDays->text().toInt() );
                nCount++;

                m_qvPatientCards[i].nErvEv = qdExp.year();
                m_qvPatientCards[i].nErvHo = qdExp.month();
                m_qvPatientCards[i].nErvNap = qdExp.day();
            }
        }
        on_pbCreatePC_clicked();
        listLog->addItem( tr( "Creating new patientcard file successfully finished." ) );
        listLog->addItem( tr( "Number of updated patientcards: %1" ).arg(nCount) );
        pbExpExport->setEnabled( false );
    }
}
//====================================================================================
// TAB 2 - Patientcards from usage
//====================================================================================
void KiwiSunBerlet::on_pbSelectPC_clicked()
//====================================================================================
{
    QFileDialog     obFileOpen;

    obFileOpen.setNameFilter(tr("Data files (*.dat)"));
    m_qsPCFileName = obFileOpen.getOpenFileName( this );

    if( m_qsPCFileName.right(11).compare("brltfsv.dat") )
    {
        QMessageBox::warning( this, tr("Attention"),
                              tr("The selected file is not contains PatientCards.\n"
                                 "Please select file with name \"brltfsv.dat\"") );
        m_qsPCFileName = "";
    }

    _enableImportButton();
}
//====================================================================================
void KiwiSunBerlet::on_pbSelectPCT_clicked()
//====================================================================================
{
    QFileDialog     obFileOpen;

    obFileOpen.setNameFilter(tr("Data files (*.dat)"));
    m_qsPCTFileName = obFileOpen.getOpenFileName( this );

    if( m_qsPCTFileName.right(14).compare("brlttpsfsv.dat") )
    {
        QMessageBox::warning( this, tr("Attention"),
                              tr("The selected file is not contains PatientCard types.\n"
                                 "Please select file with name \"brlttpsfsv.dat\"") );
        m_qsPCTFileName = "";
    }

    _enableImportButton();
}
//====================================================================================
void KiwiSunBerlet::on_pbSelectPCU_clicked()
//====================================================================================
{
    QFileDialog     obFileOpen;

    obFileOpen.setNameFilter(tr("Data files (*.dat)"));
    m_qsPCUFileName = obFileOpen.getOpenFileName( this );

    if( m_qsPCUFileName.right(17).compare("brlthsznltfsv.dat") )
    {
        QMessageBox::warning( this, tr("Attention"),
                              tr("The selected file is not contains PatientCard usages.\n"
                                 "Please select file with name \"brlthsznltfsv.dat\"") );
        m_qsPCUFileName = "";
    }

    _enableImportButton();
}
//====================================================================================
void KiwiSunBerlet::on_pbImport_clicked()
//====================================================================================
{
    if( dateImportStart->date() == QDate::currentDate() )
    {
        QMessageBox::warning( this, tr("Attention"),
                              tr("The selected date is the same like current date.\n"
                                 "Please select another date.") );
        return;
    }

    _loadPatientCards();
    _loadPatientCardTypes();
    _loadPatientCardUsages();
}
//====================================================================================
void KiwiSunBerlet::on_dateImportStart_dateChanged( QDate )
//====================================================================================
{
    _enableImportButton();
}
//====================================================================================
void KiwiSunBerlet::on_pbProcessPCU_clicked()
//====================================================================================
{
    FILE    *file = NULL;

    setCursor( Qt::WaitCursor);

    file = fopen( "connect_pc_pcu.txt", "rt" );
    if( file != NULL )
    {
        char    strLine[100];
        int     nCount = 0;

        while( fgets( strLine, 100 , file ) )
        {
            QStringList qslTemp = QString(strLine).split('\t');

            QString qsBarcode   = qslTemp.at(0).simplified();
            int     nCountUsage = qslTemp.at(1).toInt();
            QString qsPCT       = qslTemp.at(2).simplified();

            for( int i=0; i<m_qvPatientCardTypes.size(); i++ )
            {
                if( qsPCT.compare( QString(m_qvPatientCardTypes.at(i).strNev) ) == 0 )
                {
                    _fillPatientCard( qsBarcode, m_qvPatientCardTypes.at(i).nID, m_qvPatientCardTypes.at(i).nEgyseg, nCountUsage );
                    nCount++;
                    break;
                }
            }
        }
        fclose( file );
        listLog->addItem( tr( "%1 PatientCards usages processed." ).arg(nCount) );

        _savePatientCardsToFile();
    }
    else
    {
        listLog->addItem( tr( "Error occured during opening connect_pc_pcu.txt file." ) );
    }

    setCursor( Qt::ArrowCursor);
}
//====================================================================================
void KiwiSunBerlet::on_pbCreatePC_clicked()
//====================================================================================
{
    FILE    *file = NULL;
    char     strPatiencardVersion[10];
    int      nBerletCount = m_qvPatientCards.size();

    setCursor( Qt::WaitCursor);

    memset( strPatiencardVersion, 0, 10 );
    strncpy( strPatiencardVersion, ledVersionPCDAT->text().toStdString().c_str(), 9 );

    file = fopen( "brltfsv_new.dat", "wb" );
    if( file != NULL )
    {
        fwrite( strPatiencardVersion, 10, 1, file );
        fwrite( &nBerletCount, 4, 1, file );

        typ_berlet   stTemp;

        for( int i=0; i<m_qvPatientCards.size(); i++ )
        {
            stTemp = m_qvPatientCards.at(i);
            _EnCode( stTemp.strVonalkod, 20 );
            _EnCode( stTemp.strMegjegyzes, 50 );

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
    else
    {
        listLog->addItem( tr( "Error occured during opening brltfsv_new.txt file." ) );
    }

    setCursor( Qt::ArrowCursor);
}
//====================================================================================
void KiwiSunBerlet::_enableImportButton()
//====================================================================================
{
    if( m_qsPCFileName.length() > 0 &&
        m_qsPCTFileName.length() > 0 &&
        m_qsPCUFileName.length() > 0 &&
        dateImportStart->date() != QDate::currentDate() )
    {
        pbImport->setEnabled( true );
    }
    else
    {
        pbImport->setEnabled( false );
    }
}
//====================================================================================
void KiwiSunBerlet::_loadPatientCards()
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
                _DeCode( stTemp.strVonalkod, 20 );
                _DeCode( stTemp.strMegjegyzes, 50 );

                m_qvPatientCards.append( stTemp );
            }
        }
        fclose( file );
        listLog->addItem( tr("Importing %1 patientcards finished.").arg(m_qvPatientCards.size()) );
    }
    else
    {
        listLog->addItem( tr( "Error occured during opening brltfsv.dat file." ) );
    }

    setCursor( Qt::ArrowCursor);
}
//====================================================================================
void KiwiSunBerlet::_loadPatientCardTypes()
//====================================================================================
{
    FILE           *file = NULL;
    char           strTemp[10];
    unsigned int   nCount = 0;

    setCursor( Qt::WaitCursor);

    m_qvPatientCardTypes.clear();

    file = fopen( m_qsPCTFileName.toStdString().c_str(), "rb" );
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
                stTemp.nEgysegIdo = 0;

                _DeCode( stTemp.strNev, 50 );

                m_qvPatientCardTypes.append( stTemp );
            }
        }
        fclose( file );
        listLog->addItem( tr("Importing %1 patientcard types finished.").arg(m_qvPatientCardTypes.size()) );
    }
    else
    {
        listLog->addItem( tr( "Error occured during opening brlttpsfsv.dat file." ) );
    }

    setCursor( Qt::ArrowCursor);
}
//====================================================================================
void KiwiSunBerlet::_loadPatientCardUsages()
//====================================================================================
{
    FILE            *file = NULL;
    char             strTemp[10];
    unsigned int     nCount = 0;

    setCursor( Qt::WaitCursor);

    file = fopen( m_qsPCUFileName.toStdString().c_str(), "rb" );
    if( file != NULL )
    {
        memset( strTemp, 0, 10 );
        fread( strTemp, 10, 1, file );

        nCount = 0;
        fread( &nCount, 4, 1, file );
        listLog->addItem( tr("Number of patientcard uses stored in data file: %1").arg(nCount) );
        if( nCount > 0 )
        {
            m_qslPCUBarcodes.clear();
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
                _DeCode( stTemp.strVonalkod, 20 );

                QDate       tmpDate( stTemp.nEv, stTemp.nHo, stTemp.nNap );
                QTime       tmpTime( stTemp.nOra, stTemp.nPerc, 0, 0 );
                QDateTime   qdtDate( tmpDate, tmpTime );

                //listLog->addItem( tr("Date: %1  daysTo: %2").arg(qdtDate.toString("yyyy-MM-dd hh:mm")).arg(qdtDate.daysTo(dateImportStart->dateTime())) );
                if( qdtDate.daysTo(dateImportStart->dateTime()) < 1 )
                {
                    m_qvPatientCardUsage.append( stTemp );

                    if( !m_qslPCUBarcodes.contains( QString(stTemp.strVonalkod) ) )
                    {
                        m_qslPCUBarcodes << QString( stTemp.strVonalkod );
                        _addToPatientCards( stTemp );
                    }
                }
            }
            listLog->addItem( tr("%1 PatientCard imported from PatientCard usages.").arg(m_qslPCUBarcodes.count()) );
        }
        fclose( file );
        listLog->addItem( tr("Importing PatientCard usages finished.") );
    }
    else
    {
        listLog->addItem( tr( "Error occured during opening %1 file." ).arg(m_qsPCUFileName) );
    }

    setCursor( Qt::ArrowCursor);
}
//====================================================================================
void KiwiSunBerlet::_addToPatientCards( typ_berlethasznalat p_stPCU )
//====================================================================================
{
    bool bPCFound = false;

    for( int i=0; i<m_qvPatientCards.size(); i++ )
    {
        if( strcmp( m_qvPatientCards.at(i).strVonalkod, p_stPCU.strVonalkod ) == 0 )
        {
            bPCFound = true;
            break;
        }
    }

    if( !bPCFound )
    {
        typ_berlet  stTemp;

        memset( &stTemp, 0, sizeof(typ_berlet) );
        strcpy( stTemp.strVonalkod, p_stPCU.strVonalkod );
        stTemp.nErvEv   = p_stPCU.nEv + 1;
        stTemp.nErvHo   = p_stPCU.nHo;
        stTemp.nErvNap  = p_stPCU.nNap;

        m_qvPatientCards.append( stTemp );
    }
}
//====================================================================================
void KiwiSunBerlet::_fillPatientCard( QString p_qsBarcode, int p_nPCTId, int p_nMaxEgyseg, int p_nUsedEgyseg )
//====================================================================================
{
    for( int i=0; i<m_qvPatientCards.size(); i++ )
    {
        if( p_qsBarcode.compare( m_qvPatientCards.at(i).strVonalkod ) == 0 )
        {
            m_qvPatientCards[i].nBerletTipus = p_nPCTId;
            m_qvPatientCards[i].nEgyseg      = p_nMaxEgyseg - p_nUsedEgyseg;
            break;
        }
    }
}
//====================================================================================
void KiwiSunBerlet::_savePatientCardsToFile()
//====================================================================================
{
    FILE    *file = NULL;

    setCursor( Qt::WaitCursor);

    file = fopen( "patientcards.txt", "wt" );
    if( file != NULL )
    {
        for( int i=0; i<m_qvPatientCards.size(); i++ )
        {
            int nEgyseg = _getMaxUsages(m_qvPatientCards.at(i).nBerletTipus)-_getPatientCardUsages(m_qvPatientCards.at(i).strVonalkod);
            QString qsLine = QString( "%1\t%2\t%3\t%4-%5-%6\t%7\n" ).arg(m_qvPatientCards.at(i).strVonalkod).arg(m_qvPatientCards.at(i).nBerletTipus).arg(m_qvPatientCards.at(i).nEgyseg).arg(m_qvPatientCards.at(i).nErvEv).arg(m_qvPatientCards.at(i).nErvHo).arg(m_qvPatientCards.at(i).nErvNap).arg((m_qvPatientCards.at(i).nEgyseg==nEgyseg?QString("OK"):QString("HIBA")));

            fputs( qsLine.toStdString().c_str(), file );
        }
        fclose( file );
    }
    else
    {
        listLog->addItem( tr( "Error occured during opening patientcards.txt file." ) );
    }

    setCursor( Qt::ArrowCursor);
}
//====================================================================================
int KiwiSunBerlet::_getPatientCardUsages( QString p_qsBarcode )
//====================================================================================
{
    int nRet = 0;

    for( int i=0; i<m_qvPatientCardUsage.size(); i++ )
    {
        if( p_qsBarcode.compare(m_qvPatientCardUsage.at(i).strVonalkod) == 0 )
        {
            nRet += m_qvPatientCardUsage.at(i).nEgyseg;
        }
    }

    return nRet;
}
//====================================================================================
int KiwiSunBerlet::_getMaxUsages( int p_nPCTId )
//====================================================================================
{
    int nRet = 0;

    for( int i=0; i<m_qvPatientCardTypes.size(); i++ )
    {
        if( m_qvPatientCardTypes.at(i).nID == p_nPCTId )
        {
            nRet = m_qvPatientCardTypes.at(i).nEgyseg;
            break;
        }
    }

    return nRet;
}
//====================================================================================
void KiwiSunBerlet::_EnCode( char *str, int size )
//====================================================================================
{
   for(int i=0;i<size;i++)
   {
      str[i] ^= 11;
   }
}
//====================================================================================
void KiwiSunBerlet::_DeCode( char *str, int size )
//====================================================================================
{
   for(int i=0;i<size;i++)
   {
      str[i] ^= 11;
   }
}
//====================================================================================

