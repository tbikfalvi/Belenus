#include <QMessageBox>

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
#include "crud/dlgpaneltypes.h"

cWndMain::cWndMain( QWidget *parent )
    : QMainWindow( parent )
{
    cTracer obTrace( "cWndMain::cWndMain" );

    setupUi( this );

    mdiPanels = new cMdiPanels( centralwidget );
    verticalLayout->addWidget(mdiPanels);

    mdiPanels->setBackground( QBrush( Qt::black ) );

    updateTitle();

    actionLog_Out->setIcon( QIcon("./resources/40x40_logout.gif") );
    actionE_xit->setIcon( QIcon("./resources/40x40_shutdown.gif") );

    action_Patientcards->setIcon( QIcon("./resources/40x40_patientcard.gif") );
    action_Patientcardtypes->setIcon( QIcon("./resources/40x40_patientcards.gif") );
    action_Users->setIcon( QIcon("./resources/40x40_user.gif") );
    actionP_anel_types->setIcon( QIcon("./resources/40x40_panel.gif") );
    action_Preferences->setIcon( QIcon("./resources/40x40_settings.gif") );
}

cWndMain::~cWndMain()
{
    cTracer obTrace( "cWndMain::~cWndMain" );
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

void cWndMain::initPanels()
{
    mdiPanels->initPanels();
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

void cWndMain::on_action_Preferences_triggered()
{
    cTracer obTrace( "cWndMain::on_action_Preferences_triggered" );

    cDlgPreferences  obDlgPrefs( this );

    obDlgPrefs.exec();

    mdiPanels->placeSubWindows();
}

void cWndMain::on_action_Users_triggered()
{
    cTracer obTrace( "cWndMain::on_action_Users_triggered" );

    cDlgUsers  obDlgUsers( this );

    obDlgUsers.exec();

    updateTitle();  //needed in case the login or real name of current user changed
}

void cWndMain::on_action_Patientcardtypes_triggered()
{
    cTracer obTrace( "cWndMain::on_action_Usercardtypes_triggered" );

    cDlgUserCardTypes  obDlgUserCardTypes( this );

    obDlgUserCardTypes.exec();
}

void cWndMain::on_action_Patientcards_triggered()
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

void cWndMain::on_actionP_anel_types_triggered()
{
    cTracer obTrace( "cWndMain::on_actionP_anel_types_triggered" );

    cDlgPanelTypes  obDlgPanelTypes( this );

    obDlgPanelTypes.exec();
}
