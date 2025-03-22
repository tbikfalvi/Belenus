
#include <QMessageBox>

#include "dlgselectmailrecipient.h"

//==============================================================================================
//
// cDlgSelectMailRecipient
//
//==============================================================================================
cDlgSelectMailRecipient::cDlgSelectMailRecipient( QWidget *p_poParent ) : QDialog( p_poParent )
{
    setupUi( this );

    setWindowIcon( QIcon("./resources/40x40_distlist.png") );

    pbOk->setIcon( QIcon("./resources/40x40_ok.png") );
    pbCancel->setIcon( QIcon("./resources/40x40_cancel.png") );

    _refreshList();

    connect( ledRecipient, SIGNAL(textChanged(QString)), this, SLOT(_refreshList()) );

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "SelectMailRecipient", QPoint(630,430) );
    resize( qpDlgSize.x(), qpDlgSize.y() );
}
//----------------------------------------------------------------------------------------------
cDlgSelectMailRecipient::~cDlgSelectMailRecipient()
{
    g_poPrefs->setDialogSize( "SelectMailRecipient", QPoint( width(), height() ) );
}
//----------------------------------------------------------------------------------------------
void cDlgSelectMailRecipient::_refreshList()
{
    QSqlQuery  *poQuery = NULL;
    QString     qsQuery = "";
    QString     stTemp = "";

    listRecipients->clear();

    qsQuery = QString( "SELECT patientId, name, email FROM patients WHERE active=1 AND archive<>\"DEL\" AND email!=\"\" " );
    stTemp = ledRecipient->text();
    if( stTemp != "" )
    {
        qsQuery += " AND ";
        qsQuery += QString( "(name LIKE '%" + stTemp + "%' OR email LIKE '%" + stTemp + "%') " );
    }

    poQuery = g_poDB->executeQTQuery( qsQuery );
    while( poQuery->next() )
    {
        listRecipients->addItem( QString("%1 (%2)").arg( poQuery->value( 1 ).toString() )
                                                   .arg( poQuery->value( 2 ).toString() ));
    }
    if( poQuery ) delete poQuery;

    qsQuery = QString( "SELECT name FROM distlist WHERE active=1 " );
    stTemp = ledRecipient->text();
    if( stTemp != "" )
    {
        qsQuery += " AND ";
        qsQuery += QString( "name LIKE '%" + stTemp + "%'" );
    }

    poQuery = g_poDB->executeQTQuery( qsQuery );
    while( poQuery->next() )
    {
        listRecipients->addItem( QString("@%1").arg( poQuery->value( 0 ).toString() ));
    }
    if( poQuery ) delete poQuery;
}
//----------------------------------------------------------------------------------------------
void cDlgSelectMailRecipient::on_pbOk_clicked()
{
    qslRecipients.clear();

    for( int i=0; i<listRecipients->count(); i++ )
    {
        if( listRecipients->item(i)->isSelected() )
        {
            qslRecipients.append( listRecipients->item(i)->text() );
        }
    }
    QDialog::accept();
}
//----------------------------------------------------------------------------------------------
void cDlgSelectMailRecipient::on_pbCancel_clicked()
{
    qslRecipients.clear();
    QDialog::reject();
}
//----------------------------------------------------------------------------------------------
void cDlgSelectMailRecipient::on_listRecipients_clicked(const QModelIndex &/*index*/)
{
    pbOk->setEnabled( false );
    for( int i=0; i<listRecipients->count(); i++ )
    {
        if( listRecipients->item(i)->isSelected() )
        {
            pbOk->setEnabled( true );
            break;
        }
    }
}
//----------------------------------------------------------------------------------------------
