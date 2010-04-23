#include <QMessageBox>

#include "belenus.h"
#include "dlgproducttypes.h"
#include "dlgproducttypeedit.h"

cDlgProductTypes::cDlgProductTypes( QWidget *p_poParent )
    : cDlgCrud( p_poParent )
{
    setWindowTitle( tr( "Product Types List" ) );

    if( g_obUser.isInGroup( "root" ) )
    {
        m_qsQuery = "SELECT productTypeId, name FROM productTypes";
    }
    else
    {
        m_qsQuery = "SELECT productTypeId AS id, name FROM productTypes";
    }

    m_poBtnNew->setEnabled( g_obUser.isInGroup( "admin" ) );

    setupTableView();
}

cDlgProductTypes::~cDlgProductTypes()
{
}

void cDlgProductTypes::setupTableView()
{
    cTracer obTracer( "cDlgProductTypes::setupTableView" );

    refreshTable();

    cDlgCrud::setupTableView();

    if( g_obUser.isInGroup( "root" ) )
    {
        m_poModel->setHeaderData( 0, Qt::Horizontal, tr( "Id" ) );
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "Name" ) );
    }
    else
    {
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "Name" ) );
    }

    tbvCrud->resizeColumnToContents( 1 );
}

void cDlgProductTypes::enableButtons()
{
    cTracer obTracer( "cDlgProductTypes::enableButtons" );

    if( m_uiSelectedId )
    {
        bool boAdmin = g_obUser.isInGroup( "admin" );
        m_poBtnDelete->setEnabled( boAdmin );
        m_poBtnEdit->setEnabled( boAdmin );
    }
    else
    {
        m_poBtnDelete->setEnabled( false );
        m_poBtnEdit->setEnabled( false );
    }
}

void cDlgProductTypes::newClicked( bool )
{
    cDBProductType *poProductType = new cDBProductType;
    poProductType->createNew();

    cDlgProductTypeEdit  obDlgEdit( this, poProductType );
    obDlgEdit.setWindowTitle( tr( "New Product type" ) );
    if( obDlgEdit.exec() == QDialog::Accepted )
    {
        m_uiSelectedId = poProductType->id();
        refreshTable();
    }

    delete poProductType;
}

void cDlgProductTypes::deleteClicked( bool )
{
    if( QMessageBox::question( this, tr( "Confirmation" ),
                               tr( "Are you sure you want to delete this Product type?" ),
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
    {
        QString stQuery = QString( "DELETE FROM productTypes WHERE productTypeId=%1" ).arg( m_uiSelectedId );
        g_poDB->executeQuery( stQuery.toStdString(), true );

        m_uiSelectedId = 0;
        refreshTable();
    }
}

void cDlgProductTypes::editClicked( bool )
{
    cDBProductType  *poProductType = NULL;

    try
    {
        poProductType = new cDBProductType;
        poProductType->load( m_uiSelectedId );

        cDlgProductTypeEdit  obDlgEdit( this, poProductType );
        obDlgEdit.setWindowTitle( QString::fromStdString( poProductType->name() ) );
        if( obDlgEdit.exec() == QDialog::Accepted )
        {
            refreshTable();
        }

        delete poProductType;
    }
    catch( cSevException &e )
    {
        if( poProductType ) delete poProductType;

        g_obLogger << e.severity();
        g_obLogger << e.what() << cQTLogger::EOM;
    }
}

