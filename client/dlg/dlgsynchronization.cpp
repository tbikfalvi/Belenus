//====================================================================================
//
// Belenus Server alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dlgsynchronization.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#include "dlgsynchronization.h"

//====================================================================================
cDlgSynchronization::cDlgSynchronization( QWidget *p_poParent ) : QDialog( p_poParent )
//====================================================================================
{
    m_bAutoSynchronization  = false;
    m_bAutoStart            = false;
    m_bAutoClose            = false;

    setupUi( this );

    setWindowTitle( tr("Database synchronization") );
    setWindowIcon( QIcon("./resources/40x40_database_sync.png") );

    pbExit->setIcon( QIcon("./resources/40x40_exit.png") );
    pbStart->setIcon( QIcon("./resources/40x40_database_sync.png") );

    if( g_obDBMirror.checkIsSynchronizationNeeded() )
    {
        pbStart->setEnabled( true );
        listInformation->addItem( tr("Database synchronization required.") );

        checkSynchronizationStatus();
    }
    else
    {
        pbStart->setEnabled( false );
        listInformation->addItem( tr("Database synchronized with server.") );
    }
}
//====================================================================================
cDlgSynchronization::~cDlgSynchronization()
//====================================================================================
{

}
//====================================================================================
void cDlgSynchronization::autoSynchronization()
//====================================================================================
{
    setWindowTitle( tr("Automatic database synchronization") );
    listInformation->addItem( tr("Automatic database synchronization is starting ...") );
    m_bAutoSynchronization  = true;
    m_bAutoStart            = true;
    pbStart->setEnabled( false );
    pbExit->setEnabled( false );
    m_nTimer = startTimer( 5000 );
}
//====================================================================================
void cDlgSynchronization::checkSynchronizationStatus()
//====================================================================================
{
    chkUser->setChecked( g_obDBMirror.checkSyncLevel(DB_USER) );
    chkPatientOrigin->setChecked( g_obDBMirror.checkSyncLevel(DB_PATIENT_ORIGIN) );
    chkReasonToVisit->setChecked( g_obDBMirror.checkSyncLevel(DB_REASON_TO_VISIT) );
    chkIllnessGroups->setChecked( g_obDBMirror.checkSyncLevel(DB_ILLNESS_GROUP) );
    chkHealthInsurance->setChecked( g_obDBMirror.checkSyncLevel(DB_HEALTH_INSURANCE) );
    chkCompany->setChecked( g_obDBMirror.checkSyncLevel(DB_COMPANY) );
    chkDoctors->setChecked( g_obDBMirror.checkSyncLevel(DB_DOCTOR) );
    chkApplication->setChecked( g_obDBMirror.checkSyncLevel(DB_PANEL) ||
                                g_obDBMirror.checkSyncLevel(DB_PANEL_STATUS) ||
                                g_obDBMirror.checkSyncLevel(DB_PANEL_USE) ||
                                g_obDBMirror.checkSyncLevel(DB_CASSA) ||
                                g_obDBMirror.checkSyncLevel(DB_CASSA_DENOMINATION) ||
                                g_obDBMirror.checkSyncLevel(DB_CASSA_HISTORY) ||
                                g_obDBMirror.checkSyncLevel(DB_ZIP_REGION_CITY) );
    chkPatient->setChecked( g_obDBMirror.checkSyncLevel(DB_PATIENT) );
    chkAttendance->setChecked( g_obDBMirror.checkSyncLevel(DB_ATTENDANCE) );
    chkPatientcardTypes->setChecked( g_obDBMirror.checkSyncLevel(DB_PATIENTCARD_TYPE) );
    chkPatientcard->setChecked( g_obDBMirror.checkSyncLevel(DB_PATIENTCARD) );
    chkPatientcardHistory->setChecked( g_obDBMirror.checkSyncLevel(DB_PATIENTCARD_HISTORY) );
    chkDiscount->setChecked( g_obDBMirror.checkSyncLevel(DB_DISCOUNT) );
    chkLedger->setChecked( g_obDBMirror.checkSyncLevel(DB_LEDGER_DEVICE) ||
                           g_obDBMirror.checkSyncLevel(DB_LEDGER));
}
//====================================================================================
void cDlgSynchronization::on_pbStart_clicked()
//====================================================================================
{
    m_nTimer = startTimer( 500 );
    listInformation->addItem( tr("Starting database synchronization.") );
    g_obDBMirror.synchronizeAllTable();
    pbStart->setEnabled( false );
}
//====================================================================================
void cDlgSynchronization::timerEvent(QTimerEvent *)
//====================================================================================
{
    if( m_bAutoStart )
    {
        killTimer( m_nTimer );
        m_bAutoStart = false;
        on_pbStart_clicked();
        return;
    }
    else if( m_bAutoClose )
    {
        killTimer( m_nTimer );
        QDialog::accept();
        return;
    }

    checkSynchronizationStatus();

    if( g_obDBMirror.checkSynchronizationFinished() )
    {
        killTimer( m_nTimer );
        listInformation->addItem( tr("Database synchronization finished.") );
        if( m_bAutoSynchronization )
        {
            listInformation->addItem( tr("Exiting application ...") );
            m_nTimer = startTimer( 3000 );
            m_bAutoClose = true;
        }
    }
}
//====================================================================================
