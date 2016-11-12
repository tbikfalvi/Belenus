//    QMessageBox::information( this, "", "" );

//#include <windows.h>
#include <QMessageBox>
#include <QSettings>
#include <QDir>
#include <QProcessEnvironment>
#include <QMouseEvent>
#include <iostream>

#include "dlgmain.h"
#include "ui_dlgmain.h"

using namespace std;

dlgMain::dlgMain(QWidget *parent) : QDialog(parent), ui(new Ui::dlgMain)
{
    ui->setupUi(this);

    m_nTimer            = 0;
    m_bMousePressed     = false;
    m_bReloadLanguage   = false;

    setWindowFlags( Qt::Dialog | Qt::FramelessWindowHint );

    QSettings   obPref( QString( "%1/websync.inf" ).arg( QDir::currentPath() ), QSettings::IniFormat );

    trayIconMenu = new QMenu(this);
    trayIcon = new QSystemTrayIcon(this);

    _setActions();
    _setMenu();

    trayIcon->setIcon( QIcon( ":/websync.png" ) );
    trayIcon->setToolTip( tr("Belenus WebSync") );
    trayIcon->show();

    try
    {
        g_poDB = new cQTMySQLConnection;
        g_poDB->setHostName( "localhost" );
        g_poDB->setDatabaseName( "belenus" );
        g_poDB->setUserName( "belenus" );
        g_poDB->setPassword( "belenus" );
        g_poDB->open();

        ui->lblStatusIconSQL->setPixmap( QPixmap( ":/status_green.png" ) );
        ui->lblStatusIconSQL->setToolTip( tr("SQL Connection established") );
    }
    catch( cSevException &e )
    {
        cerr << ">> " << e.what() << endl << flush;;
        g_obLogger(e.severity()) << e.what() << EOM;

        ui->lblStatusIconSQL->setPixmap( QPixmap( ":/status_red.png" ) );
        ui->lblStatusIconSQL->setToolTip( tr("Error in connection: %1").arg( e.what() ) );
    }


    m_qsLang                    = obPref.value( "Lang", "en" ).toString();

    m_bShowMainWindowOnStart    = obPref.value( "ShowMainWindowOnStart", 0 ).toBool();

    int nCurrentIndex   = ui->cmbLang->findText( QString("%1 (").arg(m_qsLang), Qt::MatchContains );

    ui->cmbLang->setCurrentIndex( nCurrentIndex );

    m_nTimer = startTimer( 500 );

    resize( obPref.value( "WindowPosition/Mainwindow_width", 941 ).toInt(),
            obPref.value( "WindowPosition/Mainwindow_height", 440 ).toInt() );
    move( obPref.value( "WindowPosition/Mainwindow_left", 100).toInt(),
          obPref.value( "WindowPosition/Mainwindow_top", 100).toInt() );
}

dlgMain::~dlgMain()
{
    QSettings   obPref( QString( "%1/websync.inf" ).arg( QDir::currentPath() ), QSettings::IniFormat );

    obPref.setValue( "Lang",                    m_qsLang );
    obPref.setValue( "ShowMainWindowOnStart",   m_bShowMainWindowOnStart );

    obPref.setValue( "WindowPosition/Mainwindow_left", x() );
    obPref.setValue( "WindowPosition/Mainwindow_top", y() );
    obPref.setValue( "WindowPosition/Mainwindow_width", width() );
    obPref.setValue( "WindowPosition/Mainwindow_height", height() );

    delete g_poDB;

    g_obLogger(cSeverity::INFO) << "Belenus WebSync ended." << EOM;

    delete ui;
}

void dlgMain::timerEvent(QTimerEvent *)
{
    killTimer( m_nTimer );
    m_nTimer = 0;

    if( m_bShowMainWindowOnStart )  { show();               }
}

void dlgMain::mousePressEvent ( QMouseEvent *p_poEvent )
{
    m_bMousePressed = true;

    m_nMouseX = p_poEvent->pos().x();
    m_nMouseY = p_poEvent->pos().y();

    p_poEvent->accept();
}
void dlgMain::mouseReleaseEvent ( QMouseEvent *p_poEvent )
{
    m_bMousePressed = false;
    p_poEvent->accept();
}

void dlgMain::mouseMoveEvent ( QMouseEvent *p_poEvent )
{
    if( m_bMousePressed )
    {
        move( x() + p_poEvent->pos().x() - m_nMouseX,
              y() + p_poEvent->pos().y() - m_nMouseY );
    }
    p_poEvent->accept();
}

void dlgMain::_setActions()
{
    actionSettings = new QAction(tr("&Open main window"), this);
    actionSettings->setIcon( QIcon( ":/settings.png" ) );
    connect( actionSettings, SIGNAL(triggered()), this, SLOT(slotSettings()) );

//    actionExit = new QAction(tr("&Exit application"), this);
//    actionExit->setIcon( QIcon( ":/exit.png" ) );
//    connect( actionExit, SIGNAL(triggered()), qApp, SLOT(quit()) );
}

void dlgMain::_setMenu()
{
    trayIconMenu->addAction( actionSettings );
//    trayIconMenu->addSeparator();

//    trayIconMenu->addAction( actionExit );

    trayIcon->setContextMenu(trayIconMenu);
}

void dlgMain::slotSettings()
{
    show();
}

void dlgMain::on_pbHide_clicked()
{
    hide();
}

void dlgMain::on_pbExit_clicked()
{
    qApp->quit();
}


void dlgMain::on_cmbLang_currentIndexChanged(const QString &arg1)
{
    if( !m_bReloadLanguage )
    {
        m_qsLang = arg1.left(2);
    }
}

void dlgMain::on_pbRetranslate_clicked()
{
    m_bReloadLanguage = true;

    apMainApp->removeTranslator( poTransApp );
    apMainApp->removeTranslator( poTransQT );

    poTransApp->load( QString("%1\\websync_%2.qm").arg( QDir::currentPath() ).arg(m_qsLang) );
    poTransQT->load( QString("%1\\qt_%2.qm").arg( QDir::currentPath() ).arg(m_qsLang) );

    apMainApp->installTranslator( poTransApp );
    apMainApp->installTranslator( poTransQT );

    ui->retranslateUi( this );

    int nCurrentIndex   = ui->cmbLang->findText( QString("%1 (").arg(m_qsLang), Qt::MatchContains );

    ui->cmbLang->setCurrentIndex( nCurrentIndex );

    m_bReloadLanguage = false;
}
