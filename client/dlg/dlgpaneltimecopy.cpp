
#include <QPushButton>
#include <QMessageBox>
#include <QSettings>

#include "dlgpaneltimecopy.h"
#include "general.h"

cDlgPanelTypeCopy::cDlgPanelTypeCopy( QWidget *p_poParent, unsigned int uiPanelId ) : QDialog( p_poParent )
{
    setupUi( this );

    setWindowIcon( QIcon("./resources/40x40_clock.png") );
    pbCopyTimes->setIcon( QIcon("./resources/40x40_refresh.png") );
    pbExit->setIcon( QIcon("./resources/40x40_exit.png") );

    QString qsQuery = QString( "SELECT name, useTime, usePrice, 1 FROM panelUses WHERE panelId=%1" ).arg(uiPanelId);
    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );

    while( poQuery->next() )
    {
        cCurrency   cPrice( poQuery->value(2).toInt() );
        QString qsSellPrice = QString::number(poQuery->value(2).toInt()/100);
        listPanelTimes->addItem( QString("%1\t%2\t%3").arg(poQuery->value(0).toString()).arg(poQuery->value(1).toString()).arg(cPrice.currencyFullStringShort()) );
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
    for( int i=0; i<listPanelTimes->count(); i++ )
}
