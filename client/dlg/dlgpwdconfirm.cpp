#include <QPushButton>
#include <QMessageBox>

#include "dlgpwdconfirm.h"

cDlgPwdConfirm::cDlgPwdConfirm( QWidget *p_poParent )
    : QDialog( p_poParent )
{
    setupUi( this );

    setWindowTitle( tr( "Password check" ) );
    setWindowIcon( QIcon("./resources/belenus.ico") );

    QPushButton  *poBtnOk = new QPushButton( tr( "&Ok" ) );
    QPushButton  *poBtnCancel = new QPushButton( tr( "&Cancel" ) );
    btbButtons->addButton( poBtnOk, QDialogButtonBox::AcceptRole );
    btbButtons->addButton( poBtnCancel, QDialogButtonBox::RejectRole );
}

cDlgPwdConfirm::~cDlgPwdConfirm()
{
}

void cDlgPwdConfirm::accept ()
{
    QByteArray  obPwdHash = QCryptographicHash::hash( ledPwd->text().toAscii(), QCryptographicHash::Sha1 );
    if( QString( obPwdHash.toHex() ) != g_obUser.password() )
        QMessageBox::critical( this, tr( "Error" ), tr( "Incorrect password" ) );
    else
        QDialog::accept();
}
