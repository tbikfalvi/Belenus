
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

    ui->rbDefault->setVisible( false );
    ui->qdFilterDate->setDate( QDate::currentDate() );

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
void MainWindow::on_rbDefault_toggled(bool checked)
//-----------------------------------------------------------------------------------------------------------------------------------------
{
    ui->pbNext->setEnabled( !checked );
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

            ui->rbDefault->setChecked( true );
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
            _calculateRelatedRecords();
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

//=========================================================================================================================================
//
// PAGE PROCESS EXECUTION
//
//=========================================================================================================================================
void MainWindow::_calculateRelatedRecords()
//-----------------------------------------------------------------------------------------------------------------------------------------
{
    if( ui->rbDeactivatePatientCards->isChecked() )
    {
        _calculateDeactivatePatientcards();
    }
    else if( ui->rbDeleteDeactivatedPatientcards->isChecked() )
    {
        _calculateDeleteDeactivatedPatientcards();
    }
    else if( ui->rbDeleteObsoletePCUnits->isChecked() )
    {
        _calculateDeleteObsoletePatientcardUnits();
    }
    else if( ui->rbDeleteLedgerEntries->isChecked() )
    {
        _calculateDeleteLedgerEntries();
    }
    else if( ui->rbDeleteDeviceLedgerEntries->isChecked() )
    {
        _calculateDeleteDeviceLedger();
    }
    else if( ui->rbDeleteCassaEntries->isChecked() )
    {
        _calculateDeleteCassa();
    }
}
//=========================================================================================================================================
void MainWindow::on_pbExecuteProcess_clicked()
//-----------------------------------------------------------------------------------------------------------------------------------------
{
    ui->lblNotificationBeforeProcess->setVisible( false );
    ui->pbExecuteProcess->setEnabled( false );

    if( ui->rbDeactivatePatientCards->isChecked() )
    {
        _processDeactivatePatientcards();
    }
    else if( ui->rbDeleteDeactivatedPatientcards->isChecked() )
    {
        _processDeleteDeactivatedPatientcards();
    }
    else if( ui->rbDeleteObsoletePCUnits->isChecked() )
    {
        _processDeleteObsoletePatientcardUnits();
    }
    else if( ui->rbDeleteLedgerEntries->isChecked() )
    {
        _processDeleteLedgerEntries();
    }
    else if( ui->rbDeleteDeviceLedgerEntries->isChecked() )
    {
        _processDeleteDeviceLedger();
    }
    else if( ui->rbDeleteCassaEntries->isChecked() )
    {
        _processDeleteCassa();
    }
}

//=========================================================================================================================================
//
// Processes
//
//=========================================================================================================================================
void MainWindow::_calculateDeactivatePatientcards()
//-----------------------------------------------------------------------------------------------------------------------------------------
{
    int nCount = 0;

    QString qsSelect = QString( "SELECT COUNT(patientCardId) FROM patientcards WHERE validDateTo < \"%1\" AND active = 1 ").arg( ui->qdFilterDate->date().toString("yyyy-MM-dd") );
    QSqlQuery oSelect(qsSelect);

    if( oSelect.next() )
        nCount = oSelect.value(0).toInt();

    //*************************************
    const double avgMsPerRecord = 0.73;
    //*************************************

    int estimatedMs = static_cast<int>(nCount * avgMsPerRecord);
    int estMinutes = estimatedMs / (1000 * 60);
    int estSeconds = (estimatedMs / 1000) % 60;

    // Szöveges idő formázása
    QString estTimeString;
    if (estMinutes > 0)
    {
        estTimeString = QString("%1 %2 %3 %4")
                        .arg(estMinutes)
                        .arg((estMinutes == 1) ? tr("minute") : tr("minutes"))
                        .arg(estSeconds)
                        .arg((estSeconds == 1) ? tr("second") : tr("seconds"));
    }
    else
    {
        estTimeString = QString("%1 %2")
                        .arg(estSeconds)
                        .arg((estSeconds == 1) ? tr("second") : tr("seconds"));
    }

    ui->listProgress->addItem( tr( "Deactivating patientcards expired before " ) + ui->qdFilterDate->date().toString( "yyyy-MM-dd" ) );
    ui->listProgress->addItem( tr( "Number of affected records: " ) + QString::number( nCount ) );
    ui->listProgress->addItem( tr( "Estimated deactivation time: " ) + estTimeString);
}
//=========================================================================================================================================
void MainWindow::_processDeactivatePatientcards()
//-----------------------------------------------------------------------------------------------------------------------------------------
{
    ui->listProgress->addItem( "" );
    _logProcessInfo( tr( "Process started" ) );

    QString qsSelect = QString( "SELECT patientCardId FROM patientcards WHERE validDateTo < \"%1\" AND active = 1 ").arg( ui->qdFilterDate->date().toString("yyyy-MM-dd") );
    QSqlQuery oSelect(qsSelect);

    QStringList cardIds;

    _logProcessInfo( tr( "Retrieving record ids" ) );
    while( oSelect.next() )
        cardIds.append( oSelect.value(0).toString() );

    m_dlgProgress->setMaxValue( cardIds.size() );

    _logProcessInfo( tr( "Deactivating records" ) );
    m_dlgProgress->show();

    // ⏱ Időmérés indítása
    QTime timer;
    timer.start();

    const int batchSize = 100;
    for( int i = 0; i < cardIds.size(); i += batchSize )
    {
        QStringList batch;
        for (int j = 0; j < batchSize && (i + j) < cardIds.size(); ++j)
            batch << cardIds.at(i + j);

        QString qsUpdate = QString( "UPDATE patientcards SET "
                                    "patientCardTypeId=0, parentCardId=0, patientId=0, comment=\"\", units=0, "
                                    "amount=0, timeLeft=0, validDateFrom=\"0000-00-00\", validDateTo=\"0000-00-00\", "
                                    "pincode=NULL, modified=\"0000-00-00 00:00:00\", active=0, archive=\"ARC\" "
                                    "WHERE patientCardId IN (%1) ").arg( batch.join(",") );

        g_poDB->executeQTQuery( qsUpdate );

        for (int k = 0; k < batch.size(); ++k)
        {
            m_dlgProgress->stepValue();
            QCoreApplication::processEvents(); // hogy a GUI frissüljön
        }
    }

    int elapsedMs = timer.elapsed();
    int minutes = elapsedMs / (1000 * 60);
    int seconds = (elapsedMs / 1000) % 60;
    int tenths  = (elapsedMs % 1000) / 100;

    QString timeString = QString("%1:%2.%3").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0')).arg(tenths);

    _logProcessInfo( tr( "Process finished (mm:ss.t) %1" ).arg( timeString ) );

    m_dlgProgress->hide();
}
//=========================================================================================================================================
void MainWindow::_calculateDeleteDeactivatedPatientcards()
//-----------------------------------------------------------------------------------------------------------------------------------------
{
    int nCount = 0;

    QString qsSelect1 = QString( "SELECT COUNT(patientCardId) FROM patientcards WHERE validDateTo < \"%1\" AND active = 1 ").arg( ui->qdFilterDate->date().toString("yyyy-MM-dd") );
    QSqlQuery oSelect1(qsSelect1);

    if( oSelect1.next() )
        nCount += oSelect1.value(0).toInt();

    QString qsSelect2 = QString( "SELECT COUNT(patientCardId) FROM patientcards WHERE "
                                "patientCardId > 1 AND "
                                "patientCardTypeId = 0 AND "
                                "patientId = 0 AND "
                                "(pincode IS NULL OR pincode = \"\") AND "
                                "active = 0" );
    QSqlQuery oSelect2(qsSelect2);

    if( oSelect2.next() )
        nCount += oSelect2.value(0).toInt();

    //*************************************
    const double avgMsPerRecord = 7.1;
    //*************************************

    int estimatedMs = static_cast<int>(nCount * avgMsPerRecord);
    int estMinutes = estimatedMs / (1000 * 60);
    int estSeconds = (estimatedMs / 1000) % 60;

    // Szöveges idő formázása
    QString estTimeString;
    if (estMinutes > 0)
    {
        estTimeString = QString("%1 %2 %3 %4")
                        .arg(estMinutes)
                        .arg((estMinutes == 1) ? tr("minute") : tr("minutes"))
                        .arg(estSeconds)
                        .arg((estSeconds == 1) ? tr("second") : tr("seconds"));
    }
    else
    {
        estTimeString = QString("%1 %2")
                        .arg(estSeconds)
                        .arg((estSeconds == 1) ? tr("second") : tr("seconds"));
    }

    ui->listProgress->addItem( tr( "Deleting deactivated patientcards (except those linked to other entries in database)" ) );
    ui->listProgress->addItem( tr( "Number of affected records: " ) + QString::number( nCount ) );
    ui->listProgress->addItem( tr( "Estimated delete time: " ) + estTimeString);
}
//=========================================================================================================================================
void MainWindow::_processDeleteDeactivatedPatientcards()
//-----------------------------------------------------------------------------------------------------------------------------------------
{
    ui->listProgress->addItem( "" );
    _logProcessInfo( tr( "Process started" ) );

    QString qsSelect = QString( "SELECT patientCardId FROM patientcards WHERE "
                                "patientCardId > 1 AND "
                                "patientCardTypeId = 0 AND "
                                "patientId = 0 AND "
                                "(pincode IS NULL OR pincode = \"\") AND "
                                "active = 0" );

    QSqlQuery oSelect(qsSelect);
    QStringList cardIds;

    _logProcessInfo( tr("Retrieving affected records" ) );
    while (oSelect.next())
        cardIds.append( oSelect.value(0).toString() );

    int stepsPerCard = 5;
    m_dlgProgress->setMaxValue( cardIds.size() * stepsPerCard );

    _logProcessInfo( tr("Sorting records" ) );
    m_dlgProgress->show();

    // ⏱ Időmérés indítása
    QTime timer;
    timer.start();

    unsigned int    nSkipped = 0;
    unsigned int    nDeleted = 0;
    QStringList     deletableCardIds;

    for( int i = 0; i < cardIds.size(); ++i )
    {
        unsigned int uiId = cardIds[i].toUInt();
        QSqlQuery* poQuery = NULL;
        bool bAssignedCardExists = false;
        bool bLedgerConnected = false;

        poQuery = g_poDB->executeQTQuery( QString( "SELECT 1 FROM patientCards WHERE parentCardId = %1" ).arg( uiId ) );
        if( poQuery->next() )
            bAssignedCardExists = true;

        poQuery = g_poDB->executeQTQuery( QString( "SELECT 1 FROM ledger WHERE patientCardId = %1" ).arg( uiId ) );
        if( poQuery->next() )
            bLedgerConnected = true;

        if( bAssignedCardExists || bLedgerConnected )
        {
            nSkipped++;
            for( int s = 0; s < stepsPerCard; ++s )
            {
                m_dlgProgress->stepValue();
                QCoreApplication::processEvents();
            }
        }
        else
        {
            deletableCardIds << QString::number( uiId );
        }
    }

    _logProcessInfo( tr("Deleting records" ) );
    const int batchSize = 100;
    for( int i = 0; i < deletableCardIds.size(); i += batchSize )
    {
        QStringList batch;
        for( int j = 0; j < batchSize && (i + j) < deletableCardIds.size(); ++j )
            batch << deletableCardIds.at(i + j);

        QString ids = batch.join(",");

        QStringList deleteQueries;
        deleteQueries << QString("DELETE FROM connectPatientWithCard WHERE patientCardId IN (%1)").arg(ids);
        deleteQueries << QString("DELETE FROM patientCardHistories WHERE patientCardId IN (%1)").arg(ids);
        deleteQueries << QString("DELETE FROM patientCardUnits WHERE patientCardId IN (%1)").arg(ids);
        deleteQueries << QString("DELETE FROM patientHistory WHERE patientCardId IN (%1)").arg(ids);
        deleteQueries << QString("DELETE FROM patientCards WHERE patientCardId IN (%1)").arg(ids);

        for( int q = 0; q < deleteQueries.size(); ++q )
        {
            g_poDB->executeQTQuery( deleteQueries.at(q) );
            for (int k = 0; k < batch.size(); ++k)
            {
                m_dlgProgress->stepValue();
                QCoreApplication::processEvents();
            }
        }

        nDeleted += batch.size();
    }

    int elapsedMs = timer.elapsed();
    int minutes = elapsedMs / (1000 * 60);
    int seconds = (elapsedMs / 1000) % 60;
    int tenths  = (elapsedMs % 1000) / 100;

    QString timeString = QString("%1:%2.%3").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0')).arg(tenths);

    _logProcessInfo( tr( "Process finished (mm:ss.t) %1" ).arg( timeString ) );

    m_dlgProgress->hide();
    _logProcessInfo(tr("Records skipped: ") + QString::number(nSkipped));
    _logProcessInfo(tr("Records deleted: ") + QString::number(nDeleted));
}
//=========================================================================================================================================
void MainWindow::_calculateDeleteObsoletePatientcardUnits()
//-----------------------------------------------------------------------------------------------------------------------------------------
{
    int progressMax = 0;

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT COUNT(patientCardUnitId) FROM patientcardunits WHERE validDateTo < \"%1\" ").arg( ui->qdFilterDate->date().toString("yyyy-MM-dd") ) );

    if( poQuery->next() )
        progressMax += poQuery->value(0).toInt();

    //*************************************
    const double avgMsPerRecord = 0.5;
    //*************************************

    int estimatedMs = static_cast<int>(progressMax * avgMsPerRecord);
    int estMinutes = estimatedMs / (1000 * 60);
    int estSeconds = (estimatedMs / 1000) % 60;

    // Szöveges idő formázása
    QString estTimeString;
    if (estMinutes > 0)
    {
        estTimeString = QString("%1 %2 %3 %4")
                        .arg(estMinutes)
                        .arg((estMinutes == 1) ? tr("minute") : tr("minutes"))
                        .arg(estSeconds)
                        .arg((estSeconds == 1) ? tr("second") : tr("seconds"));
    }
    else
    {
        estTimeString = QString("%1 %2")
                        .arg(estSeconds)
                        .arg((estSeconds == 1) ? tr("second") : tr("seconds"));
    }

    ui->listProgress->addItem( tr( "Deleting patientcard units expired before " ) + ui->qdFilterDate->date().toString("yyyy-MM-dd") );
    ui->listProgress->addItem( tr( "Number of affected records: " ) + QString::number( progressMax ) );
    ui->listProgress->addItem( tr( "Estimated delete time: " ) + estTimeString );
}
//=========================================================================================================================================
void MainWindow::_processDeleteObsoletePatientcardUnits()
//-----------------------------------------------------------------------------------------------------------------------------------------
{
    ui->listProgress->addItem("");
    _logProcessInfo(tr("Process started"));

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT patientCardUnitId FROM patientcardunits WHERE validDateTo < \"%1\"" ).arg( ui->qdFilterDate->date().toString("yyyy-MM-dd") ) );

    QList<int> cardIds;

    _logProcessInfo( tr( "Retrieving affected records" ) );
    while( poQuery->next() )
    {
        cardIds.append( poQuery->value(0).toInt() );
    }

    _logProcessInfo( tr( "Deleting records" ) );
    m_dlgProgress->setMaxValue( cardIds.size() );
    m_dlgProgress->show();

    QTime timer;
    timer.start();

    const int batchSize = 100;
    for( int i = 0; i < cardIds.size(); i += batchSize )
    {
        QStringList batchIds;
        int currentBatchSize = qMin( batchSize, cardIds.size() - i );

        for( int j = 0; j < currentBatchSize; ++j )
        {
            batchIds << QString::number(cardIds[i + j]);
        }

        QString query = QString("DELETE FROM patientcardunits WHERE patientCardUnitId IN (%1)").arg(batchIds.join(","));

        g_poDB->executeQTQuery(query);

        for( int j = 0; j < currentBatchSize; ++j )
        {
            m_dlgProgress->stepValue();
            QCoreApplication::processEvents(); // GUI frissítés
        }
    }

    int elapsedMs = timer.elapsed();
    QString timeString = QString("%1:%2.%3")
        .arg(elapsedMs / 60000, 2, 10, QChar('0'))
        .arg((elapsedMs / 1000) % 60, 2, 10, QChar('0'))
        .arg((elapsedMs % 1000) / 100);

    _logProcessInfo( tr( "Process finished (mm:ss.t) %1" ).arg( timeString ) );

    m_dlgProgress->hide();
    ui->listProgress->addItem("");
}
//=========================================================================================================================================
void MainWindow::_calculateDeleteLedgerEntries()
//-----------------------------------------------------------------------------------------------------------------------------------------
{
    int progressMax = 0;

    // Ledger rekordok összegyűjtése
    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT COUNT(ledgerId) FROM ledger WHERE ledgerTime < \"%1\" AND ledgerId > 0" )
                                                          .arg(ui->qdFilterDate->date().toString("yyyy-MM-dd")));
    if( poQuery->next() )
        progressMax += poQuery->value(0).toInt() * 4;

    //*************************************
    const double avgMsPerRecord = 0.5;
    //*************************************

    int estimatedMs = static_cast<int>(progressMax * avgMsPerRecord);
    int estMinutes = estimatedMs / (1000 * 60);
    int estSeconds = (estimatedMs / 1000) % 60;

    // Szöveges idő formázása
    QString estTimeString;
    if (estMinutes > 0)
    {
        estTimeString = QString("%1 %2 %3 %4")
                        .arg(estMinutes)
                        .arg((estMinutes == 1) ? tr("minute") : tr("minutes"))
                        .arg(estSeconds)
                        .arg((estSeconds == 1) ? tr("second") : tr("seconds"));
    }
    else
    {
        estTimeString = QString("%1 %2")
                        .arg(estSeconds)
                        .arg((estSeconds == 1) ? tr("second") : tr("seconds"));
    }

    ui->listProgress->addItem( tr( "Deleting ledger entries before date " ) + ui->qdFilterDate->date().toString("yyyy-MM-dd") );
    ui->listProgress->addItem( tr( "Number of affected records: " ) + QString::number( progressMax ) );
    ui->listProgress->addItem( tr( "Estimated delete time: " ) + estTimeString );
}
//=========================================================================================================================================
void MainWindow::_processDeleteLedgerEntries()
//-----------------------------------------------------------------------------------------------------------------------------------------
{
    ui->listProgress->addItem( "" );
    _logProcessInfo( tr( "Process started" ) );

    QTime timer;

    try
    {
        QList<unsigned int> ledgerIds;
        int progressMax = 0;

        _logProcessInfo( tr("Retrieving affected records" ) );

        // Ledger rekordok összegyüjtése
        QSqlQuery *poQuery = g_poDB->executeQTQuery( QString("SELECT ledgerId FROM ledger WHERE ledgerTime < \"%1\" AND ledgerId > 0")
                                                            .arg(ui->qdFilterDate->date().toString("yyyy-MM-dd")));
        while( poQuery->next() )
            ledgerIds.append(poQuery->value(0).toUInt());

        progressMax += ledgerIds.size() * 4;

        _logProcessInfo( tr("Deleting records" ) );
        m_dlgProgress->setMaxValue(progressMax);
        m_dlgProgress->show();

        timer.start();

        // Batch ledger feldolgozás (100-asával)
        for (int i = 0; i < ledgerIds.size(); i += 100)
        {
            QStringList batchIds;
            for (int j = i; j < i + 100 && j < ledgerIds.size(); ++j)
            {
                batchIds << QString::number(ledgerIds.at(j));
            }

            QString ids = batchIds.join(",");

            QStringList updateQueries;
            updateQueries << QString("UPDATE patientCardUnits SET ledgerId = 0 WHERE ledgerId IN (%1)").arg(ids);
            updateQueries << QString("UPDATE productHistory SET ledgerId = 0 WHERE ledgerId IN (%1)").arg(ids);
            updateQueries << QString("UPDATE cassaHistory SET ledgerId = 0 WHERE ledgerId IN (%1)").arg(ids);
            updateQueries << QString("DELETE FROM ledger WHERE ledgerId IN (%1)").arg(ids);

            for (int q = 0; q < updateQueries.size(); ++q)
            {
                g_poDB->executeQTQuery(updateQueries.at(q));

                // A batch minden rekordja érintett, így mindegyikhez stepValue-t hívunk
                for (int k = 0; k < batchIds.size(); ++k)
                {
                    m_dlgProgress->stepValue();
                    QCoreApplication::processEvents();
                }
            }
        }
    }
    catch (cSevException &e)
    {
        g_obLogger(e.severity()) << e.what() << EOM;
    }

    int elapsedMs = timer.elapsed();
    int minutes = elapsedMs / (1000 * 60);
    int seconds = (elapsedMs / 1000) % 60;
    int tenths  = (elapsedMs % 1000) / 100;

    QString timeString = QString("%1:%2.%3")
        .arg(minutes, 2, 10, QChar('0'))
        .arg(seconds, 2, 10, QChar('0'))
        .arg(tenths);

    _logProcessInfo( tr( "Process finished (mm:ss.t) %1" ).arg( timeString ) );

    m_dlgProgress->hide();
    ui->listProgress->addItem("");
}
//=========================================================================================================================================
void MainWindow::_calculateDeleteDeviceLedger()
//-----------------------------------------------------------------------------------------------------------------------------------------
{
    int progressMax = 0;

    // Ledgerdevice rekordok lekérdezése
    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT COUNT(*) FROM ledgerdevice WHERE ledgerTime < \"%1\" AND ledgerDeviceId > 0" )
                                               .arg( ui->qdFilterDate->date().toString("yyyy-MM-dd") ) );
    if( poQuery->next() )
        progressMax += poQuery->value(0).toInt();

    //*************************************
    const double avgMsPerRecord = 0.5;
    //*************************************

    int estimatedMs = static_cast<int>(progressMax * avgMsPerRecord);
    int estMinutes = estimatedMs / (1000 * 60);
    int estSeconds = (estimatedMs / 1000) % 60;

    // Szöveges idő formázása
    QString estTimeString;
    if (estMinutes > 0)
    {
        estTimeString = QString("%1 %2 %3 %4")
                        .arg(estMinutes)
                        .arg((estMinutes == 1) ? tr("minute") : tr("minutes"))
                        .arg(estSeconds)
                        .arg((estSeconds == 1) ? tr("second") : tr("seconds"));
    }
    else
    {
        estTimeString = QString("%1 %2")
                        .arg(estSeconds)
                        .arg((estSeconds == 1) ? tr("second") : tr("seconds"));
    }

    ui->listProgress->addItem( tr( "Deleting device ledger entries before date " ) + ui->qdFilterDate->date().toString("yyyy-MM-dd") );
    ui->listProgress->addItem( tr( "Number of affected records: " ) + QString::number( progressMax ) );
    ui->listProgress->addItem( tr( "Estimated delete time: " ) + estTimeString);
}
//=========================================================================================================================================
void MainWindow::_processDeleteDeviceLedger()
//-----------------------------------------------------------------------------------------------------------------------------------------
{
    ui->listProgress->addItem( "" );
    _logProcessInfo( tr( "Process started" ) );

    QTime timer;

    try
    {
        int progressMax = 0;

        // LedgerDevice számolás
        QSqlQuery *poQuery = g_poDB->executeQTQuery( QString("SELECT COUNT(*) FROM ledgerdevice WHERE ledgerTime < \"%1\" AND ledgerDeviceId > 0")
                                                 .arg(ui->qdFilterDate->date().toString("yyyy-MM-dd")));
        if (poQuery->next())
            progressMax += poQuery->value(0).toInt();

        _logProcessInfo( tr("Deleting records" ) );
        m_dlgProgress->setMaxValue(progressMax);
        m_dlgProgress->show();

        timer.start();

        // LedgerDevice törlés batch-ben
        poQuery = g_poDB->executeQTQuery( QString( "SELECT ledgerDeviceId FROM ledgerdevice WHERE ledgerTime < \"%1\" AND ledgerDeviceId > 0" )
                                                 .arg(ui->qdFilterDate->date().toString("yyyy-MM-dd")));
        QStringList ledgerDeviceIds;
        while (poQuery->next())
            ledgerDeviceIds << poQuery->value(0).toString();

        const int batchSize = 100;
        for (int i = 0; i < ledgerDeviceIds.size(); i += batchSize)
        {
            QStringList batch;
            for (int j = 0; j < batchSize && (i + j) < ledgerDeviceIds.size(); ++j)
                batch << ledgerDeviceIds.at(i + j);

            QString cond = QString("ledgerDeviceId IN (%1)").arg(batch.join(","));
            QString deleteQuery = QString("DELETE FROM ledgerdevice WHERE %1").arg(cond);

            g_poDB->executeQTQuery(deleteQuery);

            for (int k = 0; k < batch.size(); ++k)
            {
                m_dlgProgress->stepValue();
                QCoreApplication::processEvents();
            }
        }
    }
    catch (cSevException &e)
    {
        g_obLogger(e.severity()) << e.what() << EOM;
    }

    int elapsedMs = timer.elapsed();
    int minutes = elapsedMs / (1000 * 60);
    int seconds = (elapsedMs / 1000) % 60;
    int tenths  = (elapsedMs % 1000) / 100;

    QString timeString = QString("%1:%2.%3")
        .arg(minutes, 2, 10, QChar('0'))
        .arg(seconds, 2, 10, QChar('0'))
        .arg(tenths);

    _logProcessInfo( tr( "Process finished (mm:ss.t) %1" ).arg( timeString ) );

    m_dlgProgress->hide();
    ui->listProgress->addItem("");
}
//=========================================================================================================================================
void MainWindow::_calculateDeleteCassa()
//-----------------------------------------------------------------------------------------------------------------------------------------
{
    int progressMax = 0;

    // Cassa rekordok lekérdezése
    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT COUNT(cassaId) FROM cassa WHERE startDateTime < \"%1\" AND stopDateTime < \"%1\" AND cassaId > 0" )
                                      .arg(ui->qdFilterDate->date().toString("yyyy-MM-dd")));
    if( poQuery->next() )
        progressMax += poQuery->value(0).toInt() * 3;

    //*************************************
    const double avgMsPerRecord = 0.5;
    //*************************************

    int estimatedMs = static_cast<int>(progressMax * avgMsPerRecord);
    int estMinutes = estimatedMs / (1000 * 60);
    int estSeconds = (estimatedMs / 1000) % 60;

    // Szöveges idő formázása
    QString estTimeString;
    if (estMinutes > 0)
    {
        estTimeString = QString("%1 %2 %3 %4")
                        .arg(estMinutes)
                        .arg((estMinutes == 1) ? tr("minute") : tr("minutes"))
                        .arg(estSeconds)
                        .arg((estSeconds == 1) ? tr("second") : tr("seconds"));
    }
    else
    {
        estTimeString = QString("%1 %2")
                        .arg(estSeconds)
                        .arg((estSeconds == 1) ? tr("second") : tr("seconds"));
    }

    ui->listProgress->addItem( tr( "Deleting cassa entries before date " ) + ui->qdFilterDate->date().toString("yyyy-MM-dd") );
    ui->listProgress->addItem( tr( "Number of affected records: " ) + QString::number( progressMax ) );
    ui->listProgress->addItem( tr( "Estimated delete time: " ) + estTimeString);
}
//=========================================================================================================================================
void MainWindow::_processDeleteCassa()
//-----------------------------------------------------------------------------------------------------------------------------------------
{
    ui->listProgress->addItem( "" );
    _logProcessInfo( tr( "Process started" ) );

    QTime timer;

    try
    {
        int progressMax = 0;

        _logProcessInfo( tr("Retrieving affected records" ) );

        // Cassa ID-k
        QSqlQuery *poQuery = g_poDB->executeQTQuery( QString("SELECT cassaId FROM cassa WHERE startDateTime < \"%1\" AND stopDateTime < \"%1\" AND cassaId > 0")
                                                 .arg(ui->qdFilterDate->date().toString("yyyy-MM-dd")));

        QStringList qslCassaIds;
        while (poQuery->next())
            qslCassaIds << poQuery->value(0).toString();

        progressMax += qslCassaIds.size() * 3;

        _logProcessInfo( tr("Deleting records" ) );
        m_dlgProgress->setMaxValue(progressMax);
        m_dlgProgress->show();

        timer.start();

        // Cassa törlés batch-ben
        const int batchSize = 100;
        for (int i = 0; i < qslCassaIds.size(); i += batchSize)
        {
            QStringList batch;
            for (int j = 0; j < batchSize && (i + j) < qslCassaIds.size(); ++j)
                batch << qslCassaIds.at(i + j);

            QString ids = batch.join(",");

            g_poDB->executeQTQuery(QString("DELETE FROM cassaHistory WHERE cassaId IN (%1)").arg(ids));
            for (int k = 0; k < batch.size(); ++k) { m_dlgProgress->stepValue(); QCoreApplication::processEvents(); }

            g_poDB->executeQTQuery(QString("DELETE FROM cassadenominations WHERE cassaId IN (%1)").arg(ids));
            for (int k = 0; k < batch.size(); ++k) { m_dlgProgress->stepValue(); QCoreApplication::processEvents(); }

            g_poDB->executeQTQuery(QString("DELETE FROM cassa WHERE cassaId IN (%1)").arg(ids));
            for (int k = 0; k < batch.size(); ++k) { m_dlgProgress->stepValue(); QCoreApplication::processEvents(); }
        }
    }
    catch (cSevException &e)
    {
        g_obLogger(e.severity()) << e.what() << EOM;
    }

    int elapsedMs = timer.elapsed();
    int minutes = elapsedMs / (1000 * 60);
    int seconds = (elapsedMs / 1000) % 60;
    int tenths  = (elapsedMs % 1000) / 100;

    QString timeString = QString("%1:%2.%3")
        .arg(minutes, 2, 10, QChar('0'))
        .arg(seconds, 2, 10, QChar('0'))
        .arg(tenths);

    _logProcessInfo( tr( "Process finished (mm:ss.t) %1" ).arg( timeString ) );

    m_dlgProgress->hide();
    ui->listProgress->addItem("");
}


