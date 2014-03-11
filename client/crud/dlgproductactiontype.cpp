#include <QMessageBox>

#include "belenus.h"
#include "dlgproduct.h"
#include "dlgproducttype.h"
#include "dlgproductactiontype.h"
#include "../edit/dlgproductactiontypeedit.h"
#include "../db/dbproduct.h"

cDlgProductActionType::cDlgProductActionType( QWidget *p_poParent )
    : cDlgCrud( p_poParent )
{
    setWindowTitle( tr( "Productaction Type List" ) );
    setWindowIcon( QIcon("./resources/40x40_productactiontype.png") );

    m_poParent = p_poParent;

    pbProduct = new QPushButton( tr( "Products" ), this );
    pbProduct->setObjectName( QString::fromUtf8( "pbProduct" ) );
    pbProduct->setIconSize( QSize(20, 20) );
    pbProduct->setIcon( QIcon("./resources/40x40_product.png") );
    btbButtonsSide->addButton( pbProduct, QDialogButtonBox::ActionRole );
    connect( pbProduct, SIGNAL(clicked()), this, SLOT(_slotProduct()) );
    pbProductType = new QPushButton( tr( "Product types" ), this );
    pbProductType->setObjectName( QString::fromUtf8( "pbProductType" ) );
    pbProductType->setIconSize( QSize(20, 20) );
    pbProductType->setIcon( QIcon("./resources/40x40_producttype.png") );
    btbButtonsSide->addButton( pbProductType, QDialogButtonBox::ActionRole );
    connect( pbProductType, SIGNAL(clicked()), this, SLOT(_slotProductTypes()) );

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "ListProductTypeActions", QPoint(520,300) );
    resize( qpDlgSize.x(), qpDlgSize.y() );

    setupTableView();
}

cDlgProductActionType::~cDlgProductActionType()
{
    g_poPrefs->setDialogSize( "ListProductTypeActions", QPoint( width(), height() ) );
}

void cDlgProductActionType::setupTableView()
{
    cTracer obTracer( "cDlgProductActionType::setupTableView" );

    refreshTable();

    cDlgCrud::setupTableView();

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_poModel->setHeaderData( 0, Qt::Horizontal, tr( "Id" ) );
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "LicenceId" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Name" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Active" ) );
        m_poModel->setHeaderData( 4, Qt::Horizontal, tr( "Archive" ) );

        tbvCrud->resizeColumnToContents( 0 );
        tbvCrud->resizeColumnToContents( 1 );
        tbvCrud->resizeColumnToContents( 2 );
        tbvCrud->resizeColumnToContents( 3 );
        tbvCrud->resizeColumnToContents( 4 );

        tbvCrud->sortByColumn( 2, Qt::AscendingOrder );
    }
    else
    {
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "Name" ) );

        tbvCrud->resizeColumnToContents( 1 );

        tbvCrud->sortByColumn( 1, Qt::AscendingOrder );
    }
}

void cDlgProductActionType::refreshTable()
{
    cTracer obTracer( "cDlgProductActionType::refreshTable" );

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_qsQuery = "SELECT productActionTypeId, licenceId, name, active, archive FROM productActionType";
    }
    else
    {
        m_qsQuery = "SELECT productActionTypeId AS id, name FROM productActionType WHERE active=1";
    }

    cDlgCrud::refreshTable();
}

void cDlgProductActionType::enableButtons()
{
    cTracer obTracer( "cDlgProductActionType::enableButtons" );

    m_poBtnNew->setEnabled( g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    m_poBtnEdit->setEnabled( m_uiSelectedId > 3 && g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    m_poBtnDelete->setEnabled( m_uiSelectedId > 3 && g_obUser.isInGroup( cAccessGroup::ADMIN ) );
}

void cDlgProductActionType::newClicked( bool )
{
    cDBProductActionType *poProductActionType = new cDBProductActionType;
    poProductActionType->createNew();

    cDlgProductActionTypeEdit  obDlgEdit( this, poProductActionType );
    obDlgEdit.setWindowTitle( tr( "New Productaction Type" ) );
    if( obDlgEdit.exec() == QDialog::Accepted )
    {
        m_uiSelectedId = poProductActionType->id();
        refreshTable();
    }

    delete poProductActionType;
}

void cDlgProductActionType::editClicked( bool )
{
    cDBProductActionType  *poProductActionType = NULL;

    try
    {
        poProductActionType = new cDBProductActionType;
        poProductActionType->load( m_uiSelectedId );

        cDlgProductActionTypeEdit  obDlgEdit( this, poProductActionType );
        obDlgEdit.setWindowTitle( poProductActionType->name() );
        if( obDlgEdit.exec() == QDialog::Accepted )
        {
            refreshTable();
        }

        if( poProductActionType ) delete poProductActionType;
    }
    catch( cSevException &e )
    {
        if( poProductActionType ) delete poProductActionType;
        g_obLogger(e.severity()) << e.what() << EOM;
    }
}

void cDlgProductActionType::deleteClicked( bool )
{
    cDBProductActionType  *poProductActionType = new cDBProductActionType;
    poProductActionType->load( m_uiSelectedId );

    QString     qsQuestion = tr( "Are you sure you want to delete this Productaction Type?" );

    if( QMessageBox::question( this, tr( "Question" ),
                               qsQuestion,
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
    {
        try
        {
            if( poProductActionType->licenceId() == 0 && !g_obUser.isInGroup( cAccessGroup::ROOT ) && !g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
            {
                QMessageBox::warning( this, tr("Warning"),
                                      tr("You are not allowed to delete studio independent data."));
                return;
            }
            poProductActionType->remove();
            m_uiSelectedId = 0;
            refreshTable();
            if( poProductActionType ) delete poProductActionType;
        }
        catch( cSevException &e )
        {
            if( poProductActionType ) delete poProductActionType;
            g_obLogger(e.severity()) << e.what() << EOM;
        }
    }
}

void cDlgProductActionType::_slotProduct()
{
    cDlgProduct obDlgProduct( m_poParent );

    QDialog::accept();
    obDlgProduct.exec();
}

void cDlgProductActionType::_slotProductTypes()
{
    cDlgProductType   obDlgProductType( m_poParent );

    QDialog::accept();
    obDlgProductType.exec();
}

