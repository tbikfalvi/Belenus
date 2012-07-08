#include "dlgstorno.h"

cDlgStorno::cDlgStorno( QWidget *p_poParent )
    : cDlgCrud( p_poParent )
{
    cTracer obTrace( "cDlgStorno::cDlgStorno" );

    setWindowTitle( tr( "Select actual patient" ) );
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
    lblComment = new QLabel( this );
    lblComment->setObjectName( QString::fromUtf8( "lblComment" ) );
    lblComment->setText( tr("Comment: ") );
    horizontalLayoutTop->addWidget( lblComment );
    ledComment = new QLineEdit( this );
    ledComment->setObjectName( QString::fromUtf8( "ledComment" ) );
    horizontalLayoutTop->addWidget( ledComment );
    horizontalSpacerTop = new QSpacerItem( 13, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    horizontalLayoutTop->addItem( horizontalSpacerTop );
    verticalLayout->insertLayout( 0, horizontalLayoutTop );

    pbStorno = new QPushButton( tr( "Select" ), this );
    pbStorno->setObjectName( QString::fromUtf8( "pbStorno" ) );
    pbStorno->setIconSize( QSize(20, 20) );
    pbStorno->setIcon( QIcon("./resources/40x40_ok.png") );
    btbButtonsSide->addButton( pbStorno, QDialogButtonBox::ActionRole );
    pbExit = new QPushButton( tr( "Exit" ), this );
    pbExit->setObjectName( QString::fromUtf8( "pbExit" ) );
    pbExit->setIconSize( QSize(20, 20) );
    pbExit->setIcon( QIcon("./resources/40x40_cancel.png") );
    btbButtonsSide->addButton( pbExit, QDialogButtonBox::RejectRole );

    setupTableView();

    connect( ledComment, SIGNAL(textChanged(QString)), this, SLOT(refreshTable()) );
    connect( pbStorno, SIGNAL(clicked(bool)), this, SLOT(on_pbStorno_clicked()) );
}

cDlgStorno::~cDlgStorno()
{
    cTracer obTrace( "cDlgStorno::~cDlgStorno" );
}

void cDlgStorno::setSearchPatientName( QString p_stName )
{
    ledComment->setText( p_stName );
}

void cDlgStorno::setupTableView()
{
    cTracer obTracer( "cDlgStorno::setupTableView" );

    refreshTable();

    cDlgCrud::setupTableView();

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_poModel->setHeaderData( 0, Qt::Horizontal, tr( "Id" ) );
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "LicenceId" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Name" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Gender" ) );
        m_poModel->setHeaderData( 4, Qt::Horizontal, tr( "Age group" ) );
        m_poModel->setHeaderData( 5, Qt::Horizontal, tr( "E-Mail" ) );
        m_poModel->setHeaderData( 6, Qt::Horizontal, tr( "Active" ) );
        m_poModel->setHeaderData( 7, Qt::Horizontal, tr( "Archive" ) );

        tbvCrud->resizeColumnToContents( 0 );
        tbvCrud->resizeColumnToContents( 1 );
        tbvCrud->resizeColumnToContents( 2 );
        tbvCrud->resizeColumnToContents( 3 );
        tbvCrud->resizeColumnToContents( 4 );
        tbvCrud->resizeColumnToContents( 5 );
        tbvCrud->resizeColumnToContents( 6 );
        tbvCrud->resizeColumnToContents( 7 );

        tbvCrud->sortByColumn( 2, Qt::AscendingOrder );
    }
    else
    {
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "Name" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Gender" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Age group" ) );
        m_poModel->setHeaderData( 4, Qt::Horizontal, tr( "E-Mail" ) );

        tbvCrud->resizeColumnToContents( 1 );
        tbvCrud->resizeColumnToContents( 2 );
        tbvCrud->resizeColumnToContents( 3 );
        tbvCrud->resizeColumnToContents( 4 );

        tbvCrud->sortByColumn( 1, Qt::AscendingOrder );
    }
}

void cDlgStorno::refreshTable()
{
    cTracer obTracer( "cDlgStorno::refreshTable" );

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_qsQuery = "SELECT patientId, patients.licenceId, name, genderName, ageTypeName, email, active, archive FROM patients, genders, ageTypes WHERE genders.genderId=patients.gender AND agetypes.ageTypeId=ageType AND patientId>0";
    }
    else
    {
        m_qsQuery = "SELECT patientId AS id, name, genderName, ageTypeName, email FROM patients, genders, ageTypes WHERE genders.genderId=patients.gender AND agetypes.ageTypeId=ageType AND patientId>0 AND active=1";
    }

    QString stTemp;

    stTemp = ledComment->text();
    if( stTemp != "" )
    {
        m_qsQuery += " AND ";
        m_qsQuery += QString( "name LIKE '\%%1\%'" ).arg( stTemp );
    }
//    stTemp = ledUniqueId->text();
//    if( stTemp != "" )
//    {
//        m_qsQuery += " AND ";
//        m_qsQuery += QString( "uniqueId LIKE '\%%1\%'" ).arg( stTemp );
//    }

    cDlgCrud::refreshTable();
}

void cDlgStorno::enableButtons()
{
    cTracer obTracer( "cDlgStorno::enableButtons" );

    if( m_uiSelectedId )
    {
        pbStorno->setEnabled( true );
    }
    else
    {
        pbStorno->setEnabled( false );
    }
}

void cDlgStorno::newClicked( bool ) {}
void cDlgStorno::deleteClicked( bool ) {}
void cDlgStorno::editClicked( bool ) {}

void cDlgStorno::on_pbStorno_clicked()
{
    try
    {
        g_obGuest.load( m_uiSelectedId );

        QDialog::accept();
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
    }
}
