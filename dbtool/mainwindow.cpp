
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QVector>
#include <QCloseEvent>
#include <QTextStream>
#include <QtSql/QSqlQuery>
#include <QCryptographicHash>
#include <QDate>
#include <QTranslator>

#include "../framework/qtmysqlquerymodel.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

//=========================================================================================================================================
MainWindow::MainWindow(QWidget *parent, QString p_qsVersion) : QMainWindow(parent), ui(new Ui::MainWindow)
//-----------------------------------------------------------------------------------------------------------------------------------------
{
    ui->setupUi(this);

    m_nCurrentPage              = CONST_PAGE_START;
    m_qdExpCurrentDir           = "C:/KiwiSun/Belenus/";

    m_dlgProgress               = new cDlgProgress( this );
    m_qsLanguage                = "hu";

    m_qsRPSW                    = "7c01fcbe9cab6ae14c98c76cf943a7b2be6a7922";

    //------------------------------------------------------------------------------------------------
    // ezekre egyelore nincs szukseg
    ui->ledPathDB->setText( m_qdExpCurrentDir.path() );
    ui->ledPathDB->setEnabled( false );
    ui->ledPathDB->setVisible( false );
    ui->pbExpSelectDir->setVisible( false );
    //------------------------------------------------------------------------------------------------

    ui->pageController->setCurrentIndex( m_nCurrentPage );

    // Initialize GUI components
    ui->cmbLanguage->addItem( "Magyar (hu)" );
    ui->cmbLanguage->addItem( "English (en)" );

    ui->imgLogo->setPixmap( QPixmap(":/imgLogo/KiwiSun.png") );

    ui->imgConnectDbFail->setVisible( true );
    ui->imgConnectDbOk->setVisible( false );
    ui->imgCheckPswFail->setVisible( true );
    ui->imgCheckPswOk->setVisible( false );

    ui->qdPatientCardValid->setDate( QDate::currentDate() );

    QDate today = QDate::currentDate();
    QDate targetMonthStart = QDate(today.year(), today.month(), 1).addMonths(-2);  // aktuális hónap első napja - 2 hónap
    QDate monthEnd = targetMonthStart.addMonths(1).addDays(-1);  // az adott hónap utolsó napja

    ui->qdLedgerDate->setDate( monthEnd );

    ui->qdObsoleteDate->setDate( QDate::currentDate().addMonths(-1) );

    setWindowTitle( tr( "Belenus database manager - v." ) + p_qsVersion );

    connect( ui->cmbName, SIGNAL(returnPressed()), this, SLOT(on_pbLogin_clicked()) );
    connect( ui->ledPassword, SIGNAL(returnPressed()), this, SLOT(on_pbLogin_clicked()) );

    _connectDatabase();

    cQTMySQLQueryModel *m_poModel = new cQTMySQLQueryModel( this );
    m_poModel->setQuery( "SELECT CONCAT(name,\" (\",realName,\")\") AS n FROM users WHERE active = 1 ORDER BY name" );
    ui->cmbName->setModel( m_poModel );

    _initializePage();
}
//=========================================================================================================================================
MainWindow::~MainWindow()
//-----------------------------------------------------------------------------------------------------------------------------------------
{
    delete m_dlgProgress;
    delete ui;

    g_obLogger(cSeverity::INFO) << "Belenus Database Manager stopped." << EOM;
}
//=========================================================================================================================================
void MainWindow::on_pbNext_clicked()
//-----------------------------------------------------------------------------------------------------------------------------------------
{
    if( m_nCurrentPage < CONST_PAGE_EXECUTE_PROCESS )
        m_nCurrentPage++;

    ui->pbPrev->setEnabled( m_nCurrentPage > CONST_PAGE_START );
    ui->pbNext->setEnabled( m_nCurrentPage < CONST_PAGE_EXECUTE_PROCESS );

    ui->pageController->setCurrentIndex( m_nCurrentPage );

    _initializePage();
}
//=========================================================================================================================================
void MainWindow::on_pbPrev_clicked()
//-----------------------------------------------------------------------------------------------------------------------------------------
{
    if( m_nCurrentPage == CONST_PAGE_SELECT_PROCESS )
    {
        if( QMessageBox::question( this, tr("Question"), tr("Do you want to authenticate with another user account?"),
                                   QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::No )
        {
            return;
        }
    }

    if( m_nCurrentPage > CONST_PAGE_START )
        m_nCurrentPage--;

    ui->pbPrev->setEnabled( m_nCurrentPage > CONST_PAGE_START );
    ui->pbNext->setEnabled( m_nCurrentPage < CONST_PAGE_EXECUTE_PROCESS );

    ui->pageController->setCurrentIndex( m_nCurrentPage );

    _initializePage();
}
//=========================================================================================================================================
void MainWindow::on_pbStartExit_clicked()
//-----------------------------------------------------------------------------------------------------------------------------------------
{
    close();
}
//=========================================================================================================================================
void MainWindow::closeEvent( QCloseEvent *p_poEvent )
//-----------------------------------------------------------------------------------------------------------------------------------------
{
    if( QMessageBox::question( this, tr("Question"),
                               tr("Are you sure you want to close the application?"),
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
    {
        p_poEvent->accept();
    }
    else
    {
        p_poEvent->ignore();
    }
}
//=========================================================================================================================================
void MainWindow::on_pbLogin_clicked()
//-----------------------------------------------------------------------------------------------------------------------------------------
{
    string  stName = ui->cmbName->currentText().toStdString();
    stName = stName.substr( 0, stName.find( '(' ) - 1 );
    authType    atRet = AUTH_ERROR;
    QByteArray  obPwdHash = QCryptographicHash::hash( ui->ledPassword->text().toAscii(), QCryptographicHash::Sha1 );

    m_enGroup = GROUP_MIN;

    if( QString::fromStdString(stName).compare( "root" ) == 0 )
    {
        if( m_qsRPSW.compare( QString( obPwdHash.toHex() ) ) == 0 )
        {
            atRet = AUTH_OK;
            m_enGroup = GROUP_ROOT;
        }
        else
        {
            atRet = AUTH_PASSWORD_INCORRECT;
        }
    }
    else
    {
        try
        {
            QSqlQuery *poQuery = g_poDB->executeQTQuery( "SELECT * FROM users WHERE name = \"" + QString::fromStdString(stName) + "\"" );

            if( poQuery->size() != 1 )
            {
                atRet = AUTH_USER_NOTFOUND;
            }
            else
            {
                poQuery->first();
                if( poQuery->value(4).toString().compare( QString( obPwdHash.toHex() ) ) == 0 )
                {
                    atRet = AUTH_OK;
                    m_enGroup = poQuery->value(5).toInt();
                    m_uiUserId = poQuery->value(0).toUInt();
                }
                else
                {
                    atRet = AUTH_PASSWORD_INCORRECT;
                }
            }
        }
        catch( cSevException &e )
        {
            atRet = AUTH_CONNECTION_FAILED;
        }
    }

    switch( atRet )
    {
        case AUTH_OK:
            ui->pbNext->setEnabled( true );
            ui->imgCheckPswFail->setVisible( false );
            ui->imgCheckPswOk->setVisible( true );
            ui->cmbName->setEnabled( false );
            ui->ledPassword->setEnabled( false );
            ui->pbLogin->setEnabled( false );
            ui->pbNext->setFocus();
            break;

        case AUTH_PASSWORD_INCORRECT:
            ui->pbNext->setEnabled( false );
            QMessageBox::warning( this, tr("Warning"), tr("Password is incorrect") );
            break;

        case AUTH_CONNECTION_FAILED:
            ui->pbNext->setEnabled( false );
            QMessageBox::critical( this, tr("Error"), tr("Unable to retrieve user information from database!") );
            break;

        default:
            ui->pbNext->setEnabled( false );
            break;
    }
}
//=========================================================================================================================================
void MainWindow::on_pbExpSelectDir_clicked()
//-----------------------------------------------------------------------------------------------------------------------------------------
{
    QString qsDirectory = QFileDialog::getExistingDirectory(this, tr("Select Directory"), m_qdExpCurrentDir.absolutePath() );

    if (!qsDirectory.isEmpty())
    {
        m_qdExpCurrentDir = QDir( qsDirectory );
        ui->ledPathDB->setText( qsDirectory );
    }
}
//=========================================================================================================================================
void MainWindow::_initializePage()
//-----------------------------------------------------------------------------------------------------------------------------------------
{
    switch( m_nCurrentPage )
    {
        case CONST_PAGE_START:
        {
            ui->cmbName->setEnabled( true );
            ui->ledPassword->setEnabled( true );
            ui->pbLogin->setEnabled( true );
            ui->imgCheckPswFail->setVisible( true );
            ui->imgCheckPswOk->setVisible( false );
            ui->pbPrev->setEnabled( false );
            ui->pbPrev->setVisible( false );
            ui->pbNext->setEnabled( false );
            ui->pbNext->setVisible( true );
            ui->ledPassword->setText( "" );
            ui->cmbName->setFocus();
            break;
        }
        case CONST_PAGE_SELECT_PROCESS:
        {
            ui->pbPrev->setEnabled( true );
            ui->pbPrev->setVisible( true );
            ui->pbNext->setEnabled( false );
            ui->pbNext->setVisible( true );

            ui->rbDeactivatePatientCards->setChecked( false );
            ui->qdPatientCardValid->setEnabled( false );
            ui->chkDeleteDeactivatedPatientCards->setEnabled( false );
            ui->chkDeleteDeactivatedPatientCards->setChecked( false );

            ui->rbDeleteLedgerEntries->setChecked( false );
            ui->qdLedgerDate->setEnabled( false );

            ui->rbDeleteObsoletePCUnits->setChecked( false );
            ui->qdObsoleteDate->setEnabled( false );
            break;
        }
        case CONST_PAGE_EXECUTE_PROCESS:
        {
            ui->pbPrev->setEnabled( true );
            ui->pbPrev->setVisible( true );
            ui->pbNext->setEnabled( false );
            ui->pbNext->setVisible( false );
            ui->lblNotificationBeforeProcess->setVisible( true );
            ui->pbExecuteProcess->setEnabled( true );
            ui->listProgress->clear();
            break;
        }
    }
}
//=========================================================================================================================================
void MainWindow::_connectDatabase()
//-----------------------------------------------------------------------------------------------------------------------------------------
{
    g_poDB = new cQTMySQLConnection;
    g_poDB->setHostName( "localhost" );
    g_poDB->setDatabaseName( "belenus" );
    g_poDB->setUserName( "belenus" );
    g_poDB->setPassword( "belenus" );
    g_poDB->open();

    if( g_poDB->isOpen() )
    {
        ui->imgConnectDbFail->setVisible( false );
        ui->imgConnectDbOk->setVisible( true );

        QSqlQuery *poQuery = g_poDB->executeQTQuery( "SELECT * FROM licences" );

        poQuery->last();
        m_nLicenceId = poQuery->value(0).toInt();

        ui->imgDatabase->setToolTip( tr("License code: %1 [%2]").arg(poQuery->value(1).toString()).arg(m_nLicenceId) );
    }
    else
    {
        ui->imgConnectDbFail->setVisible( true );
        ui->imgConnectDbOk->setVisible( false );
        ui->imgDatabase->setToolTip( "" );
    }

    ui->pbNext->setEnabled( _isSystemVerificationOk() );
}
//=========================================================================================================================================
bool MainWindow::_isSystemVerificationOk()
//-----------------------------------------------------------------------------------------------------------------------------------------
{
    return ( ui->imgCheckPswOk->isVisible() && ui->imgConnectDbOk->isVisible() );
}
//=========================================================================================================================================
void MainWindow::_logProcessInfo(QString p_qsText)
//-----------------------------------------------------------------------------------------------------------------------------------------
{
    ui->listProgress->addItem( p_qsText );
    g_obLogger(cSeverity::INFO) << p_qsText << EOM;
}


//=========================================================================================================================================
//
// PAGE START
//
//=========================================================================================================================================

//=========================================================================================================================================
void MainWindow::on_cmbLanguage_currentIndexChanged(int /*index*/)
//-----------------------------------------------------------------------------------------------------------------------------------------
{
    m_qsLanguage = ui->cmbLanguage->itemText( ui->cmbLanguage->currentIndex() ).right(3).left(2);

    apMainApp->removeTranslator( poTransTool );
    apMainApp->removeTranslator( poTransQT );

    if( m_qsLanguage.compare("en") )
    {
        QString qsLangTool = QString("%1\\lang\\dbtool_%2.qm").arg(g_qsCurrentPath).arg( m_qsLanguage );
        QString qsLangQT = QString("%1\\lang\\qt_%2.qm").arg(g_qsCurrentPath).arg( m_qsLanguage );

        poTransTool->load( qsLangTool );
        poTransQT->load( qsLangQT );

        apMainApp->installTranslator( poTransTool );
        apMainApp->installTranslator( poTransQT );
    }

    ui->retranslateUi( this );
}
//=========================================================================================================================================
//
// PAGE PROCESS SELECTION
//
//=========================================================================================================================================
void MainWindow::on_rbDeactivatePatientCards_clicked()
//-----------------------------------------------------------------------------------------------------------------------------------------
{
    ui->qdPatientCardValid->setEnabled( true );
    ui->chkDeleteDeactivatedPatientCards->setEnabled( true );
    ui->qdLedgerDate->setEnabled( false );
    ui->qdObsoleteDate->setEnabled( false );

    ui->pbNext->setEnabled( true );
}
//=========================================================================================================================================
void MainWindow::on_rbDeleteLedgerEntries_clicked()
//-----------------------------------------------------------------------------------------------------------------------------------------
{
    ui->qdPatientCardValid->setEnabled( false );
    ui->chkDeleteDeactivatedPatientCards->setEnabled( false );
    ui->qdLedgerDate->setEnabled( true );
    ui->qdObsoleteDate->setEnabled( false );

    ui->pbNext->setEnabled( true );
}
//=========================================================================================================================================
void MainWindow::on_rbDeleteObsoletePCUnits_clicked()
//-----------------------------------------------------------------------------------------------------------------------------------------
{
    ui->qdPatientCardValid->setEnabled( false );
    ui->chkDeleteDeactivatedPatientCards->setEnabled( false );
    ui->qdLedgerDate->setEnabled( false );
    ui->qdObsoleteDate->setEnabled( true );

    ui->pbNext->setEnabled( true );
}

//=========================================================================================================================================
//
// PAGE PROCESS EXECUTION
//
//=========================================================================================================================================
void MainWindow::on_pbExecuteProcess_clicked()
//-----------------------------------------------------------------------------------------------------------------------------------------
{
    ui->lblNotificationBeforeProcess->setVisible( false );
    ui->pbExecuteProcess->setEnabled( false );

    if( ui->rbDeactivatePatientCards->isChecked() )
    {
        _processDeactivatePatientcards();
        if( ui->chkDeleteDeactivatedPatientCards->isChecked() )
        {
            _processDeleteDeactivatedPatientcards();
        }
    }
    else if( ui->rbDeleteLedgerEntries->isChecked() )
    {
        _processDeleteLedgerEntries();
    }
    else if( ui->rbDeleteObsoletePCUnits->isChecked() )
    {
        _processDeleteObsoletePatientcardUnits();
    }
}

//=========================================================================================================================================
//
// Processes
//
//=========================================================================================================================================
void MainWindow::_processDeactivatePatientcards()
//-----------------------------------------------------------------------------------------------------------------------------------------
{
    _logProcessInfo( tr("Deactivating patientcards expired before ") + ui->qdPatientCardValid->date().toString("yyyy-MM-dd") );

    QString qsSelect = QString( "SELECT patientCardId FROM patientcards WHERE validDateTo < \"%1\" AND active = 1 ").arg( ui->qdPatientCardValid->date().toString("yyyy-MM-dd") );
    QSqlQuery oSelect(qsSelect);

    QList<int> cardIds;

    _logProcessInfo( tr("Retrieving affected records" ) );
    while( oSelect.next() )
    {
        cardIds.append(oSelect.value(0).toInt());
    }

    _logProcessInfo( tr("Number of affected records: " ) + QString::number( cardIds.size() ) );
    m_dlgProgress->setMaxValue( cardIds.size() );

    int nUpdated = 0;

    _logProcessInfo( tr("Deactivate records ..." ) );
    m_dlgProgress->show();
    for (int i = 0; i < cardIds.size(); ++i )
    {
        int nCardId = cardIds[i];

        QString qsUpdate = QString( "UPDATE patientcards SET "
                                    "patientCardTypeId=0, parentCardId=0, patientId=0, comment=\"\", units=0, "
                                    "amount=0, timeLeft=0, validDateFrom=\"0000-00-00\", validDateTo=\"0000-00-00\", "
                                    "pincode=NULL, modified=\"0000-00-00 00:00:00\", active=0, archive=\"ARC\" "
                                    "WHERE patientCardId=%1").arg(nCardId);

        QSqlQuery oUpdate(qsUpdate);
        ++nUpdated;

        m_dlgProgress->stepValue();
        QCoreApplication::processEvents(); // hogy a GUI frissüljön
    }
    m_dlgProgress->hide();
    _logProcessInfo( tr("Deactivation process finished" ) );
    ui->listProgress->addItem( "" );
}
//=========================================================================================================================================
void MainWindow::_processDeleteDeactivatedPatientcards()
//-----------------------------------------------------------------------------------------------------------------------------------------
{
    _logProcessInfo( tr( "Deleting deactivated patientcards" ) );

    QString qsSelect = QString( "SELECT patientCardId FROM patientcards WHERE "
                                "patientCardId > 1 AND "
                                "patientCardTypeId = 0 AND "
                                "patientId = 0 AND "
                                "(pincode IS NULL OR pincode = \"\") AND "
                                "active = 0" );

    QSqlQuery oSelect(qsSelect);
    QList<unsigned int> cardIds;

    _logProcessInfo( tr("Retrieving affected records" ) );
    while (oSelect.next())
    {
        cardIds.append(oSelect.value(0).toUInt());
    }

    _logProcessInfo( tr("Number of affected records: " ) + QString::number( cardIds.size() ) );

    int stepsPerCard = 5;
    m_dlgProgress->setMaxValue( cardIds.size() * stepsPerCard );

    _logProcessInfo( tr("Delete records ..." ) );
    m_dlgProgress->show();

    unsigned int nSkipped = 0;
    unsigned int nDeleted = 0;

    for (int i = 0; i < cardIds.size(); ++i)
    {
        unsigned int     uiId               = cardIds[i];
        QSqlQuery       *poQuery            = NULL;
        bool             bAssignedCardExists= false;
        bool             bLedgerConnected   = false;

        poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM patientCards WHERE parentCardId = %1" ).arg( uiId ) );
        if( poQuery->next() )
        {
            bAssignedCardExists = true;
        }

        poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM ledger WHERE patientCardId = %1" ).arg( uiId ) );
        if( poQuery->next() )
        {
            bLedgerConnected = true;
        }

        if( bAssignedCardExists || bLedgerConnected )
        {
            nSkipped++;
            for (int s = 0; s < stepsPerCard; ++s)
            {
                m_dlgProgress->stepValue();
                QCoreApplication::processEvents();
            }
            continue;
        }

        QStringList deleteQueries;
        deleteQueries << QString("DELETE FROM connectPatientWithCard WHERE patientCardId=%1").arg(uiId);
        deleteQueries << QString("DELETE FROM patientCardHistories WHERE patientCardId=%1").arg(uiId);
        deleteQueries << QString("DELETE FROM patientCardUnits WHERE patientCardId=%1").arg(uiId);
        deleteQueries << QString("DELETE FROM patientHistory WHERE patientCardId=%1").arg(uiId);
        deleteQueries << QString("DELETE FROM patientCards WHERE patientCardId=%1").arg(uiId);

        for (int nDelQ = 0; nDelQ < deleteQueries.size(); ++nDelQ)
        {
            QString query = deleteQueries.at(nDelQ);
            g_poDB->executeQTQuery(query);
            m_dlgProgress->stepValue();
            QCoreApplication::processEvents();
        }

        nDeleted++;
        m_dlgProgress->stepValue();
        QCoreApplication::processEvents();
    }

    m_dlgProgress->hide();
    _logProcessInfo(tr("Deletion process finished."));
    _logProcessInfo(tr("Skipped: ") + QString::number(nSkipped));
    _logProcessInfo(tr("Deleted: ") + QString::number(nDeleted));
    ui->listProgress->addItem( "" );
}
//=========================================================================================================================================
void MainWindow::_processDeleteLedgerEntries()
//-----------------------------------------------------------------------------------------------------------------------------------------
{
    _logProcessInfo( tr( "Deleting ledger entries" ) );

    try
    {
        QList<unsigned int> ledgerIds;
        int                 progressMax = 0;

        // Ledger rekordok összegyűjtése
        QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT ledgerId FROM ledger WHERE ledgerTime < \"%1\" AND ledgerId > 0" )
                                                              .arg(ui->qdLedgerDate->date().toString("yyyy-MM-dd")));

        while( poQuery->next() )
        {
            ledgerIds.append( poQuery->value(0).toUInt() );
        }

        // Előzetes számlálás a progress bar-hoz
        progressMax += ledgerIds.size() * 4; // ledger rekordonként 4 update/delete

        // Számoljuk meg a ledgerdevice törlendő rekordokat
        poQuery = g_poDB->executeQTQuery( QString( "SELECT COUNT(*) FROM ledgerdevice WHERE ledgerTime < \"%1\" AND ledgerDeviceId > 0" )
                                                   .arg( ui->qdLedgerDate->date().toString("yyyy-MM-dd") ) );
        if( poQuery->next() )
        {
            progressMax += poQuery->value(0).toInt(); // minden törölt ledgerdevice sor egy lépés
        }

        // Cassa rekordok lekérdezése
        poQuery = g_poDB->executeQTQuery( QString( "SELECT cassaId FROM cassa WHERE startDateTime < \"%1\" AND stopDateTime < \"%1\" AND cassaId > 0" )
                                          .arg(ui->qdLedgerDate->date().toString("yyyy-MM-dd")));

        QStringList qslCassaIds;
        while (poQuery->next())
        {
            qslCassaIds << poQuery->value(0).toString();
        }
        progressMax += qslCassaIds.size() * 3; // cassaHistory + cassadenominations + cassa

        _logProcessInfo( tr("Number of affected records: ")+QString::number( progressMax ) );

        // Progress bar inicializálása
        m_dlgProgress->setMaxValue(progressMax);
        m_dlgProgress->show();

        // Ledger rekordok törlése
        for( int i = 0; i < ledgerIds.size(); ++i )
        {
            unsigned int uiId = ledgerIds[i];

            QStringList deleteQueries;
            deleteQueries << QString("UPDATE patientCardUnits SET ledgerId = 0 WHERE ledgerId = %1").arg(uiId);
            deleteQueries << QString("UPDATE productHistory SET ledgerId = 0 WHERE ledgerId = %1").arg(uiId);
            deleteQueries << QString("UPDATE cassaHistory SET ledgerId = 0 WHERE ledgerId = %1").arg(uiId);
            deleteQueries << QString("DELETE FROM ledger WHERE ledgerId = %1").arg(uiId);

            for( int nDelQ = 0; nDelQ < deleteQueries.size(); ++nDelQ )
            {
                g_poDB->executeQTQuery(deleteQueries.at(nDelQ));
                m_dlgProgress->stepValue();
                QCoreApplication::processEvents();
            }
        }

        // LedgerDevice törlése
        poQuery = g_poDB->executeQTQuery( QString( "SELECT ledgerDeviceId FROM ledgerdevice WHERE ledgerTime < \"%1\" AND ledgerDeviceId > 0" )
                                                   .arg(ui->qdLedgerDate->date().toString("yyyy-MM-dd")));

        QStringList ledgerDeviceIds;
        while( poQuery->next() )
        {
            ledgerDeviceIds << poQuery->value(0).toString();
        }

        for( int i = 0; i < ledgerDeviceIds.size(); ++i )
        {
            g_poDB->executeQTQuery( QString( "DELETE FROM ledgerdevice WHERE ledgerDeviceId = %1" ).arg( ledgerDeviceIds.at(i) ) );
            m_dlgProgress->stepValue();
            QCoreApplication::processEvents();
        }

        // CassaHistory, CassaDenominations és Cassa törlése
        for( int i = 0; i < qslCassaIds.count(); ++i )
        {
            QString id = qslCassaIds.at(i);

            g_poDB->executeQTQuery( QString( "DELETE FROM cassaHistory WHERE cassaId = %1" ).arg( id ));
            m_dlgProgress->stepValue();
            QCoreApplication::processEvents();

            g_poDB->executeQTQuery( QString( "DELETE FROM cassadenominations WHERE cassaId = %1" ).arg( id ) );
            m_dlgProgress->stepValue();
            QCoreApplication::processEvents();

            g_poDB->executeQTQuery( QString( "DELETE FROM cassa WHERE cassaId = %1" ).arg( id ) );
            m_dlgProgress->stepValue();
            QCoreApplication::processEvents();
        }

    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
    }
    m_dlgProgress->hide();
    _logProcessInfo(tr("Deletion process finished."));
    ui->listProgress->addItem( "" );
}
//=========================================================================================================================================
void MainWindow::_processDeleteObsoletePatientcardUnits()
//-----------------------------------------------------------------------------------------------------------------------------------------
{
    _logProcessInfo( tr("Deleting patientcard units expired before ") + ui->qdObsoleteDate->date().toString("yyyy-MM-dd") );

    QString qsSelect = QString( "SELECT patientCardUnitId FROM patientcardunits WHERE validDateTo < \"%1\" ").arg( ui->qdObsoleteDate->date().toString("yyyy-MM-dd") );
    QSqlQuery oSelect(qsSelect);

    QList<int> cardIds;

    _logProcessInfo( tr("Retrieving affected records" ) );
    while( oSelect.next() )
    {
        cardIds.append(oSelect.value(0).toInt());
    }

    _logProcessInfo( tr("Number of affected records: " ) + QString::number( cardIds.size() ) );
    m_dlgProgress->setMaxValue( cardIds.size() );

    int nUpdated = 0;

    _logProcessInfo( tr("Deleting records ..." ) );
    m_dlgProgress->show();
    for (int i = 0; i < cardIds.size(); ++i )
    {
        int nCardId = cardIds[i];

        QString qsDelete = QString( "DELETE FROM patientcardunits WHERE patientCardUnitId=%1").arg(nCardId);

        QSqlQuery oDelete( qsDelete );
        ++nUpdated;

        m_dlgProgress->stepValue();
        QCoreApplication::processEvents(); // hogy a GUI frissüljön
    }
    m_dlgProgress->hide();
    _logProcessInfo( tr("Delete process finished" ) );
    ui->listProgress->addItem( "" );
}

