#include <QMessageBox>

#include "belenus.h"
#include "dlgpatientcardselect.h"
#include "dlg/dlgprogress.h"

cDlgPatientCardSelect::cDlgPatientCardSelect( QWidget *p_poParent )
    : cDlgPatientCard( p_poParent )
{
    setWindowTitle( tr( "PatientCard List" ) );
    setWindowIcon( QIcon("./resources/40x40_patientcard.png") );

    m_poBtnClose->setEnabled(false);
    m_poBtnDelete->setEnabled(false);
    m_poBtnEdit->setEnabled(false);
    m_poBtnNew->setEnabled(false);
    pbPatientCardType->setEnabled(false);

    m_poBtnClose->setVisible(false);
    m_poBtnDelete->setVisible(false);
    m_poBtnEdit->setVisible(false);
    m_poBtnNew->setVisible(false);
    pbPatientCardType->setVisible(false);

    pbSelect = new QPushButton( tr( "Select" ), this );
    pbSelect->setObjectName( QString::fromUtf8( "pbSelect" ) );
    pbSelect->setIconSize( QSize(20, 20) );
    pbSelect->setIcon( QIcon("./resources/40x40_ok.png") );
    btbButtonsSide->addButton( pbSelect, QDialogButtonBox::ActionRole );
    pbCancel = new QPushButton( tr( "Cancel" ), this );
    pbCancel->setObjectName( QString::fromUtf8( "pbCancel" ) );
    pbCancel->setIconSize( QSize(20, 20) );
    pbCancel->setIcon( QIcon("./resources/40x40_cancel.png") );
    btbButtonsSide->addButton( pbCancel, QDialogButtonBox::RejectRole );

    setupTableView();

    connect( pbSelect, SIGNAL(clicked(bool)), this, SLOT(on_pbSelect_clicked()) );
}

cDlgPatientCardSelect::~cDlgPatientCardSelect()
{
}

void cDlgPatientCardSelect::setupTableView()
{
    cTracer obTracer( "cDlgPatientCard::setupTableView" );

    refreshTable();

    cDlgCrud::setupTableView();

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_poModel->setHeaderData( 0, Qt::Horizontal, tr( "Id" ) );
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "LicenceId" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Barcode" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Owner" ) );
        m_poModel->setHeaderData( 4, Qt::Horizontal, tr( "Units" ) );
        m_poModel->setHeaderData( 5, Qt::Horizontal, tr( "Patientcard type" ) );
        m_poModel->setHeaderData( 6, Qt::Horizontal, tr( "All units" ) );
        m_poModel->setHeaderData( 7, Qt::Horizontal, tr( "Valid from" ) );
        m_poModel->setHeaderData( 8, Qt::Horizontal, tr( "Valid to" ) );
        m_poModel->setHeaderData( 9, Qt::Horizontal, tr( "Active" ) );
        m_poModel->setHeaderData( 10, Qt::Horizontal, tr( "Archive" ) );

        tbvCrud->resizeColumnToContents( 0 );
        tbvCrud->resizeColumnToContents( 1 );
        tbvCrud->resizeColumnToContents( 2 );
        tbvCrud->resizeColumnToContents( 3 );
        tbvCrud->resizeColumnToContents( 4 );
        tbvCrud->resizeColumnToContents( 5 );
        tbvCrud->resizeColumnToContents( 6 );
        tbvCrud->resizeColumnToContents( 7 );
        tbvCrud->resizeColumnToContents( 8 );
        tbvCrud->resizeColumnToContents( 9 );
        tbvCrud->resizeColumnToContents( 10 );

        tbvCrud->sortByColumn( 2, Qt::AscendingOrder );
    }
    else
    {
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "Barcode" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Owner" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Units" ) );
        m_poModel->setHeaderData( 4, Qt::Horizontal, tr( "Patientcard type" ) );
        m_poModel->setHeaderData( 5, Qt::Horizontal, tr( "All units" ) );
        m_poModel->setHeaderData( 6, Qt::Horizontal, tr( "Valid from" ) );
        m_poModel->setHeaderData( 7, Qt::Horizontal, tr( "Valid to" ) );
        m_poModel->setHeaderData( 8, Qt::Horizontal, tr( "Active" ) );

        tbvCrud->resizeColumnToContents( 1 );
        tbvCrud->resizeColumnToContents( 2 );
        tbvCrud->resizeColumnToContents( 3 );
        tbvCrud->resizeColumnToContents( 4 );
        tbvCrud->resizeColumnToContents( 5 );
        tbvCrud->resizeColumnToContents( 6 );
        tbvCrud->resizeColumnToContents( 7 );
        tbvCrud->resizeColumnToContents( 8 );

        tbvCrud->sortByColumn( 1, Qt::AscendingOrder );
    }
}

void cDlgPatientCardSelect::refreshTable()
{
    cDlgPatientCard::refreshTable( QString("patientCards.active=1") );
}

void cDlgPatientCardSelect::enableButtons()
{
    cTracer obTracer( "cDlgPatientCardSelect::enableButtons" );

    if( m_uiSelectedId )
    {
        pbSelect->setEnabled( true );
    }
    else
    {
        pbSelect->setEnabled( false );
    }
}

void cDlgPatientCardSelect::newClicked( bool ) {}
void cDlgPatientCardSelect::editClicked( bool ) {}
void cDlgPatientCardSelect::deleteClicked( bool ) {}

void cDlgPatientCardSelect::on_pbSelect_clicked()
{
    cDlgPatientCardSelect::accept();
}

unsigned int cDlgPatientCardSelect::selected()
{
    return m_uiSelectedId;
}
