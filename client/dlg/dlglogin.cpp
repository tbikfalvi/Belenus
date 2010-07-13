#include <QPushButton>
#include <QMessageBox>
#include <QCryptographicHash>
#include "dlglogin.h"
#include "belenus.h"

cDlgLogIn::cDlgLogIn( QWidget *p_poParent ) : QDialog( p_poParent )
{
    cTracer obTrace( "cDlgLogIn::cDlgLogIn" );

    setupUi( this );

    QPushButton *poBtnLogin = new QPushButton( tr( "&Log In" ), this );
    poBtnLogin->setIcon( QIcon("./resources/40x40_ok.gif") );
    btbButtons->addButton( poBtnLogin, QDialogButtonBox::AcceptRole );

    QPushButton *poBtnQuit  = new QPushButton( tr( "&Quit" ), this );
    poBtnQuit->setIcon( QIcon("./resources/40x40_shutdown.gif") );
    btbButtons->addButton( poBtnQuit, QDialogButtonBox::RejectRole );

    m_poModel = new cQTMySQLQueryModel( this );
    m_poModel->setQuery( "SELECT CONCAT(name,\" (\",realName,\")\") AS n FROM users WHERE active = 1 ORDER BY name" );
    cmbName->setModel( m_poModel );

    int inIdx = cmbName->findText( g_poPrefs->getLastUser() );
    if( inIdx != -1 ) cmbName->setCurrentIndex( inIdx );

    ledPassword->setFocus();
}

cDlgLogIn::~cDlgLogIn()
{
    delete m_poModel;
}

void cDlgLogIn::accept()
{
    cTracer obTrace( "cDlgLogIn::accept" );

    try
    {
        string  stName = cmbName->currentText().toStdString();
        stName = stName.substr( 0, stName.find( '(' ) - 1 );
        g_obUser.load( stName );
        QByteArray  obPwdHash = QCryptographicHash::hash( ledPassword->text().toAscii(), QCryptographicHash::Sha1 );
        g_obUser.logIn( QString( obPwdHash.toHex() ).toStdString() );

        g_poPrefs->setLastUser( cmbName->currentText(), true );

        QDialog::accept();
    }
    catch( cSevException &e )
    {
        g_obLogger << cSeverity::INFO;
        g_obLogger << "User " << cmbName->currentText() << " failed to log in";
        g_obLogger << cQTLogger::EOM;

        g_obUser.logOut();
        QMessageBox::critical( this, tr( "Login failed" ),
                               tr( "Incorrect User Name and/or Password. Please try again." ) );
    }
}

void cDlgLogIn::reject()
{
    cTracer obTrace( "cDlgLogIn::reject" );

    QDialog::reject();
}
