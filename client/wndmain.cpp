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

cWndMain::cWndMain( QWidget *parent )
    : QMainWindow( parent )
{
    cTracer obTrace( "cWndMain::cWndMain" );

    setupUi( this );

/*    g_poHardware = new CS_Hardware();

    if( g_poHardware != NULL )
    {
        // Elérhetõ COM port-ok lekérdezése
        if( g_poHardware->SP_GetCountAvailablePorts() > 0 )
        {
            //bool    bComPortFound = false;

            // Beállításokból COM port összehasonlítása az elérhetõ COM port-okkal __TO_BE_RESOLVED__
            for( int i=0;i<g_poHardware->SP_GetCountAvailablePorts(); i++ )
            {
                // COM PORT KIOLVASASA A SETTINGS-BOL __TO_BE_RESOLVED__
                // addig is az elso elerheto com portra csatlakozunk
            }
            g_poHardware->SP_InitCommunication( g_poHardware->SP_GetComPort( 0 ) );
            if( g_poHardware->SP_Open() )
            {
               if( g_poHardware->IsHardwareConnected() )
               {
                  // PANELOK SZAMANAK KIOLVASASA A SETTINGS-BOL __TO_BE_RESOLVED__

                  // addig a panelok szama 4
                  g_poHardware->SetApplicationModuleCount( 4 );

                  // TIMER INDITASA g_poHardware->HW_Kezel(); fuggvennyel
               }
            }
        }
    }
    else
    {
       // NAGY GAZ VAN __TO_BE_RESOLVED__
    }
*/
    updateTitle();
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

    int inPanelCount = g_poPrefs->getPanelCount();
    if( g_poHardware->getPanelCount() < inPanelCount )
    {
        QMessageBox::warning( this, "Panel count mismatch", QString( "There are more Panels defined in the database than supported by the current hardware. Only %1 panels will be displayed." ).arg( g_poHardware->getPanelCount() ) );
        inPanelCount = g_poHardware->getPanelCount();
    }

    QGraphicsScene *poScene = new QGraphicsScene;
    cDlgPanel       obPanel;

    QGraphicsProxyWidget *poPanelProxy = poScene->addWidget( &obPanel );

    grvMain->setScene( poScene );

    obPanel.show();
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
