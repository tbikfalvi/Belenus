
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

    ui->pbNext->setEnabled( true );
}
//=========================================================================================================================================
void MainWindow::on_rbDeleteLedgerEntries_clicked()
//-----------------------------------------------------------------------------------------------------------------------------------------
{
    ui->qdPatientCardValid->setEnabled( false );
    ui->chkDeleteDeactivatedPatientCards->setEnabled( false );

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
    ui->listProgress->addItem( tr("Deactivating patientcards expired before ") + ui->qdPatientCardValid->date().toString("yyyy-MM-dd") );

    QString qsSelect = QString( "SELECT patientCardId FROM patientcards WHERE validDateTo < \"%1\" AND active = 1 ").arg( ui->qdPatientCardValid->date().toString("yyyy-MM-dd") );
    QSqlQuery oSelect(qsSelect);

    QList<int> cardIds;

    ui->listProgress->addItem( tr("Retrieving affected records" ) );
    while( oSelect.next() )
    {
        cardIds.append(oSelect.value(0).toInt());
    }

    ui->listProgress->addItem( tr("Number of affected records: " ) + QString::number( cardIds.size() ) );
    m_dlgProgress->setMaxValue( cardIds.size() );

    int nUpdated = 0;

    ui->listProgress->addItem( tr("Deactivate records ..." ) );
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
    ui->listProgress->addItem( tr("Deactivation process finished" ) );
    ui->listProgress->addItem( "" );
}
//=========================================================================================================================================
void MainWindow::_processDeleteDeactivatedPatientcards()
//-----------------------------------------------------------------------------------------------------------------------------------------
{
    ui->listProgress->addItem( tr( "Deleting deactivated patientcards" ) );

    QString qsSelect = QString( "SELECT patientCardId FROM patientcards WHERE "
                                "patientCardId > 1 AND "
                                "patientCardTypeId = 0 AND "
                                "patientId = 0 AND "
                                "(pincode IS NULL OR pincode = \"\") AND "
                                "active = 0" );

    QSqlQuery oSelect(qsSelect);
    QList<unsigned int> cardIds;

    ui->listProgress->addItem( tr("Retrieving affected records" ) );
    while (oSelect.next())
    {
        cardIds.append(oSelect.value(0).toUInt());
    }

    ui->listProgress->addItem( tr("Number of affected records: " ) + QString::number( cardIds.size() ) );

    int stepsPerCard = 5;
    m_dlgProgress->setMaxValue( cardIds.size() * stepsPerCard );

    ui->listProgress->addItem( tr("Delete records ..." ) );
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
    ui->listProgress->addItem(tr("Deletion process finished."));
    ui->listProgress->addItem(tr("Skipped: ") + QString::number(nSkipped));
    ui->listProgress->addItem(tr("Deleted: ") + QString::number(nDeleted));
    ui->listProgress->addItem( "" );
}


