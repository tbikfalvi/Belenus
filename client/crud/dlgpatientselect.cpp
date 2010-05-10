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

    m_poBtnClose->setVisible(false);
    m_poBtnDelete->setVisible(false);
    m_poBtnEdit->setVisible(false);
    m_poBtnNew->setVisible(false);

    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName( QString::fromUtf8( "horizontalLayout" ) );
    lblName = new QLabel( this );
    lblName->setObjectName( QString::fromUtf8( "lblName" ) );
    lblName->setText( "Name: " );
    horizontalLayout->addWidget( lblName );
    ledName = new QLineEdit( this );
    ledName->setObjectName( QString::fromUtf8( "ledName" ) );
    horizontalLayout->addWidget( ledName );
    horizontalSpacer1 = new QSpacerItem( 13, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    horizontalLayout->addSpacerItem( horizontalSpacer1 );
    lblUniqueId = new QLabel( this );
    lblUniqueId->setObjectName( QString::fromUtf8( "lblUniqueId" ) );
    lblUniqueId->setText( "Unique identifier: " );
    horizontalLayout->addWidget( lblName );
    ledUniqueId = new QLineEdit( this );
    ledUniqueId->setObjectName( QString::fromUtf8( "ledUniqueId" ) );
    horizontalLayout->addWidget( ledUniqueId );
    horizontalSpacer2 = new QSpacerItem( 13, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    horizontalLayout->addSpacerItem( horizontalSpacer2 );
    verticalLayout->insertLayout( 0, horizontalLayout );

    verticalCustomLayout = new QVBoxLayout();
    verticalCustomLayout->setObjectName( QString::fromUtf8( "verticalCustomLayout" ) );
    pbSelect = new QPushButton( this );
    pbSelect->setObjectName( QString::fromUtf8( "pbSelect" ) );
    pbSelect->resize( 130, 30 );
    pbSelect->setIconSize( QSize(20, 20) );
    pbSelect->setIcon( QIcon("./resources/40x40_ok.gif") );
    pbSelect->setText( "Select" );
    verticalCustomLayout->addWidget( pbSelect );
    spacer3 = new QSpacerItem( 13, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    verticalCustomLayout->addSpacerItem( spacer3 );
    pbCancel = new QPushButton( this );
    pbCancel->setObjectName( QString::fromUtf8( "pbCancel" ) );
    pbCancel->resize( 130, 30 );
    pbCancel->setIconSize( QSize(20, 20) );
    pbCancel->setIcon( QIcon("./resources/40x40_cancel.gif") );
    pbCancel->setText( "Cancel" );
    verticalCustomLayout->addWidget( pbCancel );
    verticalLayout->insertLayout( 2, verticalCustomLayout );

    setupTableView();

    connect( ledName, SIGNAL(textChanged(QString)), this, SLOT(refreshTable()) );
    connect( ledUniqueId, SIGNAL(textChanged(QString)), this, SLOT(refreshTable()) );
    connect( pbSelect, SIGNAL(clicked(bool)), this, SLOT(on_pbSelect_clicked()) );
    connect( pbCancel, SIGNAL(clicked(bool)), this, SLOT(on_pbCancel_clicked()) );
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

    QString stTemp;

    stTemp = ledName->text();
    if( stTemp != "" )
    {
        m_qsQuery += " AND ";
        m_qsQuery += QString( "name LIKE '\%%1\%'" ).arg( stTemp );
    }
    stTemp = ledUniqueId->text();
    if( stTemp != "" )
    {
        m_qsQuery += " AND ";
        m_qsQuery += QString( "uniqueId LIKE '\%%1\%'" ).arg( stTemp );
    }

    cDlgCrud::refreshTable();
}

void cDlgPatientSelect::enableButtons()
{
    cTracer obTracer( "cDlgPatientSelect::enableButtons" );

    if( m_uiSelectedId )
    {
        pbSelect->setEnabled( true );
    }
    else
    {
        pbSelect->setEnabled( false );
    }
}

void cDlgPatientSelect::newClicked( bool ) {}
void cDlgPatientSelect::deleteClicked( bool ) {}
void cDlgPatientSelect::editClicked( bool ) {}

void cDlgPatientSelect::on_pbSelect_clicked()
{
    try
    {
        g_obPatient.load( m_uiSelectedId );

        QDialog::accept();
    }
    catch( cSevException &e )
    {
        g_obLogger << e.severity();
        g_obLogger << e.what() << cQTLogger::EOM;
    }
}

void cDlgPatientSelect::on_pbCancel_clicked()
{
    QDialog::reject();
}
