#include <QPushButton>
#include <QMessageBox>
#include <QFileDialog>

#include "belenus.h"
#include "dlgpanelsetting.h"

#include "../crud/dlgpaneltimes.h"


cDlgPanelSetting::cDlgPanelSetting( QWidget *p_poParent, unsigned int p_uiPanelId ) : QDialog( p_poParent )
{
    cTracer obTrace( "cDlgPanelSetting::cDlgPanelSetting" );

    setupUi( this );

    setWindowTitle( tr( "Panel settings" ) );
    setWindowIcon( QIcon("./resources/40x40_device.png") );

    pbEmptyImage->setIcon( QIcon("./resources/40x40_delete.png") );
    pbEmptyImage->setEnabled( g_obUser.isInGroup( cAccessGroup::SYSTEM ) );

    ledWorkTimeHour->setAlignment( Qt::AlignRight );
    ledWorkTimeHour->setEnabled( g_obUser.isInGroup( cAccessGroup::SYSTEM ) );
    ledWorkTimeMin->setAlignment( Qt::AlignCenter );
    ledWorkTimeMin->setEnabled( g_obUser.isInGroup( cAccessGroup::SYSTEM ) );
    ledWorkTimeSec->setAlignment( Qt::AlignCenter );
    ledWorkTimeSec->setEnabled( g_obUser.isInGroup( cAccessGroup::SYSTEM ) );

    if( g_obUser.isInGroup( cAccessGroup::ADMIN ) )
    {
        ledWorkTimeHour->setEchoMode( QLineEdit::Normal );
        ledWorkTimeMin->setEchoMode( QLineEdit::Normal );
        ledWorkTimeSec->setEchoMode( QLineEdit::Normal );
    }
    else
    {
        ledWorkTimeHour->setEchoMode( QLineEdit::Password );
        ledWorkTimeMin->setEchoMode( QLineEdit::Password );
        ledWorkTimeSec->setEchoMode( QLineEdit::Password );
    }

    pbWTReset->setIcon( QIcon("./resources/40x40_hourglass.png") );
    pbWTReset->setEnabled( g_obUser.isInGroup( cAccessGroup::SYSTEM ) );

    pbEnableDeviceAdmin->setIconSize( QSize(20,20) );
    pbEnableDeviceAdmin->setIcon( QIcon("./resources/40x40_key.png") );
    pbEnableDeviceAdmin->setEnabled( true );

    ledMaxWorkTime->setEnabled( g_obUser.isInGroup( cAccessGroup::SYSTEM ) );

    ledCleanTimeHour->setAlignment( Qt::AlignRight );
    ledCleanTimeHour->setEnabled( g_obUser.isInGroup( cAccessGroup::SYSTEM ) );
    ledCleanTimeMin->setAlignment( Qt::AlignCenter );
    ledCleanTimeMin->setEnabled( g_obUser.isInGroup( cAccessGroup::SYSTEM ) );
    ledCleanTimeSec->setAlignment( Qt::AlignCenter );
    ledCleanTimeSec->setEnabled( g_obUser.isInGroup( cAccessGroup::SYSTEM ) );

    if( g_obUser.isInGroup( cAccessGroup::ADMIN ) )
    {
        ledCleanTimeHour->setEchoMode( QLineEdit::Normal );
        ledCleanTimeMin->setEchoMode( QLineEdit::Normal );
        ledCleanTimeSec->setEchoMode( QLineEdit::Normal );
    }
    else
    {
        ledCleanTimeHour->setEchoMode( QLineEdit::Password );
        ledCleanTimeMin->setEchoMode( QLineEdit::Password );
        ledCleanTimeSec->setEchoMode( QLineEdit::Password );
    }

    pbCTReset->setIconSize( QSize(20,20) );
    pbCTReset->setIcon( QIcon("./resources/40x40_hourglass.png") );
    pbCTReset->setEnabled( g_obUser.isInGroup( cAccessGroup::SYSTEM ) );

    ledMaxCleanTime->setEnabled( g_obUser.isInGroup( cAccessGroup::SYSTEM ) );

    pbEnableSystemAdmin->setIconSize( QSize(40,40) );
    pbEnableSystemAdmin->setIcon( QIcon("./resources/40x40_key.png") );
    pbEnableSystemAdmin->setEnabled( true );

    pbOk->setIcon( QIcon("./resources/40x40_ok.png") );
    pbCancel->setIcon( QIcon("./resources/40x40_cancel.png") );

    pbTime->setIcon( QIcon("./resources/40x40_device_time.png") );

    m_uiPanelId         = p_uiPanelId;

    if( p_uiPanelId > 0 )
    {
        cDBPanel    obDBPanel;

        obDBPanel.load( m_uiPanelId );

        unsigned int hourWork       = obDBPanel.workTime()/3600;
        unsigned int minuteWork     = (obDBPanel.workTime()-(hourWork*3600))/60;
        unsigned int secondWork     = (obDBPanel.workTime()-(hourWork*3600))%60;
        unsigned int hourClean       = obDBPanel.cleanTime()/3600;
        unsigned int minuteClean     = (obDBPanel.cleanTime()-(hourClean*3600))/60;
        unsigned int secondClean     = (obDBPanel.cleanTime()-(hourClean*3600))%60;

        ledTitle->setText( obDBPanel.title() );
        ledWorkTimeHour->setText( QString::number(hourWork) );
        ledWorkTimeMin->setText( QString::number(minuteWork) );
        ledWorkTimeSec->setText( QString::number(secondWork) );
        ledMaxWorkTime->setText( QString::number(obDBPanel.maxWorkTime()) );
        ledCleanTimeHour->setText( QString::number(hourClean) );
        ledCleanTimeMin->setText( QString::number(minuteClean) );
        ledCleanTimeSec->setText( QString::number(secondClean) );
        ledMaxCleanTime->setText( QString::number(obDBPanel.maxCleanTime()) );
        ledImagePath->setText( obDBPanel.image() );

        QSqlQuery *poQueryType;

        poQueryType = g_poDB->executeQTQuery( QString( "SELECT panelTypeId, name FROM panelTypes WHERE active=1" ) );
        while( poQueryType->next() )
        {
            cmbPanelType->addItem( poQueryType->value( 1 ).toString(), poQueryType->value( 0 ) );
            if( poQueryType->value( 0 ).toUInt() == obDBPanel.panelTypeId() )
                cmbPanelType->setCurrentIndex( cmbPanelType->count()-1 );
        }

        poQueryType = g_poDB->executeQTQuery( QString( "SELECT panelGroupId, name FROM panelGroups WHERE active=1" ) );
        while( poQueryType->next() )
        {
            cmbPanelGroup->addItem( poQueryType->value( 1 ).toString(), poQueryType->value( 0 ) );
            if( poQueryType->value( 0 ).toUInt() == obDBPanel.panelGroupId() )
                cmbPanelGroup->setCurrentIndex( cmbPanelGroup->count()-1 );
        }
        if( poQueryType ) delete poQueryType;
    }

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "PanelSetting", QPoint(550,330) );
    resize( qpDlgSize.x(), qpDlgSize.y() );
}

cDlgPanelSetting::~cDlgPanelSetting()
{
    g_poPrefs->setDialogSize( "PanelSetting", QPoint( width(), height() ) );
}

void cDlgPanelSetting::on_pbOk_clicked()
{
    bool  boCanBeSaved = true;

    if( ledTitle->text() == "" )
    {
        boCanBeSaved = false;
        QMessageBox::critical( this, tr( "Error" ), tr( "Title of panel can not be empty." ), QMessageBox::Ok );
    }
    if( ledMaxWorkTime->isEnabled() )
    {
        if( ledMaxWorkTime->text() == "" )
        {
            boCanBeSaved = false;
            QMessageBox::critical( this, tr( "Error" ), tr( "Maximum worktime of panel can not be empty." ), QMessageBox::Ok );
        }
        else if( ledMaxWorkTime->text().toUInt() < 1 )
        {
            boCanBeSaved = false;
            QMessageBox::critical( this, tr( "Error" ), tr( "Maximum worktime has to be greater than zero." ), QMessageBox::Ok );
        }
    }
    if( ledMaxCleanTime->isEnabled() )
    {
        if( ledMaxCleanTime->text() == "" )
        {
            boCanBeSaved = false;
            QMessageBox::critical( this, tr( "Error" ), tr( "Maximum cleantime of panel can not be empty." ), QMessageBox::Ok );
        }
        else if( ledMaxCleanTime->text().toUInt() < 1 )
        {
            boCanBeSaved = false;
            QMessageBox::critical( this, tr( "Error" ), tr( "Maximum cleantime has to be greater than zero." ), QMessageBox::Ok );
        }
    }

    if( boCanBeSaved )
    {
        cDBPanel    obDBPanel;

        obDBPanel.load( m_uiPanelId );
        obDBPanel.setTitle( ledTitle->text() );
        obDBPanel.setImage( ledImagePath->text() );

        if( g_obUser.isInGroup(cAccessGroup::ADMIN) || cmbPanelGroup->isEnabled() )
        {
            obDBPanel.setPanelGroupId( cmbPanelGroup->itemData( cmbPanelGroup->currentIndex() ).toUInt() );
        }

        int hourWork    = ledWorkTimeHour->text().toInt();
        int minuteWork  = ledWorkTimeMin->text().toInt();
        int secondWork  = ledWorkTimeSec->text().toInt();
        int hourClean   = ledCleanTimeHour->text().toInt();
        int minuteClean = ledCleanTimeMin->text().toInt();
        int secondClean = ledCleanTimeSec->text().toInt();

        if( cmbPanelType->isEnabled() )
            obDBPanel.setPanelTypeId( cmbPanelType->itemData( cmbPanelType->currentIndex() ).toUInt() );
        if( ledWorkTimeHour->isEnabled() || pbWTReset->isEnabled() )
            obDBPanel.setWorkTime( hourWork*3600 + minuteWork*60 + secondWork );
        if( ledMaxWorkTime->isEnabled() )
            obDBPanel.setMaxWorkTime( ledMaxWorkTime->text().toUInt() );
        if( ledCleanTimeHour->isEnabled() || pbWTReset->isEnabled() )
            obDBPanel.setCleanTime( hourClean*3600 + minuteClean*60 + secondClean );
        if( ledMaxCleanTime->isEnabled() )
            obDBPanel.setMaxCleanTime( ledMaxCleanTime->text().toUInt() );

        obDBPanel.save();

        QDialog::accept();
    }
}

void cDlgPanelSetting::on_pbCancel_clicked()
{
    QDialog::reject();
}

void cDlgPanelSetting::on_pbTime_clicked()
{
    cDlgPanelTimes    cDlgPanelTimes( this );

    cDlgPanelTimes.exec();
}

void cDlgPanelSetting::on_pbChangeDir_clicked()
{
    QString qsFilename = QFileDialog::getOpenFileName( this, tr("Select image file"), ledImagePath->text() );

    qsFilename.replace( '\\', '/' );

    if( qsFilename.length() > 0 )
    {
        ledImagePath->setText( qsFilename );
    }
}

void cDlgPanelSetting::on_pbEmptyImage_clicked()
{
    ledImagePath->setText( "" );
}

void cDlgPanelSetting::on_pbWTReset_clicked()
{
    ledWorkTimeHour->setText( "0" );
    ledWorkTimeMin->setText( "0" );
    ledWorkTimeSec->setText( "0" );
}

void cDlgPanelSetting::on_pbCTReset_clicked()
{
    ledCleanTimeHour->setText( "0" );
    ledCleanTimeMin->setText( "0" );
    ledCleanTimeSec->setText( "0" );
}

void cDlgPanelSetting::on_pbEnableSystemAdmin_clicked()
{
    if( g_obGen.isSystemAdmin() )
    {
        cmbPanelType->setEnabled( true );
        ledWorkTimeHour->setEnabled( true );
        ledWorkTimeMin->setEnabled( true );
        ledWorkTimeSec->setEnabled( true );
        pbWTReset->setEnabled( true );
        ledMaxWorkTime->setEnabled( true );
        ledCleanTimeHour->setEnabled( true );
        ledCleanTimeMin->setEnabled( true );
        ledCleanTimeSec->setEnabled( true );
        pbCTReset->setEnabled( true );
        ledMaxCleanTime->setEnabled( true );
        cmbPanelGroup->setEnabled( true );
    }
}

void cDlgPanelSetting::on_pbEnableDeviceAdmin_clicked()
{
    if( g_obGen.isExtendedAdmin() )
    {
        pbWTReset->setEnabled( true );
        pbCTReset->setEnabled( true );
    }
}
