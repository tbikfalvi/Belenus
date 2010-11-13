//====================================================================================
//
// Belenus Server alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dlgglobals.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#include "dlgglobals.h"

//====================================================================================
cDlgDBGlobals::cDlgDBGlobals( QWidget *p_poParent ) : QDialog( p_poParent )
//====================================================================================
{
    g_obLogger(cSeverity::DEBUG) << "[cDlgDBGlobals::cDlgDBGlobals]" << EOM;

    m_bAutoSynchronization  = false;
    m_bNormalStart          = true;
    m_bAutoStart            = false;
    m_bAutoClose            = false;

    setupUi( this );

    setWindowTitle( tr("Database global data synchronization") );
    setWindowIcon( QIcon("./resources/40x40_database_sync.png") );

    pbExit->setIcon( QIcon("./resources/40x40_exit.png") );
    pbStart->setIcon( QIcon("./resources/40x40_database_sync.png") );

    pbStart->setEnabled( false );
    listInformation->addItem( tr("Please wait while checking global data on server.") );

    m_nTimer = startTimer( 5000 );
}
//====================================================================================
cDlgDBGlobals::~cDlgDBGlobals()
//====================================================================================
{
}
//====================================================================================
void cDlgDBGlobals::autoSynchronization()
//====================================================================================
{
    g_obLogger(cSeverity::DEBUG) << "[cDlgDBGlobals::autoSynchronization]" << EOM;

    setWindowTitle( tr("Automatic database synchronization") );
    listInformation->addItem( tr("Automatic database synchronization is starting ...") );
    m_bAutoSynchronization  = true;
    m_bNormalStart          = false;
    m_bAutoStart            = true;
    m_nTimer = startTimer( 5000 );
    pbStart->setEnabled( false );
    pbExit->setEnabled( false );
}
//====================================================================================
void cDlgDBGlobals::on_pbStart_clicked()
//====================================================================================
{
    g_obLogger(cSeverity::DEBUG) << "[cDlgDBGlobals::on_pbStart_clicked]" << EOM;

    m_nTimer = startTimer( 500 );
    listInformation->addItem( tr("Starting database synchronization.") );
    g_obDBMirror.acquirePatientOriginGlobals();
    pbStart->setEnabled( false );
}
//====================================================================================
void cDlgDBGlobals::timerEvent(QTimerEvent *)
//====================================================================================
{
    g_obLogger(cSeverity::DEBUG) << "[cDlgDBGlobals::timerEvent]" << EOM;

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
    else if( m_bNormalStart )
    {
        killTimer( m_nTimer );
        if( g_obDBMirror.checkIsGlobalDataModifiedOnServer() )
        {
            pbStart->setEnabled( true );
            listInformation->addItem( tr("Database synchronization required.") );
        }
        else
        {
            pbStart->setEnabled( false );
            listInformation->addItem( tr("Database synchronized with server.") );
        }
    }

    if( !g_obDBMirror.checkIsGlobalDataDownloadInProgress() )
    {
        killTimer( m_nTimer );

        chkPatientOrigin->setChecked( g_obDBMirror.checkGlobalData(DB_PATIENT_ORIGIN) );
        chkReasonToVisit->setChecked( g_obDBMirror.checkGlobalData(DB_REASON_TO_VISIT) );
        chkIllnessGroups->setChecked( g_obDBMirror.checkGlobalData(DB_ILLNESS_GROUP) );
        chkHealthInsurance->setChecked( g_obDBMirror.checkGlobalData(DB_HEALTH_INSURANCE) );
        chkCompany->setChecked( g_obDBMirror.checkGlobalData(DB_COMPANY) );
        chkDoctors->setChecked( g_obDBMirror.checkGlobalData(DB_DOCTOR_TYPE) || g_obDBMirror.checkGlobalData(DB_DOCTOR) );
        chkPatient->setChecked( g_obDBMirror.checkGlobalData(DB_PATIENT) );
        chkPatientcardTypes->setChecked( g_obDBMirror.checkGlobalData(DB_PATIENTCARD_TYPE) );
        chkPatientcard->setChecked( g_obDBMirror.checkGlobalData(DB_PATIENTCARD) );
        chkDiscount->setChecked( g_obDBMirror.checkGlobalData(DB_DISCOUNT) );
        chkApplication->setChecked( g_obDBMirror.checkGlobalData(DB_PUBLIC_PLACES) ||
                                    g_obDBMirror.checkGlobalData(DB_LEDGER) ||
                                    g_obDBMirror.checkGlobalData(DB_PRODUCT_TYPE) ||
                                    g_obDBMirror.checkGlobalData(DB_PRODUCT) ||
                                    g_obDBMirror.checkGlobalData(DB_CASSA));

        listInformation->addItem( tr("Database synchronization finished.") );
        if( m_bAutoSynchronization )
        {
            m_nTimer = startTimer( 3000 );
            m_bAutoClose = true;
        }
    }
}
//====================================================================================
