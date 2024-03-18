#include <QPushButton>
#include <QMessageBox>

#include "dlgwindowposition.h"
#include "belenus.h"

cDlgWindowPosition::cDlgWindowPosition( QWidget *p_poParent ) : QDialog( p_poParent )
{
    cTracer obTrace( "cDlgWindowPosition::cDlgWindowPosition" );

    setupUi( this );

    setWindowTitle( tr( "Set window position / size" ) );
    setWindowIcon( QIcon("./resources/40x40_device_settings.png") );

    pbClose->setIcon( QIcon("./resources/40x40_exit.png") );

    cmbWindows->addItem( tr("<No window selected>") );
    cmbWindows->addItem( tr("Main window") );
    cmbWindows->addItem( tr("Secondary window") );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( "SELECT * FROM advertisements ");

    while( poQuery->next() )
    {
        cmbWindows->addItem( QString( "Ad%1 - %2" ).arg( poQuery->value(0).toString() ).arg( poQuery->value(2).toString() ) );
    }

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "WindowPosition", QPoint(420,330) );
    resize( qpDlgSize.x(), qpDlgSize.y() );
}

cDlgWindowPosition::~cDlgWindowPosition()
{
    g_poPrefs->setDialogSize( "WindowPosition", QPoint( width(), height() ) );
}

void cDlgWindowPosition::on_cmbWindows_currentIndexChanged(const QString &itemText)
{
    if( itemText.compare( tr("<No window selected>") ) == 0 )
    {
        gbPosition->setEnabled( false );
        gbAction->setEnabled( false );

        ledPosX->setText( "" );
        ledPosY->setText( "" );
        ledWidth->setText( "" );
        ledHeight->setText( "" );
    }
    else
    {
        gbPosition->setEnabled( true );
        gbAction->setEnabled( true );

        if( cmbWindows->currentIndex() == 1 )
        {
            m_qsLeft    = "UI_MainWindowLeft";
            m_qsTop     = "UI_MainWindowTop";
            m_qsWidth   = "UI_MainWindowWidth";
            m_qsHeight  = "UI_MainWindowHeight";
        }
        else if( cmbWindows->currentIndex() == 2 )
        {
            m_qsLeft    = "EXTWIN_Left";
            m_qsTop     = "EXTWIN_Top";
            m_qsWidth   = "EXTWIN_Width";
            m_qsHeight  = "EXTWIN_Height";
        }
        else
        {
            QStringList qslItem = itemText.split( " - " );

            m_qsLeft    = QString( "DLG_%1_left" ).arg( qslItem.at(0) );
            m_qsTop     = QString( "DLG_%1_top" ).arg( qslItem.at(0) );
            m_qsWidth   = QString( "DLG_%1_width" ).arg( qslItem.at(0) );
            m_qsHeight  = QString( "DLG_%1_height" ).arg( qslItem.at(0) );

            m_qsAdId    = qslItem.at(0).right( qslItem.at(0).length()-2 );
        }

        ledPosX->setText( g_poPrefs->getValue( m_qsLeft ) );
        ledPosY->setText( g_poPrefs->getValue( m_qsTop ) );
        ledWidth->setText( g_poPrefs->getValue( m_qsWidth ) );
        ledHeight->setText( g_poPrefs->getValue( m_qsHeight ) );
    }
}

void cDlgWindowPosition::on_pbMove_clicked()
{
    if( cmbWindows->currentIndex() == 1 )
    {
        g_poPrefs->moveWindowMain( ledPosX->text().toInt(),
                                   ledPosY->text().toInt() );
    }
    else if( cmbWindows->currentIndex() == 2 )
    {
        g_poPrefs->moveWindowSecondary( ledPosX->text().toInt(), ledPosY->text().toInt() );
    }
    else
    {
        QSettings   obPrefFile( "advertisement.cmd", QSettings::IniFormat );

        g_poPrefs->setValue( m_qsLeft, ledPosX->text() );
        g_poPrefs->setValue( m_qsTop, ledPosY->text() );
        obPrefFile.setValue( QString::fromAscii( "Advertisement%1/Command" ).arg( m_qsAdId ), "RESIZE" );
    }
}

void cDlgWindowPosition::on_pbResize_clicked()
{
    if( cmbWindows->currentIndex() == 1 )
    {
        g_poPrefs->resizeWindowMain( ledWidth->text().toInt(),
                                     ledHeight->text().toInt() );
    }
    else if( cmbWindows->currentIndex() == 2 )
    {
        g_poPrefs->resizeWindowSecondary( ledWidth->text().toInt(), ledHeight->text().toInt() );
    }
    else
    {
        QSettings   obPrefFile( "advertisement.cmd", QSettings::IniFormat );

        g_poPrefs->setValue( m_qsWidth, ledWidth->text() );
        g_poPrefs->setValue( m_qsHeight, ledHeight->text() );
        obPrefFile.setValue( QString::fromAscii( "Advertisement%1/Command" ).arg( m_qsAdId ), "RESIZE" );
    }
}

void cDlgWindowPosition::on_pbClose_clicked()
{
    accept();
}

