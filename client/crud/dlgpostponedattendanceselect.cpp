//====================================================================================
//
// Belenus Kliens alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dlgpostponedattendanceselect.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#include "belenus.h"
#include "dlgpostponedattendanceselect.h"
#include "../edit/dlgattendanceedit.h"
#include "../db/dbpostponed.h"

cDlgPostponedAttendanceSelect::cDlgPostponedAttendanceSelect( QWidget *p_poParent )
    : cDlgAttendance( p_poParent )
{
    setWindowTitle( tr( "Select postponed attendance" ) );
    setWindowIcon( QIcon("./resources/40x40_attendance.gif") );

    m_poBtnClose->setEnabled(false);
    m_poBtnDelete->setEnabled(false);
    m_poBtnEdit->setEnabled(false);
    m_poBtnNew->setEnabled(false);

    m_poBtnClose->setVisible(false);
    m_poBtnDelete->setVisible(false);
    m_poBtnEdit->setVisible(false);
    m_poBtnNew->setVisible(false);

    pbSelect = new QPushButton( tr( "Select" ), this );
    pbSelect->setObjectName( QString::fromUtf8( "pbSelect" ) );
    pbSelect->setIconSize( QSize(20, 20) );
    pbSelect->setIcon( QIcon("./resources/40x40_ok.gif") );
    btbButtonsSide->addButton( pbSelect, QDialogButtonBox::ActionRole );
    pbCancel = new QPushButton( tr( "Cancel" ), this );
    pbCancel->setObjectName( QString::fromUtf8( "pbCancel" ) );
    pbCancel->setIconSize( QSize(20, 20) );
    pbCancel->setIcon( QIcon("./resources/40x40_cancel.gif") );
    btbButtonsSide->addButton( pbCancel, QDialogButtonBox::RejectRole );

    setupTableView();

    connect( pbSelect, SIGNAL(clicked(bool)), this, SLOT(on_pbSelect_clicked()) );
}

cDlgPostponedAttendanceSelect::~cDlgPostponedAttendanceSelect()
{

}

void cDlgPostponedAttendanceSelect::setupTableView()
{
    cTracer obTracer( "cDlgPostponedAttendanceSelect::setupTableView" );

    refreshTable();

    cDlgCrud::setupTableView();

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_poModel->setHeaderData( 0, Qt::Horizontal, tr( "Id" ) );
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "Name" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Date" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Time" ) );

        tbvCrud->resizeColumnToContents( 0 );
        tbvCrud->resizeColumnToContents( 1 );
        tbvCrud->resizeColumnToContents( 2 );
        tbvCrud->resizeColumnToContents( 3 );

        tbvCrud->sortByColumn( 2, Qt::AscendingOrder );
    }
    else
    {
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "Name" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Date" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Time" ) );

        tbvCrud->resizeColumnToContents( 1 );
        tbvCrud->resizeColumnToContents( 2 );
        tbvCrud->resizeColumnToContents( 3 );

        tbvCrud->sortByColumn( 2, Qt::AscendingOrder );
    }
}

void cDlgPostponedAttendanceSelect::refreshTable()
{
    cTracer obTracer( "cDlgPostponedAttendanceSelect::refreshTable" );

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_qsQuery = "SELECT attendance.attendanceId, patients.name, attendance.date, attendance.length FROM attendance, patients, toBeFilled WHERE attendance.attendanceId>0 AND toBeFilled.patientId=0 AND attendance.patientId=patients.patientId AND attendance.attendanceId=toBeFilled.attendanceId";
    }
    else
    {
        m_qsQuery = "SELECT attendance.attendanceId as id, patients.name, attendance.date, attendance.length FROM attendance, patients, toBeFilled WHERE attendance.attendanceId>0 AND toBeFilled.patientId=0 AND attendance.patientId=patients.patientId AND attendance.attendanceId=toBeFilled.attendanceId AND patients.active=1";
    }

    unsigned int uiPatientId = cmbPatient->itemData( cmbPatient->currentIndex() ).toInt();
    if( uiPatientId > 0 )
    {
        m_qsQuery += " AND ";
        m_qsQuery += QString( "patients.patientId=%1" ).arg( uiPatientId );
    }

    cDlgCrud::refreshTable();
}

void cDlgPostponedAttendanceSelect::enableButtons()
{
    cTracer obTracer( "cDlgPostponedAttendanceSelect::enableButtons" );

    if( m_uiSelectedId )
    {
        pbSelect->setEnabled( true );
    }
    else
    {
        pbSelect->setEnabled( false );
    }
}

void cDlgPostponedAttendanceSelect::on_pbSelect_clicked()
{
    cDBAttendance   *poAttendance = NULL;
    cDBPostponed    *poPostponed = new cDBPostponed;
    try
    {
        poPostponed->loadAttendance( m_uiSelectedId );

        poAttendance = new cDBAttendance;
        poAttendance->load( m_uiSelectedId );

        cDlgAttendanceEdit  obDlgEdit( this, poAttendance, poPostponed );
        obDlgEdit.setWindowTitle( tr("Edit attendance") );
        if( obDlgEdit.exec() == QDialog::Accepted )
        {
            poPostponed->removeAttendance( m_uiSelectedId );
        }
        refreshTable();

        if( poAttendance ) delete poAttendance;
        if( poPostponed ) delete poPostponed;
    }
    catch( cSevException &e )
    {
        if( poAttendance ) delete poAttendance;
        if( poPostponed ) delete poPostponed;
        g_obLogger(e.severity()) << e.what() << cQTLogger::EOM;
    }
}
