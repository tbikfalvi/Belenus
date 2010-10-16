#include <QMessageBox>

#include "belenus.h"
#include "dlgdiscount.h"
#include "../edit/dlgdiscountedit.h"

cDlgDiscount::cDlgDiscount( QWidget *p_poParent )
    : cDlgCrud( p_poParent )
{
    setWindowTitle( tr( "Discounts List" ) );
    setWindowIcon( QIcon("./resources/40x40_discount.png") );

    setupTableView();
}

cDlgDiscount::~cDlgDiscount()
{
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

void cDlgDiscount::refreshTable()
{
    cTracer obTracer( "cDlgDiscount::refreshTable" );

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_qsQuery = "SELECT discountId, licenceId, name, discountValue, discountPercent, active, archive FROM discounts";
    }
    else
    {
        m_qsQuery = "SELECT discountId AS id, name, discountValue, discountPercent FROM discounts WHERE active=1";
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
            poDiscount->remove();
            m_uiSelectedId = 0;
            refreshTable();
            if( poDiscount ) delete poDiscount;
        }
        catch( cSevException &e )
        {
            if( poDiscount ) delete poDiscount;
            g_obLogger(e.severity()) << e.what() << EOM;
        }
    }
}
