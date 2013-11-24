#include <QPushButton>
#include <QMessageBox>

#include "dlgcassaedit.h"
#include "../db/dbuser.h"
#include "../dlg/dlgcassainout.h"

cDlgCassaEdit::cDlgCassaEdit( QWidget *p_poParent )
    : QDialog( p_poParent )
{
    setupUi( this );

    setWindowTitle( tr("Cassa") );
    setWindowIcon( QIcon("./resources/40x40_cassa.png") );

    m_qsQuery       = "";
    m_poModel       = new cQTMySQLQueryModel( this );
    m_poSortedModel = new QSortFilterProxyModel();
    m_uiSelectedId  = 0;
    m_inSelectedRow = -1;

    m_poSortedModel->setSourceModel( m_poModel );
    tbvCassa->setModel( m_poSortedModel );

    cDBCassa    obCassa;
    cDBUser     obUser;

    obCassa.load( g_obCassa.cassaId() );
    obUser.load( obCassa.userId() );

    cCurrency   cBalance( obCassa.currentBalance() );

    lblBalanceValue->setText( cBalance.currencyFullStringShort() );
    lblUser->setText( obUser.realName() );

    pbClose->setIcon( QIcon("./resources/40x40_exit.png") );
    pbCashAdd->setIcon( QIcon("./resources/40x40_cassa_add.png") );
    pbCashGet->setIcon( QIcon("./resources/40x40_cassa_get.png") );

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "CassaHistory", QPoint(800,300) );
    resize( qpDlgSize.x(), qpDlgSize.y() );

    setupTableView();
}

cDlgCassaEdit::~cDlgCassaEdit()
{
    g_poPrefs->setDialogSize( "CassaHistory", QPoint( width(), height() ) );

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

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_poModel->setHeaderData( 0, Qt::Horizontal, tr( "Id" ) );
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "LicenceId" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Comment" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Date/Time" ) );
        m_poModel->setHeaderData( 4, Qt::Horizontal, tr( "Amount of money" ) );
        m_poModel->setHeaderData( 5, Qt::Horizontal, tr( "Card usage" ) );
        m_poModel->setHeaderData( 6, Qt::Horizontal, tr( "Cash/Voucher" ) );
        m_poModel->setHeaderData( 7, Qt::Horizontal, tr( "Actual balance" ) );
        m_poModel->setHeaderData( 8, Qt::Horizontal, tr( "User" ) );
    }
    else
    {
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "Comment" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Date/Time" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Amount of money" ) );
        m_poModel->setHeaderData( 4, Qt::Horizontal, tr( "Card usage" ) );
        m_poModel->setHeaderData( 5, Qt::Horizontal, tr( "Cash/Voucher" ) );
        m_poModel->setHeaderData( 6, Qt::Horizontal, tr( "Actual balance" ) );
        m_poModel->setHeaderData( 7, Qt::Horizontal, tr( "User" ) );
    }

    tbvCassa->resizeColumnToContents( 1 );
    tbvCassa->resizeColumnToContents( 2 );
    tbvCassa->resizeColumnToContents( 3 );
    tbvCassa->resizeColumnToContents( 4 );
    tbvCassa->resizeColumnToContents( 5 );
    tbvCassa->resizeColumnToContents( 6 );
    tbvCassa->resizeColumnToContents( 7 );
}

void cDlgCassaEdit::refreshTable()
{
    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_qsQuery = QString( "SELECT cassaHistoryId, cassahistory.licenceId, cassahistory.comment, actionTime, (actionValue/100) as actionValue, (actionCard/100) as actionCard, (actionCash/100) as actionCash, (actionBalance/100) as actionBalance, realName FROM cassaHistory, users WHERE cassaHistory.cassaId=%1 AND cassaHistory.userId=users.userId ORDER BY cassaHistoryId DESC" ).arg(g_obCassa.cassaId());
    }
    else
    {
        m_qsQuery = QString( "SELECT cassaHistoryId as id, cassahistory.comment, actionTime, (actionValue/100) as actionValue, (actionCard/100) as actionCard, (actionCash/100) as actionCash, (actionBalance/100) as actionBalance, realName FROM cassaHistory, users WHERE cassaHistory.cassaId=%1 AND cassaHistory.userId=users.userId ORDER BY cassaHistoryId DESC" ).arg(g_obCassa.cassaId());
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

void cDlgCassaEdit::itemSelectionChanged( const QItemSelection &p_obSelected, const QItemSelection &)
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
    if( !g_obCassa.isCassaEnabled() )
    {
        QMessageBox::warning( this, tr("Attention"),
                              tr("Cassa is disabled!\n\n"
                                 "Please relogin to enable cassa.") );
        return;
    }

    cDlgCassaInOut     obDlgCassaInOut( this, tr("Cash payment") );

    if( obDlgCassaInOut.exec() == QDialog::Accepted )
    {
        cDBCassa        obCassa;

        g_obCassa.cassaIncreaseMoney( obDlgCassaInOut.resultAmount()*100, obDlgCassaInOut.resultComment() );
        obCassa.load( g_obCassa.cassaId() );

        cCurrency   cBalance( obCassa.currentBalance() );

        lblBalanceValue->setText( cBalance.currencyFullStringShort() );
    }
    refreshTable();
}

void cDlgCassaEdit::on_pbCashGet_clicked()
{
    if( !g_obCassa.isCassaEnabled() )
    {
        QMessageBox::warning( this, tr("Attention"),
                              tr("Cassa is disabled!\n\n"
                                 "Please relogin to enable cassa.") );
        return;
    }

    cDlgCassaInOut     obDlgCassaInOut( this, tr("Cash withdrawal") );

    if( obDlgCassaInOut.exec() == QDialog::Accepted )
    {
        cDBCassa        obCassa;

        g_obCassa.cassaDecreaseMoney( obDlgCassaInOut.resultAmount()*100, obDlgCassaInOut.resultComment() );
        obCassa.load( g_obCassa.cassaId() );

        cCurrency   cBalance( obCassa.currentBalance() );

        lblBalanceValue->setText( cBalance.currencyFullStringShort() );
    }
    refreshTable();
}
