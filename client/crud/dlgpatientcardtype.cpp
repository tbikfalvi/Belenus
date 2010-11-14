#include <QMessageBox>

#include "belenus.h"
#include "dlgpatientcardtype.h"
#include "../edit/dlgpatientcardtypeedit.h"
#include "../db/dbpatientcard.h"

cDlgPatientCardType::cDlgPatientCardType( QWidget *p_poParent )
    : cDlgCrud( p_poParent )
{
    setWindowTitle( tr( "Patient Cardtype List" ) );
    setWindowIcon( QIcon("./resources/40x40_patientcardtype.png") );

    setupTableView();
}

cDlgPatientCardType::~cDlgPatientCardType()
{
}

void cDlgPatientCardType::setupTableView()
{
    cTracer obTracer( "cDlgPatientCardType::setupTableView" );

    refreshTable();

    cDlgCrud::setupTableView();

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_poModel->setHeaderData( 0, Qt::Horizontal, tr( "Id" ) );
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "LicenceId" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Name" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Price" ) );
        m_poModel->setHeaderData( 4, Qt::Horizontal, tr( "Units" ) );
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
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Price" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Units" ) );

        tbvCrud->resizeColumnToContents( 1 );
        tbvCrud->resizeColumnToContents( 2 );
        tbvCrud->resizeColumnToContents( 3 );

        tbvCrud->sortByColumn( 1, Qt::AscendingOrder );
    }
}

void cDlgPatientCardType::refreshTable()
{
    cTracer obTracer( "cDlgPatientCardType::refreshTable" );

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_qsQuery = "SELECT patientCardTypeId, licenceId, name, price, units, active, archive FROM patientCardTypes";
    }
    else
    {
        m_qsQuery = "SELECT patientCardTypeId AS id, name, price, units FROM patientCardTypes WHERE active=1";
    }

    cDlgCrud::refreshTable();
}

void cDlgPatientCardType::enableButtons()
{
    cTracer obTracer( "cDlgPatientCardType::enableButtons" );

    m_poBtnNew->setEnabled( g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    m_poBtnEdit->setEnabled( m_uiSelectedId > 1 && g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    m_poBtnDelete->setEnabled( m_uiSelectedId > 1 && g_obUser.isInGroup( cAccessGroup::ADMIN ) );
}

void cDlgPatientCardType::newClicked( bool )
{
    cDBPatientCardType *poPatientCardType = new cDBPatientCardType;
    poPatientCardType->createNew();

    cDlgPatientCardTypeEdit  obDlgEdit( this, poPatientCardType );
    obDlgEdit.setWindowTitle( tr( "New PatientCard Type" ) );
    if( obDlgEdit.exec() == QDialog::Accepted )
    {
        m_uiSelectedId = poPatientCardType->id();
        refreshTable();
    }

    delete poPatientCardType;
}

void cDlgPatientCardType::editClicked( bool )
{
    cDBPatientCardType  *poPatientCardType = NULL;

    try
    {
        poPatientCardType = new cDBPatientCardType;
        poPatientCardType->load( m_uiSelectedId );

        cDlgPatientCardTypeEdit  obDlgEdit( this, poPatientCardType );
        obDlgEdit.setWindowTitle( poPatientCardType->name() );
        if( obDlgEdit.exec() == QDialog::Accepted )
        {
            refreshTable();
        }

        if( poPatientCardType ) delete poPatientCardType;
    }
    catch( cSevException &e )
    {
        if( poPatientCardType ) delete poPatientCardType;
        g_obLogger(e.severity()) << e.what() << EOM;
    }
}

void cDlgPatientCardType::deleteClicked( bool )
{
    cDBPatientCardType  *poPatientCardType = NULL;

    if( QMessageBox::question( this, tr( "Question" ),
                               tr( "Are you sure you want to delete this PatientCard Type?" ),
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
    {
        try
        {
            cDBPatientCard  obDBPatientCard;
            if( obDBPatientCard.isPatientCardTypeLinked( m_uiSelectedId ) )
            {
                QMessageBox::warning( this, tr("Attention"),
                                      tr("Unfortunatelly delete of this card type is not possible.\n"
                                         "There are patientcards attached to this card type.") );
                return;
            }

            poPatientCardType = new cDBPatientCardType;
            poPatientCardType->load( m_uiSelectedId );
            if( poPatientCardType->licenceId() == 0 && !g_obUser.isInGroup( cAccessGroup::ROOT ) && !g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
            {
                QMessageBox::warning( this, tr("Warning"),
                                      tr("You are not allowed to delete studio independent data."));
                return;
            }
            poPatientCardType->remove();
            m_uiSelectedId = 0;
            refreshTable();
            if( poPatientCardType ) delete poPatientCardType;
        }
        catch( cSevException &e )
        {
            if( poPatientCardType ) delete poPatientCardType;
            g_obLogger(e.severity()) << e.what() << EOM;
        }
    }
}
