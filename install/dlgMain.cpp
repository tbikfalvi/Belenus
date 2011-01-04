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

#include "vregistry.h"

//====================================================================================

#include "dlgMain.h"
#include "ui_dlgMain.h"

using namespace voidrealms::win32;

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
//    QMessageBox::information( this, "", QString::number(p_nPage) );
    switch( p_nPage )
    {
        case 1:  // Process selection
        {
            QSettings   m_obSettings;
/*                    ("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Belenus",
                                     QSettings::NativeFormat);*/
            //QString qsVersion = m_obSettings.value("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Belenus\\DisplayVersion","invalid").toString();
            if( isRegKeyExists( "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall",
                                "Belenus") )
                QMessageBox::information( this, "Belenus", "van" );
            else
                QMessageBox::information( this, "Belenus", "nincs" );
            break;
        }
        case 10:
        {
            m_obFile = new QFile( QString("C:\\Program Files\\Belenus\\Kliens\\belenus.exe") );
            m_obFile->link( QString("C:\\Development\\Qt\\belenus.lnk") );
            delete m_obFile;
            break;
        }
    }
}
//====================================================================================
void dlgMain::on_pbStartInstall_clicked()
//====================================================================================
{

}
//====================================================================================
bool dlgMain::isRegKeyExists( QString p_qsKeyPath, QString p_qsKeyName )
//====================================================================================
{
    bool        bRet = false;
    QString     qsKey = QString( "%1\\%2" ).arg(p_qsKeyPath).arg(p_qsKeyName);
    VRegistry   obReg;

    if( obReg.OpenKey(HKEY_LOCAL_MACHINE,qsKey) )
    {
        bRet = true;
        obReg.CloseKey();
    }

    return bRet;
    /*
    HKEY hKey;
    QString szSubKey = "Control Panel\\Appearance";
    QString szCurrent = "Current";
    DWORD dwSize = 256;
    TCHAR keyValue[256];
    if( RegOpenKeyEx( HKEY_LOCAL_MACHINE, szSubKey.ucs2(), 0, KEY_ALL_ACCESS,&hKey)!= ERROR_SUCCESS)
    {
    // Can't find it
    }
    RegQueryValueEx(hKey, szCurrent.ucs2(), NULL, NULL, (LPBYTE)keyValue, &dwSize);

    RegCloseKey(hKey);
    */
}
//====================================================================================
