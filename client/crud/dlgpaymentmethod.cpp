#include <QMessageBox>

#include "belenus.h"
#include "dlgpaymentmethod.h"
#include "../edit/dlgpaymentmethodedit.h"

cDlgPaymentMethod::cDlgPaymentMethod( QWidget *p_poParent )
    : cDlgCrud( p_poParent )
{
    setWindowTitle( tr( "Payment Methods List" ) );
    setWindowIcon( QIcon("./resources/40x40_paymentmethod.png") );

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "ListPaymentMethod", QPoint(520,300) );
    resize( qpDlgSize.x(), qpDlgSize.y() );

    setupTableView();
}

cDlgPaymentMethod::~cDlgPaymentMethod()
{
    g_poPrefs->setDialogSize( "ListPaymentMethod", QPoint( width(), height() ) );
}

void cDlgPaymentMethod::setupTableView()
{
    cTracer obTracer( "cDlgPaymentMethod::setupTableView" );

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
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "Sort order" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Name" ) );

        tbvCrud->resizeColumnToContents( 1 );

        tbvCrud->sortByColumn( 1, Qt::AscendingOrder );
    }
}

void cDlgPaymentMethod::refreshTable()
{
    cTracer obTracer( "cDlgPaymentMethod::refreshTable" );

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_qsQuery = "SELECT paymentMethodId, licenceId, name, active, archive FROM paymentmethods";
    }
    else
    {
        m_qsQuery = "SELECT paymentMethodId AS id, paymentMethodId AS sortorder, name FROM paymentmethods WHERE active=1";
    }

    cDlgCrud::refreshTable();
}

void cDlgPaymentMethod::enableButtons()
{
    cTracer obTracer( "cDlgPaymentMethod::enableButtons" );

    m_poBtnNew->setEnabled( g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    m_poBtnEdit->setEnabled( m_uiSelectedId > 2 && g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    m_poBtnDelete->setEnabled( m_uiSelectedId > 2 && g_obUser.isInGroup( cAccessGroup::ADMIN ) );
}

void cDlgPaymentMethod::newClicked( bool )
{
    cDBPaymentMethod *poPaymentMethod = new cDBPaymentMethod;
    poPaymentMethod->createNew();

    cDlgPaymentMethodEdit  obDlgEdit( this, poPaymentMethod );
    obDlgEdit.setWindowTitle( tr( "New Payment Method" ) );
    if( obDlgEdit.exec() == QDialog::Accepted )
    {
        m_uiSelectedId = poPaymentMethod->id();
        refreshTable();
    }

    delete poPaymentMethod;
}

void cDlgPaymentMethod::editClicked( bool )
{
    cDBPaymentMethod  *poPaymentMethod = NULL;

    try
    {
        poPaymentMethod = new cDBPaymentMethod;
        poPaymentMethod->load( m_uiSelectedId );

        cDlgPaymentMethodEdit  obDlgEdit( this, poPaymentMethod );
        obDlgEdit.setWindowTitle( poPaymentMethod->name() );
        if( obDlgEdit.exec() == QDialog::Accepted )
        {
            refreshTable();
        }

        if( poPaymentMethod ) delete poPaymentMethod;
    }
    catch( cSevException &e )
    {
        if( poPaymentMethod ) delete poPaymentMethod;
        g_obLogger(e.severity()) << e.what() << EOM;
    }
}

void cDlgPaymentMethod::deleteClicked( bool )
{
    cDBPaymentMethod  *poPaymentMethod = NULL;

    if( QMessageBox::question( this, tr( "Question" ),
                               tr( "Are you sure you want to delete this Payment Method?" ),
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
    {
        try
        {
            poPaymentMethod = new cDBPaymentMethod;
            poPaymentMethod->load( m_uiSelectedId );
            if( poPaymentMethod->licenceId() == 0 && !g_obUser.isInGroup( cAccessGroup::ROOT ) && !g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
            {
                QMessageBox::warning( this, tr("Warning"),
                                      tr("You are not allowed to delete studio independent data."));
                return;
            }
            poPaymentMethod->remove();
            m_uiSelectedId = 0;
            refreshTable();
            if( poPaymentMethod ) delete poPaymentMethod;
        }
        catch( cSevException &e )
        {
            if( poPaymentMethod ) delete poPaymentMethod;
            g_obLogger(e.severity()) << e.what() << EOM;
        }
    }
}
