#include <QMessageBox>

#include "belenus.h"
#include "dlgusercardtypes.h"
#include "dlgusercardtypeedit.h"

cDlgUserCardTypes::cDlgUserCardTypes( QWidget *p_poParent )
    : cDlgCrud( p_poParent )
{
    setWindowTitle( tr( "UserCard Types List" ) );

    if( g_obUser.isInGroup( "root" ) )
    {
        m_qsQuery = "SELECT patientCardTypeId, name, price, units, unitTime FROM patientCardTypes";
    }
    else
    {
        m_qsQuery = "SELECT patientCardTypeId AS id, name, price, units, unitTime FROM patientCardTypes";
    }

    m_poBtnNew->setEnabled( g_obUser.isInGroup( "admin" ) );

    setupTableView();
}

cDlgUserCardTypes::~cDlgUserCardTypes()
{
}

void cDlgUserCardTypes::setupTableView()
{
    cTracer obTracer( "cDlgUserCardTypes::setupTableView" );

    refreshTable();

    cDlgCrud::setupTableView();

    if( g_obUser.isInGroup( "root" ) )
    {
        m_poModel->setHeaderData( 0, Qt::Horizontal, tr( "Id" ) );
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "Name" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Price" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Units" ) );
        m_poModel->setHeaderData( 4, Qt::Horizontal, tr( "Unit time" ) );
    }
    else
    {
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "Name" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Price" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Units" ) );
        m_poModel->setHeaderData( 4, Qt::Horizontal, tr( "Unit time" ) );
    }

    tbvCrud->resizeColumnToContents( 1 );
}

void cDlgUserCardTypes::enableButtons()
{
    cTracer obTracer( "cDlgUserCardTypes::enableButtons" );

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

void cDlgUserCardTypes::newClicked( bool )
{
    cDBUserCardType *poUserCardType = new cDBUserCardType;
    poUserCardType->createNew();

    cDlgUserCardTypeEdit  obDlgEdit( this, poUserCardType );
    obDlgEdit.setWindowTitle( tr( "New Usercard type" ) );
    if( obDlgEdit.exec() == QDialog::Accepted )
    {
        m_uiSelectedId = poUserCardType->id();
        refreshTable();
    }

    delete poUserCardType;
}

void cDlgUserCardTypes::deleteClicked( bool )
{
    if( QMessageBox::question( this, tr( "Confirmation" ),
                               tr( "Are you sure you want to delete this Usercard type?" ),
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
    {
        QString stQuery = QString( "DELETE FROM patientCardTypes WHERE patientCardTypeId=%1" ).arg( m_uiSelectedId );
        g_poDB->executeQuery( stQuery.toStdString(), true );

        m_uiSelectedId = 0;
        refreshTable();
    }
}

void cDlgUserCardTypes::editClicked( bool )
{
    cDBUserCardType  *poUserCardType = NULL;

    try
    {
        poUserCardType = new cDBUserCardType;
        poUserCardType->load( m_uiSelectedId );

        cDlgUserCardTypeEdit  obDlgEdit( this, poUserCardType );
        obDlgEdit.setWindowTitle( QString::fromStdString( poUserCardType->name() ) );
        if( obDlgEdit.exec() == QDialog::Accepted )
        {
            refreshTable();
        }

        delete poUserCardType;
    }
    catch( cSevException &e )
    {
        if( poUserCardType ) delete poUserCardType;

        g_obLogger << e.severity();
        g_obLogger << e.what() << cQTLogger::EOM;
    }
}
