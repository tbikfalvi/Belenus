#include <QMessageBox>

#include "belenus.h"
#include "dlgusercards.h"
#include "dlgusercardedit.h"

cDlgUserCards::cDlgUserCards( QWidget *p_poParent )
    : cDlgCrud( p_poParent )
{
    setWindowTitle( tr( "UserCards List" ) );

    if( g_obUser.isInGroup( "root" ) )
    {
        m_qsQuery = "SELECT patientCards.patientCardId, barcode, patientCardTypes.name, patientCards.units, validdate  FROM usercards, usercardtypes WHERE usercards.usercardtypeid=usercardtypes.usercardtypeid";
    }
    else
    {
        m_qsQuery = "SELECT usercardid as id, barcode, usercardtypes.name, usercards.units, validdate  FROM usercards, usercardtypes WHERE usercards.usercardtypeid=usercardtypes.usercardtypeid";
    }

    m_poBtnNew->setEnabled( g_obUser.isInGroup( "admin" ) );

    setupTableView();
}

cDlgUserCards::~cDlgUserCards()
{
}

void cDlgUserCards::setupTableView()
{
    cTracer obTracer( "cDlgUserCards::setupTableView" );

    if( g_obUser.isInGroup( "root" ) )
    {
        m_poModel->setHeaderData( 0, Qt::Horizontal, tr( "Id" ) );
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "Barcode" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Usercard type" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Units" ) );
        m_poModel->setHeaderData( 4, Qt::Horizontal, tr( "Valid" ) );
    }
    else
    {
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "Barcode" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Usercard type" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Units" ) );
        m_poModel->setHeaderData( 4, Qt::Horizontal, tr( "Valid" ) );
    }

    tbvCrud->resizeColumnToContents( 1 );
}

void cDlgUserCards::enableButtons()
{
    cTracer obTracer( "cDlgUserCards::enableButtons" );

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

void cDlgUserCards::newClicked( bool )
{
    cDBUserCard *poUserCard = new cDBUserCard;
    poUserCard->createNew();

    cDlgUserCardEdit  obDlgEdit( this, poUserCard );
    obDlgEdit.setWindowTitle( tr( "New Usercard" ) );
    if( obDlgEdit.exec() == QDialog::Accepted )
    {
        m_uiSelectedId = poUserCard->id();
        refreshTable();
    }

    delete poUserCard;
}

void cDlgUserCards::deleteClicked( bool )
{
    if( QMessageBox::question( this, tr( "Confirmation" ),
                               tr( "Are you sure you want to delete this Usercard?" ),
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
    {
        QString stQuery = QString( "DELETE FROM patientCards WHERE patientCardId=%1" ).arg( m_uiSelectedId );
        g_poDB->executeQuery( stQuery.toStdString(), true );

        m_uiSelectedId = 0;
        refreshTable();
    }
}

void cDlgUserCards::editClicked( bool )
{
    cDBUserCard  *poUserCard = NULL;

    try
    {
        poUserCard = new cDBUserCard;
        poUserCard->load( m_uiSelectedId );

        cDlgUserCardEdit  obDlgEdit( this, poUserCard );
        obDlgEdit.setWindowTitle( QString::fromStdString( poUserCard->barCode() ) );
        if( obDlgEdit.exec() == QDialog::Accepted )
        {
            refreshTable();
        }

        delete poUserCard;
    }
    catch( cSevException &e )
    {
        if( poUserCard ) delete poUserCard;

        g_obLogger << e.severity();
        g_obLogger << e.what() << cQTLogger::EOM;
    }
}
