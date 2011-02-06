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

    m_qsPCFileName  = "";
    m_qsPCTFileName = "";
    m_qsPCUFileName = "";

    dateImportStart->setDate( QDate::currentDate() );
}
//====================================================================================
KiwiSunBerlet::~KiwiSunBerlet()
//====================================================================================
{
}
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

        fgets( strLine, 100 , file );
        fclose( file );
    }
    else
    {
        listLog->addItem( tr( "Error occured during opening connect_pc_pcu.txt file." ) );
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
