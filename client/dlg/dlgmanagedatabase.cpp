
#include <QMessageBox>
#include <QDate>
#include <QStringList>

#include "dlgmanagedatabase.h"

cDlgManageDatabase::cDlgManageDatabase( QWidget *p_poParent )
    : QDialog( p_poParent )
{
    cTracer obTrace( "cDlgManageDatabase::cDlgManageDatabase" );

    setupUi( this );

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "ManageDatabases", QPoint(530,200) );
    resize( qpDlgSize.x(), qpDlgSize.y() );

    setWindowTitle( tr( "Manage database" ) );
    setWindowIcon( QIcon("./resources/40x40_connect_db.png") );

    pbExecute->setIcon( QIcon("./resources/40x40_database_sync.png") );
    pbExit->setIcon( QIcon("./resources/40x40_exit.png") );

    connect( rbDeactivatePCs,           SIGNAL(clicked()), this, SLOT(slotUpdateExecuteButton()) );
    connect( rbDeleteNotUsedPCTs,       SIGNAL(clicked()), this, SLOT(slotUpdateExecuteButton()) );
    connect( rbDeleteLedgerBeforeDate,  SIGNAL(clicked()), this, SLOT(slotUpdateExecuteButton()) );

    deFilterDate->setDate( QDate::currentDate() );

    slotUpdateExecuteButton();

    m_dlgProgress = new cDlgProgress( this );
}

cDlgManageDatabase::~cDlgManageDatabase()
{
    cTracer obTrace( "cDlgManageDatabase::~cDlgManageDatabase" );

    g_poPrefs->setDialogSize( "ManageDatabases", QPoint( width(), height() ) );
}

void cDlgManageDatabase::slotUpdateExecuteButton()
{
    if( rbDeactivatePCs->isChecked() ||
        rbDeleteNotUsedPCTs->isChecked() ||
        rbDeleteLedgerBeforeDate->isChecked() )
    {
        pbExecute->setEnabled( true );
    }
    else
    {
        pbExecute->setEnabled( false );
    }

    if( rbDeleteLedgerBeforeDate->isChecked() )
    {
        deFilterDate->setEnabled( true );
    }
    else
    {
        deFilterDate->setEnabled( false );
    }
}

void cDlgManageDatabase::on_pbExecute_clicked()
{
    if( QMessageBox::question( this, tr("Question"),
                               tr("Are you sure you want to execute the action?\n"
                                  "Please note the action can not be revoked."),
                               QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes )
    {
        if( rbDeactivatePCs->isChecked() )
        {
            _actionDeactivatePC();
        }
        else if( rbDeleteNotUsedPCTs->isChecked() )
        {
            _actionDeleteNotUsedPCT();
        }
        else if( rbDeleteLedgerBeforeDate->isChecked() )
        {
            _actionDeleteLedgerEntries();
        }
    }
}

void cDlgManageDatabase::on_pbExit_clicked()
{
    QDialog::accept();
}

void cDlgManageDatabase::_actionDeactivatePC()
{
    QString qsQuery = QString( "UPDATE patientcards SET "
                               "patientCardTypeId=0, "
                               "parentCardId=0, "
                               "patientId=0, "
                               "comment=\"\", "
                               "units=0, "
                               "amount=0, "
                               "timeLeft=0, "
                               "validDateFrom=\"0000-00-00\", "
                               "validDateTo=\"0000-00-00\", "
                               "pincode=NULL, "
                               "modified=\"0000-00-00 00:00:00\", "
                               "active=0, "
                               "archive=\"ARC\" "
                               "WHERE validDateTo<\"%1\" AND active=1" ).arg( QDate::currentDate().toString("yyyy-MM-dd") );
    QSqlQuery *poQuery = g_poDB->executeQTQuery( qsQuery );
    QMessageBox::information( this, tr("Information"),
                              tr("The action successfully finished.\n"
                                 "Number of affected records: %1").arg( poQuery->numRowsAffected() ) );
}

void cDlgManageDatabase::_actionDeleteNotUsedPCT()
{
    QSqlQuery *poQuery = g_poDB->executeQTQuery( "SELECT patientCardTypeId FROM patientcards WHERE active=1 GROUP BY patientCardTypeId" );
    QString    qsQuery = "DELETE FROM patientcardtypes WHERE patientCardTypeId NOT IN ( ";

    if( poQuery->first() )
    {
        qsQuery.append( poQuery->value(0).toString() );
        while( poQuery->next() )
        {
            qsQuery.append( ", " );
            qsQuery.append( poQuery->value(0).toString() );
        }
        qsQuery.append( " ) AND patientCardTypeId>1 " );

        poQuery = g_poDB->executeQTQuery( qsQuery );
        QMessageBox::information( this, tr("Information"),
                                  tr("The action successfully finished.\n"
                                     "Number of affected records: %1").arg( poQuery->numRowsAffected() ) );
    }
}

void cDlgManageDatabase::_actionDeleteLedgerEntries()
{
    m_dlgProgress->showProgress();

    QSqlQuery   *poQuery = g_poDB->executeQTQuery( QString("SELECT ledgerId FROM ledger WHERE ledgerTime<\"%1\" AND ledgerId>0 ").arg(deFilterDate->date().toString("yyyy-MM-dd")) );
    QStringList  qslIds;

    while( poQuery->next() )
    {
        qslIds << poQuery->value(0).toString();
    }

    int inNumRowPCUs = 0;
    int inNumRowProd = 0;
    int inNumRowCass = 0;

    for( int i=0; i<qslIds.count(); i++ )
    {
        poQuery       = g_poDB->executeQTQuery( QString("UPDATE patientCardUnits SET ledgerId=0 WHERE ledgerId=%1 ").arg(qslIds.at(i)) );
        inNumRowPCUs += poQuery->numRowsAffected();

        poQuery       = g_poDB->executeQTQuery( QString("UPDATE productHistory SET ledgerId=0 WHERE ledgerId=%1 ").arg(qslIds.at(i)) );
        inNumRowProd += poQuery->numRowsAffected();

        poQuery       = g_poDB->executeQTQuery( QString("UPDATE cassaHistory SET ledgerId=0 WHERE ledgerId=%1 ").arg(qslIds.at(i)) );
        inNumRowCass += poQuery->numRowsAffected();
    }

    poQuery = g_poDB->executeQTQuery( QString("DELETE FROM ledger WHERE ledgerTime<\"%1\" AND ledgerId>0 ").arg(deFilterDate->date().toString("yyyy-MM-dd")) );

    m_dlgProgress->hideProgress();

    QMessageBox::information( this, tr("Information"),
                              tr("The action successfully finished.\n"
                                 "Number of affected records:\n"
                                 "PatientCardUnits -> %1 records\n"
                                 "ProductHistory -> %2 records\n"
                                 "CassaHistory -> %3 records\n"
                                 "Ledger -> %4 records").arg(inNumRowPCUs).arg(inNumRowProd).arg(inNumRowCass).arg(poQuery->numRowsAffected()) );
}


