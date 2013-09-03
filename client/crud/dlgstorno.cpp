
#include <QMessageBox>
#include "dlgstorno.h"
#include "../db/dbcassahistory.h"

cDlgStorno::cDlgStorno( QWidget *p_poParent ) : cDlgCrud( p_poParent )
{
    cTracer obTrace( "cDlgStorno::cDlgStorno" );

    setWindowTitle( tr( "Revoke selected cassa action" ) );
    setWindowIcon( QIcon("./resources/40x40_cassa.png") );

    m_poBtnClose->setEnabled(false);
    m_poBtnDelete->setEnabled(false);
    m_poBtnEdit->setEnabled(false);
    m_poBtnNew->setEnabled(false);

    m_poBtnClose->setVisible(false);
    m_poBtnDelete->setVisible(false);
    m_poBtnEdit->setVisible(false);
    m_poBtnNew->setVisible(false);

    horizontalLayoutTop = new QHBoxLayout();
    horizontalLayoutTop->setObjectName( QString::fromUtf8( "horizontalLayoutTop" ) );
    lblComment = new QLabel( this );
    lblComment->setObjectName( QString::fromUtf8( "lblComment" ) );
    lblComment->setText( tr("Comment: ") );
    horizontalLayoutTop->addWidget( lblComment );
    ledComment = new QLineEdit( this );
    ledComment->setObjectName( QString::fromUtf8( "ledComment" ) );
    horizontalLayoutTop->addWidget( ledComment );
    horizontalSpacerTop = new QSpacerItem( 13, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    horizontalLayoutTop->addItem( horizontalSpacerTop );
    verticalLayout->insertLayout( 0, horizontalLayoutTop );

    pbStorno = new QPushButton( tr( "Storno" ), this );
    pbStorno->setObjectName( QString::fromUtf8( "pbStorno" ) );
    pbStorno->setIconSize( QSize(20, 20) );
    pbStorno->setIcon( QIcon("./resources/40x40_cassa_storno.png") );
    btbButtonsSide->addButton( pbStorno, QDialogButtonBox::ActionRole );
    pbExit = new QPushButton( tr( "Exit" ), this );
    pbExit->setObjectName( QString::fromUtf8( "pbExit" ) );
    pbExit->setIconSize( QSize(20, 20) );
    pbExit->setIcon( QIcon("./resources/40x40_exit.png") );
    btbButtonsSide->addButton( pbExit, QDialogButtonBox::RejectRole );

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "ListCassaStorno", QPoint(520,300) );
    resize( qpDlgSize.x(), qpDlgSize.y() );

    setupTableView();

    connect( ledComment, SIGNAL(textChanged(QString)), this, SLOT(refreshTable()) );
    connect( pbStorno, SIGNAL(clicked(bool)), this, SLOT(on_pbStorno_clicked()) );
}

cDlgStorno::~cDlgStorno()
{
    cTracer obTrace( "cDlgStorno::~cDlgStorno" );

    g_poPrefs->setDialogSize( "ListCassaStorno", QPoint( width(), height() ) );
}

void cDlgStorno::setupTableView()
{
    cTracer obTracer( "cDlgStorno::setupTableView" );

    refreshTable();

    cDlgCrud::setupTableView();

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_poModel->setHeaderData( 0, Qt::Horizontal, tr( "Id" ) );
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "LicenceId" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "ParentId" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Date/Time" ) );
        m_poModel->setHeaderData( 4, Qt::Horizontal, tr( "Amount" ) );
        m_poModel->setHeaderData( 5, Qt::Horizontal, tr( "Comment" ) );
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

        tbvCrud->sortByColumn( 3, Qt::DescendingOrder );
    }
    else
    {
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "Date/Time" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Amount" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Comment" ) );

        tbvCrud->resizeColumnToContents( 1 );
        tbvCrud->resizeColumnToContents( 2 );
        tbvCrud->resizeColumnToContents( 3 );

        tbvCrud->sortByColumn( 1, Qt::DescendingOrder );
    }
}

void cDlgStorno::refreshTable()
{
    cTracer obTracer( "cDlgStorno::refreshTable" );

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_qsQuery = "SELECT cassaHistoryId, licenceId, parentId, actionTime, (actionValue/100) as actionValue, comment, active, archive FROM cassahistory WHERE actionValue<>0";
    }
    else
    {
        m_qsQuery = QString( "SELECT cassaHistoryId AS id, actionTime, (actionValue/100) as actionValue, comment FROM cassahistory WHERE actionValue<>0 AND cassaId=%1 AND active=1" ).arg( g_obCassa.cassaId() );
        if( g_obUser.isInGroup( cAccessGroup::USER ) )
        {
            m_qsQuery += " AND ";
            m_qsQuery += QString( "userId=%1" ).arg( g_obUser.id() );
        }
    }

    QString stTemp;

    stTemp = ledComment->text();
    if( stTemp != "" )
    {
        m_qsQuery += " AND ";
        m_qsQuery += QString( "comment LIKE '\%%1\%'" ).arg( stTemp );
    }
//    stTemp = ledUniqueId->text();
//    if( stTemp != "" )
//    {
//        m_qsQuery += " AND ";
//        m_qsQuery += QString( "uniqueId LIKE '\%%1\%'" ).arg( stTemp );
//    }

    cDlgCrud::refreshTable();
}

void cDlgStorno::enableButtons()
{
    cTracer obTracer( "cDlgStorno::enableButtons" );

    bool    bEnabled = false;

    if( m_uiSelectedId )
    {
        try
        {
            cDBCassaHistory obDBCassaHistory;

            bEnabled = obDBCassaHistory.isRevokeEnabled( m_uiSelectedId );
        }
        catch( cSevException &e )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
        }
    }

    pbStorno->setEnabled( bEnabled );
}

void cDlgStorno::newClicked( bool ) {}
void cDlgStorno::deleteClicked( bool ) {}
void cDlgStorno::editClicked( bool ) {}

void cDlgStorno::on_pbStorno_clicked()
{
    try
    {
        if( QMessageBox::question( this, tr("Question"),
                                   tr( "Are you sure you want to revoke this cassa action?"),
                                   QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes )
        {
            g_obCassa.cassaProcessRevokeCassaAction( m_uiSelectedId );
        }
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
    }

    refreshTable();
}
