#include <QMessageBox>

#include "belenus.h"
#include "dlgpanelsettings.h"

cDlgPanelSettings::cDlgPanelSettings( QWidget *p_poParent, unsigned int p_uiPanelId )
    : cDlgCrud( p_poParent )
{
    setWindowTitle( tr( "Panel settings" ) );
    setWindowIcon( QIcon("./resources/40x40_device.gif") );

    m_uiPanelId = p_uiPanelId;

    setupTableView();

}

cDlgPanelSettings::~cDlgPanelSettings()
{

}

void cDlgPanelSettings::setupTableView()
{
    cTracer obTracer( "cDlgPanelSettings::setupTableView" );

    refreshTable();

    cDlgCrud::setupTableView();

    if( g_obUser.isInGroup( "root" ) )
    {
        m_poModel->setHeaderData( 0, Qt::Horizontal, tr( "Id" ) );
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "LicenceId" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Time length" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Price" ) );
        m_poModel->setHeaderData( 4, Qt::Horizontal, tr( "Archive" ) );
    }
    else
    {
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "Time length" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Price" ) );
    }
}

void cDlgPanelSettings::refreshTable()
{
    cTracer obTracer( "cDlgPanelSettings::refreshTable" );

    if( g_obUser.isInGroup( "root" ) )
    {
        m_qsQuery = QString("SELECT panelUseId, licenceId, useTime, usePrice, archive FROM panelUses WHERE panelId=%1").arg( m_uiPanelId );
    }
    else
    {
        m_qsQuery = QString("SELECT panelUseId AS Id, useTime, usePrice, archive WHERE panelId=%1").arg( m_uiPanelId );
    }

    cDlgCrud::refreshTable();
}

void cDlgPanelSettings::enableButtons()
{
    cTracer obTracer( "cDlgPanelSettings::enableButtons" );

    m_poBtnNew->setEnabled( g_obUser.isInGroup( "admin" ) );
    m_poBtnEdit->setEnabled( m_uiSelectedId > 0 && g_obUser.isInGroup( "admin" ) );
    m_poBtnDelete->setEnabled( m_uiSelectedId > 0 && g_obUser.isInGroup( "admin" ) );
}

void cDlgPanelSettings::newClicked( bool )
{
/*    cDBPatientCard *poPatientCard = new cDBPatientCard;
    poPatientCard->createNew();

    cDlgPatientCardEdit  obDlgEdit( this, poPatientCard );
    obDlgEdit.setWindowTitle( tr( "New Patientcard" ) );
    if( obDlgEdit.exec() == QDialog::Accepted )
    {
        m_uiSelectedId = poPatientCard->id();
        refreshTable();
    }

    delete poPatientCard;*/
}

void cDlgPanelSettings::editClicked( bool )
{
/*    cDBPatientCard  *poPatientCard = NULL;

    try
    {
        poPatientCard = new cDBPatientCard;
        poPatientCard->load( m_uiSelectedId );

        cDlgPatientCardEdit  obDlgEdit( this, poPatientCard );
        obDlgEdit.setWindowTitle( QString::fromStdString( poPatientCard->barcode() ) );
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
    }*/
}

void cDlgPanelSettings::deleteClicked( bool )
{
/*    cDBPatientCard  *poPatientCard = NULL;

    if( QMessageBox::question( this, tr( "Confirmation" ),
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
    }*/
}
