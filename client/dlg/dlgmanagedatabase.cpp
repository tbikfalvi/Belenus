
#include <QMessageBox>
#include <QDate>
#include <QStringList>

#include "dlgmanagedatabase.h"
#include "../db/dbpatientcardtype.h"

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

    connect( rbUpdatePCUnitType,    SIGNAL(clicked()), this, SLOT(slotUpdateExecuteButton()) );
    connect( rbDeactivatePCs,       SIGNAL(clicked()), this, SLOT(slotUpdateExecuteButton()) );
    connect( rbDeleteInactivePCs,   SIGNAL(clicked()), this, SLOT(slotUpdateExecuteButton()) );
    connect( rbDeleteNotUsedPCTs,   SIGNAL(clicked()), this, SLOT(slotUpdateExecuteButton()) );
    connect( rbAssignPCTToPC,       SIGNAL(clicked()), this, SLOT(slotUpdateExecuteButton()) );

    connect( rbDeleteLedgerBeforeDate,  SIGNAL(clicked()), this, SLOT(slotUpdateExecuteButton()) );

    QSqlQuery   *poQuery;
    QString      qsQuery;

    cmbPatientCardType->addItem( tr("<Not selected>"), -1 );
    qsQuery = QString( "SELECT patientCardTypeId, name FROM patientCardTypes WHERE patientCardTypeId>1 AND licenceId!=0 AND active=1 AND archive<>\"DEL\" ORDER BY name " );

    poQuery = g_poDB->executeQTQuery( qsQuery );
    while( poQuery->next() )
    {
        cmbPatientCardType->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
    }

    deFilterDate->setDate( QDate::currentDate() );

    rbDeleteNotUsedPCTs->setEnabled( false );
    rbDeleteNotUsedPCTs->setVisible( false );

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
    if( rbUpdatePCUnitType->isChecked() ||
        rbDeactivatePCs->isChecked() ||
        rbDeleteInactivePCs->isChecked() ||
        rbDeleteNotUsedPCTs->isChecked() ||
        rbAssignPCTToPC->isChecked() ||
        rbDeleteLedgerBeforeDate->isChecked() )
    {
        pbExecute->setEnabled( true );
    }
    else
    {
        pbExecute->setEnabled( false );
    }

    if( rbAssignPCTToPC->isChecked() )
    {
        cmbPatientCardType->setEnabled( true );
    }
    else
    {
        cmbPatientCardType->setEnabled( false );
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
        if( rbUpdatePCUnitType->isChecked() )
        {
            _actionUpdatePatientCardUnits();
        }
        else if( rbDeactivatePCs->isChecked() )
        {
            _actionDeactivatePC();
        }
        else if( rbDeleteInactivePCs->isChecked() )
        {
            _actionDeleteInactivePC();
        }
        else if( rbDeleteNotUsedPCTs->isChecked() )
        {
            _actionDeleteNotUsedPCT();
        }
        else if( rbAssignPCTToPC->isChecked() )
        {
            _actionRepairPatientcardsWithoutType();
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

void cDlgManageDatabase::_actionUpdatePatientCardUnits()
{
    m_dlgProgress->showProgress();

    try
    {
        QString      qsQuery = QString( "SELECT patientCardId FROM patientcards WHERE patientCardId>1 AND active=1" );
        QSqlQuery   *poQuery = g_poDB->executeQTQuery( qsQuery );

        m_dlgProgress->showProgressBar( poQuery->size() );

        while( poQuery->next() )
        {
            cDBPatientCard obDBPatientCard;

            obDBPatientCard.load( poQuery->value(0).toUInt() );
            obDBPatientCard.synchronizeUnitTime( 0 );
            m_dlgProgress->stepProgressBar();
        }
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
        g_obGen.showTrayError( e.what() );
    }

    m_dlgProgress->hideProgress();
}

void cDlgManageDatabase::_actionDeactivatePC()
{
    try
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
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
        g_obGen.showTrayError( e.what() );
    }
}

void cDlgManageDatabase::_actionDeleteInactivePC()
{
    m_dlgProgress->showProgress();

    try
    {
        QStringList qslCardId;
        QString qsQuery = QString( "SELECT patientCardId FROM patientcards WHERE "
                                   "patientCardId>1 AND "
                                   "patientCardTypeId=0 AND "
                                   "patientId=0 AND "
                                   "(pincode IS NULL OR pincode=\"\") AND "
                                   "active=0 "
                                  );
        QSqlQuery *poQuery = g_poDB->executeQTQuery( qsQuery );

        while( poQuery->next() )
        {
            qslCardId.append( poQuery->value(0).toString() );
        }

        m_dlgProgress->showProgressBar( qslCardId.count() );

        unsigned int nCountSkipped = 0;

        for( int i=0; i<qslCardId.count(); i++ )
        {
            unsigned int    uiId = qslCardId.at(i).toUInt();
            cDBPatientCard  obDBPatientCard;

            obDBPatientCard.load( uiId );

            if( obDBPatientCard.isAssignedCardExists() ||
                obDBPatientCard.isLedgerConnected() )
            {
                nCountSkipped++;
                m_dlgProgress->stepProgressBar();
                continue;
            }

            qsQuery = QString( "DELETE FROM connectPatientWithCard WHERE "
                               "patientCardId=%1 " ).arg( uiId );
            g_poDB->executeQTQuery( qsQuery );

            qsQuery = QString( "DELETE FROM patientCardHistories WHERE "
                               "patientCardId=%1 " ).arg( uiId );
            g_poDB->executeQTQuery( qsQuery );

            qsQuery = QString( "DELETE FROM patientCardUnits WHERE "
                               "patientCardId=%1 " ).arg( uiId );
            g_poDB->executeQTQuery( qsQuery );

            qsQuery = QString( "DELETE FROM patientHistory WHERE "
                               "patientCardId=%1 " ).arg( uiId );
            g_poDB->executeQTQuery( qsQuery );

            qsQuery = QString( "DELETE FROM patientCards WHERE "
                               "patientCardId=%1 " ).arg( uiId );
            g_poDB->executeQTQuery( qsQuery );

            m_dlgProgress->stepProgressBar();
        }

        QMessageBox::information( this, tr("Information"),
                                  tr("The action successfully finished.\n"
                                     "Number of affected records: %1\n"
                                     "Number of skipped records: %2")
                                  .arg( qslCardId.count()-nCountSkipped )
                                  .arg( nCountSkipped ) );
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
        g_obGen.showTrayError( e.what() );
    }

    m_dlgProgress->hideProgress();
}

void cDlgManageDatabase::_actionDeleteNotUsedPCT()
{
    try
    {
//        QSqlQuery *poQuery = g_poDB->executeQTQuery( "SELECT patientCardTypeId FROM patientcards WHERE active=1 GROUP BY patientCardTypeId" );
        QSqlQuery *poQuery = g_poDB->executeQTQuery( "SELECT patientcardtypes.patientcardtypeid, "
                                                     "name, "
                                                     "patientcards.patientcardid, "
                                                     "patientcardunits.patientcardunitid "
                                                     "FROM patientcardtypes "
                                                     "LEFT JOIN patientcards ON "
                                                     "patientcardtypes.patientcardtypeid=patientcards.patientcardtypeid "
                                                     "LEFT JOIN patientcardunits ON "
                                                     "patientcardtypes.patientcardtypeid=patientcardunits.patientcardtypeid "
                                                     "WHERE "
                                                     "ISNULL(patientcards.patientcardid) AND "
                                                     "ISNULL(patientcardunits.patientcardunitid) "
                                                     "GROUP BY patientcardtypes.patientcardtypeid" );
        QString    qsQuery = "DELETE FROM patientcardtypes WHERE patientCardTypeId IN ( ";

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
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
        g_obGen.showTrayError( e.what() );
    }
}

void cDlgManageDatabase::_actionDeleteLedgerEntries()
{
    m_dlgProgress->showProgress();

    try
    {
        QSqlQuery   *poQuery = g_poDB->executeQTQuery( QString("SELECT ledgerId FROM "
                                                               "ledger WHERE "
                                                               "ledgerTime<\"%1\" AND "
                                                               "ledgerId>0 ")
                                                       .arg(deFilterDate->date().toString("yyyy-MM-dd")) );
        QStringList  qslIds;

        while( poQuery->next() )
        {
            qslIds << poQuery->value(0).toString();
        }

        m_dlgProgress->showProgressBar( qslIds.count() );

        int inNumRowDeviceLedger    = 0;
        int inNumRowPCUs            = 0;
        int inNumRowProd            = 0;
        int inNumRowCass            = 0;
        int inNumRowLedger          = 0;
        int inNumRowCassa           = 0;
        int inNumRowDenominations   = 0;

        for( int i=0; i<qslIds.count(); i++ )
        {
            poQuery       = g_poDB->executeQTQuery( QString("UPDATE patientCardUnits SET "
                                                            "ledgerId=0 WHERE "
                                                            "ledgerId=%1 ")
                                                    .arg(qslIds.at(i)) );
            inNumRowPCUs += poQuery->numRowsAffected();

            poQuery       = g_poDB->executeQTQuery( QString("UPDATE productHistory SET "
                                                            "ledgerId=0 WHERE "
                                                            "ledgerId=%1 ")
                                                    .arg(qslIds.at(i)) );
            inNumRowProd += poQuery->numRowsAffected();

            poQuery = g_poDB->executeQTQuery( QString("UPDATE cassaHistory SET "
                                                      "ledgerId=0 WHERE "
                                                      "ledgerId=%1")
                                              .arg(qslIds.at(i)) );
//            inNumRowCass += poQuery->numRowsAffected();

            m_dlgProgress->stepProgressBar();
        }

        poQuery = g_poDB->executeQTQuery( QString("DELETE FROM ledger WHERE "
                                                  "ledgerTime<\"%1\" AND "
                                                  "ledgerId>0 ")
                                          .arg(deFilterDate->date().toString("yyyy-MM-dd")) );
        inNumRowLedger = poQuery->numRowsAffected();

        poQuery = g_poDB->executeQTQuery( QString("DELETE FROM ledgerdevice WHERE "
                                                  "ledgerTime<\"%1\" AND "
                                                  "ledgerDeviceId>0 ")
                                          .arg( deFilterDate->date().toString("yyyy-MM-dd") ) );
        inNumRowDeviceLedger += poQuery->numRowsAffected();

        poQuery = g_poDB->executeQTQuery( QString("SELECT cassaId FROM "
                                                  "cassa WHERE "
                                                  "startDateTime<\"%1\" AND "
                                                  "stopDateTime<\"%1\" AND "
                                                  "cassaId>0 ")
                                          .arg(deFilterDate->date().toString("yyyy-MM-dd")) );

        qslIds.clear();
        while( poQuery->next() )
        {
            qslIds << poQuery->value(0).toString();
        }

        m_dlgProgress->showProgressBar( qslIds.count() );

        for( int i=0; i<qslIds.count(); i++ )
        {
            poQuery = g_poDB->executeQTQuery( QString("DELETE FROM cassaHistory WHERE "
                                                      "cassaId=%1 ")
                                              .arg(qslIds.at(i)) );
            inNumRowCass += poQuery->numRowsAffected();

            poQuery       = g_poDB->executeQTQuery( QString("DELETE FROM cassadenominations WHERE "
                                                            "cassaId=%1 ")
                                                    .arg(qslIds.at(i)) );
            inNumRowDenominations += poQuery->numRowsAffected();

            m_dlgProgress->stepProgressBar();
        }

        poQuery = g_poDB->executeQTQuery( QString("DELETE FROM "
                                                  "cassa WHERE "
                                                  "startDateTime<\"%1\" AND "
                                                  "stopDateTime<\"%1\" AND "
                                                  "cassaId>0 ")
                                          .arg(deFilterDate->date().toString("yyyy-MM-dd")) );
        inNumRowCassa = poQuery->numRowsAffected();


        m_dlgProgress->hideProgress();

        QMessageBox::information( this, tr("Information"),
                                  tr("The action successfully finished.\n\n"
                                     "Number of affected records:\n"
                                     "PatientCardUnits -> %1 records\n"
                                     "ProductHistory -> %2 records\n"
                                     "CassaHistory -> %3 records\n"
                                     "Ledger -> %4 records\n"
                                     "LedgerDevice -> %5 records\n"
                                     "Denominations -> %6 records\n"
                                     "Cassa -> %7 records"
                                     )
                                  .arg(inNumRowPCUs)
                                  .arg(inNumRowProd)
                                  .arg(inNumRowCass)
                                  .arg(inNumRowLedger)
                                  .arg(inNumRowDeviceLedger)
                                  .arg(inNumRowDenominations)
                                  .arg(inNumRowCassa)
                                  );
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
        g_obGen.showTrayError( e.what() );
    }

    m_dlgProgress->hideProgress();
}

void cDlgManageDatabase::_actionRepairPatientcardsWithoutType()
{
    int uiPatientCardTypeId = cmbPatientCardType->itemData( cmbPatientCardType->currentIndex() ).toInt();

    if( uiPatientCardTypeId > -1 )
    {
        m_dlgProgress->showProgress();

        try
        {
            cDBPatientCardType  obDBPatientCardType;

            obDBPatientCardType.load( uiPatientCardTypeId );

            QStringList qslCardId;
            QString qsQuery = QString( "SELECT patientCardId FROM patientcards WHERE "
                                       "patientCardId>1 AND "
                                       "patientCardTypeId=0 AND "
                                       "active=1 "
                                      );
            QSqlQuery *poQuery = g_poDB->executeQTQuery( qsQuery );

            while( poQuery->next() )
            {
                qslCardId.append( poQuery->value(0).toString() );
            }

            m_dlgProgress->showProgressBar( qslCardId.count() );

            for( int i=0; i<qslCardId.count(); i++ )
            {
                unsigned int    uiId = qslCardId.at(i).toUInt();
                cDBPatientCard  obDBPatientCard;

                obDBPatientCard.load( uiId );
//                obDBPatientCard.setPatientCardTypeId( uiPatientCardTypeId );
//                obDBPatientCard.save();

                qsQuery = QString( "UPDATE patientcardunits SET "
                                   "patientCardTypeId=%1, "
                                   "unitTime=%2, "
                                   "unitPrice=%3 "
                                   "WHERE "
                                   "patientCardId=%4 AND "
                                   "(patientCardTypeId=0 OR "
                                   "unitTime=0 OR "
                                   "unitPrice=0) ")
                                .arg( uiPatientCardTypeId )
                                .arg( obDBPatientCardType.unitTime() )
                                .arg( obDBPatientCardType.price() )
                                .arg( obDBPatientCard.id() );

                m_dlgProgress->stepProgressBar();
            }

            QMessageBox::information( this, tr("Information"),
                                      tr("The action successfully finished.\n"
                                         "Number of repaired patientcards: %1")
                                      .arg( qslCardId.count() ) );
        }
        catch( cSevException &e )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
            g_obGen.showTrayError( e.what() );
        }

        m_dlgProgress->hideProgress();
    }
    else
    {
        QMessageBox::warning( this, tr("Warning"),
                              tr("Please select patientcard type to repair with the patientcards.") );
    }
}

