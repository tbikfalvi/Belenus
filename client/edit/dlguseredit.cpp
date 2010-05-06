#include <QPushButton>
#include <QMessageBox>

#include "dlguseredit.h"
#include "dlgpwdconfirm.h"

cDlgUserEdit::cDlgUserEdit( QWidget *p_poParent, cDBUser *p_poUser )
    : QDialog( p_poParent )
{
    setupUi( this );

    m_qsDefaultPwd = "pppppppp";

    m_poUser = p_poUser;
    if( m_poUser )
    {
        unsigned int uiUserId = m_poUser->id();
        if( uiUserId )
        {
            lblUserIdValue->setText( QString::number( uiUserId ) );
            if( m_poUser->password() != "da39a3ee5e6b4b0d3255bfef95601890afd80709" )  //if password is not empty
            {
                ledPwd->setText( m_qsDefaultPwd );
                ledRePwd->setText( m_qsDefaultPwd );
            }
        }
        else
        {
            lblUserIdValue->setText( "?" );
        }
        ledName->setText( QString::fromStdString( m_poUser->name() ) );
        ledRealName->setText( QString::fromStdString( m_poUser->realName() ) );
        ledGroups->setText( QString::fromStdString( m_poUser->groups() ) );
        chbActive->setChecked( m_poUser->active() );
        pteComment->setPlainText( QString::fromStdString( m_poUser->comment() ) );
    }

    if( !(g_obUser.isInGroup( "admin" )) )
    {
        ledGroups->setEnabled( false );
        chbActive->setEnabled( false );
    }

    QPushButton  *poBtnSave = new QPushButton( tr( "&Save" ) );
    QPushButton  *poBtnCancel = new QPushButton( tr( "&Cancel" ) );
    btbButtons->addButton( poBtnSave, QDialogButtonBox::AcceptRole );
    btbButtons->addButton( poBtnCancel, QDialogButtonBox::RejectRole );
}

cDlgUserEdit::~cDlgUserEdit()
{
}

void cDlgUserEdit::accept ()
{
    if( ledPwd->text() == ledRePwd->text() )
    {
        bool  boCanBeSaved = true;
        if( (ledPwd->text() == "") && !(g_obUser.isInGroup( "admin" )) )
        {
            boCanBeSaved = false;
            QMessageBox::critical( this, tr( "Error" ), tr( "Password cannot be empty." ) );
        }
        if( boCanBeSaved && ledPwd->text() != m_qsDefaultPwd )
        {
            cDlgPwdConfirm  obConfirm( this );
            boCanBeSaved = (obConfirm.exec() == QDialog::Accepted );
        }

        if( boCanBeSaved )
        {
            try
            {
                m_poUser->setName( ledName->text().toStdString() );
                if( ledPwd->text() != m_qsDefaultPwd )
                {
                    QByteArray  obPwdHash = QCryptographicHash::hash( ledPwd->text().toAscii(), QCryptographicHash::Sha1 );
                    m_poUser->setPassword( QString( obPwdHash.toHex() ).toStdString() );
                }
                m_poUser->setRealName( ledRealName->text().toStdString() );
                m_poUser->setGroups( ledGroups->text().toStdString() );
                m_poUser->setActive( chbActive->isChecked() );
                m_poUser->setComment( pteComment->toPlainText().toStdString() );
                m_poUser->save();
            }
            catch( cSevException &e )
            {
                g_obLogger << e.severity();
                g_obLogger << e.what() << cQTLogger::EOM;
            }

            QDialog::accept();
        }
    }
    else
    {
        QMessageBox::critical( this, tr( "Error" ), tr( "Values of the Password and Retype Password fields are not the same" ) );
    }
}
