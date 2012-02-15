#include <QMessageBox>

#include "belenus.h"
#include "dlgguest.h"
#include "../edit/dlgguestedit.h"

cDlgGuest::cDlgGuest( QWidget *p_poParent )
    : cDlgCrud( p_poParent )
{
    setWindowTitle( tr( "Guest List" ) );
    setWindowIcon( QIcon("./resources/40x40_patient.png") );

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "ListGuests", QPoint(520,300) );
    resize( qpDlgSize.x(), qpDlgSize.y() );

    setupTableView();
}

cDlgGuest::~cDlgGuest()
{
    g_poPrefs->setDialogSize( "ListGuests", QPoint( width(), height() ) );
}

void cDlgGuest::setupTableView()
{
    cTracer obTracer( "cDlgGuest::setupTableView" );

    refreshTable();

    cDlgCrud::setupTableView();

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_poModel->setHeaderData( 0, Qt::Horizontal, tr( "Id" ) );
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "LicenceId" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Name" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Gender" ) );
        m_poModel->setHeaderData( 4, Qt::Horizontal, tr( "Age group" ) );
        m_poModel->setHeaderData( 5, Qt::Horizontal, tr( "E-Mail" ) );
        m_poModel->setHeaderData( 6, Qt::Horizontal, tr( "Active" ) );
        m_poModel->setHeaderData( 7, Qt::Horizontal, tr( "Archive" ) );

        tbvCrud->resizeColumnToContents( 0 );
        tbvCrud->resizeColumnToContents( 1 );
        tbvCrud->resizeColumnToContents( 2 );
        tbvCrud->resizeColumnToContents( 3 );
        tbvCrud->resizeColumnToContents( 4 );
        tbvCrud->resizeColumnToContents( 5 );
        tbvCrud->resizeColumnToContents( 6 );
        tbvCrud->resizeColumnToContents( 7 );

        tbvCrud->sortByColumn( 2, Qt::AscendingOrder );
    }
    else
    {
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "Name" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Gender" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Age group" ) );
        m_poModel->setHeaderData( 4, Qt::Horizontal, tr( "E-Mail" ) );

        tbvCrud->resizeColumnToContents( 1 );
        tbvCrud->resizeColumnToContents( 2 );
        tbvCrud->resizeColumnToContents( 3 );
        tbvCrud->resizeColumnToContents( 4 );

        tbvCrud->sortByColumn( 1, Qt::AscendingOrder );
    }
}

void cDlgGuest::refreshTable()
{
    cTracer obTracer( "cDlgGuest::refreshTable" );

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_qsQuery = "SELECT patientId, patients.licenceId, name, genderName, ageTypeName, email, active, archive FROM patients, genders, ageTypes WHERE genders.genderId=patients.gender AND agetypes.ageTypeId=ageType AND patientId>0";
    }
    else
    {
        m_qsQuery = "SELECT patientId AS id, name, genderName, ageTypeName, email FROM patients, genders, ageTypes WHERE genders.genderId=patients.gender AND agetypes.ageTypeId=ageType AND patientId>0 AND active=1";
    }

    cDlgCrud::refreshTable();
}

void cDlgGuest::enableButtons()
{
    cTracer obTracer( "cDlgGuest::enableButtons" );

    m_poBtnEdit->setEnabled( m_uiSelectedId > 0 );
    m_poBtnDelete->setEnabled( m_uiSelectedId > 0 );
}

void cDlgGuest::newClicked( bool )
{
    cDBGuest *poGuest = new cDBGuest;
    poGuest->createNew();

    cDlgGuestEdit  obDlgEdit( this, poGuest );
    obDlgEdit.setWindowTitle( tr( "New Guest" ) );
    if( obDlgEdit.exec() == QDialog::Accepted )
    {
        m_uiSelectedId = poGuest->id();
    }
    refreshTable();

    delete poGuest;
}

void cDlgGuest::editClicked( bool )
{
    cDBGuest  *poGuest = NULL;

    try
    {
        poGuest = new cDBGuest;
        poGuest->load( m_uiSelectedId );

        cDlgGuestEdit  obDlgEdit( this, poGuest );
        obDlgEdit.setWindowTitle( poGuest->name() );
        obDlgEdit.exec();
        refreshTable();

        if( poGuest ) delete poGuest;
    }
    catch( cSevException &e )
    {
        if( poGuest ) delete poGuest;
        g_obLogger(e.severity()) << e.what() << EOM;
    }
}

void cDlgGuest::deleteClicked( bool )
{
    cDBGuest  *poGuest = NULL;

    if( QMessageBox::question( this, tr( "Question" ),
                               tr( "Are you sure you want to delete this Guest?" ),
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
    {
        try
        {
            poGuest = new cDBGuest;
            poGuest->load( m_uiSelectedId );
            if( poGuest->licenceId() == 0 && !g_obUser.isInGroup( cAccessGroup::ROOT ) && !g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
            {
                QMessageBox::warning( this, tr("Warning"),
                                      tr("You are not allowed to delete studio independent data."));
                return;
            }
            poGuest->remove();

            m_uiSelectedId = 0;
            refreshTable();
            if( poGuest ) delete poGuest;
        }
        catch( cSevException &e )
        {
            if( poGuest ) delete poGuest;
        g_obLogger(e.severity()) << e.what() << EOM;
        }
    }
}
