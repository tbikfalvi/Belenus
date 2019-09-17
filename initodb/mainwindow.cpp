
#include <QMessageBox>
#include <QSettings>
#include <QProcess>
#include <QDateTime>
#include <QDir>
#include <QFileDialog>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    //---------------------------------------------------------------
    // Initialize the GUI
    ui->setupUi(this);

    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    setControlsEnabled( false );

    //---------------------------------------------------------------
    // Initialize the variables
    m_teAction  = ACT_EXECUTE;
    m_nTimer    = 0;
    m_nTimer    = startTimer( 1000 );
}

MainWindow::~MainWindow()
{
    delete ui;
}

//-------------------------------------------------------------------------------------------------
void MainWindow::timerEvent(QTimerEvent *)
//-------------------------------------------------------------------------------------------------
{
    killTimer( m_nTimer );
    m_nTimer = 0;

    switch( m_teAction )
    {
        case ACT_EXECUTE:
        {
            processExecute();
            break;
        }

        default:
            close();
    }
}
/*
//-------------------------------------------------------------------------------------------------
void MainWindow::processBackup()
//-------------------------------------------------------------------------------------------------
{
    QString qsCurrentPath = QDir::currentPath().replace( "\\", "/" );

    QSettings   obPrefFile( QString( "%1/belenus.ini" ).arg( qsCurrentPath ), QSettings::IniFormat );
    QString     qsMysqlPath     = obPrefFile.value( QString::fromAscii( "DbBackup/DirDbBinaries" ), "" ).toString();
    QString     qsBackupPath    = obPrefFile.value( QString::fromAscii( "DbBackup/DirDbBackup" ), "" ).toString();
    QString     qsProcess       = QString( "\"%1/mysqldump.exe\"" ).arg(qsMysqlPath);
    QString     qsParameters    = QString( "-u belenus -pbelenus belenus > \"%1\\belenus_backup_%2.sql\" ").arg(qsBackupPath).arg( QDateTime::currentDateTime().toString("yyyyMMddhhmmss") );
    QString     qsCommand       = QString( "cmd /c %1 %2" ).arg( qsProcess ).arg( qsParameters );

    QDir    qdBackup( qsBackupPath );

    if( !qdBackup.exists() )
    {
        qdBackup.mkpath( qsBackupPath );
    }

    if( qsMysqlPath.length() > 0 )
    {
        QProcess *qpBackup = new QProcess();

        if( qpBackup->execute( qsCommand ) )
        {
            QMessageBox::warning( this, tr("Error"),
                                  tr("Unable to execute the following command:\n"
                                     "%1").arg(qsCommand) );
        }

        delete qpBackup;
    }
    else
    {
        QMessageBox::warning( this, tr("Warning"),
                              tr("Location of the MySQL archive application is not set.\n"
                                 "Please check your settings.") );
    }

    m_teAction = ACT_FINISHED;
    m_nTimer = startTimer( 500 );
}
//-------------------------------------------------------------------------------------------------
void MainWindow::processRestore()
//-------------------------------------------------------------------------------------------------
{
    QString qsCurrentPath = QDir::currentPath().replace( "\\", "/" );

    QSettings   obPrefFile( QString( "%1/belenus.ini" ).arg( qsCurrentPath ), QSettings::IniFormat );
    QString     qsMysqlPath     = obPrefFile.value( QString::fromAscii( "DbBackup/DirDbBinaries" ), "" ).toString();
    QString     qsProcess       = QString( "\"%1/mysql.exe\" -u belenus -pbelenus belenus < " ).arg(qsMysqlPath);
    QString     qsCommand;
    QString     qsImport        = QString( " \"%1\" ").arg(ui->ledDatabase->toolTip());

    //-------------------------------------------------------------------------------------
    // Recreate belenus database with temporary file 'dbrecreate.sql'
    QFile qfRecreate( "dbrecreate.sql" );

    if( qfRecreate.open( QIODevice::WriteOnly ) )
    {
        qfRecreate.write( "DROP DATABASE belenus;\nCREATE DATABASE belenus;\n" );
        qfRecreate.close();
    }
    else
    {
        QMessageBox::warning( this, tr("Error"),
                              tr("Unable to create temporary file: dbrecreate.sql") );
        setControlsEnabled( true );
        return;
    }

    QProcess *qpRestore = new QProcess();

    qsCommand = QString( "cmd /c %1 dbrecreate.sql " ).arg( qsProcess );

    if( qpRestore->execute( qsCommand ) )
    {
        QMessageBox::warning( this, tr("Error"),
                              tr("Unable to execute the following command:\n"
                                 "%1").arg(qsCommand) );
        delete qpRestore;
        setControlsEnabled( true );
        return;
    }
    qfRecreate.remove();

    //-------------------------------------------------------------------------------------
    // Execute database import
    qsCommand = QString( "cmd /c %1 %2" ).arg( qsProcess ).arg( qsImport );

    if( qpRestore->execute( qsCommand ) )
    {
        QMessageBox::warning( this, tr("Error"),
                              tr("Unable to execute the following command:\n"
                                 "%1").arg(qsCommand) );
        delete qpRestore;
        setControlsEnabled( true );
        return;
    }

    //-------------------------------------------------------------------------------------
    // Clear activation random code with temporary file 'dbreactivate.sql'
    QFile qfReactivate( "dbreactivate.sql" );

    if( qfReactivate.open( QIODevice::WriteOnly ) )
    {
        qfReactivate.write( "UPDATE licences SET Cod=\"\";\n" );
        qfReactivate.close();
    }
    else
    {
        QMessageBox::warning( this, tr("Error"),
                              tr("Unable to create temporary file: dbreactivate.sql") );
        setControlsEnabled( true );
        return;
    }

    qsCommand = QString( "cmd /c %1 dbreactivate.sql " ).arg( qsProcess );

    if( qpRestore->execute( qsCommand ) )
    {
        QMessageBox::warning( this, tr("Error"),
                              tr("Unable to execute the following command:\n"
                                 "%1").arg(qsCommand) );
        delete qpRestore;
        setControlsEnabled( true );
        return;
    }
    qfReactivate.remove();

    delete qpRestore;

    m_teAction = ACT_FINISHED;
    m_nTimer = startTimer( 500 );
}
*/
//-------------------------------------------------------------------------------------------------
void MainWindow::processExecute()
//-------------------------------------------------------------------------------------------------
{
    QString qsCurrentPath = QDir::currentPath().replace( "\\", "/" );

    QSettings   obPrefFile( QString( "%1/belenus.ini" ).arg( qsCurrentPath ), QSettings::IniFormat );
    QString     qsMysqlPath     = obPrefFile.value( QString::fromAscii( "DbBackup/DirDbBinaries" ), "" ).toString();
    QString     qsProcess       = QString( "\"%1/mysql.exe\" -u belenus -pbelenus belenus < " ).arg(qsMysqlPath);
    QString     qsExecute       = QString( " \"initodb.sql\" ");
    QString     qsCommand       = QString( "cmd /c %1 %2" ).arg( qsProcess ).arg( qsExecute );

    //-------------------------------------------------------------------------------------
    // Create temporary file 'initodb.sql' for inserting preferences to database
    QFile qfIniToDB( "initodb.sql" );

    if( qfIniToDB.open( QIODevice::WriteOnly ) )
    {
        qfIniToDB.write( "-- -----------------------------------------------------------------------------------\n" );
        qfIniToDB.write( "-- Belenus Szoftver Rendszer (c) Pagony Multimedia Studio Bt - 2013\n" );
        qfIniToDB.write( "-- -----------------------------------------------------------------------------------\n" );
        qfIniToDB.write( "-- Filename    : initodb.sql\n" );
        qfIniToDB.write( "-- AppVersion  : 1.8.0.0\n" );
        qfIniToDB.write( "-- DbVersion   : 1.8.0\n" );
        qfIniToDB.write( "-- -----------------------------------------------------------------------------------\n" );
        qfIniToDB.write( "-- Belenus.ini tartalmat adatbazisba tolto SQL script\n" );
        qfIniToDB.write( "-- -----------------------------------------------------------------------------------\n" );
        qfIniToDB.write( "\n" );
        qfIniToDB.write( "USE `belenus`;\n" );
        qfIniToDB.write( "\n" );
        qfIniToDB.write( "-- -----------------------------------------------------------------------------------\n" );
        qfIniToDB.write( "\n" );

        qfIniToDB.write( saveSettingS( "GEN_LastUser", obPrefFile.value( QString::fromAscii( "LastUser" ), "" ).toString() ).toStdString().c_str() );
        qfIniToDB.write( saveSettingU( "GEN_PanelsPerRow", obPrefFile.value( QString::fromAscii( "PanelsPerRow" ), 1 ).toUInt() ).toStdString().c_str() );
        qfIniToDB.write( saveSettingI( "GEN_BarcodeLength", obPrefFile.value( QString::fromAscii( "BarcodeLength" ), "1" ).toInt() ).toStdString().c_str() );
        qfIniToDB.write( saveSettingS( "GEN_BarcodePrefix", obPrefFile.value( QString::fromAscii( "BarcodePrefix" ), "" ).toString() ).toStdString().c_str() );
        qfIniToDB.write( saveSettingB( "GEN_CardProductBarcodeLengthDifferent", obPrefFile.value( QString::fromAscii( "CardProductBarcodeLengthDifferent" ), true ).toBool() ).toStdString().c_str() );
        qfIniToDB.write( saveSettingB( "GEN_CassaAutoClose", obPrefFile.value( QString::fromAscii( "CassaAutoClose" ), false ).toBool() ).toStdString().c_str() );
        qfIniToDB.write( saveSettingB( "GEN_CassaAutoWithdrawal", obPrefFile.value( QString::fromAscii( "CassaAutoWithdrawal" ), false ).toBool() ).toStdString().c_str() );
        qfIniToDB.write( saveSettingB( "GEN_CassaAutoCreate", obPrefFile.value( QString::fromAscii( "CassaAutoCreate" ), false ).toBool() ).toStdString().c_str() );
        qfIniToDB.write( saveSettingI( "GEN_CassaCreateType", obPrefFile.value( QString::fromAscii( "CassaCreateType" ), 4 ).toInt() ).toStdString().c_str() );
        qfIniToDB.write( saveSettingS( "GEN_DefaultCountry", obPrefFile.value( QString::fromAscii( "DefaultCountry" ), "" ).toString() ).toStdString().c_str() );
        qfIniToDB.write( saveSettingI( "GEN_ZipLength", obPrefFile.value( QString::fromAscii( "ZipLength" ), 4 ).toInt() ).toStdString().c_str() );
        qfIniToDB.write( saveSettingB( "GEN_DBAutoSynchronization", obPrefFile.value( QString::fromAscii( "DBAutoSynchronization" ), false ).toBool() ).toStdString().c_str() );
        qfIniToDB.write( saveSettingB( "GEN_DBGlobalAutoSynchronization", obPrefFile.value( QString::fromAscii( "DBGlobalAutoSynchronization" ), false ).toBool() ).toStdString().c_str() );
        qfIniToDB.write( saveSettingB( "GEN_IsSecondaryWindowVisible", obPrefFile.value( QString::fromAscii( "IsSecondaryWindowVisible" ), false ).toBool() ).toStdString().c_str() );
        qfIniToDB.write( saveSettingI( "GEN_SecondsWaitOnSlpashScreen", obPrefFile.value( QString::fromAscii( "SecondsWaitOnSlpashScreen" ), 3 ).toInt() ).toStdString().c_str() );

        qfIniToDB.write( saveSettingI( "EXTWIN_Left", obPrefFile.value( QString::fromAscii( "SecondaryWindow/Left" ), "10" ).toInt() ).toStdString().c_str() );
        qfIniToDB.write( saveSettingI( "EXTWIN_Top", obPrefFile.value( QString::fromAscii( "SecondaryWindow/Top" ), "10" ).toInt() ).toStdString().c_str() );
        qfIniToDB.write( saveSettingI( "EXTWIN_Width", obPrefFile.value( QString::fromAscii( "SecondaryWindow/Width" ), "600" ).toInt() ).toStdString().c_str() );
        qfIniToDB.write( saveSettingI( "EXTWIN_Height", obPrefFile.value( QString::fromAscii( "SecondaryWindow/Height" ), "400" ).toInt() ).toStdString().c_str() );
        qfIniToDB.write( saveSettingS( "EXTWIN_Background", obPrefFile.value( QString::fromAscii( "SecondaryWindow/Background" ), "#000000"  ).toString() ).toStdString().c_str() );
        qfIniToDB.write( saveSettingB( "EXTWIN_IsTextTubeReplaceVisible", obPrefFile.value( QString::fromAscii( "SecondaryWindow/IsTextTubeReplaceVisible"), false ).toBool() ).toStdString().c_str() );
        qfIniToDB.write( saveSettingB( "EXTWIN_IsTextSterilVisible", obPrefFile.value( QString::fromAscii( "SecondaryWindow/IsTextSterilVisible"), false ).toBool() ).toStdString().c_str() );
        qfIniToDB.write( saveSettingB( "EXTWIN_IsTextTubeCleanupVisible", obPrefFile.value( QString::fromAscii( "SecondaryWindow/IsTextTubeCleanupVisible"), false ).toBool() ).toStdString().c_str() );
        qfIniToDB.write( saveSettingS( "EXTWIN_FrameColor", obPrefFile.value( QString::fromAscii( "SecondaryWindow/FrameColor" ), "#555555" ).toString() ).toStdString().c_str() );
        qfIniToDB.write( saveSettingB( "EXTWIN_IsSecondaryCaptionVisible", obPrefFile.value( QString::fromAscii( "SecondaryWindow/IsSecondaryCaptionVisible"), true ).toBool() ).toStdString().c_str() );

        qfIniToDB.write( saveSettingS( "PANEL_ActiveCaptionBackground", obPrefFile.value( QString::fromAscii( "Panels/ActiveCaptionBackground" ), "#000099" ).toString() ).toStdString().c_str() );
        qfIniToDB.write( saveSettingS( "PANEL_ActiveCaptionColor", obPrefFile.value( QString::fromAscii( "Panels/ActiveCaptionColor" ), "#FFFFFF" ).toString() ).toStdString().c_str() );
        qfIniToDB.write( saveSettingS( "PANEL_InactiveCaptionBackground", obPrefFile.value( QString::fromAscii( "Panels/InactiveCaptionBackground" ), "#000022" ).toString() ).toStdString().c_str() );
        qfIniToDB.write( saveSettingS( "PANEL_InactiveCaptionColor", obPrefFile.value( QString::fromAscii( "Panels/InactiveCaptionColor" ), "#FFFFFF" ).toString() ).toStdString().c_str() );
        qfIniToDB.write( saveSettingS( "PANEL_SecondaryCaptionBackground", obPrefFile.value( QString::fromAscii( "Panels/SecondaryCaptionBackground" ), "#000099" ).toString() ).toStdString().c_str() );
        qfIniToDB.write( saveSettingS( "PANEL_SecondaryCaptionColor", obPrefFile.value( QString::fromAscii( "Panels/SecondaryCaptionColor" ), "#FFFFFF" ).toString() ).toStdString().c_str() );
        qfIniToDB.write( saveSettingB( "PANEL_IsStopInLine", obPrefFile.value( QString::fromAscii( "Panels/IsStopInLine"), true ).toBool() ).toStdString().c_str() );
        qfIniToDB.write( saveSettingS( "PANEL_TextSterile", obPrefFile.value( QString::fromAscii( "Panels/TextSterile"), QObject::tr( " NOT STERILE " ) ).toString() ).toStdString().c_str() );
        qfIniToDB.write( saveSettingS( "PANEL_TextTubeReplace", obPrefFile.value( QString::fromAscii( "Panels/TextTubeReplace"), QObject::tr( " TUBE REPLACEMENT NEEDED " ) ).toString() ).toStdString().c_str() );
        qfIniToDB.write( saveSettingS( "PANEL_TextTubeCleanup", obPrefFile.value( QString::fromAscii( "Panels/TextTubeCleanup"), QObject::tr( " TUBE CLEAN-UP NEEDED " ) ).toString() ).toStdString().c_str() );

        qfIniToDB.write( saveSettingI( "CARD_PriceLost", obPrefFile.value( QString::fromAscii( "PatientCard/PriceLost" ), 0 ).toUInt() ).toStdString().c_str() );
        qfIniToDB.write( saveSettingI( "CARD_PriceLostVat", obPrefFile.value( QString::fromAscii( "PatientCard/PriceLostVat" ), 0 ).toUInt() ).toStdString().c_str() );
        qfIniToDB.write( saveSettingI( "CARD_PricePartner", obPrefFile.value( QString::fromAscii( "PatientCard/PricePartner" ), 0 ).toUInt() ).toStdString().c_str() );
        qfIniToDB.write( saveSettingI( "CARD_PricePartnerVat", obPrefFile.value( QString::fromAscii( "PatientCard/PricePartnerVat" ), 0 ).toUInt() ).toStdString().c_str() );
        qfIniToDB.write( saveSettingB( "CARD_Hidden", obPrefFile.value( QString::fromAscii( "PatientCard/Hidden"), false ).toBool() ).toStdString().c_str() );
        qfIniToDB.write( saveSettingB( "CARD_ShowPatientLastVisitInfo", obPrefFile.value( QString::fromAscii( "PatientCard/ShowPatientLastVisitInfo"), false ).toBool() ).toStdString().c_str() );
        qfIniToDB.write( saveSettingI( "CARD_ShowInfoOnWindow", obPrefFile.value( QString::fromAscii( "PatientCard/ShowInfoOnWindow"), 1 ).toInt() ).toStdString().c_str() );
        qfIniToDB.write( saveSettingI( "CARD_CloseInfoWindowAfterSecs", obPrefFile.value( QString::fromAscii( "PatientCard/CloseInfoWindowAfterSecs"), 5 ).toInt() ).toStdString().c_str() );

        qfIniToDB.write( saveSettingU( "LOG_FileLogLevel", obPrefFile.value( QString::fromAscii( "LogLevels/ConsoleLogLevel" ), 5 ).toUInt() ).toStdString().c_str() );

        qfIniToDB.write( saveSettingS( "SERVER_Address", obPrefFile.value( QString::fromAscii( "Server/Address" ), "http://www.kiwisun.hu" ).toString() ).toStdString().c_str() );
        qfIniToDB.write( saveSettingS( "SERVER_Port", obPrefFile.value( QString::fromAscii( "Server/Port" ), "1000" ).toString() ).toStdString().c_str() );

        qfIniToDB.write( saveSettingI( "HW_ComPort", obPrefFile.value( QString::fromAscii( "Hardware/ComPort" ), "1" ).toInt() ).toStdString().c_str() );
        qfIniToDB.write( saveSettingB( "HW_ForceModuleSendTime", obPrefFile.value( QString::fromAscii( "Hardware/ForceModuleSendTime") ).toBool() ).toStdString().c_str() );
        qfIniToDB.write( saveSettingB( "HW_ForceModuleCheckButton", obPrefFile.value( QString::fromAscii( "Hardware/ForceModuleCheckButton") ).toBool() ).toStdString().c_str() );
        qfIniToDB.write( saveSettingB( "HW_ForceTimeSendCounter", obPrefFile.value( QString::fromAscii( "Hardware/ForceTimeSendCounter") ).toInt() ).toStdString().c_str() );

        qfIniToDB.write( saveSettingS( "UI_MainWindowLeft", obPrefFile.value( QString::fromAscii( "UserInterface/MainWindowLeft" ), "#000000"  ).toString() ).toStdString().c_str() );
        qfIniToDB.write( saveSettingS( "UI_MainWindowTop", obPrefFile.value( QString::fromAscii( "UserInterface/MainWindowTop" ), "#000000"  ).toString() ).toStdString().c_str() );
        qfIniToDB.write( saveSettingS( "UI_MainWindowWidth", obPrefFile.value( QString::fromAscii( "UserInterface/MainWindowWidth" ), "#000000"  ).toString() ).toStdString().c_str() );
        qfIniToDB.write( saveSettingS( "UI_MainWindowHeight", obPrefFile.value( QString::fromAscii( "UserInterface/MainWindowHeight" ), "#000000"  ).toString() ).toStdString().c_str() );
        qfIniToDB.write( saveSettingS( "UI_MainBackground", obPrefFile.value( QString::fromAscii( "UserInterface/MainBackground" ), "#000000"  ).toString() ).toStdString().c_str() );

        qfIniToDB.write( saveSettingS( "CURR_Short", obPrefFile.value( QString::fromAscii( "Currency/Short" ), "Ft." ).toString() ).toStdString().c_str() );
        qfIniToDB.write( saveSettingS( "CURR_Long", obPrefFile.value( QString::fromAscii( "Currency/Long" ), "Forint" ).toString() ).toStdString().c_str() );
        qfIniToDB.write( saveSettingS( "CURR_Separator", obPrefFile.value( QString::fromAscii( "Currency/Separator" ), "," ).toString() ).toStdString().c_str() );
        qfIniToDB.write( saveSettingS( "CURR_Decimal", obPrefFile.value( QString::fromAscii( "Currency/Decimal" ), "." ).toString() ).toStdString().c_str() );

        qfIniToDB.write( saveSettingU( "DEVICE_MaxTreatLength", obPrefFile.value( QString::fromAscii( "Device/MaxTreatLength" ), 100 ).toUInt() ).toStdString().c_str() );
        qfIniToDB.write( saveSettingI( "DEVICE_VAT", obPrefFile.value( QString::fromAscii( "Device/VAT" ), 25 ).toInt() ).toStdString().c_str() );

        qfIniToDB.write( saveSettingB( "SYNC_Enabled", obPrefFile.value( QString::fromAscii( "BlnsHttp/Enabled" ) ).toBool() ).toStdString().c_str() );
        qfIniToDB.write( saveSettingB( "SYNC_WebSyncAutoStart", obPrefFile.value( QString::fromAscii( "BlnsHttp/WebSyncAutoStart" ) ).toBool() ).toStdString().c_str() );

        qfIniToDB.write( saveSettingS( "BACKUP_DirDbBinaries", obPrefFile.value( QString::fromAscii( "DbBackup/DirDbBinaries" ) ).toString().replace( "\\", "/" ) ).toStdString().c_str() );
        qfIniToDB.write( saveSettingS( "BACKUP_DirDbBackup", obPrefFile.value( QString::fromAscii( "DbBackup/DirDbBackup" ) ).toString().replace( "\\", "/" ) ).toStdString().c_str() );
        qfIniToDB.write( saveSettingB( "BACKUP_BackupDb", obPrefFile.value( QString::fromAscii( "DbBackup/BackupDb" ) ).toBool() ).toStdString().c_str() );
        qfIniToDB.write( saveSettingI( "BACKUP_DbBackupType", obPrefFile.value( QString::fromAscii( "DbBackup/DbBackupType" ) ).toInt() ).toStdString().c_str() );
        qfIniToDB.write( saveSettingS( "BACKUP_DbBackupDays", obPrefFile.value( QString::fromAscii( "DbBackup/DbBackupDays" ) ).toString() ).toStdString().c_str() );

        qfIniToDB.write( saveSettingS( "GEN_DateFormat", obPrefFile.value( QString::fromAscii( "DateFormat" ), "yyyy-MM-dd" ).toString() ).toStdString().c_str() );
        qfIniToDB.write( saveSettingB( "GEN_Component", obPrefFile.value( QString::fromAscii( "Component" ), false ).toBool() ).toStdString().c_str() );

/*        bool bFapados = obPrefFile.value( QString::fromAscii( "Component" ), false ).toBool();
        if( bFapados )
        {
            qfIniToDB.write( saveSettingI( "COMPONENT_ID", 66 ).toStdString().c_str() );
        }
        else
        {
            qfIniToDB.write( saveSettingI( "COMPONENT_ID", 42 ).toStdString().c_str() );
        }*/

        qfIniToDB.write( saveSettingB( "PANEL_UsageVisibleOnMain", obPrefFile.value( QString::fromAscii( "PanelSettings/UsageVisibleOnMain"), true ).toBool() ).toStdString().c_str() );

        qfIniToDB.write( saveSettingB( "AUTOMAIL_OnSell", obPrefFile.value( QString::fromAscii( "AutoMail/OnSell"), false ).toBool() ).toStdString().c_str() );
        qfIniToDB.write( saveSettingB( "AUTOMAIL_OnUse", obPrefFile.value( QString::fromAscii( "AutoMail/OnUse"), false ).toBool() ).toStdString().c_str() );
        qfIniToDB.write( saveSettingB( "AUTOMAIL_OnExpiration", obPrefFile.value( QString::fromAscii( "AutoMail/OnExpiration"), false ).toBool() ).toStdString().c_str() );
        qfIniToDB.write( saveSettingI( "AUTOMAIL_ExpirationDays", obPrefFile.value( QString::fromAscii( "AutoMail/ExpirationDays"), 7 ).toInt() ).toStdString().c_str() );

        qfIniToDB.write( "\n" );
        qfIniToDB.write( "-- -----------------------------------------------------------------------------------\n" );
        qfIniToDB.write( "\n" );

        qfIniToDB.close();
    }
    else
    {
        QMessageBox::warning( this, tr("Error"),
                              tr("Unable to create temporary file: initodb.sql") );
        setControlsEnabled( true );
        return;
    }
/*
    QProcess *qpExecute = new QProcess();

    if( qpExecute->execute( qsCommand ) )
    {
        QMessageBox::warning( this, tr("Error"),
                              tr("Unable to execute the following command:\n"
                                 "%1").arg(qsCommand) );
        delete qpExecute;
        setControlsEnabled( true );
        return;
    }

    delete qpExecute;
*/
    m_teAction = ACT_FINISHED;
    m_nTimer = startTimer( 500 );
}

QString MainWindow::saveSettingU( QString p_Identifier, unsigned int p_Value )
{
    return QString( "INSERT INTO `settings` (`settingId`, `identifier`, `value`) VALUES (NULL, '%1', '%2');\n" ).arg( p_Identifier ).arg( p_Value );
}

QString MainWindow::saveSettingS( QString p_Identifier, QString p_Value )
{
    return QString( "INSERT INTO `settings` (`settingId`, `identifier`, `value`) VALUES (NULL, '%1', '%2');\n" ).arg( p_Identifier ).arg( p_Value );
}

QString MainWindow::saveSettingB( QString p_Identifier, bool p_Value )
{
    return QString( "INSERT INTO `settings` (`settingId`, `identifier`, `value`) VALUES (NULL, '%1', '%2');\n" ).arg( p_Identifier ).arg( p_Value );
}

QString MainWindow::saveSettingI( QString p_Identifier, int p_Value )
{
    return QString( "INSERT INTO `settings` (`settingId`, `identifier`, `value`) VALUES (NULL, '%1', '%2');\n" ).arg( p_Identifier ).arg( p_Value );
}


void MainWindow::on_pbExit_clicked()
{
    close();
}

void MainWindow::on_pbSelect_clicked()
{
    QString qsCurrentPath = QDir::currentPath().replace( "\\", "/" );

    QSettings   obPrefFile( QString( "%1/belenus.ini" ).arg( qsCurrentPath ), QSettings::IniFormat );
    QString     qsBackupPath = obPrefFile.value( QString::fromAscii( "DbBackup/DirDbBackup" ), "" ).toString();
    QString     qsFile = "";

    if( qsBackupPath.length() == 0 )    qsBackupPath = QDir::currentPath();

    QFileDialog dlgFileOpen( this );

    dlgFileOpen.setDirectory( qsBackupPath );
    dlgFileOpen.setFileMode( QFileDialog::ExistingFile );
    dlgFileOpen.setOptions( QFileDialog::DontResolveSymlinks );
    dlgFileOpen.setViewMode( QFileDialog::Detail );

    if( dlgFileOpen.exec() )
    {
        QString qsDir  = dlgFileOpen.directory().absolutePath();
        qsDir.replace( '/', '\\' );
        if( qsDir.right(1).compare("\\") == 0 )
        {
            qsDir = qsDir.left(qsDir.length()-1);
        }
        qsFile = dlgFileOpen.selectedFiles().at(0).right( dlgFileOpen.selectedFiles().at(0).length()-qsDir.length()-1 );

        ui->ledDatabase->setText( qsFile );
        ui->ledDatabase->setToolTip( QString("%1\\%2").arg( qsDir ).arg( qsFile ) );
    }
}

void MainWindow::on_pbStart_clicked()
{
    QFile   fileDb( ui->ledDatabase->toolTip() );

    if( fileDb.exists() )
    {
        setControlsEnabled( false );
        ui->lblInfo->setText( tr("Please wait until the process finish.") );
        m_nTimer = startTimer( 1000 );
    }
    else
    {
        QMessageBox::warning( this, tr("Error"),
                              tr("The selected file not exists or not readable\n\n%1").arg( ui->ledDatabase->toolTip() ) );
        setControlsEnabled( true );
        return;
    }
}

void MainWindow::setControlsEnabled(bool p_bEnable)
{
    ui->ledDatabase->setVisible( p_bEnable );
    ui->ledDatabase->setEnabled( p_bEnable );
    ui->pbSelect->setVisible( p_bEnable );
    ui->pbSelect->setEnabled( p_bEnable );
    ui->pbStart->setVisible( p_bEnable );
    ui->pbStart->setEnabled( p_bEnable );
}
