//====================================================================================
//
// Belenus Kliens alkalmazas (c) Pagony Multimedia Studio Bt - 2010
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
#include "../framework/logger/DatabaseWriter.h"

//====================================================================================

#include "db/dbpostponed.h"
#include "db/dbpatient.h"
#include "db/dbpatientcard.h"

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
#include "crud/dlgpostponedpatientselect.h"
#include "crud/dlgpanelsettings.h"
#include "crud/dlgpostponedattendanceselect.h"
#include "crud/dlgattendanceselect.h"

//====================================================================================

#include "edit/dlguseredit.h"
#include "edit/dlgpatientedit.h"
#include "edit/dlgattendanceedit.h"
#include "edit/dlgpatientcardedit.h"
#include "edit/dlgcassaedit.h"
#include "edit/dlgpatientcarduse.h"

//====================================================================================

#include "dlg/dlglogin.h"
#include "dlg/dlgpreferences.h"
#include "dlg/dlghardwaretest.h"
#include "dlg/dlglogs.h"
#include "dlg/dlginputstart.h"
#include "dlg/dlgpatientcardadd.h"
#include "dlg/dlgserialreg.h"


extern DatabaseWriter g_obLogDBWriter;


//====================================================================================
cWndMain::cWndMain( QWidget *parent )
    : QMainWindow( parent )
{
    cTracer obTrace( "cWndMain::cWndMain" );

    setupUi( this );

    m_uiPatientId           = 0;
    m_uiAttendanceId        = 0;
    g_uiPatientAttendanceId = 0;

    mdiPanels = new cMdiPanels( centralwidget );
    verticalLayout->addWidget(mdiPanels);

    mdiPanels->setBackground( QBrush( QColor( g_poPrefs->getMainBackground() ) ) );

    connect( mdiPanels, SIGNAL( activePanelChanged() ), this, SLOT( updateToolbar() ) );

    updateTitle();
    setWindowIcon( QIcon("./resources/belenus.ico") );

    action_LogOut->setIcon( QIcon("./resources/40x40_lock.gif") );
    action_Exit->setIcon( QIcon("./resources/40x40_shutdown.gif") );
    action_Patients->setIcon( QIcon("./resources/40x40_patient.gif") );
    menuAdministrator->setIcon( QIcon("./resources/40x40_key.gif") );
    action_Users->setIcon( QIcon("./resources/40x40_user.gif") );
    action_Attendances->setIcon( QIcon("./resources/40x40_attendance.gif") );
    action_Patientorigin->setIcon( QIcon("./resources/40x40_patientorigin.gif") );
    action_ReasonToVisit->setIcon( QIcon("./resources/40x40_reasontovisit.gif") );
    action_Paneltypes->setIcon( QIcon("./resources/40x40_panel.gif") );
    action_Preferences->setIcon( QIcon("./resources/40x40_settings.gif") );
    action_PatientNew->setIcon( QIcon("./resources/40x40_patientnew.gif") );
    action_PatientSelect->setIcon( QIcon("./resources/40x40_patient_select.gif") );
    action_PatientEmpty->setIcon( QIcon("./resources/40x40_patient_deselect.gif") );
    action_AttendanceNew->setIcon( QIcon("./resources/40x40_attendance.gif") );
    action_DeviceStart->setIcon( QIcon( "./resources/40x40_device_start.gif" ) );
    action_DeviceReset->setIcon( QIcon( "./resources/40x40_stop.gif" ) );
    action_DeviceSettings->setIcon( QIcon( "./resources/40x40_device_settings.gif" ) );
    action_UseWithCard->setIcon( QIcon( "./resources/40x40_device_withcard.gif" ) );
    action_UseByTime->setIcon( QIcon( "./resources/40x40_device_withtime.gif" ) );
    action_Cards->setIcon( QIcon( "./resources/40x40_patientcards.gif" ) );
    action_PanelStatuses->setIcon( QIcon( "./resources/40x40_device_settings.gif" ) );
    action_CardTypes->setIcon( QIcon( "./resources/40x40_patientcardtype.gif" ) );
    action_PCSaveToDatabase->setIcon( QIcon( "./resources/40x40_patientcardadd.gif" ) );
    action_Cassa->setIcon( QIcon( "./resources/40x40_cassa.gif" ) );
    action_Accounting->setIcon( QIcon( "./resources/40x40_book.gif" ) );
    action_DeviceSkipStatus->setIcon( QIcon( "./resources/40x40_device_next.gif" ) );
    action_PatientNew->setIcon( QIcon("./resources/40x40_patient_new.gif") );
    action_PatientCardSell->setIcon( QIcon("./resources/40x40_patientcard_sell.gif") );
    action_DoctorSchedule->setIcon( QIcon("./resources/40x40_doctor_schedule.gif") );
    action_DeviceSchedule->setIcon( QIcon("./resources/40x40_device_schedule.gif") );
    action_PostponedPatient->setIcon( QIcon("./resources/40x40_patient_later.gif") );
    action_PostponedAttendance->setIcon( QIcon("./resources/40x40_attendance_later.gif") );
    action_ValidateSerialKey->setIcon( QIcon( "./resources/40x40_key.gif" ) );
    action_EditActualPatient->setIcon( QIcon("./resources/40x40_patientedit.gif") );
    action_SelectActualAttendance->setIcon( QIcon("./resources/40x40_attendance_select.gif") );
    action_DeselectActualAttendance->setIcon( QIcon("./resources/40x40_attendance_deselect.gif") );
    action_EditActualAttendance->setIcon( QIcon("./resources/40x40_attendance_edit.gif") );

    menuPatient->setIcon( QIcon("./resources/40x40_patient.gif") );
    menuAttendance->setIcon( QIcon("./resources/40x40_attendance.gif") );
    menuPatientCard->setIcon( QIcon("./resources/40x40_patientcard.gif") );
    menuDevice->setIcon( QIcon( "./resources/40x40_device.gif" ) );
    action_PCActivate->setIcon( QIcon("./resources/40x40_patientcard_sell.gif") );

    cDBPostponed    *poDBPostPoned = new cDBPostponed();

    g_poPrefs->setPostponedPatients( poDBPostPoned->countPostponedPatients() );
    g_poPrefs->setPostponedAttendances( poDBPostPoned->countPostponedAttendances() );

    delete poDBPostPoned;

    action_PatientEmpty->setEnabled( false );
    action_PatientEmpty->setVisible( false );
    action_EditActualPatient->setEnabled( false );
    action_PostponedPatient->setEnabled( false );
    action_SelectActualAttendance->setEnabled( false );
    action_DeselectActualAttendance->setEnabled( false );
    action_DeselectActualAttendance->setVisible( false );
    action_EditActualAttendance->setEnabled( false );
    action_AttendanceNew->setEnabled( false );
    action_PostponedAttendance->setEnabled( false );
    action_UseWithCard->setEnabled( false );
    action_UseByTime->setEnabled( false );
    action_DeviceStart->setEnabled( false );
    action_DeviceReset->setEnabled( false );
    action_DoctorSchedule->setEnabled( false );
    action_DeviceSchedule->setEnabled( false );
}
//====================================================================================
cWndMain::~cWndMain()
{
    cTracer obTrace( "cWndMain::~cWndMain" );

    killTimer( m_nTimer );
}
//====================================================================================
void cWndMain::startMainTimer()
{
    m_nTimer = startTimer( 300 );
}
//====================================================================================
bool cWndMain::showLogIn()
{
    cTracer obTrace( "cWndMain::showLogIn" );

    cDlgLogIn  obDlgLogIn( this );
    bool       boLogInOK = ( obDlgLogIn.exec() == QDialog::Accepted );

    if( boLogInOK )
    {
        g_obLogDBWriter.setAppUser( g_obUser.id() );
        g_obLogger(cSeverity::INFO) << "User " << g_obUser.name().c_str() << " (" << g_obUser.realName().c_str() << ") logged in" << EOM;

        if( g_obUser.password() == "da39a3ee5e6b4b0d3255bfef95601890afd80709" ) //password is an empty string
        {
            QMessageBox::warning( this, tr( "Password" ),
                                  tr( "Your password is empty. Please change it to a valid password." ) );

            cDlgUserEdit  obDlgEdit( this, &g_obUser );
            obDlgEdit.setWindowTitle( QString::fromStdString( g_obUser.realName() ) );
            obDlgEdit.exec();
        }

        updateTitle();

        checkDemoLicenceKey();
        loginUser();

    } // if ( boLogInOK )

    obTrace << boLogInOK;
    return boLogInOK;
}
//====================================================================================
void cWndMain::initPanels()
{
    mdiPanels->initPanels();
}
//====================================================================================
void cWndMain::checkDemoLicenceKey()
{
    if( g_poPrefs->getClientSerial().compare("BLNS_SERIAL_DEMO") == 0 &&
        QString::fromStdString( g_poHardware->getCustomCaption() ).compare( "DEMO" ) != 0 )
                                                           // GABOR : ezt allitsd at == -re, hogy tesztelni tudd
    {
        if( QMessageBox::warning( this,
                                  tr("Attention"),
                                  tr("The application has no valid serial key registered.\n"
                                     "The application will only control the hardware with DEMO serial key for 7 days.\n\n"
                                     "Do you want to enter a valid serial key and register the application?\n"
                                     "Please note you need live internet connection for the registration process."),
                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes ) == QMessageBox::Yes )
        {
            cDlgSerialReg   obDlgSerialReg( this );
            obDlgSerialReg.exec();
        }
    }
}
//====================================================================================
void cWndMain::loginUser()
{
    //----------------------------------------------
    // Penztar betoltese, ellenorzese
    //----------------------------------------------
    g_obCassa.init();

    // Penztar ellenorzese
    if( g_obCassa.isCassaExists() )
    { // Penztar rekord letezik, utolso bejegyzes betoltve

        // Penztar le van zarva?
        if( g_obCassa.isCassaClosed() )
        { // Penztar le van zarva

            // Ugyanaz zarta le az elozo penztarat, aki most be van jelentkezve?
            if( g_obUser.id() == g_obCassa.cassaOwner() )
            { // Ugyanaz van bejelentkezve

                // Uj nap kezdodott, vagy visszajelentkezett
                if( QMessageBox::question( this, tr("Question"),
                                           tr( "Do you want to continue the previous cassa record?" ),
                                           QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes )
                { // Elozo penztar folytatasa

                    g_obCassa.cassaReOpen();
                }
                else
                { // Uj penztar nyitasa

                    g_obCassa.createNew( g_obUser.id() );
                }
            } // Ugyanaz volt bejelentkezve
            else
            { // Mas jelentkezett be, uj penztar bejegyzes nyitasa

                g_obCassa.createNew( g_obUser.id() );
            } // Mas volt bejelentkezve
        } // Penztar le volt zarva
        else
        { // Penztar nincs lezarva

            // Ugyanaz van bejelentkezve, aki a penztart eddig hasznalta?
            if( g_obUser.id() != g_obCassa.cassaOwner() )
            { // Mas jelentkezett be

                // Be akarja zarni a korabbi penztarat es indit egy ujat?
                if( QMessageBox::warning( this, tr("Warning"),
                                          tr("The last cassa record is assigned to a different user.\n"
                                             "You are not able to start new cassa record assigned to you\n"
                                             "until the previous is still open.\n\n"
                                             "Do you want to close the previous cassa and start a new one?\n\n"
                                             "Please note if you don't open a cassa record assigned to you\n"
                                             "the application can not record any money related action."),
                                          QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes )
                { // Korabbi penztar bezarasa, uj nyitasa

                    g_obCassa.cassaClose();
                    g_obCassa.createNew( g_obUser.id() );
                } // Uj penztar nyitva
                else
                { // Korabbi penztar nem lesz lezarva, penztar szolgaltatasa letiltasa

                    g_obCassa.setDisabled();
                } // Penztar letiltva
            } // Mas volt bejelentkezve
            else
            { // Ugyanaz jelentkezett be

                g_obCassa.setEnabled();
            } // Ugyanaz jelentkezett be
        } // Penztar nem volt lezarva
    } // Penztar rekord letezett
    else
    { // Penztar rekord nem letezik

        if( QMessageBox::critical( this, tr("Question"),
                                   tr("There is no data recorded in database for cassa.\n\n"
                                      "Do you want to start cassa recording with the current user?\n\n"
                                      "Please note the application can not record any money related\n"
                                      "action without valid cassa data record.\n"
                                      "If you want to start cassa with different user, please log out\n"
                                      "and relogin with the desired user account."),
                                   QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes )
        { // Elso rekord letrehozasa

            g_obCassa.createNew( g_obUser.id() );
        } // Elso rekord letrehozva
        else
        { // Penztar szolgaltatas letiltasa

            g_obCassa.setDisabled();
        } // Penztar letiltva
    } // Penztar rekord nem letezett
}
//====================================================================================
void cWndMain::logoutUser()
{
    if( g_obCassa.isCassaEnabled() &&
        g_obCassa.cassaOwner() == g_obUser.id() )
    {
        if( QMessageBox::question( this, tr("Question"),
                                   tr("Do you want to close your cassa?"),
                                   QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes )
        {
            on_action_Cassa_triggered();
            g_obCassa.cassaClose();
        }
    }
}
//====================================================================================
void cWndMain::keyPressEvent ( QKeyEvent *p_poEvent )
{
    if( (p_poEvent->key() >= Qt::Key_0 && p_poEvent->key() <= Qt::Key_9) ||
        (p_poEvent->key() >= Qt::Key_A && p_poEvent->key() <= Qt::Key_Z) ||
        (p_poEvent->key() == Qt::Key_Space) )
    {
        cDlgInputStart  obDlgInputStart( this );

        obDlgInputStart.setInitialText( p_poEvent->text() );
        if( obDlgInputStart.exec() == QDialog::Accepted )
        {
            if( obDlgInputStart.m_bPat )
            {
                processInputPatient( obDlgInputStart.getEditText().trimmed() );
            }
            else if( obDlgInputStart.m_bCard )
            {
                processInputPatientCard( obDlgInputStart.getEditText() );
            }
            else if( obDlgInputStart.m_bTime )
            {
                processInputTimePeriod( obDlgInputStart.getEditText().toInt() );
            }
        }

        p_poEvent->ignore();
    }
    else if( p_poEvent->key() == Qt::Key_Escape && !mdiPanels->isPanelWorking(mdiPanels->activePanel()) )
    {
        mdiPanels->reset();
    }

    QMainWindow::keyPressEvent( p_poEvent );
}
//====================================================================================
void cWndMain::updateTitle()
{
    //cTracer obTrace( "cWndMain::updateTitle" );

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
        qsTitle += cAccessGroup::toStr( g_obUser.group() );
        qsTitle += "]";
    }

    if( g_obPatient.id() > 0 )
    {
        qsTitle += tr(" <=> Current patient: [");
        qsTitle += QString::fromStdString( g_obPatient.name() );
        qsTitle += "]";
    }
    else
    {
        qsTitle += tr(" <=> NO PATIENT SELECTED");
    }

    action_Paneltypes->setEnabled( g_obUser.isInGroup( cAccessGroup::SYSTEM ) );
    action_PanelStatuses->setEnabled( g_obUser.isInGroup( cAccessGroup::ADMIN ) );

    setWindowTitle( qsTitle );
}
//====================================================================================
void cWndMain::updateToolbar()
{
    action_Exit->setEnabled( !mdiPanels->isPanelWorking() );

    action_PatientSelect->setEnabled( !(g_obPatient.id()>0) );
    action_PatientSelect->setVisible( !(g_obPatient.id()>0) );
    action_PatientEmpty->setEnabled( g_obPatient.id()>0 );
    action_PatientEmpty->setVisible( g_obPatient.id()>0 );
    action_EditActualPatient->setEnabled( g_obPatient.id()>0 );
    action_PostponedPatient->setEnabled( g_poPrefs->postponedPatients()>0 );

    action_SelectActualAttendance->setEnabled( g_uiPatientAttendanceId == 0 && g_obPatient.id()>0 );
    action_SelectActualAttendance->setVisible( g_uiPatientAttendanceId == 0 );
    action_DeselectActualAttendance->setEnabled( g_uiPatientAttendanceId > 0 );
    action_DeselectActualAttendance->setVisible( g_uiPatientAttendanceId > 0 );
    action_EditActualAttendance->setEnabled( g_uiPatientAttendanceId > 0 );

    action_AttendanceNew->setEnabled( g_obPatient.id()>0 );
    action_PostponedAttendance->setEnabled( g_poPrefs->postponedAttendances()>0 );

    action_UseWithCard->setEnabled( g_obPatient.id() > 0 &&
                                    g_uiPatientAttendanceId > 0 );
    action_UseByTime->setEnabled( g_obPatient.id() > 0 &&
                                  g_uiPatientAttendanceId > 0 );

    action_DeviceStart->setEnabled( !mdiPanels->isPanelWorking(mdiPanels->activePanel()) &&
                                    g_obPatient.id() > 0 &&
                                    g_uiPatientAttendanceId > 0 );
    action_DeviceSkipStatus->setEnabled( mdiPanels->isStatusCanBeSkipped( mdiPanels->activePanel()) );
    action_DeviceReset->setEnabled( mdiPanels->isPanelWorking(mdiPanels->activePanel()) );

    action_DeviceSettings->setEnabled( !mdiPanels->isPanelWorking(mdiPanels->activePanel()) );

    action_DoctorSchedule->setEnabled( false );
    action_DeviceSchedule->setEnabled( false );

    action_Cassa->setEnabled( g_obCassa.isCassaEnabled() );
}
//====================================================================================
void cWndMain::timerEvent(QTimerEvent *)
{
    bool    bUpdateTitle = false;

    updateToolbar();

    if( m_uiPatientId != g_obPatient.id() )
    {
        bUpdateTitle = true;

        m_uiPatientId = g_obPatient.id();

        if( m_uiPatientId )
        {
            switch( QMessageBox::question( this, tr("Question"),
                                       tr("Do you want to create new attendance for this patient?\n\n"
                                          "Press the Ignore button to select an existing attendance\n"
                                          "that saved previously for this patient."),
                                       QMessageBox::Yes, QMessageBox::No, QMessageBox::Ignore ) )
            {
                case QMessageBox::Yes:
                    on_action_AttendanceNew_triggered();
                    break;

                case QMessageBox::Ignore:
                    on_action_SelectActualAttendance_triggered();
                    break;
            }
        }
    }
    if( m_uiAttendanceId != g_uiPatientAttendanceId )
    {
        bUpdateTitle = true;

        m_uiAttendanceId = g_uiPatientAttendanceId;
    }
    if( bUpdateTitle )
    {
        updateTitle();
    }
}
//====================================================================================
void cWndMain::closeEvent( QCloseEvent *p_poEvent )
{
    if( mdiPanels->isPanelWorking() )
    {
        QMessageBox::warning( this, tr("Attention"),
                              tr("At least one Panel is still working.\n"
                                 "Please stop them before closing the application.") );
        p_poEvent->ignore();
    }
    else
    {
        logoutUser();

        if( QMessageBox::question( this, tr("Attention"),
                                   tr("Are you sure you want to close the application?"),
                                   QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
        {
            p_poEvent->accept();
        }
        else
        {
            p_poEvent->ignore();
        }
    }
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

    if( g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
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

    logoutUser();

    g_obLogger(cSeverity::INFO) << "User " << g_obUser.name().c_str() << " (" << g_obUser.realName().c_str() << ") logged out" << EOM;

    g_obUser.logOut();
    g_obLogDBWriter.setAppUser( 0 );
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

    if( poPatient->id() > 0 )
    {
        if( QMessageBox::question( this, tr("Question"),
                                   tr("Do you want to select the created patient as actual?"),
                                   QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
        {
            g_obPatient.load( poPatient->id() );
        }
    }

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
void cWndMain::on_action_DeviceStart_triggered()
{
    mdiPanels->start();

    on_action_PatientEmpty_triggered();
}
//====================================================================================
void cWndMain::on_action_DeviceReset_triggered()
{
    mdiPanels->reset();
}
//====================================================================================
void cWndMain::on_action_PatientSelect_triggered()
{
    cTracer obTrace( "cWndMain::on_action_PatientSelect_triggered" );

    cDlgPatientSelect  obDlgPatientSelect( this );

    obDlgPatientSelect.exec();
}
//====================================================================================
void cWndMain::on_action_PatientEmpty_triggered()
{
    cTracer obTrace( "cWndMain::on_action_PatientEmpty_triggered" );

    g_obPatient.createNew();
    g_uiPatientAttendanceId = 0;
}
//====================================================================================
void cWndMain::on_action_AttendanceNew_triggered()
{
    cDBAttendance *poAttendance = new cDBAttendance;
    poAttendance->createNew();
    poAttendance->setPatientId( g_obPatient.id() );

    cDlgAttendanceEdit obDlgEdit( this, poAttendance );
    obDlgEdit.setWindowTitle( tr( "New Attendance" ) );
    obDlgEdit.exec();

    if( poAttendance->id() )
    {
        if( QMessageBox::question( this, tr("Question"),
                                   tr("Do you want to select the created attendance as actual?"),
                                   QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
        {
            g_uiPatientAttendanceId = poAttendance->id();
        }
    }

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

    obDlgInputStart.m_bCard = true;
    obDlgInputStart.init();

    QString      qsBarcode = "";
    QSqlQuery   *poQuery;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT barcode FROM patientCards WHERE patientId=%1" ).arg(g_obPatient.id()) );
    if( poQuery->first() )
    {
        qsBarcode = poQuery->value(0).toString();

        if( QMessageBox::question( this, tr("Question"),
                                   tr("A patientcard with barcode [%1]\n"
                                      "attached to the actual patient.\n\n"
                                      "Do you want to use this patientcard?").arg(qsBarcode),
                                   QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
        {
            processInputPatientCard( qsBarcode );
        }
        else
        {
            if( obDlgInputStart.exec() == QDialog::Accepted )
                processInputPatientCard( obDlgInputStart.getEditText() );
        }
    }
    else
    {
        if( QMessageBox::question( this, tr("Question"),
                                   tr("There is no patientcard attached to the actual patient.\n"
                                      "Do you want to sell a patientcard for the actual patient?"),
                                   QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
        {
            on_action_PatientCardSell_triggered();
        }
        else
        {
            if( obDlgInputStart.exec() == QDialog::Accepted )
                processInputPatientCard( obDlgInputStart.getEditText() );
        }
    }
}
//====================================================================================
void cWndMain::on_action_UseByTime_triggered()
{
    cDlgInputStart  obDlgInputStart( this );

    obDlgInputStart.m_bTime = true;
    obDlgInputStart.init();
    if( obDlgInputStart.exec() == QDialog::Accepted )
        processInputTimePeriod( obDlgInputStart.getEditText().toInt() );
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
    cDlgCassaEdit   obDlgCassaEdit( this );

    obDlgCassaEdit.exec();
}
//====================================================================================
void cWndMain::on_action_Accounting_triggered()
{
}
//====================================================================================
void cWndMain::on_action_DeviceSkipStatus_triggered()
{
    mdiPanels->next();
}
//====================================================================================
void cWndMain::on_action_PostponedPatient_triggered()
{
    cTracer obTrace( "cWndMain::on_action_PostponedPatient_triggered" );

    cDlgPostponedPatientSelect  obDlgPostponedPatientSelect( this );

    obDlgPostponedPatientSelect.exec();
}
//====================================================================================
void cWndMain::on_action_PostponedAttendance_triggered()
{
    cDlgPostponedAttendanceSelect   obDlgSelect( this );

    obDlgSelect.exec();
}
//====================================================================================
void cWndMain::on_action_ValidateSerialKey_triggered()
{
    cDlgSerialReg   obDlgSerialReg( this );
    obDlgSerialReg.exec();
}
//====================================================================================
void cWndMain::on_action_PatientCardSell_triggered()
{
    cDlgInputStart  obDlgInputStart( this );

    obDlgInputStart.m_bCard = true;
    obDlgInputStart.init();
    if( obDlgInputStart.exec() == QDialog::Accepted )
    {
        cDBPatientCard  obDBPatientCard;
        try
        {
            obDBPatientCard.load( obDlgInputStart.getEditText() );
        }
        catch( cSevException &e )
        {
            if( QString(e.what()).compare("Patientcard barcode not found") != 0 )
            {
                g_obLogger(e.severity()) << e.what() << EOM;
            }
            else
            {
                if( QMessageBox::question( this, tr("Question"),
                                           tr("This barcode has not found in the database.\n"
                                              "Do you want to register it for a new patientcard?"),
                                           QMessageBox::Yes,QMessageBox::No ) == QMessageBox::Yes )
                {
                    obDBPatientCard.createNew();
                    obDBPatientCard.setBarcode( obDlgInputStart.getEditText() );
                    obDBPatientCard.save();
                }
            }
        }
        if( obDBPatientCard.active() != 0 && obDBPatientCard.units() > 0 )
        {
            QMessageBox::warning( this, tr("Attention"),
                                  tr("This patientcard already sold.\n"
                                     "Please select another inactive patientcard.") );
        }
        else
        {
            cDlgPatientCardEdit obDlgPatientCardEdit( this, &obDBPatientCard );

            obDlgPatientCardEdit.setPatientCardOwner( g_obPatient.id() );
            obDlgPatientCardEdit.activatePatientCard();
            obDlgPatientCardEdit.exec();
        }
    }
}
//====================================================================================
void cWndMain::processInputPatient( QString p_stPatientName )
{
    cDBPatient      obDBPatient;
    unsigned int    uiPatientCount = obDBPatient.getPatientCount(p_stPatientName.trimmed().toStdString());

    if( uiPatientCount > 1 )
    {
        cDlgPatientSelect  obDlgPatientSelect( this );
        obDlgPatientSelect.setSearchPatientName( p_stPatientName.trimmed() );
        obDlgPatientSelect.exec();
    }
    else if( uiPatientCount == 1 )
    {
        g_obPatient.load( obDBPatient.id() );
    }
    else
    {
        if( QMessageBox::question( this, tr("Question"),
                                   tr("There is no patient in the database with name like\n\n"
                                      "\'%1\'\n\n"
                                      "Do you want to create a new patient record with this name?").arg(p_stPatientName.trimmed()),
                                   QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
        {
            cDBPatient  obDBPatient;

            obDBPatient.createNew();
            obDBPatient.setName( p_stPatientName.toStdString() );

            cDlgPatientEdit  obDlgEdit( this, &obDBPatient );
            obDlgEdit.setWindowTitle( QString::fromStdString( obDBPatient.name() ) );
            obDlgEdit.exec();
        }
    }
}
//====================================================================================
void cWndMain::processInputPatientCard( QString p_stBarcode )
{
    cDBPatientCard  obDBPatientCard;

    try
    {
        obDBPatientCard.load( p_stBarcode );

        if( obDBPatientCard.active() )
        {
            if( obDBPatientCard.patientId() != g_obPatient.id() )
            {
                if( QMessageBox::question( this, tr("Question"),
                                           tr("This patientcard has been assigned to a different patient.\n"
                                              "Are you sure you want to use this patientcard?"),
                                           QMessageBox::Yes,QMessageBox::No ) == QMessageBox::No )
                {
                    return;
                }
            }
            cDlgPatientCardUse  obDlgPatientCardUse( this, &obDBPatientCard );

            if( obDlgPatientCardUse.exec() == QDialog::Accepted )
            {
                cDBAttendance   obDBAttendance;
                QTime           tCurrent;
                QTime           tLength;
                QTime           tNewLength;
                int             inNewLength;
                int             inUnits;
                QString         qsLength;

                obDlgPatientCardUse.getUseUnitsTime( &inUnits, &qsLength );

                obDBAttendance.load( g_uiPatientAttendanceId );
                tCurrent = QTime::fromString(obDBAttendance.length(),"hh:mm:ss");
                tLength  = QTime::fromString(qsLength,"mm:ss");
                inNewLength = tCurrent.minute()*60 + tCurrent.second() +
                              tLength.minute()*60 + tLength.second();
                tNewLength = QTime( 0, inNewLength/60, inNewLength%60, 0 );
                obDBAttendance.setLength( tNewLength.toString("hh:mm:ss") );
                obDBAttendance.save();
                mdiPanels->setMainProcessTime( obDBPatientCard.id(), inUnits, inNewLength );
            }
        }
        else
        {
            if( QMessageBox::question( this, tr("Question"),
                                       tr("This barcode has not been activated yet.\n"
                                          "Do you want to activate it now?"),
                                       QMessageBox::Yes,QMessageBox::No ) == QMessageBox::Yes )
            {
                cDlgPatientCardEdit     obDlgPatientCardEdit( this, &obDBPatientCard );

                obDlgPatientCardEdit.activatePatientCard();
                if( g_obPatient.id() > 0 )
                {
                    if( QMessageBox::question( this, tr("Question"),
                                               tr("Do you want to assign this patientcard to the actual patient?"),
                                               QMessageBox::Yes,QMessageBox::No ) == QMessageBox::Yes )
                    {
                        obDlgPatientCardEdit.setPatientCardOwner( g_obPatient.id() );
                    }
                }
                obDlgPatientCardEdit.exec();
            }
        }
    }
    catch( cSevException &e )
    {
        if( QString(e.what()).compare("Patientcard barcode not found") != 0 )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
        }
        else
        {
            if( QMessageBox::question( this, tr("Question"),
                                       tr("This barcode has not found in the database.\n"
                                          "Do you want to register it for a new patientcard?"),
                                       QMessageBox::Yes,QMessageBox::No ) == QMessageBox::Yes )
            {
                obDBPatientCard.createNew();
                obDBPatientCard.setBarcode( p_stBarcode );
                obDBPatientCard.save();
            }
        }
    }
}
//====================================================================================
void cWndMain::processInputTimePeriod( int p_inSecond )
{
    int inPrice;

    if( mdiPanels->isTimeIntervallValid( p_inSecond, &inPrice ) )
    {
        cDBAttendance   obDBAttendance;
        QTime           tLength;

        obDBAttendance.load( g_uiPatientAttendanceId );
        tLength = QTime::fromString(obDBAttendance.length(),"hh:mm:ss");
        obDBAttendance.setLength( tLength.addSecs( p_inSecond*60 ).toString("hh:mm:ss") );
        obDBAttendance.save();
        mdiPanels->setMainProcessTime( p_inSecond*60, inPrice );
    }
    else
    {
        QMessageBox::warning( this, tr("Warning"),
                              tr("This time period did not saved in the database\n"
                                 "for the actually selected device.") );
    }
}
//====================================================================================
void cWndMain::on_action_EditActualPatient_triggered()
{
    cDlgPatientEdit  obDlgEdit( this, &g_obPatient );
    obDlgEdit.setWindowTitle( QString::fromStdString( g_obPatient.name() ) );
    if( obDlgEdit.exec() == QDialog::Accepted )
    {
        cDBPostponed    obDBPostponed;
        obDBPostponed.removePatient( g_obPatient.id() );
    }
}
//====================================================================================
void cWndMain::on_action_DeviceSettings_triggered()
{
    cDlgPanelSettings   obDlgEdit( this, mdiPanels->activePanel()+1 );

    if( obDlgEdit.exec() == QDialog::Accepted )
    {
        mdiPanels->reload();
    }
}
//====================================================================================
void cWndMain::on_action_SelectActualAttendance_triggered()
{
    cDlgAttendanceSelect    obDlgSelect( this );

    obDlgSelect.exec();
}
//====================================================================================
void cWndMain::on_action_DeselectActualAttendance_triggered()
{
    g_uiPatientAttendanceId = 0;
}
//====================================================================================
void cWndMain::on_action_EditActualAttendance_triggered()
{
    cDBAttendance   obDBAttendance;

    obDBAttendance.load( g_uiPatientAttendanceId );

    cDlgAttendanceEdit  obDlgEdit( this, &obDBAttendance );

    obDlgEdit.exec();
}
//====================================================================================
