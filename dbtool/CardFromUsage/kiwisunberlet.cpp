//====================================================================================
//
//====================================================================================

#include <QFileDialog>
#include <QMessageBox>
#include <QFile>

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

    m_qsPCUFileName = "";

    dateImportStart->setDate( QDate::currentDate() );
}
//====================================================================================
KiwiSunBerlet::~KiwiSunBerlet()
//====================================================================================
{
}
//====================================================================================
void KiwiSunBerlet::on_pbSelectPCU_clicked()
//====================================================================================
{
    QFileDialog     obFileOpen;

    obFileOpen.setNameFilter(tr("Data files (*.dat)"));
    m_qsPCUFileName = obFileOpen.getOpenFileName( this );

    pbImportPCU->setEnabled( m_qsPCUFileName.length() );
}
//====================================================================================
void KiwiSunBerlet::on_pbImportPCU_clicked()
//====================================================================================
{
    if( dateImportStart->date() == QDate::currentDate() )
    {
        QMessageBox::warning( this, tr("Attention"),
                              tr("The selected date is the same like current date.\n"
                                 "Please select another date.") );
        return;
    }
    if( m_qsPCUFileName.right(17).compare("brlthsznltfsv.dat") )
    {
        QMessageBox::warning( this, tr("Attention"),
                              tr("The selected file is not contains PatientCard usages.\n"
                                 "Please select file with name \"brlthsznltfsv.dat\"") );
        return;
    }
    _loadPatientCardUsages();
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
