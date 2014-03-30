//===========================================================================================================
//
// Belenus Updater alkalmazas (c) Pagony Multimedia Studio Bt - 2014
//
//===========================================================================================================
//
// Filename    : mainwindow.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//===========================================================================================================

#include <QDateTime>
#include <QTextStream>
#include <QMessageBox>
#include <QProcess>
#include "windows.h"

//===========================================================================================================

#include "BelenusUpdater.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

//===========================================================================================================
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
//-----------------------------------------------------------------------------------------------------------
{
    m_obLog = new QFile( QString("%1\\update.log").arg(g_qsCurrentPath) );

    m_bUpdateProcessFinished    = false;

    _logProcess( "" );
    _logProcess( QString( "Starting Belenus Updater - %1" ).arg( QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") ) );
    _logProcess( "" );

    setupUi(this);

    lblCheckEnvironment->setText( tr("Checking Belenus Application System's environment ... ") );
    lblCheckOk_1->setVisible( false );
    lblCheckFail_1->setVisible( false );

    lblCheckInternet->setText("Checking internet connection ... ");
    lblCheckOk_2->setVisible( false );
    lblCheckFail_2->setVisible( false );

    lblCheckUpdateDir->setText("Checking local update directory ... ");
    lblCheckOk_3->setVisible( false );
    lblCheckFail_3->setVisible( false );

    m_nTimer = startTimer( 1000 );
}
//===========================================================================================================
MainWindow::~MainWindow()
//-----------------------------------------------------------------------------------------------------------
{
    _logProcess( "" );
    _logProcess( QString( "Exiting Belenus Updater - %1" ).arg( QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") ) );
    _logProcess( "" );

    if( m_obLog != NULL )       delete m_obLog;
}
//===========================================================================================================
void MainWindow::timerEvent(QTimerEvent *)
//-----------------------------------------------------------------------------------------------------------
{
    killTimer( m_nTimer );

    if( !m_bUpdateProcessFinished )
    {
        executeUpdateProcess();
    }
    else
    {
        close();
    }
}
//===========================================================================================================
void MainWindow::closeEvent( QCloseEvent *p_poEvent )
//-----------------------------------------------------------------------------------------------------------
{
    // if something notok
//    p_poEvent->ignore();

    p_poEvent->accept();
}
//===========================================================================================================
//
//  Update process
//
//===========================================================================================================
void MainWindow::executeUpdateProcess()
//-----------------------------------------------------------------------------------------------------------
{
    checkEnvironment( isEnvironmentOk() );

    m_bUpdateProcessFinished = true;

    m_nTimer = startTimer( 2000 );
}
//===========================================================================================================
//
//  Checking environment, directories, files
//
//===========================================================================================================
void MainWindow::checkEnvironment(bool p_bVisible)
//-----------------------------------------------------------------------------------------------------------
{
    lblCheckOk_1->setVisible( p_bVisible );
    lblCheckFail_1->setVisible( !p_bVisible );
}
//===========================================================================================================
bool MainWindow::isEnvironmentOk()
//-----------------------------------------------------------------------------------------------------------
{
    // Check if the full directory structure is ok and create missing ones
    _logProcess( "Checking Belenus Application System's environment" );

    prgProcess->setValue( 0 );
    prgProcess->setMaximum( 116 );

    if( !checkDirectories() )   return false;   //   7
    checkFiles();                               // 109

    return true;
}
//===========================================================================================================
bool MainWindow::checkDirectories()
//-----------------------------------------------------------------------------------------------------------
{
    if( !_checkAndCreateDirectory( "c:\\Windows\\Temp\\BelenusInstall" ) )              return false;
    if( !_checkAndCreateDirectory( QString("%1\\docs").arg( g_qsCurrentPath ) ) )       return false;
    if( !_checkAndCreateDirectory( QString("%1\\lang").arg( g_qsCurrentPath ) ) )       return false;
    if( !_checkAndCreateDirectory( QString("%1\\resources").arg( g_qsCurrentPath ) ) )  return false;
    if( !_checkAndCreateDirectory( QString("%1\\sql").arg( g_qsCurrentPath ) ) )        return false;
    if( !_checkAndCreateDirectory( QString("%1\\backup").arg( g_qsCurrentPath ) ) )     return false;
    if( !_checkAndCreateDirectory( QString("%1\\download").arg( g_qsCurrentPath ) ) )   return false;

    return true;
}
//===========================================================================================================
bool MainWindow::checkFiles()
//-----------------------------------------------------------------------------------------------------------
{
    _isFileExists( QString("%1\\libgcc_s_dw2-1.dll").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\libmySQL.dll").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\mingwm10.dll").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\QtCore4.dll").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\QtGui4.dll").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\QtNetwork4.dll").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\QtSql4.dll").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\belenus.exe").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\ReportViewer.exe").arg( g_qsCurrentPath ) );

    _isFileExists( QString("%1\\docs\\FelhasznaloiUtmutato.pdf").arg( g_qsCurrentPath ) );

    _isFileExists( QString("%1\\lang\\belenus_hu.qm").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\lang\\belenus_us.qm").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\lang\\BelenusUpdater_hu.qm").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\lang\\brv_hu.qm").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\lang\\brv_us.qm").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\lang\\qt_hu.qm").arg( g_qsCurrentPath ) );

    _isFileExists( QString("%1\\resources\\40x40_address.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_book.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_book_daily.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_book_ledger.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_cancel.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_cassa.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_cassa_add.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_cassa_get.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_cassa_storno.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_clock.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_company.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_connect_db.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_database_sync.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_delete.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_device.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_device_clear.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_device_later.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_device_next.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_device_pause.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_device_settings.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_device_start.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_discount.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_edit.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_exit.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_font.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_hourglass.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_information.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_key.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_lock.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_logout.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_minus.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_new.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_next.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_ok.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_panel.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_patient.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_patientcard.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_patientcardadd.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_patientcards.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_patientcardtype.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_patientcard_active.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_patientcard_assign.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_patientcard_disjoin.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_patientcard_inactive.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_patientcard_join.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_patientcard_sell.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_patientcard_usage.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_patientorigin.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_patient_deselect.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_patient_edit.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_patient_later.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_patient_new.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_patient_select.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_paycash.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_paymentmethod.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_paywithcard.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_paywithcash.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_plus.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_print.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_product.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_productactiontype.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_productsell.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_producttype.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_question.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_reasontovisit.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_refresh.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_report_patientcardtypes.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_report_patientcard_debt.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_report_patientcard_details.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_report_patientcard_inactive.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_report_patientcard_sell.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_report_patientcard_usages.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_report_products.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_report_product_history.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_report_product_status.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_save.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_search.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_secondary_off.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_secondary_on.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_settings.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_shoppingcart.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_shutdown.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_start.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_stock_add.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_stock_remove.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_stop.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_storage.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_user.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\40x40_validate.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\77x40_off.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\77x40_on.png").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\belenus.ico").arg( g_qsCurrentPath ) );
    _isFileExists( QString("%1\\resources\\splash.png").arg( g_qsCurrentPath ) );

    return true;
}
//===========================================================================================================
//
//  Checking internet connections
//
//===========================================================================================================
void MainWindow::checkInternetConnection()
//-----------------------------------------------------------------------------------------------------------
{
}
//===========================================================================================================
//
//  General usage private procedures
//
//===========================================================================================================
bool MainWindow::_checkAndCreateDirectory(QString p_qsPath)
//-----------------------------------------------------------------------------------------------------------
{
    QDir    qdDocs( p_qsPath );

    _logProcess( QString("Checking directory: %1 - ").arg( p_qsPath ), false );
    if( !qdDocs.exists() )
    {
        if( !_createTargetDirectory( qdDocs.absolutePath() ) )
        {
            _logProcess( "FAILED" );
            return false;
        }
    }
    _logProcess( "OK" );
    prgProcess->setValue( prgProcess->value()+1 );

    return true;
}
//===========================================================================================================
bool MainWindow::_createTargetDirectory( QString p_qsPath )
//-----------------------------------------------------------------------------------------------------------
{
    QDir    qdDir( p_qsPath );

    if( !qdDir.mkpath( p_qsPath ) )
    {
        QMessageBox::critical( this, tr("System error"),
                               tr("Unable to create directory:\n\n%1").arg(p_qsPath));
        return false;
    }

    return true;
}
//===========================================================================================================
bool MainWindow::_isFileExists(QString p_qsFilename)
//-----------------------------------------------------------------------------------------------------------
{
    QFile   qfTemp( p_qsFilename );

    _logProcess( QString( "Checking file: %1 - " ).arg( p_qsFilename ), false );
    if( !qfTemp.exists() )
    {
        _logProcess( "MISSING" );
        m_qslMissingFiles << p_qsFilename;
        return false;
    }
    _logProcess( "OK" );
    prgProcess->setValue( prgProcess->value()+1 );

    return true;
}
//===========================================================================================================
bool MainWindow::_createRegistryEntries()
//-----------------------------------------------------------------------------------------------------------
{
    return true;
}
//===========================================================================================================
void MainWindow::_logProcess( QString p_qsLog, bool p_bInsertNewLine )
//-----------------------------------------------------------------------------------------------------------
{
    if( m_obLog->open( QIODevice::Append | QIODevice::Text ) )
    {
        QTextStream out( m_obLog );
        out << p_qsLog;
        if( p_bInsertNewLine )
            out << "\n";
        m_obLog->close();
    }
}
//===========================================================================================================
