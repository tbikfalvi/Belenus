#include <QMessageBox>

#include "belenus.h"
#include "dlgpatientcard.h"
#include "../edit/dlgpatientcardedit.h"

cDlgPatientCard::cDlgPatientCard( QWidget *p_poParent )
    : cDlgCrud( p_poParent )
{
    setWindowTitle( tr( "Patient Card List" ) );
    setWindowIcon( QIcon("./resources/40x40_patientcard.gif") );

    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName( QString::fromUtf8( "horizontalLayout" ) );
    lblPatientCardType = new QLabel( this );
    lblPatientCardType->setObjectName( QString::fromUtf8( "lblPatientCardType" ) );
    lblPatientCardType->setText( tr("Patientcard type: ") );
    horizontalLayout->addWidget( lblPatientCardType );
    cmbPatientCardType = new QComboBox( this );
    cmbPatientCardType->setObjectName( QString::fromUtf8( "cmbPatientCardType" ) );
    cmbPatientCardType->resize( 400, 20 );
    horizontalLayout->addWidget( cmbPatientCardType );

    lblBarcode = new QLabel( this );
    lblBarcode->setObjectName( QString::fromUtf8( "lblBarcode" ) );
    lblBarcode->setText( tr("Barcode: ") );
    horizontalLayout->addWidget( lblBarcode );
    ledBarcode = new QLineEdit( this );
    ledBarcode->setObjectName( QString::fromUtf8( "ledBarcode" ) );
    horizontalLayout->addWidget( ledBarcode );

    horizontalSpacer1 = new QSpacerItem( 10, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    horizontalLayout->addItem( horizontalSpacer1 );
    verticalLayout->insertLayout( 0, horizontalLayout );

    QSqlQuery *poQuery;

    cmbPatientCardType->addItem( tr("<All patientcard type>"), -1 );
    poQuery = g_poDB->executeQTQuery( QString( "SELECT patientCardTypeId, name FROM patientCardTypes WHERE archive<>\"DEL\"" ) );
    while( poQuery->next() )
    {
        cmbPatientCardType->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
    }

    setupTableView();

    connect( cmbPatientCardType, SIGNAL(currentIndexChanged(int)), this, SLOT(refreshTable()) );
    connect( ledBarcode, SIGNAL(textChanged(QString)), this, SLOT(refreshTable()) );
}

cDlgPatientCard::~cDlgPatientCard()
{
}

void cDlgPatientCard::setupTableView()
{
    cTracer obTracer( "cDlgPatientCard::setupTableView" );

    refreshTable();

    cDlgCrud::setupTableView();

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_poModel->setHeaderData( 0, Qt::Horizontal, tr( "Id" ) );
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "LicenceId" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Barcode" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Owner" ) );
        m_poModel->setHeaderData( 4, Qt::Horizontal, tr( "Units" ) );
        m_poModel->setHeaderData( 5, Qt::Horizontal, tr( "Patientcard type" ) );
        m_poModel->setHeaderData( 6, Qt::Horizontal, tr( "All units" ) );
        m_poModel->setHeaderData( 7, Qt::Horizontal, tr( "Active" ) );
        m_poModel->setHeaderData( 8, Qt::Horizontal, tr( "Archive" ) );

        tbvCrud->resizeColumnToContents( 0 );
        tbvCrud->resizeColumnToContents( 1 );
        tbvCrud->resizeColumnToContents( 2 );
        tbvCrud->resizeColumnToContents( 3 );
        tbvCrud->resizeColumnToContents( 4 );
        tbvCrud->resizeColumnToContents( 5 );
        tbvCrud->resizeColumnToContents( 6 );
        tbvCrud->resizeColumnToContents( 7 );
        tbvCrud->resizeColumnToContents( 8 );

        tbvCrud->sortByColumn( 2, Qt::AscendingOrder );
    }
    else
    {
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "Barcode" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Owner" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Units" ) );
        m_poModel->setHeaderData( 4, Qt::Horizontal, tr( "Patientcard type" ) );
        m_poModel->setHeaderData( 5, Qt::Horizontal, tr( "All units" ) );

        tbvCrud->resizeColumnToContents( 1 );
        tbvCrud->resizeColumnToContents( 2 );
        tbvCrud->resizeColumnToContents( 3 );
        tbvCrud->resizeColumnToContents( 4 );
        tbvCrud->resizeColumnToContents( 5 );

        tbvCrud->sortByColumn( 1, Qt::AscendingOrder );
    }
}

void cDlgPatientCard::refreshTable()
{
    cTracer obTracer( "cDlgPatientCard::refreshTable" );

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_qsQuery = "SELECT patientCards.patientCardId, patientCards.licenceId, patientCards.barcode, patients.name, patientCards.units, patientCardTypes.name, patientCardTypes.units, patientCards.active, patientCards.archive FROM patientCards, patientCardTypes, patients WHERE patientCards.patientCardTypeId=patientCardTypes.patientCardTypeId AND patientCards.patientId=patients.patientId";
    }
    else
    {
        m_qsQuery = "SELECT patientCards.patientCardId AS id, patientCards.barcode, patients.name, patientCards.units, patientCardTypes.name, patientCardTypes.units FROM patientCards, patientCardTypes, patients WHERE patientCards.patientCardTypeId=patientCardTypes.patientCardTypeId AND patientCards.patientId=patients.patientId";
    }

    int uiPatientCardTypeId = cmbPatientCardType->itemData( cmbPatientCardType->currentIndex() ).toInt();
    if( uiPatientCardTypeId > -1 )
    {
        m_qsQuery += " AND ";
        m_qsQuery += QString( "patientCards.patientCardTypeId=%1" ).arg( uiPatientCardTypeId );
    }
    QString stTemp;

    stTemp = ledBarcode->text();
    if( stTemp != "" )
    {
        m_qsQuery += " AND ";
        m_qsQuery += QString( "barcode LIKE '\%%1\%'" ).arg( stTemp );
    }

    cDlgCrud::refreshTable();
}

void cDlgPatientCard::enableButtons()
{
    cTracer obTracer( "cDlgPatientCard::enableButtons" );

    m_poBtnNew->setEnabled( g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    m_poBtnEdit->setEnabled( m_uiSelectedId > 0 && g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    m_poBtnDelete->setEnabled( m_uiSelectedId > 0 && g_obUser.isInGroup( cAccessGroup::ADMIN ) );
}

void cDlgPatientCard::newClicked( bool )
{
    cDBPatientCard *poPatientCard = new cDBPatientCard;
    poPatientCard->createNew();

    cDlgPatientCardEdit  obDlgEdit( this, poPatientCard );
    obDlgEdit.setWindowTitle( tr( "New Patientcard" ) );
    if( obDlgEdit.exec() == QDialog::Accepted )
    {
        m_uiSelectedId = poPatientCard->id();
        refreshTable();
    }

    delete poPatientCard;
}

void cDlgPatientCard::editClicked( bool )
{
    cDBPatientCard  *poPatientCard = NULL;

    try
    {
        poPatientCard = new cDBPatientCard;
        poPatientCard->load( m_uiSelectedId );

        cDlgPatientCardEdit  obDlgEdit( this, poPatientCard );
        obDlgEdit.setWindowTitle( poPatientCard->barcode() );
        if( obDlgEdit.exec() == QDialog::Accepted )
        {
            refreshTable();
        }

        if( poPatientCard ) delete poPatientCard;
    }
    catch( cSevException &e )
    {
        if( poPatientCard ) delete poPatientCard;

        g_obLogger << e.severity();
        g_obLogger << e.what() << cQTLogger::EOM;
    }
}

void cDlgPatientCard::deleteClicked( bool )
{
    cDBPatientCard  *poPatientCard = NULL;

    if( QMessageBox::question( this, tr( "Question" ),
                               tr( "Are you sure you want to delete this Patientcard?" ),
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
    {
        try
        {
            poPatientCard = new cDBPatientCard;
            poPatientCard->load( m_uiSelectedId );
            poPatientCard->remove();
            m_uiSelectedId = 0;
            refreshTable();
            if( poPatientCard ) delete poPatientCard;
        }
        catch( cSevException &e )
        {
            if( poPatientCard ) delete poPatientCard;

            g_obLogger << e.severity();
            g_obLogger << e.what() << cQTLogger::EOM;
        }
    }
}
