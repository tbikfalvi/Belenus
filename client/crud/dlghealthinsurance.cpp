#include <QMessageBox>

#include "belenus.h"
#include "dlghealthinsurance.h"
#include "../edit/dlghealthinsuranceedit.h"

cDlgHealthInsurance::cDlgHealthInsurance( QWidget *p_poParent )
    : cDlgCrud( p_poParent )
{
    setWindowTitle( tr( "Health Insurance Fund List" ) );
    setWindowIcon( QIcon("./resources/40x40_healthinsurance.png") );

    setupTableView();
}

cDlgHealthInsurance::~cDlgHealthInsurance()
{
}

void cDlgHealthInsurance::setupTableView()
{
    cTracer obTracer( "cDlgHealthInsurance::setupTableView" );

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

void cDlgHealthInsurance::refreshTable()
{
    cTracer obTracer( "cDlgHealthInsurance::refreshTable" );

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_qsQuery = "SELECT healthInsuranceId, licenceId, name, active, archive FROM healthInsurances";
    }
    else
    {
        m_qsQuery = "SELECT healthInsuranceId AS id, name FROM healthInsurances WHERE active=1";
    }

    cDlgCrud::refreshTable();
}

void cDlgHealthInsurance::enableButtons()
{
    cTracer obTracer( "cDlgHealthInsurance::enableButtons" );

    m_poBtnNew->setEnabled( g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    m_poBtnEdit->setEnabled( m_uiSelectedId > 0 && g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    m_poBtnDelete->setEnabled( m_uiSelectedId > 0 && g_obUser.isInGroup( cAccessGroup::ADMIN ) );
}

void cDlgHealthInsurance::newClicked( bool )
{
    cDBHealthInsurance *poHealthInsurance = new cDBHealthInsurance;
    poHealthInsurance->createNew();

    cDlgHealthInsuranceEdit  obDlgEdit( this, poHealthInsurance );
    obDlgEdit.setWindowTitle( tr( "New Health Insurance Fund" ) );
    if( obDlgEdit.exec() == QDialog::Accepted )
    {
        m_uiSelectedId = poHealthInsurance->id();
        refreshTable();
    }

    delete poHealthInsurance;
}

void cDlgHealthInsurance::editClicked( bool )
{
    cDBHealthInsurance  *poHealthInsurance = NULL;

    try
    {
        poHealthInsurance = new cDBHealthInsurance;
        poHealthInsurance->load( m_uiSelectedId );

        cDlgHealthInsuranceEdit  obDlgEdit( this, poHealthInsurance );
        obDlgEdit.setWindowTitle( poHealthInsurance->name() );
        if( obDlgEdit.exec() == QDialog::Accepted )
        {
            refreshTable();
        }

        if( poHealthInsurance ) delete poHealthInsurance;
    }
    catch( cSevException &e )
    {
        if( poHealthInsurance ) delete poHealthInsurance;
        g_obLogger(e.severity()) << e.what() << EOM;
    }
}

void cDlgHealthInsurance::deleteClicked( bool )
{
    cDBHealthInsurance  *poHealthInsurance = NULL;

    if( QMessageBox::question( this, tr( "Question" ),
                               tr( "Are you sure you want to delete this Health Insurance Fund?" ),
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
    {
        try
        {
            poHealthInsurance = new cDBHealthInsurance;
            poHealthInsurance->load( m_uiSelectedId );
            if( poHealthInsurance->licenceId() == 0 && !g_obUser.isInGroup( cAccessGroup::ROOT ) && !g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
            {
                QMessageBox::warning( this, tr("Warning"),
                                      tr("You are not allowed to delete studio independent data."));
                return;
            }
            poHealthInsurance->remove();
            m_uiSelectedId = 0;
            refreshTable();
            if( poHealthInsurance ) delete poHealthInsurance;
        }
        catch( cSevException &e )
        {
            if( poHealthInsurance ) delete poHealthInsurance;
            g_obLogger(e.severity()) << e.what() << EOM;
        }
    }
}
