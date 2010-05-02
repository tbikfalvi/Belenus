#include <QMessageBox>

#include "belenus.h"
#include "dlgpaneltypes.h"
#include "dlgpaneltypeedit.h"

cDlgPanelTypes::cDlgPanelTypes( QWidget *p_poParent )
    : cDlgCrud( p_poParent )
{
    setWindowTitle( tr( "Panel Types List" ) );

    if( g_obUser.isInGroup( "root" ) )
    {
        m_qsQuery = "SELECT panelTypeId, name FROM panelTypes";
    }
    else
    {
        m_qsQuery = "SELECT panelTypeId AS id, name FROM panelTypes";
    }

    m_poBtnNew->setEnabled( g_obUser.isInGroup( "admin" ) );

    setupTableView();
}

cDlgPanelTypes::~cDlgPanelTypes()
{
}

void cDlgPanelTypes::setupTableView()
{
    cTracer obTracer( "cDlgPanelTypes::setupTableView" );

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
}

void cDlgPanelTypes::enableButtons()
{
    cTracer obTracer( "cDlgPanelTypes::enableButtons" );

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

void cDlgPanelTypes::newClicked( bool )
{
    cDlgPanelTypeEdit  obDlgEdit( this );
    obDlgEdit.setWindowTitle( tr( "New Panel Type" ) );
    if( obDlgEdit.exec() == QDialog::Accepted )
    {
        QSqlQuery  *poQuery = NULL;
        try
        {
            QString qsQuery = QString( "INSERT INTO panelTypes (licenceId, name) VALUES (%1, \"%2\")" ).arg( g_poPrefs->getLicenceId() ).arg( obDlgEdit.panelTypeName() );
            poQuery = g_poDB->executeQTQuery( qsQuery );
            m_uiSelectedId = poQuery->lastInsertId().toUInt();
            delete poQuery;
        }
        catch( cSevException &e )
        {
            if( poQuery ) delete poQuery;

            g_obLogger << e.severity();
            g_obLogger << e.what() << cQTLogger::EOM;
        }

        refreshTable();
    }
}

void cDlgPanelTypes::deleteClicked( bool )
{
    if( QMessageBox::question( this, tr( "Confirmation" ),
                               tr( "Are you sure you want to delete this Panel Type?" ),
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
    {
        try
        {
            QString stQuery = QString( "DELETE FROM panelTypes WHERE panelTypeId=%1" ).arg( m_uiSelectedId );
            g_poDB->executeQuery( stQuery.toStdString(), true );

            m_uiSelectedId = 0;
            refreshTable();
        }
        catch( cSevException &e )
        {
            g_obLogger << e.severity();
            g_obLogger << e.what() << cQTLogger::EOM;
        }
    }
}

void cDlgPanelTypes::editClicked( bool )
{
    try
    {
        cDlgPanelTypeEdit  obDlgEdit( this );
        obDlgEdit.setWindowTitle( tr( "Edit Panel Type" ) );
        obDlgEdit.setPanelTypeName( m_poModel->index( m_inSelectedRow, 1 ).data().toString() );
        if( obDlgEdit.exec() == QDialog::Accepted )
        {
            QString qsQuery = QString( "UPDATE panelTypes SET name=\"%1\" WHERE panelTypeId=%2" ).arg( obDlgEdit.panelTypeName() ).arg( m_uiSelectedId );
            g_poDB->executeQuery( qsQuery.toStdString(), true );

            refreshTable();
        }
    }
    catch( cSevException &e )
    {
        g_obLogger << e.severity();
        g_obLogger << e.what() << cQTLogger::EOM;
    }
}
