#include "dlgpatientselect.h"

cDlgPatientSelect::cDlgPatientSelect( QWidget *p_poParent )
    : cDlgCrud( p_poParent )
{
    cTracer obTrace( "cDlgPatientSelect::cDlgPatientSelect" );

    setWindowTitle( tr( "Select actual patient" ) );
    setWindowIcon( QIcon("./resources/40x40_patient.gif") );

    m_poBtnClose->setEnabled(false);
    m_poBtnDelete->setEnabled(false);
    m_poBtnEdit->setEnabled(false);
    m_poBtnNew->setEnabled(false);

    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName( QString::fromUtf8( "horizontalLayout" ) );
    lblName = new QLabel( this );
    lblName->setObjectName( QString::fromUtf8( "lblName" ) );
    lblName->setText( "Name: " );
    horizontalLayout->addWidget( lblName );
    ledName = new QLineEdit( this );
    ledName->setObjectName( QString::fromUtf8( "ledName" ) );
    horizontalLayout->addWidget( ledName );
    verticalLayout->insertLayout( 0, horizontalLayout );

    setupTableView();

    connect( ledName, SIGNAL(textChanged()), this, SLOT(refreshTable()) );
}

cDlgPatientSelect::~cDlgPatientSelect()
{
    cTracer obTrace( "cDlgPatientSelect::~cDlgPatientSelect" );
}

void cDlgPatientSelect::setupTableView()
{
    cTracer obTracer( "cDlgPatientSelect::setupTableView" );

    refreshTable();

    cDlgCrud::setupTableView();

    if( g_obUser.isInGroup( "root" ) )
    {
        m_poModel->setHeaderData( 0, Qt::Horizontal, tr( "Id" ) );
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "LicenceId" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Name" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "UniqueId" ) );
    }
    else
    {
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "Name" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "UniqueId" ) );
    }
}

void cDlgPatientSelect::refreshTable()
{
    cTracer obTracer( "cDlgPatientSelect::refreshTable" );

    if( g_obUser.isInGroup( "root" ) )
    {
        m_qsQuery = "SELECT patientId, licenceId, name, uniqueId FROM patients WHERE patientId>0";
    }
    else
    {
        m_qsQuery = "SELECT patientId AS id, name, uniqueId FROM patients WHERE patientId>0 AND archive<>\"DEL\"";
    }

    QString stTemp = ledName->text();

    if( stTemp != "" )
    {
        m_qsQuery += " AND ";
        m_qsQuery += QString( "name LIKE '%1'" ).arg( stTemp );
    }

    cDlgCrud::refreshTable();
}

void cDlgPatientSelect::enableButtons()
{
    cTracer obTracer( "cDlgPatientSelect::enableButtons" );

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
