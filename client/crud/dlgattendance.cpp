#include <QMessageBox>

#include "belenus.h"
#include "dlgattendance.h"
#include "../edit/dlgattendanceedit.h"

cDlgAttendance::cDlgAttendance( QWidget *p_poParent )
    : cDlgCrud( p_poParent )
{
    setWindowTitle( tr( "Attendance List" ) );
    setWindowIcon( QIcon("./resources/40x40_attendance.gif") );

    if( g_obUser.isInGroup( "root" ) )
    {
        m_qsQuery = "SELECT attendanceId, licenceId, date, length, archive FROM attendance WHERE attendanceId>0";
    }
    else
    {
        m_qsQuery = "SELECT attendanceId AS id, date, length FROM attendance WHERE attendanceId>0 AND archive<>\"DEL\"";
    }

    setupTableView();
}

cDlgAttendance::~cDlgAttendance()
{
}

void cDlgAttendance::setupTableView()
{
    cTracer obTracer( "cDlgAttendance::setupTableView" );

    refreshTable();

    cDlgCrud::setupTableView();

    if( g_obUser.isInGroup( "root" ) )
    {
        m_poModel->setHeaderData( 0, Qt::Horizontal, tr( "Id" ) );
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "LicenceId" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Date" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Time" ) );
        m_poModel->setHeaderData( 4, Qt::Horizontal, tr( "Archive" ) );
    }
    else
    {
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "Date" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Time" ) );
    }
}

void cDlgAttendance::enableButtons()
{
    cTracer obTracer( "cDlgAttendance::enableButtons" );

    if( m_uiSelectedId )
    {
        m_poBtnDelete->setEnabled( true );
        m_poBtnEdit->setEnabled( true );
    }
    else
    {
        m_poBtnDelete->setEnabled( false );
        m_poBtnEdit->setEnabled( false );
    }
}

void cDlgAttendance::newClicked( bool )
{
    cDBAttendance *poAttendance = new cDBAttendance;
    poAttendance->createNew();

    cDlgAttendanceEdit  obDlgEdit( this, poAttendance );
    obDlgEdit.setWindowTitle( tr( "New Attendance" ) );
    if( obDlgEdit.exec() == QDialog::Accepted )
    {
        m_uiSelectedId = poAttendance->id();
        refreshTable();
    }

    delete poAttendance;
}

void cDlgAttendance::deleteClicked( bool )
{
    if( QMessageBox::question( this, tr( "Confirmation" ),
                               tr( "Are you sure you want to delete this Attendance?" ),
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
    {
        try
        {
            QString stQuery = QString( "UPDATE attendance SET archive=\"DEL\" WHERE attendanceId=%1" ).arg( m_uiSelectedId );
            g_poDB->executeQuery( stQuery.toStdString(), true );

            m_uiSelectedId = 0;
            refreshTable();
        }
        catch( cSevException &e )
        {
            g_obLogger << e.severity();
            g_obLogger << e.what() << cQTLogger::EOM;
        }
    }
}

void cDlgAttendance::editClicked( bool )
{
    cDBAttendance  *poAttendance = NULL;

    try
    {
        poAttendance = new cDBAttendance;
        poAttendance->load( m_uiSelectedId );

        cDlgAttendanceEdit  obDlgEdit( this, poAttendance );
//        obDlgEdit.setWindowTitle( QString::fromStdString( poAttendance->name() ) );
        if( obDlgEdit.exec() == QDialog::Accepted )
        {
            refreshTable();
        }

        if( poAttendance ) delete poAttendance;
    }
    catch( cSevException &e )
    {
        if( poAttendance ) delete poAttendance;

        g_obLogger << e.severity();
        g_obLogger << e.what() << cQTLogger::EOM;
    }
}
