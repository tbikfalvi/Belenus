#include <QMessageBox>

#include "belenus.h"
#include "dlgguest.h"
#include "../edit/dlgguestedit.h"

cDlgGuest::cDlgGuest( QWidget *p_poParent )
    : cDlgCrud( p_poParent )
{
    setWindowTitle( tr( "Guest List" ) );
    setWindowIcon( QIcon("./resources/40x40_patient.png") );

    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName( QString::fromUtf8( "horizontalLayout" ) );
    lblFilterName = new QLabel( this );
    lblFilterName->setObjectName( QString::fromUtf8( "lblFilterName" ) );
    lblFilterName->setText( tr("Guest name: ") );
    horizontalLayout->addWidget( lblFilterName );
    ledFilterName = new QLineEdit( this );
    ledFilterName->setObjectName( QString::fromUtf8( "ledFilterName" ) );
    ledFilterName->setMaximumWidth( 150 );
    horizontalLayout->addWidget( ledFilterName );
    lblFilterGender = new QLabel( this );
    lblFilterGender->setObjectName( QString::fromUtf8( "lblFilterGender" ) );
    lblFilterGender->setText( tr("Gender: ") );
    horizontalLayout->addWidget( lblFilterGender );
    cmbFilterGender = new QComboBox( this );
    cmbFilterGender->setObjectName( QString::fromUtf8( "cmbFilterGender" ) );
    horizontalLayout->addWidget( cmbFilterGender );
    lblFilterAgeType = new QLabel( this );
    lblFilterAgeType->setObjectName( QString::fromUtf8( "lblFilterAgeType" ) );
    lblFilterAgeType->setText( tr("Age group: ") );
    horizontalLayout->addWidget( lblFilterAgeType );
    cmbFilterAgeType = new QComboBox( this );
    cmbFilterAgeType->setObjectName( QString::fromUtf8( "cmbFilterAgeType" ) );
    horizontalLayout->addWidget( cmbFilterAgeType );

    horizontalSpacer1 = new QSpacerItem( 10, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    horizontalLayout->addItem( horizontalSpacer1 );

    verticalLayout->insertLayout( 0, horizontalLayout );

    QSqlQuery   *poQuery;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT genderId, genderName FROM genders" ) );
    while( poQuery->next() )
    {
        cmbFilterGender->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
    }

    poQuery = g_poDB->executeQTQuery( QString( "SELECT ageTypeId, ageTypeName FROM agetypes" ) );
    while( poQuery->next() )
    {
        cmbFilterAgeType->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
    }

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "ListGuests", QPoint(520,300) );
    resize( qpDlgSize.x(), qpDlgSize.y() );

    connect( ledFilterName, SIGNAL(textChanged(QString)), this, SLOT(refreshTable()) );
    connect( cmbFilterGender, SIGNAL(currentIndexChanged(int)), this, SLOT(refreshTable()) );
    connect( cmbFilterAgeType, SIGNAL(currentIndexChanged(int)), this, SLOT(refreshTable()) );

    setupTableView();
}

cDlgGuest::~cDlgGuest()
{
    g_poPrefs->setDialogSize( "ListGuests", QPoint( width(), height() ) );
}

void cDlgGuest::setupTableView()
{
    cTracer obTracer( "cDlgGuest::setupTableView" );

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

void cDlgGuest::refreshTable()
{
    cTracer obTracer( "cDlgGuest::refreshTable" );

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_qsQuery = "SELECT patientId, patients.licenceId, name, genderName, ageTypeName, email, active, archive FROM patients, genders, ageTypes WHERE genders.genderId=patients.gender AND agetypes.ageTypeId=ageType AND patientId>0";
    }
    else
    {
        m_qsQuery = "SELECT patientId AS id, name, genderName, ageTypeName, email FROM patients, genders, ageTypes WHERE genders.genderId=patients.gender AND agetypes.ageTypeId=ageType AND patientId>0 AND active=1";
    }

    if( ledFilterName->text().length() )
    {
        m_qsQuery += QString( " AND name LIKE '\%%1\%'" ).arg( ledFilterName->text() );
    }

    int nFilterId;

    nFilterId = cmbFilterGender->itemData( cmbFilterGender->currentIndex() ).toInt();
    if( nFilterId > 0 )
    {
        m_qsQuery += QString( " AND patients.gender=%1" ).arg( nFilterId );
    }

    nFilterId = cmbFilterAgeType->itemData( cmbFilterAgeType->currentIndex() ).toInt();
    if( nFilterId > 0 )
    {
        m_qsQuery += QString( " AND patients.ageType=%1" ).arg( nFilterId );
    }

    cDlgCrud::refreshTable();
}

void cDlgGuest::enableButtons()
{
    cTracer obTracer( "cDlgGuest::enableButtons" );

    m_poBtnEdit->setEnabled( m_uiSelectedId > 0 );
    m_poBtnDelete->setEnabled( m_uiSelectedId > 0 );
}

void cDlgGuest::newClicked( bool )
{
    cDBGuest *poGuest = new cDBGuest;
    poGuest->createNew();

    cDlgGuestEdit  obDlgEdit( this, poGuest );
    obDlgEdit.setWindowTitle( tr( "New Guest" ) );
    if( obDlgEdit.exec() == QDialog::Accepted )
    {
        m_uiSelectedId = poGuest->id();
    }
    refreshTable();

    delete poGuest;
}

void cDlgGuest::editClicked( bool )
{
    cDBGuest  *poGuest = NULL;

    try
    {
        poGuest = new cDBGuest;
        poGuest->load( m_uiSelectedId );

        cDlgGuestEdit  obDlgEdit( this, poGuest );
        obDlgEdit.setWindowTitle( poGuest->name() );
        obDlgEdit.exec();
        refreshTable();

        if( poGuest ) delete poGuest;
    }
    catch( cSevException &e )
    {
        if( poGuest ) delete poGuest;
        g_obLogger(e.severity()) << e.what() << EOM;
        g_obGen.showTrayError( e.what() );
    }
}

void cDlgGuest::deleteClicked( bool )
{
    cDBGuest  *poGuest = NULL;

    if( QMessageBox::question( this, tr( "Question" ),
                               tr( "Are you sure you want to delete this Guest?\n"
                                   "This action can not be undone!"),
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
    {
        try
        {
            poGuest = new cDBGuest;
            poGuest->load( m_uiSelectedId );
            if( poGuest->licenceId() == 0 && !g_obUser.isInGroup( cAccessGroup::ROOT ) && !g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
            {
                QMessageBox::warning( this, tr("Warning"),
                                      tr("You are not allowed to delete studio independent data."));
                return;
            }
            poGuest->remove();

            m_uiSelectedId = 0;
            refreshTable();
            if( poGuest ) delete poGuest;
        }
        catch( cSevException &e )
        {
            if( poGuest ) delete poGuest;
            g_obLogger(e.severity()) << e.what() << EOM;
            g_obGen.showTrayError( e.what() );
        }
    }
}
