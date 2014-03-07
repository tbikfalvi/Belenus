
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

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_nCurrentPage              = CONST_PAGE_START;
    m_qdExpCurrentDir           = "C:/Program Files/Solarium/";
    m_nProgramType              = DBTool::KiwiSun;
    m_poDB                      = NULL;
    m_qsLogFileName             = "";
    m_dlgProgress               = new cDlgProgress( this );
    m_qsLanguage                = "hu";
    m_bIsPatientCardTypesLoaded = false;
    m_bIsPatientCardsLoaded     = false;
    m_bIsProductTypesLoaded     = false;
    m_bIsProductsLoaded         = false;
    m_bIsUsersLoaded            = false;

    ui->ledPathDB->setText( m_qdExpCurrentDir.path() );
    ui->pageController->setCurrentIndex( m_nCurrentPage );

    // Initialize GUI components
    ui->cmbLanguage->addItem( "Magyar (hu)" );
    ui->cmbLanguage->addItem( "English (en)" );

    ui->imgLogo->setPixmap( QPixmap(":/imgLogo/KiwiSun.png") );

    connect( ui->rbTypeSolarium, SIGNAL(clicked()), this, SLOT(slotProgramTypeClicked()) );
    connect( ui->rbTypeSensolite, SIGNAL(clicked()), this, SLOT(slotProgramTypeClicked()) );

    _initializePage();
}

MainWindow::~MainWindow()
{
    delete m_dlgProgress;
    delete ui;
}

void MainWindow::closeEvent( QCloseEvent *p_poEvent )
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


void MainWindow::on_pbNext_clicked()
{
    if( m_nCurrentPage < CONST_PAGE_EXPORT )
        m_nCurrentPage++;

    ui->pbPrev->setEnabled( m_nCurrentPage > CONST_PAGE_START );
    ui->pbNext->setEnabled( m_nCurrentPage < CONST_PAGE_EXPORT );

    ui->pageController->setCurrentIndex( m_nCurrentPage );

    _initializePage();
}

void MainWindow::on_pbPrev_clicked()
{
    if( m_nCurrentPage > CONST_PAGE_START )
        m_nCurrentPage--;

    ui->pbPrev->setEnabled( m_nCurrentPage > CONST_PAGE_START );
    ui->pbNext->setEnabled( m_nCurrentPage < CONST_PAGE_EXPORT );

    ui->pageController->setCurrentIndex( m_nCurrentPage );

    // Enable Cancel button
    ui->pbCancel->setEnabled( true );
    // Disable Start/Exit button
    ui->pbStartExit->setEnabled( false );
}

void MainWindow::_initializePage()
{
    switch( m_nCurrentPage )
    {
        case CONST_PAGE_START:
        {
            break;
        }
        case CONST_PAGE_VERIFICATION:
        {
            ui->ledSystemPassword->setText( "" );
            ui->ledPathDB->setText( m_qdExpCurrentDir.absolutePath() );
            ui->pbExpSelectDir->setEnabled( false );
            ui->pbCheckSVDatFiles->setEnabled( false );
            ui->pbPExportConnect->setEnabled( false );
            ui->ledSystemPassword->setFocus();
            ui->imgCheckPswFail->setVisible( false );
            ui->imgCheckPswOk->setVisible( false );
            ui->lblCheckPswInfo->setVisible( false );
            ui->imgCheckDbFail->setVisible( false );
            ui->imgCheckDbOk->setVisible( false );
            ui->lblCheckDbInfo->setVisible( false );
            ui->imgConnectDbFail->setVisible( false );
            ui->imgConnectDbOk->setVisible( false );
            ui->lblConnectDbInfo->setVisible( false );
            ui->pbNext->setEnabled( false );
            break;
        }
        case CONST_PAGE_IMPORT:
        {
            ui->listResultImport->clear();
            ui->pbNext->setEnabled( false );
            break;
        }
        case CONST_PAGE_PREPROC_PCT:
        {
            ui->ledPCTUnitTimeGeneral->setText( "" );
            ui->listPatientCardTypes->clear();
            _fillListPatientCardTypes();
            ui->ledPCTName->setText( "" );
            ui->ledPCTUnitTime->setText( "" );
            ui->pbNext->setEnabled( false );
            break;
        }
        case CONST_PAGE_PREPROC_PC:
        {
            ui->ledBarcodeLength->setFocus();
            ui->ledBarcodeLength->selectAll();
            ui->pbNext->setEnabled( false );
            break;
        }
        case CONST_PAGE_EXPORT:
        {
            _checkExportSelection();
            ui->pbNext->setEnabled( false );
            break;
        }
    }
}

void MainWindow::on_pbExpSelectDir_clicked()
{
    QString qsDirectory = QFileDialog::getExistingDirectory(this, tr("Select Directory"), m_qdExpCurrentDir.absolutePath() );

    if (!qsDirectory.isEmpty())
    {
        m_qdExpCurrentDir = QDir( qsDirectory );
        ui->ledPathDB->setText( qsDirectory );
    }
}



int MainWindow::_getPatientCardTypeId()
{
    int nRet = 0;

    for( int i=0; i<m_qvPatientCardTypes.size(); i++ )
    {
        if( m_qvPatientCardTypes.at(i).nID > nRet )
            nRet = m_qvPatientCardTypes.at(i).nID;
    }

    return ++nRet;
}




int MainWindow::_getTimeLeft(int p_nBerletTipus, int p_nEgyseg )
{
    int nRet = 0;

    int nBerletTypeCount = m_qvPatientCardTypes.size();

    for( int i=0; i<nBerletTypeCount; i++ )
    {
        if( m_qvPatientCardTypes.at(i).nID == p_nBerletTipus )
        {
            if( m_qvPatientCardTypes.at(i).nEgysegIdo > 0 )
            {
                nRet = m_qvPatientCardTypes.at(i).nEgysegIdo * p_nEgyseg * 60;
            }
            else
            {
                nRet = ui->ledPCTUnitTimeGeneral->text().toInt() * p_nEgyseg * 60;
            }
            break;
        }
    }

    return nRet;
}

void MainWindow::_exportToBelenusPatientCardTypes()
{
    int nBerletTypeCount = m_qvPatientCardTypes.size();

    _logAction( "Export to database Patientcard types:" );
    ui->listExport->addItem( tr("Exporting Patientcard types") );
    update();

    for( int i=0; i<nBerletTypeCount; i++ )
    {
        if( m_qvPatientCardTypes.at(i).nEgysegIdo < 1 )
        {
            m_qvPatientCardTypes[i].nEgysegIdo = ui->ledPCTUnitTimeGeneral->text().toInt();
        }

        QString qsSQLCommand = QString( "INSERT INTO `patientCardTypes` (`patientCardTypeId`, `licenceId`, `name`, `price`, `vatpercent`, `units`, `validDateFrom`, `validDateTo`, `validDays`, `unitTime`, `active`, `archive`) VALUES ( " );

        qsSQLCommand += QString( "%1, " ).arg( m_qvPatientCardTypes.at(i).nID+1 );
        qsSQLCommand += QString( "%1, " ).arg( m_nLicenceId );
        qsSQLCommand += QString( "'%1', " ).arg( m_qvPatientCardTypes.at(i).strNev );
        qsSQLCommand += QString( "%1, " ).arg( m_qvPatientCardTypes.at(i).nAr * 100 );
        qsSQLCommand += QString( "0, " );
        qsSQLCommand += QString( "%1, " ).arg( m_qvPatientCardTypes.at(i).nEgyseg );
        qsSQLCommand += QString( "'%1-%2-%3', " ).arg( m_qvPatientCardTypes.at(i).nErvTolEv ).arg( m_qvPatientCardTypes.at(i).nErvTolHo ).arg( m_qvPatientCardTypes.at(i).nErvTolNap );
        qsSQLCommand += QString( "'%1-%2-%3', " ).arg( m_qvPatientCardTypes.at(i).nErvIgEv ).arg( m_qvPatientCardTypes.at(i).nErvIgHo ).arg( m_qvPatientCardTypes.at(i).nErvIgNap );
        qsSQLCommand += QString( "%1, " ).arg( m_qvPatientCardTypes.at(i).nErvNapok );
        qsSQLCommand += QString( "%1, " ).arg( m_qvPatientCardTypes.at(i).nEgysegIdo );
        qsSQLCommand += QString( "1, 'ARC');" );

        _logAction( qsSQLCommand );

        QSqlQuery query = m_poDB->exec( qsSQLCommand );
        m_qvPatientCardTypes[i].nNewID = query.lastInsertId().toInt();

        qsSQLCommand = QString( "INSERT INTO `patientcardtypeenabled` (`licenceId`, `patientCardTypeId`, `validWeekDays`, `start`, `stop`, `modified`, `archive`) VALUES ( " );
        qsSQLCommand += QString( "%1, " ).arg( m_nLicenceId );
        qsSQLCommand += QString( "%1, " ).arg( m_qvPatientCardTypes.at(i).nID+1 );
        qsSQLCommand += QString( "'127', " );
        qsSQLCommand += QString( "'00:00:00', " );
        qsSQLCommand += QString( "'23:59:00', " );
        qsSQLCommand += QString( "0, 'ARC');" );

        m_poDB->exec( qsSQLCommand );
        m_dlgProgress->stepValue();
    }

    ui->listExport->addItem( tr("Exporting Patientcard types FINISHED") );
    update();

    _logAction( "Export finished\n" );
}

void MainWindow::_exportToBelenusPatientCards()
{
    _logAction( "Export to database Patientcards:" );

    ui->listExport->addItem( tr("Exporting Patientcards") );
    update();

    int nBerletCount = m_qvPatientCards.size();

    for( int i=0; i<nBerletCount; i++ )
    {
        QDate obDateValid( m_qvPatientCards.at(i).nErvEv, m_qvPatientCards.at(i).nErvHo, m_qvPatientCards.at(i).nErvNap );

        if( m_qvPatientCards.at(i).nEgyseg < 1 || obDateValid <= QDate::currentDate() )
        {
            m_dlgProgress->stepValue();

            for( int j=0; j<m_qvPatientCards.at(i).nEgyseg; j++ )
                m_dlgProgress->stepValue();

            continue;
        }

        QString qsBarcode = m_qvPatientCards.at(i).strVonalkod;

        while ( qsBarcode.length() < ui->ledBarcodeLength->text().toInt() )
        {
            qsBarcode = QString( "0%1" ).arg(qsBarcode);
        }

        QString qsSQLCommand = QString( "INSERT INTO `patientCards` ( `licenceId`, `patientCardTypeId`, `patientId`, `barcode`, `comment`, `units`, `timeLeft`, `validDateFrom`, `validDateTo`, `pincode`, `active`, `archive`) VALUES ( " );

        qsSQLCommand += QString( "%1, " ).arg( m_nLicenceId );
        qsSQLCommand += QString( "%1, " ).arg( _getPatientCardTypeNewId( m_qvPatientCards.at(i).nBerletTipus ) );
        qsSQLCommand += QString( "0, " );
        qsSQLCommand += QString( "'%1', " ).arg( qsBarcode );
        qsSQLCommand += QString( "'%1', " ).arg( m_qvPatientCards.at(i).strMegjegyzes );
        qsSQLCommand += QString( "%1, " ).arg( m_qvPatientCards.at(i).nEgyseg );
        qsSQLCommand += QString( "%1, " ).arg( _getTimeLeft(m_qvPatientCards.at(i).nBerletTipus, m_qvPatientCards.at(i).nEgyseg) );
        qsSQLCommand += QString( "'%1-%2-%3', " ).arg( m_qvPatientCards.at(i).nErvEv-1 ).arg( m_qvPatientCards.at(i).nErvHo ).arg( m_qvPatientCards.at(i).nErvNap );
        qsSQLCommand += QString( "'%1-%2-%3', " ).arg( m_qvPatientCards.at(i).nErvEv ).arg( m_qvPatientCards.at(i).nErvHo ).arg( m_qvPatientCards.at(i).nErvNap );
        qsSQLCommand += QString( "NULL, 1, 'ARC' );" );

        _logAction( qsSQLCommand );

        QSqlQuery query = m_poDB->exec( qsSQLCommand );

        unsigned int uiPatientcardId = query.lastInsertId().toInt();
        unsigned int uiPatientCardUnitPrice = _getPatientCardTypeUnitPrice( m_qvPatientCards.at(i).nBerletTipus );

        for( int j=0; j<m_qvPatientCards.at(i).nEgyseg; j++ )
        {
            qsSQLCommand = QString( "INSERT INTO `patientcardunits` (`licenceId` ,`patientCardId` ,`ledgerId` ,`panelId` ,`unitTime` ,`unitPrice` ,`validDateFrom` ,`validDateTo` ,`dateTimeUsed` ,`active` ,`archive` ) VALUES ( " );

            qsSQLCommand += QString( "%1, " ).arg( m_nLicenceId );
            qsSQLCommand += QString( "%1, " ).arg( uiPatientcardId );
            qsSQLCommand += QString( "0, " );
            qsSQLCommand += QString( "0, " );
            qsSQLCommand += QString( "%1, " ).arg( _getPatientCardTypeUnitTime( m_qvPatientCards.at(i).nBerletTipus ) );
            qsSQLCommand += QString( "%1, " ).arg( uiPatientCardUnitPrice );
            qsSQLCommand += QString( "'2013-01-01', " );
            qsSQLCommand += QString( "'%1-%2-%3', " ).arg( m_qvPatientCards.at(i).nErvEv ).arg( m_qvPatientCards.at(i).nErvHo ).arg( m_qvPatientCards.at(i).nErvNap );
            qsSQLCommand += QString( "CURRENT_TIMESTAMP, 1, 'ARC' );" );

            _logAction( qsSQLCommand );

            query = m_poDB->exec( qsSQLCommand );

            m_dlgProgress->stepValue();
        }
/*
        qsSQLCommand = QString( "INSERT INTO `ledger` (`licenceId`, `parentId` , `ledgerTypeId` , `ledgerDeviceId` , `paymentMethodId` , `userId` , `productId` , `patientCardTypeId` , `patientCardId` , `panelId` , `patientId` , `name` , `netPrice` , `discount` , `vatpercent` , `totalPrice` , `ledgerTime` , `comment` , `modified` , `active` , `archive` ) VALUES ( " );

        qsSQLCommand += QString( "%1, " ).arg( m_nLicenceId );
        qsSQLCommand += QString( "0, " );
        qsSQLCommand += QString( "2, " );
        qsSQLCommand += QString( "0, " );
        qsSQLCommand += QString( "1, " );
        qsSQLCommand += QString( "2, " );
        qsSQLCommand += QString( "0, " );
        qsSQLCommand += QString( "0, " );
        qsSQLCommand += QString( "%1, " ).arg( uiPatientcardId );
        qsSQLCommand += QString( "0, " );
        qsSQLCommand += QString( "0, " );
        qsSQLCommand += QString( "'%1', " ).arg( qsBarcode );
        qsSQLCommand += QString( "'%1', " ).arg( _getPatientCardTypeUnitPrice( m_qvPatientCards.at(i).nBerletTipus ) );
        qsSQLCommand += QString( "0, " );
        qsSQLCommand += QString( "0, " );
        qsSQLCommand += QString( "'%1', " ).arg( _getPatientCardTypeUnitPrice( m_qvPatientCards.at(i).nBerletTipus ) );
        qsSQLCommand += QString( "'%1-%2-%3', " ).arg( m_qvPatientCards.at(i).nErvEv-1 ).arg( m_qvPatientCards.at(i).nErvHo ).arg( m_qvPatientCards.at(i).nErvNap );
        qsSQLCommand += QString( "NULL ,  '',  '1',  'ARC' );" );

        _logAction( qsSQLCommand );

        query = m_poDB->exec( qsSQLCommand );
*/
        m_dlgProgress->stepValue();
    }

    ui->listExport->addItem( tr("Exporting Patientcards FINISHED") );
    _logAction( "Export finished\n" );
    update();
}

void MainWindow::_exportToBelenusProductTypes()
{
    int nProductTypeCount = m_qvProductTypes.size();

    ui->listExport->addItem( tr("Exporting Product types") );
    _logAction( "Export to database Product types:" );
    update();

    for( int i=0; i<nProductTypeCount; i++ )
    {
        QString qsSQLCommand = QString( "INSERT INTO `productTypes` (`licenceId`, `name`, `active`, `archive`) VALUES ( " );

        qsSQLCommand += QString( "%1, " ).arg( m_nLicenceId );
        qsSQLCommand += QString( "'%1', " ).arg( m_qvProductTypes.at(i).strNev );
        qsSQLCommand += QString( "1, 'ARC' );" );

        _logAction( qsSQLCommand );

        QSqlQuery query = m_poDB->exec( qsSQLCommand );
        m_qvProductTypes[i].nNewID = query.lastInsertId().toInt();
        m_dlgProgress->stepValue();
    }

    ui->listExport->addItem( tr("Exporting Product types FINISHED") );
    _logAction( "Export finished\n" );
    update();
}

void MainWindow::_exportToBelenusProducts()
{
    int nProductCount = m_qvProducts.size();

    ui->listExport->addItem( tr("Exporting Products") );
    _logAction( "Export to database Products:" );
    update();

    for( int i=0; i<nProductCount; i++ )
    {
        QString qsSQLCommand;
        QSqlQuery query;

        qsSQLCommand = QString( "INSERT INTO `producthistory` ( `productHistoryId`, `licenceId`, `productId`, `ledgerId`, `productActionTypeId`, `userId`, `productItemCount`, `netPrice`, `vatpercent`, `actionDateTime`, `modified`, `active`, `archive` ) VALUES ( "
                                "NULL , '0', '1', '0', '1', '1', '%1', '0', '0', CURRENT_TIMESTAMP, CURRENT_TIMESTAMP, '1', 'ARC' );" ).arg( m_qvProducts.at(i).nDarab );
        _logAction( qsSQLCommand );
        query = m_poDB->exec( qsSQLCommand );

        qsSQLCommand = QString( "INSERT INTO `products` (`licenceId`, `name`, `barcode`, `netPriceBuy`, `vatpercentBuy`, `netPriceSell`, `vatpercentSell`, `productCount`, `modified`, `active`, `archive`) VALUES ( " );

        qsSQLCommand += QString( "%1, " ).arg( m_nLicenceId );
        qsSQLCommand += QString( "'%1', " ).arg( m_qvProducts.at(i).strNev );
        qsSQLCommand += QString( "'%1', " ).arg( m_qvProducts.at(i).strVonalkod );
        qsSQLCommand += QString( "%1, " ).arg( m_qvProducts.at(i).nArBeszerzes * 100 );
        qsSQLCommand += QString( "0, " );
        qsSQLCommand += QString( "%1, " ).arg( m_qvProducts.at(i).nAr * 100 );
        qsSQLCommand += QString( "0, " );
        qsSQLCommand += QString( "%1, " ).arg( m_qvProducts.at(i).nDarab );
        qsSQLCommand += QString( "0, 1, 'ARC');" );

        _logAction( qsSQLCommand );

        query = m_poDB->exec( qsSQLCommand );
        m_qvProducts[i].nNewID = query.lastInsertId().toInt();
        m_dlgProgress->stepValue();
    }

    int nProductAssign = m_qvProductAssigns.size();

    ui->listExport->addItem( tr("Exporting Products FINISHED") );
    ui->listExport->addItem( tr("Exporting Product types / Product connections") );
    update();

    _logAction( "Export to database Product assigns:" );

    for( int i=0; i<nProductAssign; i++ )
    {
        QString qsSQLCommand = QString( "INSERT INTO  `connectproductwithtype` (`productTypeId` ,`productId` ,`licenceId`) VALUES ( " );

        qsSQLCommand += QString( "%1, " ).arg( _getProductTypeNewId(m_qvProductAssigns.at(i).nTTipusID) );
        qsSQLCommand += QString( "%1, " ).arg( _getProductNewId(m_qvProductAssigns.at(i).nTermekID) );
        qsSQLCommand += QString( "%1 ); " ).arg( m_nLicenceId );

        ////ui->listLog->addItem( qsSQLCommand );
        _logAction( qsSQLCommand );

        m_poDB->exec( qsSQLCommand );
        m_dlgProgress->stepValue();
    }

    ui->listExport->addItem( tr("Exporting Product types / Product connections FINISHED") );
    _logAction( "Export finished\n" );
    update();
}

void MainWindow::_exportToBelenusUsers()
{
    int nUSerCount = m_qvUsers.size();

    ui->listExport->addItem( tr("Exporting Application users") );
    _logAction( "Export to database Users:" );
    update();

    for( int i=0; i<nUSerCount; i++ )
    {
        QString qsPassword;

        if( QString(m_qvUsers.at(i).strJelszo).length() == 0 )
        {
            qsPassword = "da39a3ee5e6b4b0d3255bfef95601890afd80709";
        }
        else
        {
            QByteArray  obPwdHash = QCryptographicHash::hash( QString( m_qvUsers.at(i).strJelszo ).toAscii(), QCryptographicHash::Sha1 );
            qsPassword = QString( obPwdHash.toHex() );
        }
        QString qsSQLCommand = QString( "INSERT INTO `users` (`licenceId`, `name`, `realName`, `password`, `accgroup`, `active`, `comment`, `archive`) VALUES ( " );

        qsSQLCommand += QString( "%1, " ).arg( m_nLicenceId );
        qsSQLCommand += QString( "'%1', " ).arg( m_qvUsers.at(i).strLoginNev );
        qsSQLCommand += QString( "'%1', " ).arg( m_qvUsers.at(i).strNevCsalad );
        qsSQLCommand += QString( "'%1', 1, 1, " ).arg( qsPassword );
        qsSQLCommand += QString( "'%1', " ).arg( m_qvUsers.at(i).strMegjegyzes );
        qsSQLCommand += QString( "'ARC');" );

        ////ui->listLog->addItem( qsSQLCommand );
        _logAction( qsSQLCommand );

        m_poDB->exec( qsSQLCommand );
        m_dlgProgress->stepValue();
    }

    ui->listExport->addItem( tr("Exporting Application users FINISHED") );
    _logAction( "Export finished\n" );
    update();
}

int MainWindow::_getPatientCardTypeNewId( int p_nID )
{
    int nRet = 0;
    int nCount = m_qvPatientCardTypes.size();

    for( int i=0; i<nCount; i++ )
    {
        if( m_qvPatientCardTypes.at(i).nID == p_nID )
        {
            nRet = m_qvPatientCardTypes.at(i).nNewID;
            break;
        }
    }
    return nRet;
}

int MainWindow::_getPatientCardTypeUnitPrice( int p_nID )
{
    int nRet = 0;
    int nCount = m_qvPatientCardTypes.size();

    for( int i=0; i<nCount; i++ )
    {
        if( m_qvPatientCardTypes.at(i).nID == p_nID )
        {
            nRet = ( m_qvPatientCardTypes.at(i).nAr * 100 ) / m_qvPatientCardTypes.at(i).nEgyseg;
            break;
        }
    }
    return nRet;
}

int MainWindow::_getPatientCardTypeUnitTime( int p_nID )
{
    int nRet = 0;
    int nCount = m_qvPatientCardTypes.size();

    for( int i=0; i<nCount; i++ )
    {
        if( m_qvPatientCardTypes.at(i).nID == p_nID )
        {
            nRet = m_qvPatientCardTypes.at(i).nEgysegIdo;
            break;
        }
    }
    return nRet;
}

int MainWindow::_getProductTypeNewId( int p_nID )
{
    int nRet = 0;
    int nCount = m_qvProductTypes.size();

    for( int i=0; i<nCount; i++ )
    {
        if( m_qvProductTypes.at(i).nID == p_nID )
        {
            nRet = m_qvProductTypes.at(i).nNewID;
            break;
        }
    }
    return nRet;
}

int MainWindow::_getProductNewId( int p_nID )
{
    int nRet = 0;
    int nCount = m_qvProducts.size();

    for( int i=0; i<nCount; i++ )
    {
        if( m_qvProducts.at(i).nID == p_nID )
        {
            nRet = m_qvProducts.at(i).nNewID;
            break;
        }
    }
    return nRet;
}

void MainWindow::_logAction(QString p_qsLogMessage)
{
    QFile   qfFileLog( m_qsLogFileName );

    qfFileLog.open( QIODevice::Append | QIODevice::Text );
    qfFileLog.write( QString("%1\n").arg(p_qsLogMessage).toStdString().c_str() );
    qfFileLog.close();
}

void MainWindow::on_pbCancel_clicked()
{
    close();
}

void MainWindow::on_pbStartExit_clicked()
{
    close();
}

//====================================================================================
// START
//====================================================================================
void MainWindow::on_cmbLanguage_currentIndexChanged(int index)
//====================================================================================
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
void MainWindow::slotProgramTypeClicked()
{
    if( ui->rbTypeSolarium->isChecked() )
    {
        m_nProgramType = DBTool::KiwiSun;
    }
    else if( ui->rbTypeSensolite->isChecked() )
    {
        m_nProgramType = DBTool::Sensolite;
    }
}
//====================================================================================
// SYSTEM VERIFICATION
//====================================================================================
void MainWindow::on_pbLogin_clicked()
//====================================================================================
{
    QByteArray  obPwdHash = QCryptographicHash::hash( ui->ledSystemPassword->text().toAscii(), QCryptographicHash::Sha1 );

    if( QString( obPwdHash.toHex() ).compare( "a382329cfe97ae74677649d1f7fc03986b27cf3f" ) == 0 )
    {
        ui->pbExpSelectDir->setEnabled( true );
        ui->pbCheckSVDatFiles->setEnabled( true );
        ui->pbPExportConnect->setEnabled( true );
        ui->lblCheckPswInfo->setText( tr("Password verification succeeded") );
        ui->imgCheckPswFail->setVisible( false );
        ui->imgCheckPswOk->setVisible( true );
        ui->lblCheckPswInfo->setVisible( true );
    }
    else
    {
        ui->ledSystemPassword->setText( "" );
        ui->pbExpSelectDir->setEnabled( false );
        ui->pbCheckSVDatFiles->setEnabled( false );
        ui->pbPExportConnect->setEnabled( false );
        ui->lblCheckPswInfo->setText( tr("Password verification failed") );
        ui->imgCheckPswFail->setVisible( true );
        ui->imgCheckPswOk->setVisible( false );
        ui->lblCheckPswInfo->setVisible( true );

        QMessageBox::warning( this, tr("Warning"),
                              tr("The password you entered is not match with the\n"
                                 "requested System Administrator password.\n"
                                 "Please contact your System Administrator.") );
    }
}
//====================================================================================
void MainWindow::on_pbCheckSVDatFiles_clicked()
//====================================================================================
{
    QString qsToolTip   = "";
    bool    bIsOk       = true;

    m_qsPCTFileName = QString( "%1/brlttpsfsv.dat" ).arg( ui->ledPathDB->text() ).replace( "/", "\\" );
    m_qsPCFileName  = QString( "%1/brltfsv.dat" ).arg( ui->ledPathDB->text() ).replace( "/", "\\" );
    m_qsPTFileName  = QString( "%1/trmktpsfsv.dat" ).arg( ui->ledPathDB->text() ).replace( "/", "\\" );
    m_qsPFileName   = QString( "%1/trmkfsv.dat" ).arg( ui->ledPathDB->text() ).replace( "/", "\\" );
    m_qsPAFileName  = QString( "%1/trmktpssgfsv.dat" ).arg( ui->ledPathDB->text() ).replace( "/", "\\" );
    m_qsUFileName   = QString( "%1/srfsv.dat" ).arg( ui->ledPathDB->text() ).replace( "/", "\\" );

    if( !QFile::exists( m_qsPCTFileName ) )
    {
        bIsOk = false;
        qsToolTip.append( tr("%1 file does not exists\n").arg( m_qsPCTFileName ) );
    }
    if( !QFile::exists( m_qsPCFileName ) )
    {
        bIsOk = false;
        qsToolTip.append( tr("%1 file does not exists\n").arg( m_qsPCFileName ) );
    }
    if( !QFile::exists( m_qsPTFileName ) )
    {
        bIsOk = false;
        qsToolTip.append( tr("%1 file does not exists\n").arg( m_qsPTFileName ) );
    }
    if( !QFile::exists( m_qsPFileName ) )
    {
        bIsOk = false;
        qsToolTip.append( tr("%1 file does not exists\n").arg( m_qsPFileName ) );
    }
    if( !QFile::exists( m_qsPAFileName ) )
    {
        bIsOk = false;
        qsToolTip.append( tr("%1 file does not exists\n").arg( m_qsPAFileName ) );
    }
    if( !QFile::exists( m_qsUFileName ) )
    {
        bIsOk = false;
        qsToolTip.append( tr("%1 file does not exists\n").arg( m_qsUFileName ) );
    }

    if( bIsOk )
    {
        ui->lblCheckDbInfo->setText( tr("Database files are exists and checked successfully.") );
        ui->lblCheckDbInfo->setToolTip( "" );
        ui->imgCheckDbFail->setVisible( false );
        ui->imgCheckDbOk->setVisible( true );
        ui->lblCheckDbInfo->setVisible( true );
    }
    else
    {
        ui->lblCheckDbInfo->setText( tr("Database files are missing or corrupt.") );
        ui->lblCheckDbInfo->setToolTip( qsToolTip );
        ui->imgCheckDbFail->setVisible( true );
        ui->imgCheckDbOk->setVisible( false );
        ui->lblCheckDbInfo->setVisible( true );
    }

    ui->pbNext->setEnabled( _isSystemVerificationOk() );
}
//====================================================================================
bool MainWindow::_isSystemVerificationOk()
//====================================================================================
{
    return ( ui->imgCheckPswOk->isVisible() && ui->imgCheckDbOk->isVisible() && ui->imgConnectDbOk->isVisible() );
}
//====================================================================================
void MainWindow::on_pbPExportConnect_clicked()
//====================================================================================
{
    m_poDB = new QSqlDatabase( QSqlDatabase::addDatabase( "QMYSQL" ) );

    m_poDB->setHostName( "localhost" );
    m_poDB->setDatabaseName( "belenus" );
    m_poDB->setUserName( "belenus" );
    m_poDB->setPassword( "belenus" );

    if( m_poDB->open() )
    {
        ui->lblConnectDbInfo->setText( tr("Connection with Belenus database established") );
        ui->imgConnectDbFail->setVisible( false );
        ui->imgConnectDbOk->setVisible( true );
        ui->lblConnectDbInfo->setVisible( true );

        QSqlQuery qsQuery = m_poDB->exec( "SELECT * FROM licences" );

        qsQuery.last();
        m_nLicenceId = qsQuery.value(0).toInt();

        ui->lblConnectDbInfo->setToolTip( tr("License code: %1 [%2]").arg(qsQuery.value(1).toString()).arg(m_nLicenceId) );
    }
    else
    {
        ui->lblConnectDbInfo->setText( tr("Unable to connect to Belenus database") );
        ui->imgConnectDbFail->setVisible( true );
        ui->imgConnectDbOk->setVisible( false );
        ui->lblConnectDbInfo->setVisible( true );
        ui->lblConnectDbInfo->setToolTip( "" );
    }

    ui->pbNext->setEnabled( _isSystemVerificationOk() );
}
//====================================================================================
// DATABASE IMPORT
//====================================================================================
void MainWindow::on_pbImportDB_clicked()
//====================================================================================
{
    ui->listResultImport->clear();

    m_qsLogFileName = QString("c:/belenus_dbtool_%1.log").arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmm"));

    _logAction( tr("Filenames of patientcard and patientcard types data:") );
    _logAction( m_qsPCTFileName );
    _logAction( m_qsPCFileName );
    _logAction( m_qsPTFileName );
    _logAction( m_qsPFileName );
    _logAction( m_qsUFileName );

    m_bIsPatientCardTypesLoaded = false;
    m_bIsPatientCardsLoaded     = false;
    m_bIsProductTypesLoaded     = false;
    m_bIsProductsLoaded         = false;
    m_bIsUsersLoaded            = false;

    _loadPatientCardTypes();
    _loadPatientCards();
    _loadProductTypes();
    _loadProducts();
    _loadProductAssign();
    _loadUsers();
    _loadDevices();

    if( m_bIsPatientCardTypesLoaded &&
        m_bIsPatientCardsLoaded &&
        m_bIsProductTypesLoaded &&
        m_bIsProductsLoaded &&
        m_bIsUsersLoaded )
    {
        ui->pbNext->setEnabled( true );
    }
}
//====================================================================================
void MainWindow::_loadDevices()
//====================================================================================
{
    FILE           *file = NULL;
    QString         qsSolInfo = "";

    setCursor( Qt::WaitCursor);

    for( int i=0; i<20; i++ )
    {
        QString         qsFileName = "";
        typ_szoliadat   stTemp;

        qsFileName = QString( "%1/sdfsv%2.dat" ).arg( ui->ledPathDB->text() ).arg(i).replace( "/", "\\" );

        _logAction( tr("Load device file: %1").arg(qsFileName) );

        file = fopen( qsFileName.toStdString().c_str(), "rb" );
        if( file != NULL )
        {
            _logAction( tr("Read device data") );
            memset( m_strPatiencardTypeVersion, 0, 10 );
            fread( m_strPatiencardTypeVersion, 10, 1, file );
            fread( stTemp.strNev, 50, 1, file );
            fread( &stTemp.bInfraSzolarium, 1, 1, file );
            fread( &stTemp.nIdoVetkozes, 4, 1, file );
            fread( &stTemp.nIdoUtohutes, 4, 1, file );
            fread( &stTemp.nIdoSzauna, 4, 1, file );
            fread( &stTemp.nKedvezmenyIdoStart, 4, 1, file );
            fread( &stTemp.nKedvezmenyIdoStop, 4, 1, file );
            fread( &stTemp.nCsoUzemora, 4, 1, file );
            fclose( file );

            _DeCode( stTemp.strNev, 50 );

            QString qsUseTime = "";

            qsUseTime.append( QString::number(stTemp.nCsoUzemora/3600) );
            qsUseTime.append( ":" );
            qsUseTime.append( QString::number((stTemp.nCsoUzemora%3600)/60) );
            qsUseTime.append( ":" );
            qsUseTime.append( QString::number((stTemp.nCsoUzemora%3600)%60) );

            qsSolInfo.append( QString( "%1\t%2\n" ).arg( qsUseTime ).arg( stTemp.strNev ) );
        }
        else
        {
            _logAction( tr("File not found, exiting ...") );
            break;
        }
    }
    QMessageBox::information( this, tr("Information"), qsSolInfo );

    setCursor( Qt::ArrowCursor);
}
//====================================================================================
void MainWindow::_loadPatientCardTypes()
//====================================================================================
{
    FILE           *file = NULL;
    unsigned int   nCount = 0;

    setCursor( Qt::WaitCursor);

    m_qvPatientCardTypes.clear();
    m_nCountPCT = 0;

    file = fopen( m_qsPCTFileName.toStdString().c_str(), "rb" );
    if( file != NULL )
    {
        memset( m_strPatiencardTypeVersion, 0, 10 );
        fread( m_strPatiencardTypeVersion, 10, 1, file );
        nCount = 0;
        fread( &nCount, 4, 1, file );
        ui->listResultImport->addItem( tr("Count of patientcard types to be imported: %1").arg(nCount) );
        _logAction( tr("Count of patientcard types to be imported: %1").arg(nCount) );
        m_nCountPCT += nCount;
        if( nCount > 0 )
        {
            typ_berlettipus stTemp;
            for( unsigned int i=0; i<nCount; i++ )
            {
                fread( &stTemp.nID, 4, 1, file );
                stTemp.nNewID = 0;
                fread( &stTemp.nAr, 4, 1, file );
                fread( &stTemp.nEgyseg, 4, 1, file );
                fread( stTemp.strNev, 50, 1, file );
                fread( &stTemp.nErvTolEv, 4, 1, file );
                fread( &stTemp.nErvTolHo, 4, 1, file );
                fread( &stTemp.nErvTolNap, 4, 1, file );
                fread( &stTemp.nErvIgEv, 4, 1, file );
                fread( &stTemp.nErvIgHo, 4, 1, file );
                fread( &stTemp.nErvIgNap, 4, 1, file );
                fread( &stTemp.nErvNapok, 4, 1, file );
                fread( &stTemp.bSzolariumHaszn, 1, 1, file );
                if( m_nProgramType == DBTool::Sensolite )
                {
                    fread( &stTemp.nEgysegIdo, 4, 1, file );
                }
                else
                {
                    stTemp.nEgysegIdo = 0;
                }

                _DeCode( stTemp.strNev, 50 );

                m_qvPatientCardTypes.append( stTemp );
            }
        }
        fclose( file );
        ui->listResultImport->addItem( tr("Importing %1 patientcard types finished.").arg(m_qvPatientCardTypes.size()) );
        _logAction( tr("Importing %1 patientcard types finished.").arg(m_qvPatientCardTypes.size()) );
        m_bIsPatientCardTypesLoaded = true;
    }
    else
    {
        ui->listResultImport->addItem( tr( "Error occured during opening brlttpsfsv.dat file." ) );
    }

    setCursor( Qt::ArrowCursor);
}

//====================================================================================
void MainWindow::_loadPatientCards()
//====================================================================================
{
    FILE           *file = NULL;
//FILE           *filemod = NULL;
FILE *filetxt = NULL;
    unsigned int    nCount = 0;
    char            m_strPatiencardVersion[10];

    setCursor( Qt::WaitCursor);

    m_qvPatientCards.clear();
    m_nCountPC = 0;

    file = fopen( m_qsPCFileName.toStdString().c_str(), "rb" );
//filemod = fopen( QString("c:\\mod_brltfsv.dat").toStdString().c_str(), "wb" );
filetxt = fopen( QString("c:\\brltfsv.txt").toStdString().c_str(), "wt" );
    if( file != NULL )
    {
        memset( m_strPatiencardVersion, 0, 10 );
        fread( m_strPatiencardVersion, 10, 1, file );
//fwrite( m_strPatiencardVersion, 10, 1, filemod );
        //ledVersionPCDAT->setText( QString::fromStdString(m_strPatiencardVersion) );

        nCount = 0;
        fread( &nCount, 4, 1, file );
//fwrite( &nCount, 4, 1, filemod );
        ui->listResultImport->addItem( tr("Count of patientcards to be imported: %1").arg(nCount) );
        _logAction( tr("Count of patientcards to be imported: %1").arg(nCount) );
        m_nCountPC += nCount;
        if( nCount > 0 )
        {
            typ_berlet   stTemp;
            for( unsigned int i=0; i<nCount; i++ )
            {
                fread( stTemp.strVonalkod, 20, 1, file );
                fread( stTemp.strMegjegyzes, 50, 1, file );
                fread( &stTemp.nBerletTipus, 4, 1, file );
                fread( &stTemp.nEgyseg, 4, 1, file );
                fread( &stTemp.nErvEv, 4, 1, file );
                fread( &stTemp.nErvHo, 4, 1, file );
                fread( &stTemp.nErvNap, 4, 1, file );
                fread( &stTemp.nPin, 4, 1, file );
/*
int nErvEv = 0;
if( stTemp.nErvEv < 2014 && stTemp.nEgyseg > 0 )
{
    nErvEv = stTemp.nErvEv + 1;
}
else
{
    nErvEv = stTemp.nErvEv;
}
fwrite( stTemp.strVonalkod, 20, 1, filemod );
fwrite( stTemp.strMegjegyzes, 50, 1, filemod );
fwrite( &stTemp.nBerletTipus, 4, 1, filemod );
fwrite( &stTemp.nEgyseg, 4, 1, filemod );
fwrite( &nErvEv, 4, 1, filemod );
fwrite( &stTemp.nErvHo, 4, 1, filemod );
fwrite( &stTemp.nErvNap, 4, 1, filemod );
fwrite( &stTemp.nPin, 4, 1, filemod );
*/
                _DeCode( stTemp.strVonalkod, 20 );
                _DeCode( stTemp.strMegjegyzes, 50 );
QString qsLine = "";
qsLine.append( QString("%1\t").arg( stTemp.strVonalkod ) );
qsLine.append( QString("%1\t").arg( stTemp.strMegjegyzes ) );
qsLine.append( QString("%1\t").arg( stTemp.nBerletTipus ) );
qsLine.append( QString("%1\t").arg( stTemp.nEgyseg ) );
qsLine.append( QString("%1\t").arg( stTemp.nErvEv ) );
qsLine.append( QString("%1\t").arg( stTemp.nErvHo ) );
qsLine.append( QString("%1\t").arg( stTemp.nErvNap ) );
fputs( qsLine.toStdString().c_str(), filetxt );
fputs( "\n", filetxt );
                m_qvPatientCards.append( stTemp );

                m_nCountPC += stTemp.nEgyseg;
            }
        }
        fclose( file );
//fclose( filemod );
fclose( filetxt );
        ui->listResultImport->addItem( tr("Importing %1 patientcards finished.").arg(m_qvPatientCards.size()) );
        _logAction( tr("Importing %1 patientcards finished.").arg(m_qvPatientCards.size()) );
        m_bIsPatientCardsLoaded = true;
    }
    else
    {
        ui->listResultImport->addItem( tr( "Error occured during opening brltfsv.dat file." ) );
    }

    setCursor( Qt::ArrowCursor);
}
//====================================================================================
void MainWindow::_loadProductTypes()
//====================================================================================
{
    FILE           *file = NULL;
    unsigned int    nCount = 0;
    char            m_strVersion[10];

    setCursor( Qt::WaitCursor);

    m_qvProductTypes.clear();
    m_nCountPT = 0;

    file = fopen( m_qsPTFileName.toStdString().c_str(), "rb" );
    if( file != NULL )
    {
        memset( m_strVersion, 0, 10 );
        fread( m_strVersion, 10, 1, file );

        nCount = 0;
        fread( &nCount, 4, 1, file );
        ui->listResultImport->addItem( tr("Count of product types to be imported: %1").arg(nCount) );
        _logAction( tr("Count of product types to be imported: %1").arg(nCount) );
        m_nCountPT += nCount;
        if( nCount > 0 )
        {
            typ_termektipus stTemp;
            for( unsigned int i=0; i<nCount; i++ )
            {
                fread( &stTemp.nID, 4, 1, file );
                stTemp.nNewID = 0;
                fread( stTemp.strNev, 100, 1, file );
                _DeCode( stTemp.strNev, 100 );

                ////ui->listLog->addItem( QString( "[%1] \'%2\'" ).arg(stTemp.nID).arg(stTemp.strNev) );

                m_qvProductTypes.append( stTemp );
            }
        }

        fclose( file );
        ui->listResultImport->addItem( tr("Importing %1 product types finished.").arg(m_qvProductTypes.size()) );
        _logAction( tr("Importing %1 product types finished.").arg(m_qvProductTypes.size()) );
        m_bIsProductTypesLoaded = true;
    }
    else
    {
        ui->listResultImport->addItem( tr( "Error occured during opening trmktpsfsv.dat file." ) );
    }

    setCursor( Qt::ArrowCursor);
}
//====================================================================================
void MainWindow::_loadProducts()
//====================================================================================
{
    FILE           *file = NULL;
    unsigned int    nCount = 0;
    char            m_strVersion[10];

    setCursor( Qt::WaitCursor);

    m_qvProducts.clear();
    m_nCountP = 0;

    file = fopen( m_qsPFileName.toStdString().c_str(), "rb" );
    if( file != NULL )
    {
        memset( m_strVersion, 0, 10 );
        fread( m_strVersion, 10, 1, file );

        nCount = 0;
        fread( &nCount, 4, 1, file );
        ui->listResultImport->addItem( tr("Count of products to be imported: %1").arg(nCount) );
        _logAction( tr("Count of products to be imported: %1").arg(nCount) );
        m_nCountP += nCount;
        if( nCount > 0 )
        {
            typ_termek stTemp;
            for( unsigned int i=0; i<nCount; i++ )
            {
                fread( &stTemp.nID, 4, 1, file );
                stTemp.nNewID = 0;
                fread( stTemp.strVonalkod, 20, 1, file );
                fread( stTemp.strNev, 100, 1, file );
                fread( &stTemp.nAr, 4, 1, file );
                fread( &stTemp.nDarab, 4, 1, file );
                fread( &stTemp.nArBeszerzes, 4, 1, file );

                _DeCode( stTemp.strVonalkod, 20 );
                _DeCode( stTemp.strNev, 100 );

                ////ui->listLog->addItem( QString( "[%1] \'%2\' \'%3\' [%4] [%5] [%6]" ).arg(stTemp.nID).arg(stTemp.strVonalkod).arg(stTemp.strNev).arg(stTemp.nAr).arg(stTemp.nDarab).arg(stTemp.nArBeszerzes) );

                m_qvProducts.append( stTemp );
            }
        }

        fclose( file );
        ui->listResultImport->addItem( tr("Importing %1 products finished.").arg(m_qvProducts.size()) );
        _logAction( tr("Importing %1 products finished.").arg(m_qvProducts.size()) );
        m_bIsProductsLoaded = true;
    }
    else
    {
        ui->listResultImport->addItem( tr( "Error occured during opening trmkfsv.dat file." ) );
    }

    setCursor( Qt::ArrowCursor);
}
//====================================================================================
void MainWindow::_loadProductAssign()
//====================================================================================
{
    FILE           *file = NULL;
    unsigned int    nCount = 0;
    char            m_strVersion[10];

    setCursor( Qt::WaitCursor);

    m_qvProductAssigns.clear();
    m_nCountPA = 0;

    file = fopen( m_qsPAFileName.toStdString().c_str(), "rb" );
    if( file != NULL )
    {
        memset( m_strVersion, 0, 10 );
        fread( m_strVersion, 10, 1, file );

        nCount = 0;
        fread( &nCount, 4, 1, file );
        ui->listResultImport->addItem( tr("Count of product assigns to be imported: %1").arg(nCount) );
        _logAction( tr("Count of product assigns to be imported: %1").arg(nCount) );
        m_nCountPA += nCount;
        if( nCount > 0 )
        {
            typ_termektipusassign stTemp;
            for( unsigned int i=0; i<nCount; i++ )
            {
                fread( &stTemp.nTermekID, 4, 1, file );
                fread( &stTemp.nTTipusID, 4, 1, file );

                ////ui->listLog->addItem( QString( "[%1] [%2]" ).arg(stTemp.nTermekID).arg(stTemp.nTTipusID) );

                m_qvProductAssigns.append( stTemp );
            }
        }

        fclose( file );
        ui->listResultImport->addItem( tr("Importing %1 product assigns finished.").arg(m_qvProductAssigns.size()) );
        _logAction( tr("Importing %1 product assigns finished.").arg(m_qvProductAssigns.size()) );
    }
    else
    {
        ui->listResultImport->addItem( tr( "Error occured during opening trmktpssgfsv.dat file." ) );
    }

    setCursor( Qt::ArrowCursor);
}
//====================================================================================
void MainWindow::_loadUsers()
//====================================================================================
{
    FILE           *file = NULL;
    unsigned int    nCount = 0;
    char            m_strVersion[10];

    setCursor( Qt::WaitCursor);

    m_qvUsers.clear();
    m_nCountU = 0;

    file = fopen( m_qsUFileName.toStdString().c_str(), "rb" );
    if( file != NULL )
    {
        memset( m_strVersion, 0, 10 );
        fread( m_strVersion, 10, 1, file );

        nCount = 0;
        fread( &nCount, 4, 1, file );
        ui->listResultImport->addItem( tr("Count of users to be imported: %1").arg(nCount) );
        _logAction( tr("Count of users to be imported: %1").arg(nCount) );
        m_nCountU += nCount;
        if( nCount > 0 )
        {
            typ_user stTemp;
            for( unsigned int i=0; i<nCount; i++ )
            {
                fread( &stTemp.nID, 4, 1, file );
                fread( stTemp.strAzonosito, 20, 1, file );
                fread( stTemp.strLoginNev, 20, 1, file );
                fread( stTemp.strNevCsalad, 100, 1, file );
                fread( stTemp.strJelszo, 20, 1, file );
                fread( stTemp.strMegjegyzes, 1000, 1, file );
                fread( &stTemp.nUserLevel, 4, 1, file );

                _DeCode( stTemp.strAzonosito, 20 );
                _DeCode( stTemp.strLoginNev, 20 );
                _DeCode( stTemp.strNevCsalad, 100 );
                _DeCode( stTemp.strJelszo, 20 );
                _DeCode( stTemp.strMegjegyzes, 1000 );

                //ui->listLog->addItem( QString( "\'%1\' \'%2\' \'%3\' \'%4\' \'%5\' [%6]" ).arg(stTemp.strAzonosito).arg(stTemp.strLoginNev).arg(stTemp.strNevCsalad).arg(stTemp.strJelszo).arg(stTemp.strMegjegyzes).arg(stTemp.nUserLevel) );

                m_qvUsers.append( stTemp );
            }
        }

        fclose( file );
        ui->listResultImport->addItem( tr("Importing %1 users finished.").arg(m_qvUsers.size()) );
        _logAction( tr("Importing %1 users finished.").arg(m_qvUsers.size()) );
        m_bIsUsersLoaded = true;
    }
    else
    {
        ui->listResultImport->addItem( tr( "Error occured during opening srfsv.dat file." ) );
    }

    setCursor( Qt::ArrowCursor);
}
//====================================================================================
void MainWindow::_EnCode( char *str, int size )
//====================================================================================
{
   for(int i=0;i<size;i++)
   {
      str[i] ^= 11;
   }
}
//====================================================================================
void MainWindow::_DeCode( char *str, int size )
//====================================================================================
{
   for(int i=0;i<size;i++)
   {
      str[i] ^= 11;
   }
}
//====================================================================================
// PREPROCESS PATIENTCARD TYPE
//====================================================================================
void MainWindow::_fillListPatientCardTypes()
//====================================================================================
{
    setCursor( Qt::WaitCursor);

    ui->listPatientCardTypes->clear();
    for( int i=0; i<m_qvPatientCardTypes.count(); i++ )
    {
        ui->listPatientCardTypes->addItem( tr("%1\t%2 eur\t%3 minutes").arg( QString(m_qvPatientCardTypes.at(i).strNev) ).arg( m_qvPatientCardTypes.at(i).nAr ).arg( m_qvPatientCardTypes.at(i).nEgysegIdo ) );
    }

    setCursor( Qt::ArrowCursor);
}
//====================================================================================
void MainWindow::on_pbSaveUnitTimeGeneral_clicked()
//====================================================================================
{
    for( int i=0; i<m_qvPatientCardTypes.count(); i++ )
    {
        m_qvPatientCardTypes[i].nEgysegIdo = ui->ledPCTUnitTimeGeneral->text().toInt();
    }
    _fillListPatientCardTypes();

    if( ui->ledPCTUnitTimeGeneral->text().toInt() > 0 )
    {
        ui->pbNext->setEnabled( true );
    }
}
//====================================================================================
void MainWindow::on_listPatientCardTypes_itemDoubleClicked(QListWidgetItem *item)
//====================================================================================
{
    QStringList qslItem = item->text().split( '\t' );

    QString qsUnitTime = qslItem.at(2);

    ui->ledPCTName->setText( qslItem.at(0) );
    ui->ledPCTUnitTime->setText( qsUnitTime.remove( tr(" minutes") ) );
}
//====================================================================================
void MainWindow::on_pbSaveUnitTime_clicked()
//====================================================================================
{
    int nSelected = ui->listPatientCardTypes->currentRow();

    if( nSelected < m_qvPatientCardTypes.count() )
    {
        m_qvPatientCardTypes[ nSelected ].nEgysegIdo = ui->ledPCTUnitTime->text().toInt();
    }

    QListWidgetItem *item = ui->listPatientCardTypes->currentItem();

    item->setText( tr("%1\t%2 eur\t%3 minutes").arg( QString(m_qvPatientCardTypes.at(nSelected).strNev) ).arg( m_qvPatientCardTypes.at(nSelected).nAr ).arg( m_qvPatientCardTypes.at(nSelected).nEgysegIdo ) );
}
//====================================================================================
// PREPROCESS PATIENTCARDS
//====================================================================================
void MainWindow::on_ledBarcodeLength_textEdited(const QString &arg1)
//====================================================================================
{
    ui->pbNext->setEnabled( ui->ledBarcodeLength->text().toInt() > 0 );
}
//====================================================================================
// EXPORT
//====================================================================================
void MainWindow::on_chkPExportPCT_toggled(bool checked)
//====================================================================================
{
    ui->chkPExportPC->setChecked( checked );
    _checkExportSelection();
}
//====================================================================================
void MainWindow::on_chkPExportPC_toggled(bool checked)
//====================================================================================
{
    ui->chkPExportPCT->setChecked( checked );
    _checkExportSelection();
}
//====================================================================================
void MainWindow::on_chkPExportPT_toggled(bool checked)
//====================================================================================
{
    ui->chkPExportP->setChecked( checked );
    _checkExportSelection();
}
//====================================================================================
void MainWindow::on_chkPExportP_toggled(bool checked)
//====================================================================================
{
    ui->chkPExportPT->setChecked( checked );
    _checkExportSelection();
}
//====================================================================================
void MainWindow::on_chkPExportUser_toggled(bool checked)
//====================================================================================
{
    _checkExportSelection();
}
//====================================================================================
void MainWindow::_checkExportSelection()
//====================================================================================
{
    if( ui->chkPExportPCT->isChecked() ||
        ui->chkPExportPC->isChecked() ||
        ui->chkPExportPT->isChecked() ||
        ui->chkPExportP->isChecked() ||
        ui->chkPExportUser->isChecked() )
    {
        ui->pbExportProcess->setEnabled( true );
    }
    else
    {
        ui->pbExportProcess->setEnabled( false );
    }
}
//====================================================================================
void MainWindow::on_pbExportProcess_clicked()
//====================================================================================
{
    if( m_poDB->isOpen() )
    {
        setCursor( Qt::WaitCursor);

        ui->chkPExportPCT->setEnabled( false );
        ui->chkPExportPC->setEnabled( false );
        ui->chkPExportPT->setEnabled( false );
        ui->chkPExportP->setEnabled( false );
        ui->chkPExportUser->setEnabled( false );
        ui->pbExportProcess->setEnabled( false );
        ui->pbCancel->setEnabled( false );
        ui->pbPrev->setEnabled( false );

        m_nCountItems = 0;

        if( ui->chkPExportPCT->isChecked() )
        {
            m_nCountItems += m_nCountPCT;
            m_nCountItems += m_nCountPC;
        }
        if( ui->chkPExportPT->isChecked() )
        {
            m_nCountItems += m_nCountPT;
            m_nCountItems += m_nCountP;
            m_nCountItems += m_nCountPA;
        }
        if( ui->chkPExportUser->isChecked() )
        {
            m_nCountItems += m_nCountU;
        }

        m_dlgProgress->setMaxValue( m_nCountItems );

        hide();
        m_dlgProgress->showProgress();

        if( ui->chkPExportPCT->isChecked() )
        {
            _exportToBelenusPatientCardTypes();
        }

        if( ui->chkPExportPC->isChecked() )
        {
            _exportToBelenusPatientCards();
        }

        if( ui->chkPExportPT->isChecked() )
        {
            _exportToBelenusProductTypes();
        }

        if( ui->chkPExportP->isChecked() )
        {
            _exportToBelenusProducts();
        }

        if( ui->chkPExportUser->isChecked() )
        {
            _exportToBelenusUsers();
        }

        m_poDB->close();
        m_dlgProgress->hideProgress();
        show();

        setCursor( Qt::ArrowCursor);

        ui->listExport->addItem( tr("Export process finished") );
        ui->listExport->addItem( tr("Connection to Belenus database closed") );
        update();

        ui->pbStartExit->setEnabled( true );
    }
    ui->pbExportProcess->setEnabled( false );
    if( m_poDB != NULL ) delete m_poDB;

    m_poDB = NULL;
}
//====================================================================================
