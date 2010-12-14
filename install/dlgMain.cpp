
#include <QMessageBox>

#include "dlgMain.h"
#include "ui_dlgMain.h"

dlgMain::dlgMain(QWidget *parent) : QDialog(parent)
{
    setupUi(this);

    pagerControl->setCurrentIndex( 0 );
}

dlgMain::~dlgMain()
{
}

void dlgMain::on_pbStartInstall_clicked()
{

}

void dlgMain::on_pbNext_clicked()
{
    if( pagerControl->currentIndex() < pagerControl->count()-1 )
        pagerControl->setCurrentIndex( pagerControl->currentIndex()+1 );

    if( pagerControl->currentIndex() > 0 )
        pbPrev->setEnabled( true );
    if( pagerControl->currentIndex() == pagerControl->count()-1 )
        pbNext->setEnabled( false );
}

void dlgMain::on_pbPrev_clicked()
{
    if( pagerControl->currentIndex() > 0 )
        pagerControl->setCurrentIndex( pagerControl->currentIndex()-1 );

    if( pagerControl->currentIndex() == 0 )
        pbPrev->setEnabled( false );
    if( pagerControl->currentIndex() < pagerControl->count()-1 )
        pbNext->setEnabled( true );
}

void dlgMain::on_pbCancel_clicked()
{
    if( QMessageBox::question( this, tr("Question"),
                               tr("Are you sure you want to abort installation."),
                               QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes )
    {
        QDialog::reject();
    }
}
