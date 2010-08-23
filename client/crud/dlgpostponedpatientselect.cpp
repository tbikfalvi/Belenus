#include "dlgpostponedpatientselect.h"
#include "../edit/dlgpatientedit.h"
#include "../db/dbpostponed.h"

cDlgPostponedPatientSelect::cDlgPostponedPatientSelect( QWidget *p_poParent )
    : cDlgCrud( p_poParent )
{
    cTracer obTrace( "cDlgPostponedPatientSelect::cDlgPostponedPatientSelect" );

    setWindowTitle( tr( "Select postponed patient" ) );
    setWindowIcon( QIcon("./resources/40x40_patient.png") );

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
    pbSelect->setIcon( QIcon("./resources/40x40_ok.png") );
    btbButtonsSide->addButton( pbSelect, QDialogButtonBox::ActionRole );
    pbCancel = new QPushButton( tr( "Cancel" ), this );
    pbCancel->setObjectName( QString::fromUtf8( "pbCancel" ) );
    pbCancel->setIconSize( QSize(20, 20) );
    pbCancel->setIcon( QIcon("./resources/40x40_cancel.png") );
    btbButtonsSide->addButton( pbCancel, QDialogButtonBox::RejectRole );

    setupTableView();

    connect( ledName, SIGNAL(textChanged(QString)), this, SLOT(refreshTable()) );
    connect( ledUniqueId, SIGNAL(textChanged(QString)), this, SLOT(refreshTable()) );
    connect( pbSelect, SIGNAL(clicked(bool)), this, SLOT(on_pbSelect_clicked()) );
}

cDlgPostponedPatientSelect::~cDlgPostponedPatientSelect()
{
    cTracer obTrace( "cDlgPostponedPatientSelect::~cDlgPostponedPatientSelect" );
}

void cDlgPostponedPatientSelect::setupTableView()
{
    cTracer obTracer( "cDlgPostponedPatientSelect::setupTableView" );

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

void cDlgPostponedPatientSelect::refreshTable()
{
    cTracer obTracer( "cDlgPostponedPatientSelect::refreshTable" );

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_qsQuery = "SELECT patients.patientId, patients.licenceId, patients.name, patients.uniqueId FROM patients, toBeFilled WHERE patients.patientId>0 AND patients.patientId=toBeFilled.patientId AND toBeFilled.attendanceId=0";
    }
    else
    {
        m_qsQuery = "SELECT patients.patientId AS id, patients.name, patients.uniqueId FROM patients, toBeFilled WHERE patients.patientId>0 AND patients.patientId=toBeFilled.patientId AND patients.active=1 AND toBeFilled.attendanceId=0";
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

void cDlgPostponedPatientSelect::enableButtons()
{
    cTracer obTracer( "cDlgPostponedPatientSelect::enableButtons" );

    if( m_uiSelectedId )
    {
        pbSelect->setEnabled( true );
    }
    else
    {
        pbSelect->setEnabled( false );
    }
}

void cDlgPostponedPatientSelect::newClicked( bool ) {}
void cDlgPostponedPatientSelect::deleteClicked( bool ) {}
void cDlgPostponedPatientSelect::editClicked( bool ) {}

void cDlgPostponedPatientSelect::on_pbSelect_clicked()
{
    cDBPatient  *poPatient = NULL;
    try
    {
        cDBPostponed    *poPostponed = new cDBPostponed;
        poPostponed->loadPatient( m_uiSelectedId );

        poPatient = new cDBPatient;
        poPatient->load( m_uiSelectedId );

        cDlgPatientEdit  obDlgEdit( this, poPatient, poPostponed );
        obDlgEdit.setWindowTitle( poPatient->name() );
        if( obDlgEdit.exec() == QDialog::Accepted )
        {
            poPostponed->removePatient( m_uiSelectedId );
        }
        refreshTable();

        if( poPatient ) delete poPatient;
        if( poPostponed ) delete poPostponed;
    }
    catch( cSevException &e )
    {
        if( poPatient ) delete poPatient;
        g_obLogger(e.severity()) << e.what() << EOM;
    }
}
