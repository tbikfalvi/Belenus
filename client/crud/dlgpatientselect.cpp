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

    horizontalLayoutTop = new QHBoxLayout();
    horizontalLayoutTop->setObjectName( QString::fromUtf8( "horizontalLayoutTop" ) );
    lblName = new QLabel( this );
    lblName->setObjectName( QString::fromUtf8( "lblName" ) );
    lblName->setText( tr("Name: ") );
    horizontalLayoutTop->addWidget( lblName );
    ledName = new QLineEdit( this );
    ledName->setObjectName( QString::fromUtf8( "ledName" ) );
    horizontalLayoutTop->addWidget( ledName );
    horizontalSpacerTop = new QSpacerItem( 13, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    horizontalLayoutTop->addItem( horizontalSpacerTop );
    lblUniqueId = new QLabel( this );
    lblUniqueId->setObjectName( QString::fromUtf8( "lblUniqueId" ) );
    lblUniqueId->setText( tr("Unique identifier: ") );
    horizontalLayoutTop->addWidget( lblUniqueId );
    ledUniqueId = new QLineEdit( this );
    ledUniqueId->setObjectName( QString::fromUtf8( "ledUniqueId" ) );
    horizontalLayoutTop->addWidget( ledUniqueId );
    verticalLayout->insertLayout( 0, horizontalLayoutTop );

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

    connect( ledName, SIGNAL(textChanged(QString)), this, SLOT(refreshTable()) );
    connect( ledUniqueId, SIGNAL(textChanged(QString)), this, SLOT(refreshTable()) );
    connect( pbSelect, SIGNAL(clicked(bool)), this, SLOT(on_pbSelect_clicked()) );
}

cDlgPatientSelect::~cDlgPatientSelect()
{
    cTracer obTrace( "cDlgPatientSelect::~cDlgPatientSelect" );
}

void cDlgPatientSelect::setSearchPatientName( QString p_stName )
{
    ledName->setText( p_stName );
}

void cDlgPatientSelect::setupTableView()
{
    cTracer obTracer( "cDlgPatientSelect::setupTableView" );

    refreshTable();

    cDlgCrud::setupTableView();

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_poModel->setHeaderData( 0, Qt::Horizontal, tr( "Id" ) );
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "LicenceId" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Name" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "UniqueId" ) );

        tbvCrud->resizeColumnToContents( 0 );
        tbvCrud->resizeColumnToContents( 1 );
        tbvCrud->resizeColumnToContents( 2 );
        tbvCrud->resizeColumnToContents( 3 );

        tbvCrud->sortByColumn( 2, Qt::AscendingOrder );
    }
    else
    {
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "Name" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "UniqueId" ) );

        tbvCrud->resizeColumnToContents( 1 );
        tbvCrud->resizeColumnToContents( 2 );

        tbvCrud->sortByColumn( 1, Qt::AscendingOrder );
    }
}

void cDlgPatientSelect::refreshTable()
{
    cTracer obTracer( "cDlgPatientSelect::refreshTable" );

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_qsQuery = "SELECT patientId, licenceId, name, uniqueId FROM patients WHERE patientId>0";
    }
    else
    {
        m_qsQuery = "SELECT patientId AS id, name, uniqueId FROM patients WHERE patientId>0 AND active=1";
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
        g_obLogger(e.severity()) << e.what() << cQTLogger::EOM;
    }
}
