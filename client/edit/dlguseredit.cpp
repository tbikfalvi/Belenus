#include <QPushButton>
#include <QMessageBox>

#include "dlguseredit.h"
#include "../dlg/dlgpwdconfirm.h"

cDlgUserEdit::cDlgUserEdit( QWidget *p_poParent, cDBUser *p_poUser )
    : QDialog( p_poParent )
{
    setupUi( this );

    setWindowTitle( tr( "User" ) );
    setWindowIcon( QIcon("./resources/40x40_user.png") );

    QPushButton  *poBtnSave = new QPushButton( tr( "&Save" ) );
    QPushButton  *poBtnCancel = new QPushButton( tr( "&Cancel" ) );
    btbButtons->addButton( poBtnSave, QDialogButtonBox::AcceptRole );
    btbButtons->addButton( poBtnCancel, QDialogButtonBox::RejectRole );
    poBtnSave->setIcon( QIcon("./resources/40x40_ok.png") );
    poBtnCancel->setIcon( QIcon("./resources/40x40_cancel.png") );

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
        ledName->setText( m_poUser->name() );
        ledRealName->setText( m_poUser->realName() );
        for( int i = cAccessGroup::MIN + 1; i < cAccessGroup::MAX; i++ )
            if( g_obUser.isInGroup( (cAccessGroup::teAccessGroup)i ) ) cmbGroup->addItem( cAccessGroup::toStr( (cAccessGroup::teAccessGroup)i ) );
        cmbGroup->setCurrentIndex( (int)p_poUser->group() - 1 );
        chbActive->setChecked( m_poUser->active() );
        pteComment->setPlainText( m_poUser->comment() );

        if( m_poUser->licenceId() == 0 && m_poUser->id() > 0 )
            checkIndependent->setChecked( true );

        if( !g_obUser.isInGroup( cAccessGroup::ROOT ) && !g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
        {
            checkIndependent->setEnabled( false );
            if( m_poUser->licenceId() == 0 && m_poUser->id() > 0 )
            {
                ledName->setEnabled( false );
                ledPwd->setEnabled( false );
                ledRePwd->setEnabled( false );
                ledRealName->setEnabled( false );
                cmbGroup->setEnabled( false );
                chbActive->setEnabled( false );
                pteComment->setEnabled( false );
                poBtnSave->setEnabled( false );
            }
        }
    }

    if( !(g_obUser.isInGroup( cAccessGroup::ADMIN )) )
    {
        chbActive->setEnabled( false );
    }
}

cDlgUserEdit::~cDlgUserEdit()
{
}

void cDlgUserEdit::accept ()
{
    if( ledPwd->text() == ledRePwd->text() )
    {
        bool  boCanBeSaved = true;
        if( (ledPwd->text() == "") && !(g_obUser.isInGroup( cAccessGroup::ADMIN )) )
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
                if( checkIndependent->isChecked() )
                {
                    m_poUser->setLicenceId( 0 );
                }
                else
                {
                    m_poUser->setLicenceId( g_poPrefs->getLicenceId() );
                }
                m_poUser->setName( ledName->text() );
                if( ledPwd->text() != m_qsDefaultPwd )
                {
                    QByteArray  obPwdHash = QCryptographicHash::hash( ledPwd->text().toAscii(), QCryptographicHash::Sha1 );
                    m_poUser->setPassword( QString( obPwdHash.toHex() ) );
                }
                m_poUser->setRealName( ledRealName->text() );
                m_poUser->setGroup( (cAccessGroup::teAccessGroup)(cmbGroup->currentIndex() + 1) );
                m_poUser->setActive( chbActive->isChecked() );
                m_poUser->setComment( pteComment->toPlainText() );
                m_poUser->save();
            }
            catch( cSevException &e )
            {
                g_obLogger(e.severity()) << e.what() << EOM;
            }

            QDialog::accept();
        }
    }
    else
    {
        QMessageBox::critical( this, tr( "Error" ), tr( "Values of the Password and Retype Password fields are not the same" ) );
    }
}
