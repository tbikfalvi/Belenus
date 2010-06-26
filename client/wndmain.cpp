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

//====================================================================================

#include "edit/dlguseredit.h"
#include "edit/dlgpatientedit.h"
#include "edit/dlgattendanceedit.h"
#include "edit/dlgpatientcardedit.h"

//====================================================================================

#include "dlg/dlglogin.h"
#include "dlg/dlgpreferences.h"
#include "dlg/dlghardwaretest.h"
#include "dlg/dlglogs.h"
#include "dlg/dlginputstart.h"
#include "dlg/dlgpatientcardadd.h"
#include "dlg/dlgserialreg.h"

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

    menuPatient->setIcon( QIcon("./resources/40x40_patient.gif") );
    menuAttendance->setIcon( QIcon("./resources/40x40_attendance.gif") );
    menuPatientCard->setIcon( QIcon("./resources/40x40_patientcard.gif") );
    menuDevice->setIcon( QIcon( "./resources/40x40_device.gif" ) );
    action_PCActivate->setIcon( QIcon("./resources/40x40_patientcard_sell.gif") );

    cDBPostponed    *poDBPostPoned = new cDBPostponed();

    g_poPrefs->setPostponedPatients( poDBPostPoned->countPostponedPatients() );
    g_poPrefs->setPostponedAttendances( poDBPostPoned->countPostponedAttendances() );

    m_nTimer = startTimer( 300 );

    delete poDBPostPoned;
}
//====================================================================================
cWndMain::~cWndMain()
{
    cTracer obTrace( "cWndMain::~cWndMain" );

    killTimer( m_nTimer );
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

        //----------------------------------------------
        // Penztar betoltese, ellenorzese
        //----------------------------------------------
        try
        {
            g_obCassa.load();
        }
        catch( cSevException &e )
        {
            if( QString(e.what()).compare("Cassa table empty") != 0 )
            {
                g_obLogger << e.severity();
                g_obLogger << e.what() << cQTLogger::EOM;
            }
            else
            {
                g_obCassa.createNew();
                if( QMessageBox::critical( this, tr("Question"),
                                           tr("There is no data recorded in database for cassa.\n"
                                              "The application can not record any money related\n"
                                              "action without valid cassa data record.\n"
                                              "Do you want to start cassa recording with the current user?\n\n"
                                              "If you want to start cassa with different user, please log out\n"
                                              "and relogin with the desired user account."),
                                           QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes )
                {
                    g_obCassa.setUserId( g_obUser.id() );
                    g_obCassa.setLicenceId( g_poPrefs->getLicenceId() );
                    g_obCassa.setCurrentBalance( 0 );
                    g_obCassa.setStartDateTime( QDateTime::currentDateTime().toString( QString("yyyy-MM-dd hh:mm:ss") ).toStdString() );
                    g_obCassa.setActive( true );
                    g_obCassa.save();
                }
            }
        }
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
        qsTitle += QString::fromStdString( g_obUser.groups() );
        qsTitle += "]";
    }

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
    action_PanelStatuses->setEnabled( g_obUser.isInGroup( "admin" ) );

    setWindowTitle( qsTitle );

    updateToolbar();
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

    action_UseWithCard->setEnabled( true );
    action_UseByTime->setEnabled( true );

    action_AttendanceNew->setEnabled( g_obPatient.id()>0 );
    action_PostponedAttendance->setEnabled( g_poPrefs->postponedAttendances()>0 );

    action_DeviceStart->setEnabled( !mdiPanels->isPanelWorking(mdiPanels->activePanel()) );
    action_DeviceSkipStatus->setEnabled( mdiPanels->isPanelWorking(mdiPanels->activePanel()) );
    action_DeviceReset->setEnabled( mdiPanels->isPanelWorking(mdiPanels->activePanel()) );

    action_DeviceSettings->setEnabled( false );

    action_DoctorSchedule->setEnabled( false );
    action_DeviceSchedule->setEnabled( false );
}
//====================================================================================
void cWndMain::timerEvent(QTimerEvent *)
{
    updateTitle();
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
void cWndMain::on_action_DeviceStart_triggered()
{
    mdiPanels->start();
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
    updateTitle();
}
//====================================================================================
void cWndMain::on_action_PatientEmpty_triggered()
{
    cTracer obTrace( "cWndMain::on_action_PatientEmpty_triggered" );

    g_obPatient.createNew();
    updateTitle();
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
    if( obDlgInputStart.exec() == QDialog::Accepted )
        processInputPatientCard( obDlgInputStart.getEditText() );
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
        QMessageBox::warning( this, tr("Attention"),
                              tr("There is no patient in the database with name like\n\n"
                              "\'%1\'").arg(p_stPatientName.trimmed()) );
    }
}
//====================================================================================
void cWndMain::processInputPatientCard( QString p_stBarcode )
{
    cDBPatientCard  obDBPatientCard;

    try
    {
        obDBPatientCard.load( p_stBarcode.toStdString() );

        if( obDBPatientCard.active() )
        {

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
            g_obLogger << e.severity();
            g_obLogger << e.what() << cQTLogger::EOM;
        }
        else
        {
            if( QMessageBox::question( this, tr("Question"),
                                       tr("This barcode has not found in the database.\n"
                                          "Do you want to register it for a new patientcard?"),
                                       QMessageBox::Yes,QMessageBox::No ) == QMessageBox::Yes )
            {
                obDBPatientCard.createNew();
                obDBPatientCard.setBarcode( p_stBarcode.toStdString() );
                obDBPatientCard.save();
            }
        }
    }
}
//====================================================================================
void cWndMain::processInputTimePeriod( int p_inSecond )
{

}
//====================================================================================
void cWndMain::on_action_EditActualPatient_triggered()
{
    cDlgPatientEdit  obDlgEdit( this, &g_obPatient );
    obDlgEdit.setWindowTitle( QString::fromStdString( g_obPatient.name() ) );
    obDlgEdit.exec();
}
//====================================================================================
