//====================================================================================
//
// Belenus Kliens alkalmazas © Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : wndmain.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Ballok Peter, Bikfalvi Tamas
//
//====================================================================================
// Alkalmazas fo ablakat kezelo allomany.
//====================================================================================

#include <QMessageBox>

#include "wndmain.h"

//====================================================================================

#include "crud/dlgpaneltypes.h"
#include "crud/dlgpatientorigin.h"
#include "crud/dlgreasontovisit.h"
#include "crud/dlgusers.h"
#include "crud/dlgpatient.h"
#include "crud/dlgattendance.h"

//====================================================================================

#include "edit/dlguseredit.h"
#include "edit/dlgpatientedit.h"

//====================================================================================

#include "dlglogin.h"
#include "dlgpreferences.h"
#include "dlghardwaretest.h"
#include "dlglogs.h"

//====================================================================================
cWndMain::cWndMain( QWidget *parent )
    : QMainWindow( parent )
{
    cTracer obTrace( "cWndMain::cWndMain" );

    setupUi( this );

    mdiPanels = new cMdiPanels( centralwidget );
    verticalLayout->addWidget(mdiPanels);

    mdiPanels->setBackground( QBrush( QColor( g_poPrefs->getMainBackground() ) ) );

    updateTitle();
    setWindowIcon( QIcon("./resources/belenus.ico") );

    action_LogOut->setIcon( QIcon("./resources/40x40_lock.gif") );
    action_Exit->setIcon( QIcon("./resources/40x40_shutdown.gif") );

    action_Patients->setIcon( QIcon("./resources/40x40_patient.gif") );
    //--------------------------------------------------------------------------------
    menuAdministrator->setIcon( QIcon("./resources/40x40_key.gif") );
        action_Users->setIcon( QIcon("./resources/40x40_user.gif") );
        action_Attendances->setIcon( QIcon("./resources/40x40_attendance.gif") );
        //----------------------------------------------------------------------------
        action_Patientorigin->setIcon( QIcon("./resources/40x40_patientorigin.gif") );
        action_ReasonToVisit->setIcon( QIcon("./resources/40x40_reasontovisit.gif") );
        //----------------------------------------------------------------------------
        action_Paneltypes->setIcon( QIcon("./resources/40x40_panel.gif") );
    //--------------------------------------------------------------------------------
    action_Preferences->setIcon( QIcon("./resources/40x40_settings.gif") );

    action_PatientNew->setIcon( QIcon("./resources/40x40_patientnew.gif") );

    actionDeviceStart->setIcon( QIcon( "./resources/40x40_sensolite_start.gif" ) );
    actionDevicePause->setIcon( QIcon( "./resources/40x40_sensolite_pause.gif" ) );

    connect( mdiPanels, SIGNAL( activePanelChanged( bool ) ), this, SLOT( refreshPanelButtons( bool ) ) );
}
//====================================================================================
cWndMain::~cWndMain()
{
    cTracer obTrace( "cWndMain::~cWndMain" );
}
//====================================================================================
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
//====================================================================================
void cWndMain::initPanels()
{
    mdiPanels->initPanels();
}
//====================================================================================
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
//====================================================================================
void cWndMain::on_action_Preferences_triggered()
{
    cTracer obTrace( "cWndMain::on_action_Preferences_triggered" );

    cDlgPreferences  obDlgPrefs( this );

    if( obDlgPrefs.exec() == QDialog::Accepted )
    {
        mdiPanels->hide();
        mdiPanels->placeSubWindows();
        mdiPanels->setBackground( QBrush( QColor( g_poPrefs->getMainBackground() ) ) );
        mdiPanels->show();
    }
}
//====================================================================================
void cWndMain::on_action_Users_triggered()
{
    cTracer obTrace( "cWndMain::on_action_Users_triggered" );

    cDlgUsers  obDlgUsers( this );

    obDlgUsers.exec();

    updateTitle();  //needed in case the login or real name of current user changed
}
//====================================================================================
void cWndMain::on_action_Logs_triggered()
{
    cTracer obTrace( "cWndMain::on_action_Logs_triggered" );

    cDlgLogs  obDlgLogs( this );

    obDlgLogs.exec();
}
//====================================================================================
void cWndMain::on_action_Hardwaretest_triggered()
{
    cTracer obTrace( "cWndMain::on_action_Hardwaretest_triggered" );

    cDlgHardwareTest  obDlgHardwareTest( this );

    obDlgHardwareTest.exec();
}
//====================================================================================
void cWndMain::on_action_LogOut_triggered()
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
//====================================================================================
void cWndMain::on_action_Paneltypes_triggered()
{
    cTracer obTrace( "cWndMain::on_action_Paneltypes_triggered" );

    cDlgPanelTypes  obDlgPanelTypes( this );

    obDlgPanelTypes.exec();
}
//====================================================================================
void cWndMain::on_action_Patientorigin_triggered()
{
    cTracer obTrace( "cWndMain::on_action_Patientorigin_triggered" );

    cDlgPatientOrigin  obDlgPatientOrigin( this );

    obDlgPatientOrigin.exec();
}
//====================================================================================
void cWndMain::on_action_ReasonToVisit_triggered()
{
    cTracer obTrace( "cWndMain::on_action_ReasonToVisit_triggered" );

    cDlgReasonToVisit  obDlgReasonToVisit( this );

    obDlgReasonToVisit.exec();
}
//====================================================================================
void cWndMain::on_action_Patients_triggered()
{
    cTracer obTrace( "cWndMain::on_action_Patiens_triggered" );

    cDlgPatient  obDlgPatient( this );

    obDlgPatient.exec();
}
//====================================================================================
void cWndMain::on_action_PatientNew_triggered()
{
    cTracer obTrace( "cWndMain::on_action_PatienNew_triggered" );

    cDBPatient *poPatient = new cDBPatient;
    poPatient->createNew();

    cDlgPatientEdit  obDlgEdit( this, poPatient );
    obDlgEdit.setWindowTitle( tr( "New Patient" ) );
    obDlgEdit.exec();

    delete poPatient;
}
//====================================================================================
void cWndMain::on_action_Attendances_triggered()
{
    cTracer obTrace( "cWndMain::on_action_Attendances_triggered" );

    cDlgAttendance  obDlgAttendance( this );

    obDlgAttendance.exec();
}
//====================================================================================
void cWndMain::on_actionDeviceStart_triggered()
{
    mdiPanels->start();
    refreshPanelButtons( true );
}
//====================================================================================
void cWndMain::on_actionDevicePause_triggered()
{
    mdiPanels->pause();
    refreshPanelButtons( false );
}
//====================================================================================
void cWndMain::on_actionDeviceReset_triggered()
{
    mdiPanels->reset();
    refreshPanelButtons( false );
}
//====================================================================================
void cWndMain::refreshPanelButtons( bool p_boPanelWorking )
{
    actionDeviceStart->setEnabled( !p_boPanelWorking );
    actionDevicePause->setEnabled( p_boPanelWorking );
    actionDeviceReset->setEnabled( !p_boPanelWorking );
}
