#include <QMessageBox>

#include "belenus.h"
#include "dlgdiscount.h"
#include "../edit/dlgdiscountedit.h"

cDlgDiscount::cDlgDiscount( QWidget *p_poParent ) : cDlgCrud( p_poParent )
{
    setWindowTitle( tr( "Discounts List" ) );
    setWindowIcon( QIcon("./resources/40x40_discount.png") );

    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName( QString::fromUtf8( "horizontalLayout" ) );
    rbFilterAll = new QRadioButton( tr("All discounts"), this );
    rbFilterAll->setObjectName( QString::fromUtf8( "rbFilterAll" ) );
    horizontalLayout->addWidget( rbFilterAll );
    rbFilterGeneral = new QRadioButton( tr("General discounts"), this );
    rbFilterGeneral->setObjectName( QString::fromUtf8( "rbFilterGeneral" ) );
    horizontalLayout->addWidget( rbFilterGeneral );
    rbFilterGuest = new QRadioButton( tr("Guest discounts"), this );
    rbFilterGuest->setObjectName( QString::fromUtf8( "rbFilterGuest" ) );
    horizontalLayout->addWidget( rbFilterGuest );
    rbFilterProduct = new QRadioButton( tr("Product discounts"), this );
    rbFilterProduct->setObjectName( QString::fromUtf8( "rbFilterProduct" ) );
    horizontalLayout->addWidget( rbFilterProduct );
    lblFilterName = new QLabel( this );
    lblFilterName->setObjectName( QString::fromUtf8( "lblFilterName" ) );
    lblFilterName->setText( tr("Name: ") );
    horizontalLayout->addWidget( lblFilterName );
    ledFilterName = new QLineEdit( this );
    ledFilterName->setObjectName( QString::fromUtf8( "ledFilterName" ) );
    horizontalLayout->addWidget( ledFilterName );

    rbFilterAll->setChecked( true );
    ledFilterName->setEnabled( false );

    connect( rbFilterAll, SIGNAL(clicked()), this, SLOT(slotFilterRadioClicked()) );
    connect( rbFilterGeneral, SIGNAL(clicked()), this, SLOT(slotFilterRadioClicked()) );
    connect( rbFilterGuest, SIGNAL(clicked()), this, SLOT(slotFilterRadioClicked()) );
    connect( rbFilterProduct, SIGNAL(clicked()), this, SLOT(slotFilterRadioClicked()) );
    connect( ledFilterName, SIGNAL(textChanged(QString)), this, SLOT(slotFilterNameChanged()) );

    horizontalSpacer1 = new QSpacerItem( 10, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    horizontalLayout->addItem( horizontalSpacer1 );

    verticalLayout->insertLayout( 0, horizontalLayout );

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "ListDiscounts", QPoint(400,300) );
    resize( qpDlgSize.x(), qpDlgSize.y() );

    setupTableView();
}

cDlgDiscount::~cDlgDiscount()
{
    g_poPrefs->setDialogSize( "ListDiscounts", QPoint( width(), height() ) );
}

void cDlgDiscount::setupTableView()
{
    cTracer obTracer( "cDlgDiscount::setupTableView" );

    refreshTable();

    cDlgCrud::setupTableView();

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_poModel->setHeaderData( 0, Qt::Horizontal, tr( "Id" ) );
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "LicenceId" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Name" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Exact value" ) );
        m_poModel->setHeaderData( 4, Qt::Horizontal, tr( "Percent" ) );
        m_poModel->setHeaderData( 5, Qt::Horizontal, tr( "Active" ) );
        m_poModel->setHeaderData( 6, Qt::Horizontal, tr( "Archive" ) );

        tbvCrud->resizeColumnToContents( 0 );
        tbvCrud->resizeColumnToContents( 1 );
        tbvCrud->resizeColumnToContents( 2 );
        tbvCrud->resizeColumnToContents( 3 );
        tbvCrud->resizeColumnToContents( 4 );
        tbvCrud->resizeColumnToContents( 5 );
        tbvCrud->resizeColumnToContents( 6 );

        tbvCrud->sortByColumn( 2, Qt::AscendingOrder );
    }
    else
    {
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "Name" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Exact value" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Percent" ) );

        tbvCrud->resizeColumnToContents( 1 );
        tbvCrud->resizeColumnToContents( 2 );
        tbvCrud->resizeColumnToContents( 3 );

        tbvCrud->sortByColumn( 1, Qt::AscendingOrder );
    }
}

void cDlgDiscount::slotFilterRadioClicked()
{
    if( rbFilterAll->isChecked() || rbFilterGeneral->isChecked() )
    {
        ledFilterName->setText( "" );
        ledFilterName->setEnabled( false );
    }
    else if( rbFilterGuest->isChecked() || rbFilterProduct->isChecked() )
    {
        ledFilterName->setText( "" );
        ledFilterName->setEnabled( true );
    }
    refreshTable();
}

void cDlgDiscount::slotFilterNameChanged()
{
    refreshTable();
}

void cDlgDiscount::refreshTable()
{
    cTracer obTracer( "cDlgDiscount::refreshTable" );

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_qsQuery = "SELECT discountId, licenceId, name, discountValue/100, discountPercent, active, archive FROM discounts WHERE discountId>0";
    }
    else
    {
        m_qsQuery = "SELECT discountId AS id, name, discountValue/100, discountPercent FROM discounts WHERE discountId>0 AND active=1";
    }

    if( rbFilterGeneral->isChecked() )
    {
        m_qsQuery += QString( " AND (regularCustomer=1 OR employee=1 OR service=1)" );
    }
    else if( rbFilterGuest->isChecked() )
    {
        m_qsQuery += QString( " AND patientId>0 AND name LIKE '%" + ledFilterName->text() + "%'" );
    }
    else if( rbFilterProduct->isChecked() )
    {
        m_qsQuery += QString( " AND productId>0 AND name LIKE '%" + ledFilterName->text() + "%'" );
    }

    cDlgCrud::refreshTable();
}

void cDlgDiscount::enableButtons()
{
    cTracer obTracer( "cDlgDiscount::enableButtons" );

    m_poBtnNew->setEnabled( g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    m_poBtnEdit->setEnabled( m_uiSelectedId > 0 && g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    m_poBtnDelete->setEnabled( m_uiSelectedId > 0 && g_obUser.isInGroup( cAccessGroup::ADMIN ) );
}

void cDlgDiscount::newClicked( bool )
{
    cDBDiscount *poDiscount = new cDBDiscount;
    poDiscount->createNew();

    cDlgDiscountEdit  obDlgEdit( this, poDiscount );
    obDlgEdit.setWindowTitle( tr( "New Discount" ) );
    if( obDlgEdit.exec() == QDialog::Accepted )
    {
        m_uiSelectedId = poDiscount->id();
        refreshTable();
    }

    delete poDiscount;
}

void cDlgDiscount::editClicked( bool )
{
    cDBDiscount  *poDiscount = NULL;

    try
    {
        poDiscount = new cDBDiscount;
        poDiscount->load( m_uiSelectedId );

        cDlgDiscountEdit  obDlgEdit( this, poDiscount );
        obDlgEdit.setWindowTitle( poDiscount->name() );
        if( obDlgEdit.exec() == QDialog::Accepted )
        {
            refreshTable();
        }

        if( poDiscount ) delete poDiscount;
    }
    catch( cSevException &e )
    {
        if( poDiscount ) delete poDiscount;
        g_obLogger(e.severity()) << e.what() << EOM;
        g_obGen.showTrayError( e.what() );
    }
}

void cDlgDiscount::deleteClicked( bool )
{
    cDBDiscount  *poDiscount = NULL;

    if( QMessageBox::question( this, tr( "Question" ),
                               tr( "Are you sure you want to delete this Discount?" ),
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
    {
        try
        {
            poDiscount = new cDBDiscount;
            poDiscount->load( m_uiSelectedId );
            if( poDiscount->licenceId() == 0 && !g_obUser.isInGroup( cAccessGroup::ROOT ) && !g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
            {
                QMessageBox::warning( this, tr("Warning"),
                                      tr("You are not allowed to delete studio independent data."));
                return;
            }
            poDiscount->remove();
            m_uiSelectedId = 0;
            refreshTable();
            if( poDiscount ) delete poDiscount;
        }
        catch( cSevException &e )
        {
            if( poDiscount ) delete poDiscount;
            g_obLogger(e.severity()) << e.what() << EOM;
            g_obGen.showTrayError( e.what() );
        }
    }
}

