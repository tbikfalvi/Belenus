#include "dlgpatientselect.h"

cDlgPatientSelect::cDlgPatientSelect(QWidget *p_poParent , QString p_qsNameFilter) : cDlgCrud( p_poParent )
{
    cTracer obTrace( "cDlgPatientSelect::cDlgPatientSelect" );

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

    lblName = new QLabel( this );
    lblName->setObjectName( QString::fromUtf8( "lblName" ) );
    lblName->setText( tr("Name: ") );
    horizontalLayoutTop->addWidget( lblName );

    ledName = new QLineEdit( this );
    ledName->setObjectName( QString::fromUtf8( "ledName" ) );
    horizontalLayoutTop->addWidget( ledName );

    lblEmail = new QLabel( this );
    lblEmail->setObjectName( QString::fromUtf8( "lblEmail" ) );
    lblEmail->setText( tr("E-Mail: ") );
    horizontalLayoutTop->addWidget( lblEmail );

    ledEmail = new QLineEdit( this );
    ledEmail->setObjectName( QString::fromUtf8( "ledEmail" ) );
    horizontalLayoutTop->addWidget( ledEmail );

    lblMessage = new QLabel( this );
    lblMessage->setObjectName( QString::fromUtf8( "lblMessage" ) );
    lblMessage->setStyleSheet( "QLabel {font: bold; color: blue;}" );
    lblMessage->setText( "" );
    horizontalLayoutTop->addWidget( lblMessage );

    horizontalSpacerTop = new QSpacerItem( 13, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    horizontalLayoutTop->addItem( horizontalSpacerTop );
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

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "ListPatientSelect", QPoint(520,300) );
    resize( qpDlgSize.x(), qpDlgSize.y() );

    if( p_qsNameFilter.length() > 0 )
    {
        ledName->setText( p_qsNameFilter );
    }

    setupTableView();

    connect( ledName, SIGNAL(textChanged(QString)), this, SLOT(refreshTable()) );
    connect( ledEmail, SIGNAL(textChanged(QString)), this, SLOT(refreshTable()) );
    connect( pbSelect, SIGNAL(clicked(bool)), this, SLOT(on_pbSelect_clicked()) );
}

cDlgPatientSelect::~cDlgPatientSelect()
{
    cTracer obTrace( "cDlgPatientSelect::~cDlgPatientSelect" );

    g_poPrefs->setDialogSize( "ListPatientSelect", QPoint( width(), height() ) );
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

void cDlgPatientSelect::refreshTable()
{
    cTracer obTracer( "cDlgPatientSelect::refreshTable" );

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_qsQuery = "SELECT patientId, patients.licenceId, name, genderName, ageTypeName, email, active, archive FROM patients, genders, ageTypes WHERE genders.genderId=patients.gender AND agetypes.ageTypeId=ageType AND patientId>0";
    }
    else
    {
        m_qsQuery = "SELECT patientId AS id, name, genderName, ageTypeName, email FROM patients, genders, ageTypes WHERE genders.genderId=patients.gender AND agetypes.ageTypeId=ageType AND patientId>0 AND active=1";
    }

    if( ledName->text().length() < 3 )
    {
        lblMessage->setText( tr("   >> Please enter part of the guest's name! <<") );

        m_qsQuery += " AND ";
        m_qsQuery += QString( "patients.name = 'ilyen_allat_nem_letezik'" );
        cDlgCrud::refreshTable();
        return;
    }
    lblMessage->setText( "" );

    QString stTemp;

    stTemp = ledName->text();
    if( stTemp != "" )
    {
        m_qsQuery += " AND ";
        m_qsQuery += QString( "name LIKE '%" + stTemp + "%'" );
    }
    stTemp = ledEmail->text();
    if( stTemp != "" )
    {
        m_qsQuery += " AND ";
        m_qsQuery += QString( "email LIKE '%" + stTemp + "%'" );
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
    QDialog::accept();
/*    try
    {
        g_obGuest.load( m_uiSelectedId );

        QDialog::accept();
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
        g_obGen.showTrayError( e.what() );
    }*/
}
