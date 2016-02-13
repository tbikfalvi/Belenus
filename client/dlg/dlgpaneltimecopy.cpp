
#include <QPushButton>
#include <QMessageBox>
#include <QSettings>

#include "dlgpaneltimecopy.h"
#include "general.h"
#include "db/dbpaneluses.h"

cDlgPanelTypeCopy::cDlgPanelTypeCopy( QWidget *p_poParent, unsigned int uiPanelId ) : QDialog( p_poParent )
{
    setupUi( this );

    setWindowTitle( tr( "Copy panel use times" ) );
    setWindowIcon( QIcon("./resources/40x40_clock.png") );
    pbCopyTimes->setIcon( QIcon("./resources/40x40_refresh.png") );
    pbExit->setIcon( QIcon("./resources/40x40_exit.png") );

    m_dlgProgress = new cDlgProgress( this );

    QString qsQuery = QString( "SELECT name, useTime, usePrice FROM panelUses WHERE panelId=%1" ).arg(uiPanelId);
    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );

    while( poQuery->next() )
    {
        cCurrency   cPrice( poQuery->value(2).toInt() );
        listPanelTimes->addItem( QString("%1\t%2\t%3").arg(poQuery->value(0).toString())
                                                      .arg(poQuery->value(1).toString())
                                                      .arg(cPrice.currencyString()) );
    }

    qsQuery = QString( "SELECT panelId, title FROM panels WHERE panelId<>%1 AND panelId>0" ).arg(uiPanelId);
    poQuery = g_poDB->executeQTQuery( qsQuery );

    while( poQuery->next() )
    {
        listPanels->addItem( QString("%1\t%2").arg(poQuery->value(0).toString()).arg(poQuery->value(1).toString()) );
    }

    connect( listPanelTimes, SIGNAL(itemSelectionChanged()), this, SLOT(updateButtonCopy()) );
    connect( listPanels, SIGNAL(itemSelectionChanged()), this, SLOT(updateButtonCopy()) );

    updateButtonCopy();
}

cDlgPanelTypeCopy::~cDlgPanelTypeCopy()
{
    delete m_dlgProgress;
}

void cDlgPanelTypeCopy::updateButtonCopy()
{
    if( listPanelTimes->selectedItems().count() > 0 && listPanels->selectedItems().count() > 0 )
    {
        pbCopyTimes->setEnabled( true );
    }
    else
    {
        pbCopyTimes->setEnabled( false );
    }
}


void cDlgPanelTypeCopy::on_pbExit_clicked()
{
    QDialog::accept();
}

void cDlgPanelTypeCopy::on_pbCopyTimes_clicked()
{
    m_dlgProgress->showProgress();

    int nCountCopied = 0;
    int nCountSkipped = 0;

    for( int i=0; i<listPanelTimes->selectedItems().count(); i++ )
    {
        for( int j=0; j<listPanels->selectedItems().count(); j++ )
        {
            QStringList qslPanelUse = listPanelTimes->selectedItems().at(i)->text().split("\t");
            QStringList qslPanel    = listPanels->selectedItems().at(j)->text().split("\t");

            cDBPanelUses    obDbPanelUse;

            obDbPanelUse.setPanelId( qslPanel.at(0).toUInt() );
            obDbPanelUse.setName( qslPanelUse.at(0) );
            obDbPanelUse.setUseTime( qslPanelUse.at(1).toInt() );
            obDbPanelUse.setUsePrice( qslPanelUse.at(2).toDouble()*100 );

            if( obDbPanelUse.isPanelUseExists() )
            {
                nCountSkipped++;
            }
            else
            {
                obDbPanelUse.save();
                nCountCopied++;
            }
        }
    }

    m_dlgProgress->hideProgress();

    QMessageBox::information( this, tr("Information"),
                              tr("Copy of the selected panel uses has been finished.\n"
                                 "Number of copied items: %1\n"
                                 "Number of skipped items (due to avoid duplication): %2").arg(nCountCopied).arg(nCountSkipped) );
}
