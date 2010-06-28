#include <QPushButton>
#include <QMessageBox>

#include "dlgcassaedit.h"
#include "../db/dbuser.h"

cDlgCassaEdit::cDlgCassaEdit( QWidget *p_poParent, cDBCassa *p_poCassa )
    : QDialog( p_poParent )
{
    setupUi( this );

    resize( width(), 148 );

    m_qsQuery       = "";
    m_poModel       = new cQTMySQLQueryModel( this );
    m_poSortedModel = new QSortFilterProxyModel();
    m_uiSelectedId  = 0;
    m_inSelectedRow = -1;

    m_poSortedModel->setSourceModel( m_poModel );
    tbvCassa->setModel( m_poSortedModel );

    m_poCassa = p_poCassa;

    if( m_poCassa )
    {
        cDBUser obUser;

        obUser.load( m_poCassa->userId() );

        QString m_qsStart = "";
        QString m_qsStop = "";

        if( QString::fromStdString(m_poCassa->startDateTime()).length() > 0 )
        {
            m_qsStart = QString::fromStdString(m_poCassa->startDateTime());
            m_qsStart.truncate( m_qsStart.length()-3 );
            m_qsStart.replace( 10, 1, " " );
        }
        if( QString::fromStdString(m_poCassa->stopDateTime()).length() > 0 )
        {
            m_qsStop = QString::fromStdString(m_poCassa->stopDateTime());
            m_qsStop.truncate( m_qsStop.length()-3 );
            m_qsStop.replace( 10, 1, " " );
        }

        dtStartDate->setText( m_qsStart );
        dtStopDate->setText( m_qsStop );
        lblBalanceValue->setText( convertCurrency( m_poCassa->currentBalance(), g_poPrefs->getCurrencyShort() ) );
        lblUser->setText( QString::fromStdString(obUser.realName()) );
    }

    pbClose->setIcon( QIcon("./resources/40x40_exit.gif") );
    pbCashAdd->setIcon( QIcon("./resources/40x40_cassa_add.gif") );
    pbCashGet->setIcon( QIcon("./resources/40x40_cassa_get.gif") );

    pbMore->setEnabled( false );

    dtStartDate->setEnabled( false );
    dtStopDate->setEnabled( false );

    setupTableView();
}

cDlgCassaEdit::~cDlgCassaEdit()
{
    if( m_poSortedModel ) delete m_poSortedModel;
    if( m_poModel ) delete m_poModel;
}

void cDlgCassaEdit::setupTableView()
{
    cTracer obTracer( "cDlgCassaEdit::setupTableView" );

    refreshTable();

    if( m_poModel->headerData( 0, Qt::Horizontal ) == "id" )
        tbvCassa->setColumnHidden( 0, true );

    connect( tbvCassa->selectionModel(), SIGNAL( selectionChanged( const QItemSelection &, const QItemSelection & ) ),
             this, SLOT( itemSelectionChanged( const QItemSelection &, const QItemSelection & ) ) );

    if( g_obUser.isInGroup( "root" ) )
    {
        m_poModel->setHeaderData( 0, Qt::Horizontal, tr( "Id" ) );
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "LicenceId" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Denomination" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Value" ) );
        m_poModel->setHeaderData( 4, Qt::Horizontal, tr( "Comment" ) );
        m_poModel->setHeaderData( 5, Qt::Horizontal, tr( "Archive" ) );
    }
    else
    {
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "Denomination" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Value" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Comment" ) );
    }
}

void cDlgCassaEdit::refreshTable()
{
    if( g_obUser.isInGroup( "root" ) )
    {
        m_qsQuery = QString( "SELECT cassadenominations.denominationId, cassadenominations.licenceId, denominations.denomination, cassadenominations.value, denominations.comment, cassadenominations.archive FROM cassadenominations, denominations WHERE denominations.denominationId=cassadenominations.denominationId AND cassadenominations.cassaId=%1 GROUP BY denominations.denominationId" ).arg(m_poCassa->id());
    }
    else
    {
        m_qsQuery = QString( "SELECT cassadenominations.denominationId as id, denominations.denomination, cassadenominations.value, denominations.comment FROM cassadenominations, denominations WHERE denominations.denominationId=cassadenominations.denominationId AND cassadenominations.cassaId=%1 GROUP BY denominations.denominationId" ).arg(m_poCassa->id());
    }

    tbvCassa->selectionModel()->blockSignals( true );

    m_poModel->setQuery( m_qsQuery );

    if( m_uiSelectedId )
    {
        QModelIndex      obFirstIdx = m_poSortedModel->index( 0, 0 );
        QModelIndexList  obResults  = m_poSortedModel->match( obFirstIdx, Qt::DisplayRole, m_uiSelectedId, 1, Qt::MatchExactly );
        if( obResults.empty() )
        {
            m_uiSelectedId = 0;
            m_inSelectedRow = -1;
        }
        else
        {
            m_inSelectedRow = obResults.at( 0 ).row();
            tbvCassa->selectRow( m_inSelectedRow );
        }
    }
    else
    {
        m_inSelectedRow = -1;
    }
    enableButtons();

    tbvCassa->selectionModel()->blockSignals( false );
}

void cDlgCassaEdit::itemSelectionChanged( const QItemSelection &p_obSelected,
                                     const QItemSelection &)
{
    m_inSelectedRow = -1;
    m_uiSelectedId  = 0;
    if( tbvCassa->selectionModel()->hasSelection() )
    {
        m_inSelectedRow = p_obSelected.indexes().at( 0 ).row();
        m_uiSelectedId = m_poSortedModel->index( m_inSelectedRow, 0 ).data().toUInt();
    }

    enableButtons();
}

QString cDlgCassaEdit::convertCurrency( int p_nCurrencyValue, QString p_qsCurrency )
{
    QString qsValue = QString::number( p_nCurrencyValue );
    QString qsRet = "";

    if( qsValue.length() > 3 )
    {
        while( qsValue.length() > 3 )
        {
            qsRet.insert( 0, qsValue.right(3) );
            qsRet.insert( 0, g_poPrefs->getCurrencySeparator() );
            qsValue.truncate( qsValue.length()-3 );
        }
    }
    qsRet.insert( 0, qsValue );
    qsRet += " " + p_qsCurrency;

    return qsRet;
}

void cDlgCassaEdit::enableButtons()
{

}

void cDlgCassaEdit::on_pbClose_clicked()
{
    QDialog::close();
}

void cDlgCassaEdit::on_pbCashAdd_clicked()
{

}

void cDlgCassaEdit::on_pbCashGet_clicked()
{

}

void cDlgCassaEdit::on_pbMore_clicked()
{
    if( height() == 421 )
    {
        resize( width(), 148 );
    }
    else if( height() == 148 )
    {
        resize( width(), 421 );
    }
}
