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
#include "crud/dlgpatientselect.h"
#include "crud/dlgpanelstatuses.h"
#include "crud/dlgpatientcardtype.h"
#include "crud/dlgpatientcard.h"

//====================================================================================

#include "edit/dlguseredit.h"
#include "edit/dlgpatientedit.h"
#include "edit/dlgattendanceedit.h"

//====================================================================================

#include "dlg/dlglogin.h"
#include "dlg/dlgpreferences.h"
#include "dlg/dlghardwaretest.h"
#include "dlg/dlglogs.h"
#include "dlg/dlginputstart.h"
#include "dlg/dlgpatientcardadd.h"

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

    actionPatientSelect->setIcon( QIcon("./resources/40x40_patient_select.gif") );
    actionPatientEmpty->setIcon( QIcon("./resources/40x40_patient_deselect.gif") );
    actionAttendanceNew->setIcon( QIcon("./resources/40x40_attendance.gif") );
    actionDeviceStart->setIcon( QIcon( "./resources/40x40_device_start.gif" ) );
    actionDeviceReset->setIcon( QIcon( "./resources/40x40_stop.gif" ) );
    actionDeviceSettings->setIcon( QIcon( "./resources/40x40_device_settings.gif" ) );
    action_UseWithCard->setIcon( QIcon( "./resources/40x40_device_withcard.gif" ) );
    action_UseByTime->setIcon( QIcon( "./resources/40x40_device_withtime.gif" ) );
    action_Cards->setIcon( QIcon( "./resources/40x40_patientcards.gif" ) );
    action_PanelStatuses->setIcon( QIcon( "./resources/40x40_device_settings.gif" ) );
    action_CardTypes->setIcon( QIcon( "./resources/40x40_patientcardtype.gif" ) );
    action_PCSaveToDatabase->setIcon( QIcon( "./resources/40x40_patientcardadd.gif" ) );
    action_Cassa->setIcon( QIcon( "./resources/40x40_cassa.gif" ) );
    action_Accounting->setIcon( QIcon( "./resources/40x40_book.gif" ) );

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
void cWndMain::keyPressEvent ( QKeyEvent *p_poEvent )
{
    if( (p_poEvent->key() >= Qt::Key_0 && p_poEvent->key() <= Qt::Key_9) ||
        (p_poEvent->key() >= Qt::Key_A && p_poEvent->key() <= Qt::Key_Z) )
    {
        cDlgInputStart  obDlgInputStart( this );

        obDlgInputStart.setInitialText( p_poEvent->text() );
        obDlgInputStart.exec();

        p_poEvent->ignore();
    }

    QMainWindow::keyPressEvent( p_poEvent );
}
//====================================================================================
void cWndMain::updateTitle()
{
    cTracer obTrace( "cWndMain::updateTitle" );

    QString  qsTitle = tr( "Belenus " );
    qsTitle += g_poPrefs->getVersion();

    qsTitle += " - ";
    qsTitle += QString::fromStdString( g_poHardware->getCustomCaption() );

    if( QString::fromStdString( g_poHardware->getCustomCaption() ).compare( "DEMO" ) == 0 )
    {
        action_Hardwaretest->setEnabled( false );
    }

    if( g_obUser.isLoggedIn() )
    {
        qsTitle += " - ";
        qsTitle += QString::fromStdString( g_obUser.realName() );
        qsTitle += " [";
        qsTitle += QString::fromStdString( g_obUser.groups() );
        qsTitle += "]";
    }

    actionPatientEmpty->setEnabled( g_obPatient.id()>0 );
    actionAttendanceNew->setEnabled( g_obPatient.id()>0 );
    if( g_obPatient.id() > 0 )
    {
        qsTitle += " <=> Current patient: [";
        qsTitle += QString::fromStdString( g_obPatient.name() );
        qsTitle += "]";
    }
    else
    {
        qsTitle += " <=> NO PATIENT SELECTED";
    }

    action_Paneltypes->setEnabled( g_obUser.isInGroup( "system" ) );
    action_PanelStatuses->setEnabled( g_obUser.isInGroup( "system" ) );

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

    if( g_obUser.isInGroup( "system" ) )
    {
        cDlgHardwareTest  obDlgHardwareTest( this );

        obDlgHardwareTest.exec();
    }
    else
    {
        QMessageBox::warning( this, tr( "Information" ),
                              tr( "This area is restricted for system administrators only!" ) );
    }
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
void cWndMain::on_actionDeviceReset_triggered()
{
    mdiPanels->reset();
    refreshPanelButtons( false );
}
//====================================================================================
void cWndMain::refreshPanelButtons( bool p_boPanelWorking )
{
    actionDeviceStart->setEnabled( !p_boPanelWorking );
    actionDeviceReset->setEnabled( p_boPanelWorking );
}
//====================================================================================
void cWndMain::on_actionPatientSelect_triggered()
{
    cTracer obTrace( "cWndMain::on_actionPatientSelect_triggered" );

    cDlgPatientSelect  obDlgPatientSelect( this );

    obDlgPatientSelect.exec();
    updateTitle();
}
//====================================================================================
void cWndMain::on_actionPatientEmpty_triggered()
{
    cTracer obTrace( "cWndMain::on_actionPatientEmpty_triggered" );

    g_obPatient.createNew();
    updateTitle();
}
//====================================================================================
void cWndMain::on_actionAttendanceNew_triggered()
{
    cDBAttendance *poAttendance = new cDBAttendance;
    poAttendance->createNew();
    poAttendance->setPatientId( g_obPatient.id() );

    cDlgAttendanceEdit obDlgEdit( this, poAttendance );
    obDlgEdit.setWindowTitle( tr( "New Attendance" ) );
    obDlgEdit.exec();

    delete poAttendance;
}
//====================================================================================
void cWndMain::on_action_PanelStatuses_triggered()
{
    cDlgPanelStatuses   obDlgPanelStatuses( this );

    obDlgPanelStatuses.exec();
}
//====================================================================================
void cWndMain::on_action_UseWithCard_triggered()
{
    cDlgInputStart  obDlgInputStart( this );

    obDlgInputStart.exec();
}
//====================================================================================
void cWndMain::on_action_UseByTime_triggered()
{
    cDlgInputStart  obDlgInputStart( this );

    obDlgInputStart.exec();
}
//====================================================================================
void cWndMain::on_action_Cards_triggered()
{
    cDlgPatientCard obDlgPatientCard( this );

    obDlgPatientCard.exec();
}
//====================================================================================
void cWndMain::on_action_CardTypes_triggered()
{
    cDlgPatientCardType obDlgPatientCardType( this );

    obDlgPatientCardType.exec();
}
//====================================================================================
void cWndMain::on_action_PCSaveToDatabase_triggered()
{
    cDlgPatientCardAdd  obDlgPatientCardAdd( this );

    obDlgPatientCardAdd.exec();
}
//====================================================================================
void cWndMain::on_action_Cassa_triggered()
{
}
//====================================================================================
void cWndMain::on_action_Accounting_triggered()
{
}
//====================================================================================
