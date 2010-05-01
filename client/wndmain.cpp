#include <QMessageBox>
#include <QMdiSubWindow>

#include "wndmain.h"
#include "dlglogin.h"
#include "dlgpreferences.h"
#include "crud/dlgusers.h"
#include "dlguseredit.h"
#include "dlglogs.h"
#include "crud/dlgusercardtypes.h"
#include "crud/dlgusercards.h"
#include "dlghardwaretest.h"
#include "crud/dlgproducttypes.h"
#include "crud/dlgproducts.h"

cWndMain::cWndMain( QWidget *parent )
    : QMainWindow( parent )
{
    cTracer obTrace( "cWndMain::cWndMain" );

    setupUi( this );

    updateTitle();
}

cWndMain::~cWndMain()
{
    cTracer obTrace( "cWndMain::~cWndMain" );

    for( unsigned int i = 0; i < m_obPanels.size(); i++ ) delete m_obPanels.at( i );
}

bool cWndMain::showLogIn()
{
    cTracer obTrace( "cWndMain::showLogIn" );

    cDlgLogIn  obDlgLogIn( this );
    bool       boLogInOK = ( obDlgLogIn.exec() == QDialog::Accepted );

    if ( boLogInOK )
    {
        g_obLogger.setAppUser( g_obUser.id() );
        g_obLogger << cSeverity::INFO;
        g_obLogger << "User " << g_obUser.name() << " (" << g_obUser.realName() << ") logged in";
        g_obLogger << cQTLogger::EOM;

        if( g_obUser.password() == "da39a3ee5e6b4b0d3255bfef95601890afd80709" ) //password is an empty string
        {
            QMessageBox::warning( this, tr( "Password" ),
                                  tr( "Your password is empty. Please change it to a valid password." ) );

            cDlgUserEdit  obDlgEdit( this, &g_obUser );
            obDlgEdit.setWindowTitle( QString::fromStdString( g_obUser.realName() ) );
            obDlgEdit.exec();
        }

        updateTitle();
    }

    obTrace << boLogInOK;
    return boLogInOK;
}

void cWndMain::updateTitle()
{
    cTracer obTrace( "cWndMain::updateTitle" );

    QString  qsTitle = tr( "Belenus " );
    qsTitle += g_poPrefs->getVersion();

    qsTitle += " - ";
    qsTitle += QString::fromStdString( g_poHardware->getCustomCaption() );

    if( g_obUser.isLoggedIn() )
    {
        qsTitle += " - ";
        qsTitle += QString::fromStdString( g_obUser.realName() );
        qsTitle += " [";
        qsTitle += QString::fromStdString( g_obUser.groups() );
        qsTitle += "]";
    }

    setWindowTitle( qsTitle );
}

void cWndMain::initPanels()
{
    cTracer obTrace( "cWndMain::initPanels" );

    int        inPanelCount = g_poPrefs->getPanelCount();

    if( g_poHardware->getPanelCount() < inPanelCount )
    {
        QMessageBox::warning( this, "Panel count mismatch", QString( "There are more Panels defined in the database than supported by the current hardware. Only %1 panels will be displayed." ).arg( g_poHardware->getPanelCount() ) );
        inPanelCount = g_poHardware->getPanelCount();
    }

    m_obPanels.reserve( g_poPrefs->getPanelCount() );

    QFrame *poFrame;
    QMdiSubWindow *poPanel;
    for( int i = 0; i < inPanelCount; i++ )
    {
        poFrame = new QFrame();
        poFrame->setFrameShape( QFrame::StyledPanel);
        poFrame->setFrameShadow( QFrame::Plain );

        poPanel = new QMdiSubWindow( 0, Qt::FramelessWindowHint );
        poPanel->setWidget( poFrame );

        m_obPanels.push_back( poFrame );
        mdiPanels->addSubWindow( poPanel );
        m_obPanels.at( i )->show();
    }

    mdiPanels->tileSubWindows();
}

void cWndMain::on_action_Preferences_triggered()
{
    cTracer obTrace( "cWndMain::on_action_Preferences_triggered" );

    cDlgPreferences  obDlgPrefs( this );

    obDlgPrefs.exec();
}

void cWndMain::on_action_Users_triggered()
{
    cTracer obTrace( "cWndMain::on_action_Users_triggered" );

    cDlgUsers  obDlgUsers( this );

    obDlgUsers.exec();

    updateTitle();  //needed in case the login or real name of current user changed
}

void cWndMain::on_action_Usercardtypes_triggered()
{
    cTracer obTrace( "cWndMain::on_action_Usercardtypes_triggered" );

    cDlgUserCardTypes  obDlgUserCardTypes( this );

    obDlgUserCardTypes.exec();
}

void cWndMain::on_action_Usercards_triggered()
{
    cTracer obTrace( "cWndMain::on_action_Usercards_triggered" );

    cDlgUserCards  obDlgUserCards( this );

    obDlgUserCards.exec();
}

void cWndMain::on_action_Producttypes_triggered()
{
    cTracer obTrace( "cWndMain::on_action_Producttypes_triggered" );

    cDlgProductTypes  obDlgProductTypes( this );

    obDlgProductTypes.exec();
}

void cWndMain::on_action_Products_triggered()
{
    cTracer obTrace( "cWndMain::on_action_Products_triggered" );

    cDlgProducts  obDlgProducts( this );

    obDlgProducts.exec();
}

void cWndMain::on_action_Logs_triggered()
{
    cTracer obTrace( "cWndMain::on_action_Logs_triggered" );

    cDlgLogs  obDlgLogs( this );

    obDlgLogs.exec();
}

void cWndMain::on_actionHardware_test_triggered()
{
    cTracer obTrace( "cWndMain::on_actionHardware_test_triggered" );

    cDlgHardwareTest  obDlgHardwareTest( this );

    obDlgHardwareTest.exec();
}

void cWndMain::on_actionLog_Out_triggered()
{
    cTracer obTrace( "cWndMain::on_action_Log_Out_triggered" );

    g_obLogger << cSeverity::INFO;
    g_obLogger << "User " << g_obUser.name() << " (" << g_obUser.realName() << ") logged out";
    g_obLogger << cQTLogger::EOM;

    g_obUser.logOut();
    g_obLogger.setAppUser( 0 );
    updateTitle();

    if( !showLogIn() ) close();
}
