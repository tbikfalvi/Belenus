#include <QMessageBox>

#include "belenus.h"
#include "dlgpatientcard.h"
#include "dlgpatientcardtype.h"
#include "../edit/dlgpatientcardedit.h"
#include "../edit/dlgaddunits.h"
#include "../edit/dlgremoveunits.h"
#include "../communication_rfid.h"

extern cCommRFID       *g_poCommRFID;

//===========================================================================================================
cDlgPatientCard::cDlgPatientCard( QWidget *p_poParent ) : cDlgCrud( p_poParent )
//-----------------------------------------------------------------------------------------------------------
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

    chkRFID = new QCheckBox( this );
    chkRFID->setObjectName( QString::fromUtf8( "chkRFID" ) );
    chkRFID->setText( tr("RFID card") );
    horizontalLayout->addWidget( chkRFID );

    chkAssigned = new QCheckBox( this );
    chkAssigned->setObjectName( QString::fromUtf8( "chkAssigned" ) );
    chkAssigned->setText( tr("Assigned card") );
    horizontalLayout->addWidget( chkAssigned );

    chkHasComment = new QCheckBox( this );
    chkHasComment->setObjectName( QString::fromUtf8( "chkRFID" ) );
    chkHasComment->setText( tr("Card with comment") );
    horizontalLayout->addWidget( chkHasComment );

    lblMessage = new QLabel( this );
    lblMessage->setObjectName( QString::fromUtf8( "lblMessage" ) );
    lblMessage->setStyleSheet( "QLabel {font: bold; color: blue;}" );
    lblMessage->setText( "" );
    horizontalLayout->addWidget( lblMessage );

    horizontalSpacer1 = new QSpacerItem( 10, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    horizontalLayout->addItem( horizontalSpacer1 );
    verticalLayout->insertLayout( 0, horizontalLayout );

    pbAddUnits = new QPushButton( tr( "Add units" ), this );
    pbAddUnits->setObjectName( QString::fromUtf8( "pbAddUnits" ) );
    pbAddUnits->setIconSize( QSize(20, 20) );
    pbAddUnits->setIcon( QIcon("./resources/40x40_patientcard.png") );
    btbButtons->addButton( pbAddUnits, QDialogButtonBox::ActionRole );
    connect( pbAddUnits, SIGNAL(clicked()), this, SLOT(_slotAddUnits()) );

    pbRemoveUnits = new QPushButton( tr( "Remove units" ), this );
    pbRemoveUnits->setObjectName( QString::fromUtf8( "pbRemoveUnits" ) );
    pbRemoveUnits->setIconSize( QSize(20, 20) );
    pbRemoveUnits->setIcon( QIcon("./resources/40x40_patientcard.png") );
    btbButtons->addButton( pbRemoveUnits, QDialogButtonBox::ActionRole );
    connect( pbRemoveUnits, SIGNAL(clicked()), this, SLOT(_slotRemoveUnits()) );

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

    connect( ledBarcode, SIGNAL(textChanged(QString)), this, SLOT(refreshTable()) );
    connect( ledOwner, SIGNAL(textChanged(QString)), this, SLOT(refreshTable()) );
    connect( chkRFID, SIGNAL(clicked()), this, SLOT(refreshTable()) );
    connect( chkAssigned, SIGNAL(clicked()), this, SLOT(refreshTable()) );
    connect( chkHasComment, SIGNAL(clicked()), this, SLOT(refreshTable()) );

    m_qsCondition = "";

    m_nTimer = startTimer( 1000 );
}
//===========================================================================================================
cDlgPatientCard::~cDlgPatientCard()
//-----------------------------------------------------------------------------------------------------------
{
    g_poPrefs->setDialogSize( "ListPatientCards", QPoint( width(), height() ) );
}
//===========================================================================================================
void cDlgPatientCard::timerEvent(QTimerEvent *)
//-----------------------------------------------------------------------------------------------------------
{
    if( g_poCommRFID != NULL && g_poCommRFID->isRFIDConnected() )
    {
        QString qsRFID = g_poCommRFID->readRFID();

        if( qsRFID.length() > 0 )
        {
            try
            {
                cDBPatientCard  obDBPatientCard;

                // remove \n\r from the end
                qsRFID = qsRFID.left( qsRFID.length()-2 );

                obDBPatientCard.loadRFID( qsRFID );
                ledBarcode->setText( obDBPatientCard.barcode() );
            }
            catch( cSevException &e )
            {
                g_obLogger(cSeverity::INFO) << "RFID [" << qsRFID << "] not found in database" << EOM;
                g_obGen.showTrayWarning( tr( "Reading card data failed or this card is not registered in database." ) );
            }
            g_obLogger(cSeverity::INFO) << "RFID read [" << qsRFID << "] " << EOM;
        }
    }
}
//===========================================================================================================
void cDlgPatientCard::setupTableView()
//-----------------------------------------------------------------------------------------------------------
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
//===========================================================================================================
void cDlgPatientCard::refreshTable( QString p_qsCondition )
//-----------------------------------------------------------------------------------------------------------
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

    QString stTemp;

    if( ledBarcode->text().length() < g_poPrefs->getBarcodeLength()-2 && ledOwner->text().length() < 3 )
    {
        lblMessage->setText( tr("   >> Please enter part of the barcode or the owner's name! <<") );

        m_qsQuery += " AND ";
        m_qsQuery += QString( "barcode = '9999999999'" );
        cDlgCrud::refreshTable();
        return;
    }
    lblMessage->setText( "" );

    stTemp = ledBarcode->text();
    if( stTemp != "" )
    {
        m_qsQuery += " AND ";
        m_qsQuery += QString( "barcode LIKE '%" + stTemp + "%'" );
    }

    stTemp = ledOwner->text();
    if( stTemp != "" )
    {
        m_qsQuery += " AND ";
        m_qsQuery += QString( "patients.name LIKE '%" + stTemp + "%'" );
    }

    if( chkRFID->isChecked() )
    {
        m_qsQuery += " AND ";
        m_qsQuery += "patientCards.rfid != \"\"";
    }

    if( chkAssigned->isChecked() )
    {
        m_qsQuery += " AND ";
        m_qsQuery += "patientCards.parentCardId > 0";
    }

    if( chkHasComment->isChecked() )
    {
        m_qsQuery += " AND ";
        m_qsQuery += "patientCards.comment != \"\"";
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
//===========================================================================================================
void cDlgPatientCard::enableButtons()
//-----------------------------------------------------------------------------------------------------------
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
    m_poBtnSave->setEnabled( false );
    pbAddUnits->setEnabled( bUserCanModify && g_obUser.isInGroup( cAccessGroup::ADMIN ) && m_uiSelectedId > 1 );
    pbRemoveUnits->setEnabled( bUserCanModify && g_obUser.isInGroup( cAccessGroup::ADMIN ) && m_uiSelectedId > 1 );
    pbPatientCardReplace->setEnabled( bCanBeReplaced );
    pbPartnerCardAssign->setEnabled( bCanBeParent );

    pbAddUnits->setVisible( g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    pbRemoveUnits->setVisible( g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    m_poBtnSave->setVisible( false );
}
//===========================================================================================================
void cDlgPatientCard::newClicked( bool )
//-----------------------------------------------------------------------------------------------------------
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
//===========================================================================================================
void cDlgPatientCard::editClicked( bool )
//-----------------------------------------------------------------------------------------------------------
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
        g_obGen.showTrayError( e.what() );
    }
}
//===========================================================================================================
void cDlgPatientCard::deleteClicked( bool )
//-----------------------------------------------------------------------------------------------------------
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
            g_obGen.showTrayError( e.what() );
        }
    }
}
//===========================================================================================================
bool cDlgPatientCard::_isPatientCardNotForService()
//-----------------------------------------------------------------------------------------------------------
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
        g_obGen.showTrayError( e.what() );
    }

    return bRet;
}
//===========================================================================================================
void cDlgPatientCard::_slotAddUnits()
//-----------------------------------------------------------------------------------------------------------
{
    cDBPatientCard  *poPatientCard = new cDBPatientCard;
    poPatientCard->load( m_uiSelectedId );

    cDlgAddUnits    obDlgAddUnits( this, poPatientCard );

    obDlgAddUnits.exec();

    refreshTable();
}
//===========================================================================================================
void cDlgPatientCard::_slotRemoveUnits()
//-----------------------------------------------------------------------------------------------------------
{
    cDBPatientCard  *poPatientCard = new cDBPatientCard;
    poPatientCard->load( m_uiSelectedId );

    cDlgRemoveUnits    obDlgRemoveUnits( this, poPatientCard );

    obDlgRemoveUnits.exec();

    refreshTable();
}
//===========================================================================================================
void cDlgPatientCard::_slotPatientCardReplace()
//-----------------------------------------------------------------------------------------------------------
{
    cDBPatientCard  *poPatientCard = new cDBPatientCard;
    poPatientCard->load( m_uiSelectedId );

    emit signalReplacePatientCard( poPatientCard->barcode() );

    refreshTable();
}
//===========================================================================================================
void cDlgPatientCard::_slotPartnerCardAssign()
//-----------------------------------------------------------------------------------------------------------
{
    cTracer obTracer( "cDlgPatientCard::_slotPartnerCardAssign" );

    cDBPatientCard  *poPatientCard = new cDBPatientCard;
    poPatientCard->load( m_uiSelectedId );

    emit signalAssignPartnerCard( poPatientCard->barcode() );

    refreshTable();
}
//===========================================================================================================

