#include <QMessageBox>

#include "belenus.h"
#include "dlgattendance.h"
#include "../edit/dlgattendanceedit.h"

cDlgAttendance::cDlgAttendance( QWidget *p_poParent )
    : cDlgCrud( p_poParent )
{
    setWindowTitle( tr( "Attendance List" ) );
    setWindowIcon( QIcon("./resources/40x40_attendance.gif") );

    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName( QString::fromUtf8( "horizontalLayout" ) );
    lblPatient = new QLabel( this );
    lblPatient->setObjectName( QString::fromUtf8( "lblPatient" ) );
    lblPatient->setText( tr("Patient: ") );
    horizontalLayout->addWidget( lblPatient );
    cmbPatient = new QComboBox( this );
    cmbPatient->setObjectName( QString::fromUtf8( "cmbPatient" ) );
    cmbPatient->resize( 200, 20 );
    horizontalLayout->addWidget( cmbPatient );
    horizontalSpacer1 = new QSpacerItem( 10, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    horizontalLayout->addItem( horizontalSpacer1 );
    verticalLayout->insertLayout( 0, horizontalLayout );

    QSqlQuery *poQuery = NULL;

    cmbPatient->addItem( tr("<All patient>"), 0 );
    try
    {
        poQuery = g_poDB->executeQTQuery( QString( "SELECT patientId, name FROM patients WHERE active=1 AND patientId>0" ) );
        while( poQuery->next() )
        {
            cmbPatient->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
            if( g_obPatient.id() == poQuery->value( 0 ) )
                cmbPatient->setCurrentIndex( cmbPatient->count()-1 );
        }
    }
    catch( cSevException &e )
    {
        g_obLogger << e.severity();
        g_obLogger << e.what() << cQTLogger::EOM;
    }
    if( poQuery ) delete poQuery;

    setupTableView();

    connect( cmbPatient, SIGNAL(currentIndexChanged(int)), this, SLOT(refreshTable()) );
}

cDlgAttendance::~cDlgAttendance()
{
}

void cDlgAttendance::setupTableView()
{
    cTracer obTracer( "cDlgAttendance::setupTableView" );

    refreshTable();

    cDlgCrud::setupTableView();

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_poModel->setHeaderData( 0, Qt::Horizontal, tr( "Id" ) );
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "LicenceId" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Date" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Time" ) );
        m_poModel->setHeaderData( 4, Qt::Horizontal, tr( "Active" ) );
        m_poModel->setHeaderData( 5, Qt::Horizontal, tr( "Archive" ) );

        tbvCrud->resizeColumnToContents( 0 );
        tbvCrud->resizeColumnToContents( 1 );
        tbvCrud->resizeColumnToContents( 2 );
        tbvCrud->resizeColumnToContents( 3 );
        tbvCrud->resizeColumnToContents( 4 );
        tbvCrud->resizeColumnToContents( 5 );

        tbvCrud->sortByColumn( 2, Qt::AscendingOrder );
    }
    else
    {
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "Date" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Time" ) );

        tbvCrud->resizeColumnToContents( 1 );
        tbvCrud->resizeColumnToContents( 2 );

        tbvCrud->sortByColumn( 1, Qt::AscendingOrder );
    }
}

void cDlgAttendance::refreshTable()
{
    cTracer obTracer( "cDlgAttendance::refreshTable" );

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_qsQuery = "SELECT attendanceId, licenceId, date, length, active, archive FROM attendance WHERE attendanceId>0";
    }
    else
    {
        m_qsQuery = "SELECT attendanceId AS id, date, length FROM attendance WHERE attendanceId>0 AND active=1";
    }

    unsigned int uiPatientId = cmbPatient->itemData( cmbPatient->currentIndex() ).toInt();
    if( uiPatientId > 0 )
    {
        m_qsQuery += " AND ";
        m_qsQuery += QString( "patientId=%1" ).arg( uiPatientId );
    }

    cDlgCrud::refreshTable();
}

void cDlgAttendance::enableButtons()
{
    cTracer obTracer( "cDlgAttendance::enableButtons" );

    m_poBtnNew->setEnabled( g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    m_poBtnDelete->setEnabled( m_uiSelectedId > 0 && g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    m_poBtnEdit->setEnabled( m_uiSelectedId > 0 );
}

void cDlgAttendance::newClicked( bool )
{
    cDBAttendance *poAttendance = new cDBAttendance;
    poAttendance->createNew();
    poAttendance->setPatientId( g_obPatient.id() );

    cDlgAttendanceEdit  obDlgEdit( this, poAttendance );
    obDlgEdit.setWindowTitle( tr( "New Attendance" ) );
    if( obDlgEdit.exec() == QDialog::Accepted )
    {
        m_uiSelectedId = poAttendance->id();
        refreshTable();

        if( QMessageBox::question( this, tr("Question"),
                                   tr("Do you want to select the created attendance as actual?"),
                                   QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
        {
            g_uiPatientAttendanceId = poAttendance->id();
        }
    }

    delete poAttendance;
}

void cDlgAttendance::editClicked( bool )
{
    cDBAttendance  *poAttendance = NULL;

    try
    {
        poAttendance = new cDBAttendance;
        poAttendance->load( m_uiSelectedId );

        cDlgAttendanceEdit  obDlgEdit( this, poAttendance );
        obDlgEdit.setWindowTitle( tr("Edit attendance") );
        if( obDlgEdit.exec() == QDialog::Accepted )
        {
            refreshTable();

            if( QMessageBox::question( this, tr("Question"),
                                       tr("Do you want to select the created attendance as actual?"),
                                       QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
            {
                g_uiPatientAttendanceId = poAttendance->id();
            }
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

void cDlgAttendance::deleteClicked( bool )
{
    cDBAttendance  *poAttendance = NULL;

    if( QMessageBox::question( this, tr( "Question" ),
                               tr( "Are you sure you want to delete this Attendance?" ),
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
    {
        try
        {
            poAttendance = new cDBAttendance;
            poAttendance->load( m_uiSelectedId );
            poAttendance->remove();
            m_uiSelectedId = 0;
            refreshTable();
            if( poAttendance ) delete poAttendance;
        }
        catch( cSevException &e )
        {
            if( poAttendance ) delete poAttendance;

            g_obLogger << e.severity();
            g_obLogger << e.what() << cQTLogger::EOM;
        }
    }
}
