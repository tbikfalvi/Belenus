#include <QMessageBox>

#include "belenus.h"
#include "dlgpatientcard.h"
#include "dlgpatientcardtype.h"
#include "../edit/dlgpatientcardedit.h"

cDlgPatientCard::cDlgPatientCard( QWidget *p_poParent ) : cDlgCrud( p_poParent )
{
    setWindowTitle( tr( "Patient Card List" ) );
    setWindowIcon( QIcon("./resources/40x40_patientcard.png") );

    m_poParent = p_poParent;

    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName( QString::fromUtf8( "horizontalLayout" ) );
//    lblPatientCardType = new QLabel( this );
//    lblPatientCardType->setObjectName( QString::fromUtf8( "lblPatientCardType" ) );
//    lblPatientCardType->setText( tr("Patientcard type: ") );
//    horizontalLayout->addWidget( lblPatientCardType );
//    cmbPatientCardType = new QComboBox( this );
//    cmbPatientCardType->setObjectName( QString::fromUtf8( "cmbPatientCardType" ) );
//    cmbPatientCardType->resize( 400, 20 );
//    horizontalLayout->addWidget( cmbPatientCardType );

    lblBarcode = new QLabel( this );
    lblBarcode->setObjectName( QString::fromUtf8( "lblBarcode" ) );
    lblBarcode->setText( tr("Barcode: ") );
    horizontalLayout->addWidget( lblBarcode );
    ledBarcode = new QLineEdit( this );
    ledBarcode->setObjectName( QString::fromUtf8( "ledBarcode" ) );
    ledBarcode->setMinimumWidth( 100 );
    ledBarcode->setMaximumWidth( 100 );
    horizontalLayout->addWidget( ledBarcode );

    lblOwner = new QLabel( this );
    lblOwner->setObjectName( QString::fromUtf8( "lblOwner" ) );
    lblOwner->setText( tr("Owner: ") );
    horizontalLayout->addWidget( lblOwner );
    ledOwner = new QLineEdit( this );
    ledOwner->setObjectName( QString::fromUtf8( "ledOwner" ) );
    ledOwner->setMinimumWidth( 100 );
    ledOwner->setMaximumWidth( 100 );
    horizontalLayout->addWidget( ledOwner );

    horizontalSpacer1 = new QSpacerItem( 10, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    horizontalLayout->addItem( horizontalSpacer1 );
    verticalLayout->insertLayout( 0, horizontalLayout );

/*    QSqlQuery   *poQuery;
    QString      qsQuery;

    cmbPatientCardType->addItem( tr("<All patientcard types>"), -1 );
    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        qsQuery = QString( "SELECT patientCardTypeId, name FROM patientCardTypes WHERE archive<>\"DEL\"" );
    }
    else
    {
        qsQuery = QString( "SELECT patientCardTypeId, name FROM patientCardTypes WHERE ((patientCardTypeId>0 AND licenceId!=0) OR (patientCardTypeId=0 AND licenceId=0)) AND active=1 AND archive<>\"DEL\"" );
    }

    poQuery = g_poDB->executeQTQuery( qsQuery );
    while( poQuery->next() )
    {
        cmbPatientCardType->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
    }*/
/*
    pbPatientCardType = new QPushButton( tr( "Patientcard types" ), this );
    pbPatientCardType->setObjectName( QString::fromUtf8( "pbPatientCardType" ) );
    pbPatientCardType->setIconSize( QSize(20, 20) );
    pbPatientCardType->setIcon( QIcon("./resources/40x40_patientcardtype.png") );
    btbButtonsSide->addButton( pbPatientCardType, QDialogButtonBox::RejectRole );
    connect( pbPatientCardType, SIGNAL(clicked()), this, SLOT(_slotPatientCardTypes()) );
*/
    pbPatientCardReplace = new QPushButton( tr( "Replace lost" ), this );
    pbPatientCardReplace->setObjectName( QString::fromUtf8( "pbPatientCardReplace" ) );
    pbPatientCardReplace->setIconSize( QSize(20, 20) );
    pbPatientCardReplace->setIcon( QIcon("./resources/40x40_patientcard_sell.png") );
    btbButtons->addButton( pbPatientCardReplace, QDialogButtonBox::ActionRole );
    connect( pbPatientCardReplace, SIGNAL(clicked()), this, SLOT(_slotPatientCardReplace()) );

    pbPartnerCardAssign = new QPushButton( tr( "Assign partner" ), this );
    pbPartnerCardAssign->setObjectName( QString::fromUtf8( "pbPartnerCardAssign" ) );
    pbPartnerCardAssign->setIconSize( QSize(20, 20) );
    pbPartnerCardAssign->setIcon( QIcon("./resources/40x40_patientcard_sell.png") );
    btbButtons->addButton( pbPartnerCardAssign, QDialogButtonBox::ActionRole );
    connect( pbPartnerCardAssign, SIGNAL(clicked()), this, SLOT(_slotPartnerCardAssign()) );

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "ListPatientCards", QPoint(520,300) );
    resize( qpDlgSize.x(), qpDlgSize.y() );

    setupTableView();

//    connect( cmbPatientCardType, SIGNAL(currentIndexChanged(int)), this, SLOT(refreshTable()) );
    connect( ledBarcode, SIGNAL(textChanged(QString)), this, SLOT(refreshTable()) );
    connect( ledOwner, SIGNAL(textChanged(QString)), this, SLOT(refreshTable()) );

    m_qsCondition = "";
}

cDlgPatientCard::~cDlgPatientCard()
{
    g_poPrefs->setDialogSize( "ListPatientCards", QPoint( width(), height() ) );
}

void cDlgPatientCard::setupTableView()
{
//    cTracer obTracer( "cDlgPatientCard::setupTableView" );

    refreshTable();

    cDlgCrud::setupTableView();

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_poModel->setHeaderData( 0, Qt::Horizontal, tr( "Id" ) );
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "LicenceId" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Barcode" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Owner" ) );
        m_poModel->setHeaderData( 4, Qt::Horizontal, tr( "Valid from" ) );
        m_poModel->setHeaderData( 5, Qt::Horizontal, tr( "Valid to" ) );
        m_poModel->setHeaderData( 6, Qt::Horizontal, tr( "Comment" ) );
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
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Valid from" ) );
        m_poModel->setHeaderData( 4, Qt::Horizontal, tr( "Valid to" ) );
        m_poModel->setHeaderData( 5, Qt::Horizontal, tr( "Comment" ) );
        m_poModel->setHeaderData( 6, Qt::Horizontal, tr( "Active" ) );

        tbvCrud->resizeColumnToContents( 1 );
        tbvCrud->resizeColumnToContents( 2 );
        tbvCrud->resizeColumnToContents( 3 );
        tbvCrud->resizeColumnToContents( 4 );
        tbvCrud->resizeColumnToContents( 5 );
        tbvCrud->resizeColumnToContents( 6 );

        tbvCrud->sortByColumn( 1, Qt::AscendingOrder );
    }
}

void cDlgPatientCard::refreshTable( QString p_qsCondition )
{
//    cTracer obTracer( "cDlgPatientCard::refreshTable" );

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_qsQuery = "SELECT patientCards.patientCardId, patientCards.licenceId, patientCards.barcode, patients.name, patientCards.validDateFrom, patientCards.validDateTo, patientCards.comment, patientCards.active, patientCards.archive FROM patientCards, patientCardTypes, patients WHERE patientCards.patientCardTypeId=patientCardTypes.patientCardTypeId AND patientCards.patientId=patients.patientId";
    }
    else
    {
        m_qsQuery = "SELECT patientCards.patientCardId AS id, patientCards.barcode, patients.name, patientCards.validDateFrom, patientCards.validDateTo, patientCards.comment, patientCards.active FROM patientCards, patientCardTypes, patients WHERE patientCards.patientCardTypeId=patientCardTypes.patientCardTypeId AND patientCards.patientId=patients.patientId AND patientCards.patientCardId>0";
    }

    /*int uiPatientCardTypeId = cmbPatientCardType->itemData( cmbPatientCardType->currentIndex() ).toInt();
    if( uiPatientCardTypeId > -1 )
    {
        m_qsQuery += " AND ";
        m_qsQuery += QString( "patientCards.patientCardTypeId=%1" ).arg( uiPatientCardTypeId );
    }*/
    QString stTemp;

    stTemp = ledBarcode->text();
    if( stTemp != "" )
    {
        m_qsQuery += " AND ";
        m_qsQuery += QString( "barcode LIKE '\%%1\%'" ).arg( stTemp );
    }

    stTemp = ledOwner->text();
    if( stTemp != "" )
    {
        m_qsQuery += " AND ";
        m_qsQuery += QString( "patients.name LIKE '\%%1\%'" ).arg( stTemp );
    }

    if( p_qsCondition != "" )
        m_qsCondition = p_qsCondition;

    if( m_qsCondition != "" )
    {
        m_qsQuery += " AND ";
        m_qsQuery += m_qsCondition;
    }

    cDlgCrud::refreshTable();
}

void cDlgPatientCard::enableButtons()
{
//    cTracer obTracer( "cDlgPatientCard::enableButtons" );

    bool bUserCanModify = false;
    bool bCanBeReplaced = false;
    bool bCanBeParent   = false;

    if( m_uiSelectedId > 0 )
    {
        if( g_obUser.isInGroup( cAccessGroup::ADMIN ) && _isPatientCardNotForService() )
        {
            bUserCanModify = true;
        }
        else if( g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
        {
            bUserCanModify = true;
        }

        cDBPatientCard  *poPatientCard = new cDBPatientCard;
        poPatientCard->load( m_uiSelectedId );
        bCanBeReplaced = poPatientCard->isPatientCardCanBeReplaced();
        bCanBeParent = poPatientCard->isPatientCardCanBeParent();

        if( poPatientCard->pincode().compare("LOST") == 0 )
        {
            bUserCanModify = false;
        }
    }

    m_poBtnNew->setEnabled( g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    m_poBtnEdit->setEnabled( bUserCanModify );
    m_poBtnDelete->setEnabled( bUserCanModify && m_uiSelectedId > 1 );
    pbPatientCardReplace->setEnabled( bCanBeReplaced );
    pbPartnerCardAssign->setEnabled( bCanBeParent );
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
    }
    refreshTable();

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
        obDlgEdit.exec();
        refreshTable();

        if( poPatientCard ) delete poPatientCard;
    }
    catch( cSevException &e )
    {
        if( poPatientCard ) delete poPatientCard;
        g_obLogger(e.severity()) << e.what() << EOM;
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
            if( poPatientCard->licenceId() == 0 && !g_obUser.isInGroup( cAccessGroup::ROOT ) && !g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
            {
                QMessageBox::warning( this, tr("Warning"),
                                      tr("You are not allowed to delete studio independent data."));
                return;
            }
            poPatientCard->deactivate();
            refreshTable();
            if( poPatientCard ) delete poPatientCard;
        }
        catch( cSevException &e )
        {
            if( poPatientCard ) delete poPatientCard;
            g_obLogger(e.severity()) << e.what() << EOM;
        }
    }
}

bool cDlgPatientCard::_isPatientCardNotForService()
{
    bool             bRet           = true;
    cDBPatientCard  *poPatientCard  = NULL;

    try
    {
        poPatientCard = new cDBPatientCard;
        poPatientCard->load( m_uiSelectedId );

        if( poPatientCard->isServiceCard() )
        {
            bRet = false;
        }

        if( poPatientCard ) delete poPatientCard;
    }
    catch( cSevException &e )
    {
        if( poPatientCard ) delete poPatientCard;
        g_obLogger(e.severity()) << e.what() << EOM;
    }

    return bRet;
}
/*
void cDlgPatientCard::_slotPatientCardTypes()
{
    cDlgPatientCardType   obDlgPatientCardType( m_poParent );

    QDialog::accept();
    obDlgPatientCardType.exec();
}
*/
void cDlgPatientCard::_slotPatientCardReplace()
{
    cDBPatientCard  *poPatientCard = new cDBPatientCard;
    poPatientCard->load( m_uiSelectedId );

    emit signalReplacePatientCard( poPatientCard->barcode() );

    refreshTable();
}

void cDlgPatientCard::_slotPartnerCardAssign()
{
    cTracer obTracer( "cDlgPatientCard::_slotPartnerCardAssign" );

    cDBPatientCard  *poPatientCard = new cDBPatientCard;
    poPatientCard->load( m_uiSelectedId );

    emit signalAssignPartnerCard( poPatientCard->barcode() );

    refreshTable();
}

