#include <QPushButton>
#include <QMessageBox>

#include "dlgcassaedit.h"
#include "../db/dbuser.h"
#include "../dlg/dlgcassaaction.h"

cDlgCassaEdit::cDlgCassaEdit( QWidget *p_poParent )
    : QDialog( p_poParent )
{
    setupUi( this );

    setWindowTitle( tr("Cassa") );
    setWindowIcon( QIcon("./resources/40x40_cassa.png") );

    setMinimumHeight( 147 );
    setMaximumHeight( 147 );
    tbvCassa->setEnabled( false );
    gbCassaHistory->setVisible( false );

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

    QString m_qsStart = "";
    QString m_qsStop = "";

    if( obCassa.startDateTime().length() > 0 )
    {
        m_qsStart = obCassa.startDateTime();
        m_qsStart.truncate( m_qsStart.length()-3 );
        m_qsStart.replace( 10, 1, " " );
    }
    if( obCassa.stopDateTime().length() > 0 )
    {
        m_qsStop = obCassa.stopDateTime();
        m_qsStop.truncate( m_qsStop.length()-3 );
        m_qsStop.replace( 10, 1, " " );
    }

    cCurrency   cBalance( obCassa.currentBalance() );

    dtStartDate->setText( m_qsStart );
    dtStopDate->setText( m_qsStop );
    lblBalanceValue->setText( cBalance.currencyFullStringShort() );
    lblUser->setText( obUser.realName() );

    pbClose->setIcon( QIcon("./resources/40x40_exit.png") );
    pbCashAdd->setIcon( QIcon("./resources/40x40_cassa_add.png") );
    pbCashGet->setIcon( QIcon("./resources/40x40_cassa_get.png") );

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

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_poModel->setHeaderData( 0, Qt::Horizontal, tr( "Id" ) );
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "LicenceId" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Comment" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Date/Time" ) );
        m_poModel->setHeaderData( 4, Qt::Horizontal, tr( "Amount of money" ) );
        m_poModel->setHeaderData( 5, Qt::Horizontal, tr( "Actual balance" ) );
        m_poModel->setHeaderData( 6, Qt::Horizontal, tr( "User" ) );
    }
    else
    {
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "Comment" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Date/Time" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Amount of money" ) );
        m_poModel->setHeaderData( 4, Qt::Horizontal, tr( "Actual balance" ) );
        m_poModel->setHeaderData( 5, Qt::Horizontal, tr( "User" ) );
    }

    tbvCassa->resizeColumnToContents( 1 );
    tbvCassa->resizeColumnToContents( 2 );
    tbvCassa->resizeColumnToContents( 3 );
    tbvCassa->resizeColumnToContents( 4 );
    tbvCassa->resizeColumnToContents( 5 );
}

void cDlgCassaEdit::refreshTable()
{
    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_qsQuery = QString( "SELECT cassaHistoryId, cassahistory.licenceId, cassahistory.comment, actionTime, actionValue, actionBalance, realName FROM cassaHistory, users WHERE cassaHistory.cassaId=%1 AND cassaHistory.userId=users.userId ORDER BY cassaHistoryId DESC" ).arg(g_obCassa.cassaId());
    }
    else
    {
        m_qsQuery = QString( "SELECT cassaHistoryId as id, cassahistory.comment, actionTime, actionValue, actionBalance, realName FROM cassaHistory, users WHERE cassaHistory.cassaId=%1 AND cassaHistory.userId=users.userId ORDER BY cassaHistoryId DESC" ).arg(g_obCassa.cassaId());
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
    if( !g_obCassa.isCassaEnabled() )
    {
        QMessageBox::warning( this, tr("Attention"),
                              tr("Cassa is disabled!\n\n"
                                 "Please relogin to enable cassa.") );
        return;
    }

    cDlgCassaAction     obDlgCassaAction( this );

    obDlgCassaAction.actionCassaInOut();
    if( obDlgCassaAction.exec() == QDialog::Accepted )
    {
        cDBCassa    obCassa;
        QString     qsComment;
        int         nTemp = 0;
        bool        bShoppingCart = false;
        QString     stRet = obDlgCassaAction.cassaResult( &nTemp, &qsComment, &bShoppingCart );

        g_obCassa.cassaIncreaseMoney( stRet.toInt(), qsComment );
        obCassa.load( g_obCassa.cassaId() );
        lblBalanceValue->setText( convertCurrency( obCassa.currentBalance(), g_poPrefs->getCurrencyShort() ) );
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

    cDlgCassaAction     obDlgCassaAction( this );

    obDlgCassaAction.actionCassaInOut();
    if( obDlgCassaAction.exec() == QDialog::Accepted )
    {
        cDBCassa    obCassa;
        QString     qsComment;
        int         nTemp = 0;
        bool        bShoppingCart = false;
        QString     stRet = obDlgCassaAction.cassaResult( &nTemp, &qsComment, &bShoppingCart );

        g_obCassa.cassaDecreaseMoney( stRet.toInt(), qsComment );
        obCassa.load( g_obCassa.cassaId() );
        lblBalanceValue->setText( convertCurrency( obCassa.currentBalance(), g_poPrefs->getCurrencyShort() ) );
    }
    refreshTable();
}

void cDlgCassaEdit::on_pbMore_clicked()
{
    if( pbMore->text().compare( tr("Less <<") ) == 0 )
    {
        gbCassaHistory->setVisible( false );
        tbvCassa->setEnabled( false );
        pbMore->setText( tr("More >>") );
        setMinimumHeight( 147 );
        setMaximumHeight( 147 );
//        resize( 706, 147 );
        move( x(), y()+137 );
    }
    else
    {
        gbCassaHistory->setVisible( true );
        tbvCassa->setEnabled( true );
        pbMore->setText( tr("Less <<") );
        setMinimumHeight( 421 );
        setMaximumHeight( 421 );
//        resize( 706, 421 );
        move( x(), y()-137 );
    }
}
