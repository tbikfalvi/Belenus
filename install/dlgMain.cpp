//====================================================================================
//
// Belenus telepito alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dlgMain.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#include <QMessageBox>

//====================================================================================

#include "dlgMain.h"
#include "ui_dlgMain.h"

//====================================================================================
dlgMain::dlgMain(QWidget *parent) : QDialog(parent)
//====================================================================================
{
    setupUi(this);

    pagerControl->setCurrentIndex( 0 );

    Logo->setPixmap( QPixmap( QString(":/images/Logo.png") ) );
    setWindowIcon( QIcon( QString(":/icons/belenus.ico") ) );
}
//====================================================================================
dlgMain::~dlgMain()
//====================================================================================
{
}
//====================================================================================
void dlgMain::on_pbNext_clicked()
//====================================================================================
{
    if( pagerControl->currentIndex() < pagerControl->count()-1 )
        pagerControl->setCurrentIndex( pagerControl->currentIndex()+1 );

    if( pagerControl->currentIndex() > 0 )
        pbPrev->setEnabled( true );
    if( pagerControl->currentIndex() == pagerControl->count()-1 )
        pbNext->setEnabled( false );

    processPage( pagerControl->currentIndex() );
}
//====================================================================================
void dlgMain::on_pbPrev_clicked()
//====================================================================================
{
    if( pagerControl->currentIndex() > 0 )
        pagerControl->setCurrentIndex( pagerControl->currentIndex()-1 );

    if( pagerControl->currentIndex() == 0 )
        pbPrev->setEnabled( false );
    if( pagerControl->currentIndex() < pagerControl->count()-1 )
        pbNext->setEnabled( true );

    processPage( pagerControl->currentIndex() );
}
//====================================================================================
void dlgMain::on_pbCancel_clicked()
//====================================================================================
{
    if( QMessageBox::question( this, tr("Question"),
                               tr("Are you sure you want to abort installation?"),
                               QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes )
    {
        QDialog::reject();
    }
}
//====================================================================================
void dlgMain::processPage( int p_nPage )
//====================================================================================
{
    QMessageBox::information( this, "", QString::number(p_nPage) );
}
//====================================================================================
void dlgMain::on_pbStartInstall_clicked()
//====================================================================================
{

}
//====================================================================================
