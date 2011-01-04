//=======================================================================================
//
// Belenus telepito alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//=======================================================================================
//
// Filename    : dlgMain.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//=======================================================================================
//
//=======================================================================================

#include <QMessageBox>

#include "vregistry.h"

//=======================================================================================

#include "dlgMain.h"
#include "ui_dlgMain.h"

using namespace voidrealms::win32;

//=======================================================================================
dlgMain::dlgMain(QWidget *parent) : QDialog(parent)
//=======================================================================================
{
    setupUi(this);

    pagerControl->setCurrentIndex( 0 );

    Logo->setPixmap( QPixmap( QString(":/images/Logo.png") ) );
    setWindowIcon( QIcon( QString(":/icons/belenus.ico") ) );

    if( isRegKeyExists( "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Belenus" ) )
        m_pInstallType = rbUpdate;
    else
        m_pInstallType = rbInstall;
}
//=======================================================================================
dlgMain::~dlgMain()
//=======================================================================================
{
}
//=======================================================================================
void dlgMain::on_pbNext_clicked()
//=======================================================================================
{
    if( pagerControl->currentIndex() < pagerControl->count()-1 )
        pagerControl->setCurrentIndex( pagerControl->currentIndex()+1 );

    if( pagerControl->currentIndex() > 0 )
        pbPrev->setEnabled( true );
    if( pagerControl->currentIndex() == pagerControl->count()-1 )
        pbNext->setEnabled( false );

    processPage( pagerControl->currentIndex() );
}
//=======================================================================================
void dlgMain::on_pbPrev_clicked()
//=======================================================================================
{
    if( pagerControl->currentIndex() > 0 )
        pagerControl->setCurrentIndex( pagerControl->currentIndex()-1 );

    if( pagerControl->currentIndex() == 0 )
        pbPrev->setEnabled( false );
    if( pagerControl->currentIndex() < pagerControl->count()-1 )
        pbNext->setEnabled( true );

    processPage( pagerControl->currentIndex() );
}
//=======================================================================================
void dlgMain::on_pbCancel_clicked()
//=======================================================================================
{
    if( QMessageBox::question( this, tr("Question"),
                               tr("Are you sure you want to abort installation?"),
                               QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes )
    {
        QDialog::reject();
    }
}
//=======================================================================================
void dlgMain::on_rbInstall_clicked()
//=======================================================================================
{
    m_pInstallType = rbInstall;
}
//=======================================================================================
void dlgMain::on_rbUpdate_clicked()
//=======================================================================================
{
    m_pInstallType = rbUpdate;
}
//=======================================================================================

void dlgMain::on_rbRemove_clicked()
//=======================================================================================
{
    m_pInstallType = rbRemove;
}
//=======================================================================================
void dlgMain::processPage( int p_nPage )
//=======================================================================================
{
//    QMessageBox::information( this, "", QString::number(p_nPage) );
    switch( p_nPage )
    {
        //-------------------------------------------------------------------------------
        case 1:  // Process selection
        //-------------------------------------------------------------------------------
        {
            if( isRegKeyExists( "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Belenus" ) )
            {
                rbInstall->setEnabled( false );
                rbUpdate->setEnabled( true );
                rbRemove->setEnabled( true );
            }
            else
            {
                rbInstall->setEnabled( true );
                rbUpdate->setEnabled( false );
                rbRemove->setEnabled( false );
                rbInstall->setChecked( true );
            }
            m_pInstallType->setChecked( true );
            break;
        }
        //-------------------------------------------------------------------------------
        case 10: // Installation
        //-------------------------------------------------------------------------------
        {
            m_obFile = new QFile( QString("C:\\Program Files\\Belenus\\Kliens\\belenus.exe") );
            m_obFile->link( QString("C:\\Development\\Qt\\belenus.lnk") );
            delete m_obFile;
            break;
        }
    }
}
//=======================================================================================
void dlgMain::on_pbStartInstall_clicked()
//=======================================================================================
{

}
//=======================================================================================
bool dlgMain::isRegKeyExists( QString p_qsKeyName )
//=======================================================================================
{
    bool        bRet = false;
    VRegistry   obReg;

    if( obReg.OpenKey( HKEY_LOCAL_MACHINE, p_qsKeyName ) )
    {
        bRet = true;
        obReg.CloseKey();
    }

    return bRet;
}
//=======================================================================================
