//====================================================================================
//
// Belenus Kliens alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dlgattendanceselect.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#include "belenus.h"
#include "dlgattendanceselect.h"
#include "../edit/dlgattendanceedit.h"
//#include "../db/dbpostponed.h"

cDlgAttendanceSelect::cDlgAttendanceSelect( QWidget *p_poParent )
    : cDlgAttendance( p_poParent )
{
    setWindowTitle( tr( "Select attendance" ) );
    setWindowIcon( QIcon("./resources/40x40_attendance.png") );

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
    pbSelect->setIcon( QIcon("./resources/40x40_ok.png") );
    btbButtonsSide->addButton( pbSelect, QDialogButtonBox::ActionRole );
    pbCancel = new QPushButton( tr( "Cancel" ), this );
    pbCancel->setObjectName( QString::fromUtf8( "pbCancel" ) );
    pbCancel->setIconSize( QSize(20, 20) );
    pbCancel->setIcon( QIcon("./resources/40x40_cancel.png") );
    btbButtonsSide->addButton( pbCancel, QDialogButtonBox::RejectRole );

    cmbPatient->setEnabled( false );

    setupTableView();

    connect( pbSelect, SIGNAL(clicked(bool)), this, SLOT(on_pbSelect_clicked()) );
}

cDlgAttendanceSelect::~cDlgAttendanceSelect()
{

}

void cDlgAttendanceSelect::setupTableView()
{
    cTracer obTracer( "cDlgAttendanceSelect::setupTableView" );

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

        tbvCrud->sortByColumn( 1, Qt::AscendingOrder );
    }
    else
    {
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "Name" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Date" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Time" ) );

        tbvCrud->resizeColumnToContents( 1 );
        tbvCrud->resizeColumnToContents( 2 );
        tbvCrud->resizeColumnToContents( 3 );

        tbvCrud->sortByColumn( 1, Qt::AscendingOrder );
    }
}

void cDlgAttendanceSelect::refreshTable()
{
    cTracer obTracer( "cDlgAttendanceSelect::refreshTable" );

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_qsQuery = "SELECT attendance.attendanceId, patients.name, attendance.date, attendance.length FROM attendance, patients WHERE attendance.attendanceId>0 AND attendance.patientId=patients.patientId";
    }
    else
    {
        m_qsQuery = "SELECT attendance.attendanceId as id, patients.name, attendance.date, attendance.length FROM attendance, patients WHERE attendance.attendanceId>0 AND attendance.patientId=patients.patientId AND patients.active=1";
    }

    unsigned int uiPatientId = cmbPatient->itemData( cmbPatient->currentIndex() ).toInt();
    if( uiPatientId > 0 )
    {
        m_qsQuery += " AND ";
        m_qsQuery += QString( "patients.patientId=%1" ).arg( uiPatientId );
    }

    cDlgCrud::refreshTable();
}

void cDlgAttendanceSelect::enableButtons()
{
    cTracer obTracer( "cDlgAttendanceSelect::enableButtons" );

    if( m_uiSelectedId )
    {
        pbSelect->setEnabled( true );
    }
    else
    {
        pbSelect->setEnabled( false );
    }
}

void cDlgAttendanceSelect::on_pbSelect_clicked()
{
    g_uiPatientAttendanceId = m_uiSelectedId;

    QDialog::accept();
}
