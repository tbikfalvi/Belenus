
#include <QMessageBox>
#include <QFileDialog>

#include "belenus.h"
#include "dlgpanelsettings.h"
#include "../edit/dlgpaneluseedit.h"
#include "../db/dbpanels.h"
#include "../dlg/dlgpaneltimecopy.h"

cDlgPanelSettings::cDlgPanelSettings( QWidget *p_poParent, unsigned int p_uiPanelId ) : cDlgCrud( p_poParent )
{
    setWindowTitle( tr( "Panel settings" ) );
    setWindowIcon( QIcon("./resources/40x40_device.png") );

    m_poBtnClose->setIcon( QIcon("./resources/40x40_exit.png") );

    m_uiPanelId         = p_uiPanelId;
    m_bIsSettingChanged = false;

    addTitleLayout();
    addImageLayout();
    addWorkTimeLayout();
    addCleanTimeLayout();
    addCopyTimeValuesLayout();

    verticalLayout->insertLayout( 0, horizontalLayout1 );
    verticalLayout->insertLayout( 1, horizontalLayout5 );
    verticalLayout->insertLayout( 2, horizontalLayout2 );
    verticalLayout->insertLayout( 3, horizontalLayout4 );
    verticalLayout->insertLayout( 5, horizontalLayout3 );

    m_poBtnSave->setEnabled( true );
    m_poBtnSave->setVisible( true );

    connect( m_poBtnSave, SIGNAL( clicked( bool ) ), this, SLOT( saveClicked( bool ) ) );
    connect( pbWTReset, SIGNAL( clicked( bool ) ), this, SLOT( on_pbWTReset_clicked( bool ) ) );
    connect( pbCTReset, SIGNAL( clicked( bool ) ), this, SLOT( on_pbCTReset_clicked( bool ) ) );
    connect( pbCopyToAll, SIGNAL( clicked( bool ) ), this, SLOT( on_pbCopyToAll_clicked( bool ) ) );
    connect( pbEnableSystemAdmin, SIGNAL(clicked()), this, SLOT(on_pbEnableSystemAdmin_clicked()) );
    connect( pbEnableDeviceAdmin, SIGNAL(clicked()), this, SLOT(on_pbEnableDeviceAdmin_clicked()) );
    connect( pbChangeDir, SIGNAL( clicked( bool ) ), this, SLOT( on_pbChangeDir_clicked( bool ) ) );
    connect( pbEmptyImage, SIGNAL( clicked( bool ) ), this, SLOT( on_pbEmptyImage_clicked( bool ) ) );

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

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "ListPanelSettings", QPoint(600,300) );
    resize( qpDlgSize.x(), qpDlgSize.y() );

    setupTableView();
}

void cDlgPanelSettings::addTitleLayout()
{
    horizontalLayout1 = new QHBoxLayout();
    horizontalLayout1->setObjectName( QString::fromUtf8( "horizontalLayout1" ) );

    lblTitle = new QLabel( this );
    lblTitle->setObjectName( QString::fromUtf8( "lblTitle" ) );
    lblTitle->setText( tr("Title: ") );
    horizontalLayout1->addWidget( lblTitle );

    ledTitle = new QLineEdit( this );
    ledTitle->setObjectName( QString::fromUtf8( "ledTitle" ) );
    horizontalLayout1->addWidget( ledTitle );

    lblType = new QLabel( this );
    lblType->setObjectName( QString::fromUtf8( "lblType" ) );
    lblType->setText( tr("Type: ") );
    horizontalLayout1->addWidget( lblType );

    cmbPanelType = new QComboBox( this );
    cmbPanelType->setObjectName( QString::fromUtf8( "cmbPanelType" ) );
    cmbPanelType->setEnabled( g_obUser.isInGroup( cAccessGroup::SYSTEM ) );
    horizontalLayout1->addWidget( cmbPanelType );

    lblGroup = new QLabel( this );
    lblGroup->setObjectName( QString::fromUtf8( "lblGroup" ) );
    lblGroup->setText( tr("Group: ") );
    horizontalLayout1->addWidget( lblGroup );

    cmbPanelGroup = new QComboBox( this );
    cmbPanelGroup->setObjectName( QString::fromUtf8( "cmbPanelGroup" ) );
    cmbPanelGroup->setEnabled( g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    horizontalLayout1->addWidget( cmbPanelGroup );
}

void cDlgPanelSettings::addImageLayout()
{
    horizontalLayout5 = new QHBoxLayout();
    horizontalLayout5->setObjectName( QString::fromUtf8( "horizontalLayout5" ) );

    lblImagePath = new QLabel( this );
    lblImagePath->setObjectName( QString::fromUtf8( "lblImagePath" ) );
    lblImagePath->setText( tr("Image path: ") );
    horizontalLayout5->addWidget( lblImagePath );

    ledImagePath = new QLineEdit( this );
    ledImagePath->setObjectName( QString::fromUtf8( "ledImagePath" ) );
    ledImagePath->setReadOnly( true );
    horizontalLayout5->addWidget( ledImagePath );

    pbChangeDir = new QPushButton( this );
    pbChangeDir->setObjectName( QString::fromUtf8( "pbChangeDir" ) );
    pbChangeDir->setMinimumHeight( 30 );
    pbChangeDir->setMaximumHeight( 30 );
    pbChangeDir->setText( "..." );
    pbChangeDir->setToolTip( tr("Change directory and filename of panel image.") );
    pbChangeDir->setEnabled( g_obUser.isInGroup( cAccessGroup::SYSTEM ) );
    horizontalLayout5->addWidget( pbChangeDir );

    pbEmptyImage = new QPushButton( this );
    pbEmptyImage->setObjectName( QString::fromUtf8( "pbEmptyImage" ) );
    pbEmptyImage->setMinimumHeight( 30 );
    pbEmptyImage->setMaximumHeight( 30 );
    pbEmptyImage->setIconSize( QSize(20, 20) );
    pbEmptyImage->setIcon( QIcon("./resources/40x40_delete.png") );
    pbEmptyImage->setText( tr("Remove image") );
    pbEmptyImage->setToolTip( tr("Remove image from panel.") );
    pbEmptyImage->setEnabled( g_obUser.isInGroup( cAccessGroup::SYSTEM ) );
    horizontalLayout5->addWidget( pbEmptyImage );

    horizontalSpacer5 = new QSpacerItem( 300, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    horizontalLayout5->addItem( horizontalSpacer5 );
}

void cDlgPanelSettings::addWorkTimeLayout()
{
    horizontalLayout2 = new QHBoxLayout();
    horizontalLayout2->setObjectName( QString::fromUtf8( "horizontalLayout2" ) );

    lblWorkTime = new QLabel( this );
    lblWorkTime->setObjectName( QString::fromUtf8( "lblWorkTime" ) );
    lblWorkTime->setText( tr("Work time (hh:mm:ss): ") );
    horizontalLayout2->addWidget( lblWorkTime );

    ledWorkTimeHour = new QLineEdit( this );
    ledWorkTimeHour->setObjectName( QString::fromUtf8( "ledWorkTimeHour" ) );
    ledWorkTimeHour->setMinimumWidth( 35 );
    ledWorkTimeHour->setMaximumWidth( 35 );
    ledWorkTimeHour->setAlignment( Qt::AlignRight );
    ledWorkTimeHour->setEnabled( g_obUser.isInGroup( cAccessGroup::SYSTEM ) );
    horizontalLayout2->addWidget( ledWorkTimeHour );

    ledWorkTimeMin = new QLineEdit( this );
    ledWorkTimeMin->setObjectName( QString::fromUtf8( "ledWorkTimeMin" ) );
    ledWorkTimeMin->setMinimumWidth( 20 );
    ledWorkTimeMin->setMaximumWidth( 20 );
    ledWorkTimeMin->setAlignment( Qt::AlignCenter );
    ledWorkTimeMin->setEnabled( g_obUser.isInGroup( cAccessGroup::SYSTEM ) );
    horizontalLayout2->addWidget( ledWorkTimeMin );

    ledWorkTimeSec = new QLineEdit( this );
    ledWorkTimeSec->setObjectName( QString::fromUtf8( "ledWorkTimeSec" ) );
    ledWorkTimeSec->setMinimumWidth( 20 );
    ledWorkTimeSec->setMaximumWidth( 20 );
    ledWorkTimeSec->setAlignment( Qt::AlignCenter );
    ledWorkTimeSec->setEnabled( g_obUser.isInGroup( cAccessGroup::SYSTEM ) );
    horizontalLayout2->addWidget( ledWorkTimeSec );

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

    pbWTReset = new QPushButton( this );
    pbWTReset->setObjectName( QString::fromUtf8( "pbWTReset" ) );
    pbWTReset->setMinimumHeight( 30 );
    pbWTReset->setMaximumHeight( 30 );
    pbWTReset->setText( tr("Reset") );
    pbWTReset->setToolTip( tr("Reset the worktime of the device.") );
    pbWTReset->setIconSize( QSize(20,20) );
    pbWTReset->setIcon( QIcon("./resources/40x40_hourglass.png") );
    pbWTReset->setEnabled( g_obUser.isInGroup( cAccessGroup::SYSTEM ) );
    horizontalLayout2->addWidget( pbWTReset );

    pbEnableDeviceAdmin = new QPushButton( this );
    pbEnableDeviceAdmin->setObjectName( QString::fromUtf8( "pbEnableDeviceAdmin" ) );
    pbEnableDeviceAdmin->setMinimumWidth( 30 );
    pbEnableDeviceAdmin->setMaximumWidth( 30 );
    pbEnableDeviceAdmin->setMinimumHeight( 30 );
    pbEnableDeviceAdmin->setMaximumHeight( 30 );
    pbEnableDeviceAdmin->setText( "" );
    pbEnableDeviceAdmin->setToolTip( tr("Enable to reset work time for Device Administrator.") );
    pbEnableDeviceAdmin->setIconSize( QSize(20,20) );
    pbEnableDeviceAdmin->setIcon( QIcon("./resources/40x40_key.png") );
    pbEnableDeviceAdmin->setEnabled( true );
    horizontalLayout2->addWidget( pbEnableDeviceAdmin );

    lblMaxWorkTime = new QLabel( this );
    lblMaxWorkTime->setObjectName( QString::fromUtf8( "lblMaxWorkTime" ) );
    lblMaxWorkTime->setText( tr("Maximum work time (hour): ") );
    horizontalLayout2->addWidget( lblMaxWorkTime );

    ledMaxWorkTime = new QLineEdit( this );
    ledMaxWorkTime->setObjectName( QString::fromUtf8( "ledMaxWorkTime" ) );
    ledMaxWorkTime->setMinimumWidth( 50 );
    ledMaxWorkTime->setMaximumWidth( 50 );
    ledMaxWorkTime->setEnabled( g_obUser.isInGroup( cAccessGroup::SYSTEM ) );
    horizontalLayout2->addWidget( ledMaxWorkTime );

    horizontalSpacer2 = new QSpacerItem( 300, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    horizontalLayout2->addItem( horizontalSpacer2 );
}

void cDlgPanelSettings::addCleanTimeLayout()
{
    horizontalLayout4 = new QHBoxLayout();
    horizontalLayout4->setObjectName( QString::fromUtf8( "horizontalLayout4" ) );

    lblCleanTime = new QLabel( this );
    lblCleanTime->setObjectName( QString::fromUtf8( "lblCleanTime" ) );
    lblCleanTime->setText( tr("Clean time (hh:mm:ss): ") );
    horizontalLayout4->addWidget( lblCleanTime );

    ledCleanTimeHour = new QLineEdit( this );
    ledCleanTimeHour->setObjectName( QString::fromUtf8( "ledCleanTimeHour" ) );
    ledCleanTimeHour->setMinimumWidth( 35 );
    ledCleanTimeHour->setMaximumWidth( 35 );
    ledCleanTimeHour->setAlignment( Qt::AlignRight );
    ledCleanTimeHour->setEnabled( g_obUser.isInGroup( cAccessGroup::SYSTEM ) );
    horizontalLayout4->addWidget( ledCleanTimeHour );

    ledCleanTimeMin = new QLineEdit( this );
    ledCleanTimeMin->setObjectName( QString::fromUtf8( "ledCleanTimeMin" ) );
    ledCleanTimeMin->setMinimumWidth( 20 );
    ledCleanTimeMin->setMaximumWidth( 20 );
    ledCleanTimeMin->setAlignment( Qt::AlignCenter );
    ledCleanTimeMin->setEnabled( g_obUser.isInGroup( cAccessGroup::SYSTEM ) );
    horizontalLayout4->addWidget( ledCleanTimeMin );

    ledCleanTimeSec = new QLineEdit( this );
    ledCleanTimeSec->setObjectName( QString::fromUtf8( "ledCleanTimeSec" ) );
    ledCleanTimeSec->setMinimumWidth( 20 );
    ledCleanTimeSec->setMaximumWidth( 20 );
    ledCleanTimeSec->setAlignment( Qt::AlignCenter );
    ledCleanTimeSec->setEnabled( g_obUser.isInGroup( cAccessGroup::SYSTEM ) );
    horizontalLayout4->addWidget( ledCleanTimeSec );

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

    pbCTReset = new QPushButton( this );
    pbCTReset->setObjectName( QString::fromUtf8( "pbCTReset" ) );
    pbCTReset->setMinimumHeight( 30 );
    pbCTReset->setMaximumHeight( 30 );
    pbCTReset->setText( tr("Reset") );
    pbCTReset->setToolTip( tr("Reset the cleantime of the device.") );
    pbCTReset->setIconSize( QSize(20,20) );
    pbCTReset->setIcon( QIcon("./resources/40x40_hourglass.png") );
    pbCTReset->setEnabled( g_obUser.isInGroup( cAccessGroup::SYSTEM ) );
    horizontalLayout4->addWidget( pbCTReset );

    lblMaxCleanTime = new QLabel( this );
    lblMaxCleanTime->setObjectName( QString::fromUtf8( "lblMaxCleanTime" ) );
    lblMaxCleanTime->setText( tr("Maximum clean time (hour): ") );
    horizontalLayout4->addWidget( lblMaxCleanTime );

    ledMaxCleanTime = new QLineEdit( this );
    ledMaxCleanTime->setObjectName( QString::fromUtf8( "ledMaxCleanTime" ) );
    ledMaxCleanTime->setMinimumWidth( 50 );
    ledMaxCleanTime->setMaximumWidth( 50 );
    ledMaxCleanTime->setEnabled( g_obUser.isInGroup( cAccessGroup::SYSTEM ) );
    horizontalLayout4->addWidget( ledMaxCleanTime );

    horizontalSpacer4 = new QSpacerItem( 300, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    horizontalLayout4->addItem( horizontalSpacer4 );
}

void cDlgPanelSettings::addCopyTimeValuesLayout()
{
    horizontalLayout3 = new QHBoxLayout();
    horizontalLayout3->setObjectName( QString::fromUtf8( "horizontalLayout3" ) );

    horizontalSpacer3 = new QSpacerItem( 400, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    horizontalLayout3->addItem( horizontalSpacer3 );

    pbCopyToAll = new QPushButton( this );
    pbCopyToAll->setObjectName( QString::fromUtf8( "pbCopyToAll" ) );
    pbCopyToAll->setMinimumHeight( 30 );
    pbCopyToAll->setMaximumHeight( 30 );
    pbCopyToAll->setText( tr(" Copy to all ") );
    pbCopyToAll->setToolTip( tr("Copy and assign all device usages to all other devices.") );
    pbCopyToAll->setIconSize( QSize(20,20) );
    pbCopyToAll->setIcon( QIcon("./resources/40x40_save.png") );
    pbCopyToAll->setEnabled( g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    horizontalLayout3->addWidget( pbCopyToAll );

    pbEnableSystemAdmin = new QPushButton( this );
    pbEnableSystemAdmin->setObjectName( QString::fromUtf8( "pbEnableSystemAdmin" ) );
    pbEnableSystemAdmin->setMinimumWidth( 30 );
    pbEnableSystemAdmin->setMaximumWidth( 30 );
    pbEnableSystemAdmin->setMinimumHeight( 30 );
    pbEnableSystemAdmin->setMaximumHeight( 30 );
    pbEnableSystemAdmin->setText( "" );
    pbEnableSystemAdmin->setToolTip( tr("Enable temporary controls can be used by System Administrator.") );
    pbEnableSystemAdmin->setIconSize( QSize(20,20) );
    pbEnableSystemAdmin->setIcon( QIcon("./resources/40x40_key.png") );
    pbEnableSystemAdmin->setEnabled( true );
    horizontalLayout3->addWidget( pbEnableSystemAdmin );
}

cDlgPanelSettings::~cDlgPanelSettings()
{
    g_poPrefs->setDialogSize( "ListPanelSettings", QPoint( width(), height() ) );
}

void cDlgPanelSettings::setupTableView()
{
    cTracer obTracer( "cDlgPanelSettings::setupTableView" );

    refreshTable();

    cDlgCrud::setupTableView();

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_poModel->setHeaderData( 0, Qt::Horizontal, tr( "Id" ) );
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "LicenceId" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Name" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Time length" ) );
        m_poModel->setHeaderData( 4, Qt::Horizontal, tr( "Price" ) );
        m_poModel->setHeaderData( 5, Qt::Horizontal, tr( "Archive" ) );

        tbvCrud->resizeColumnToContents( 0 );
        tbvCrud->resizeColumnToContents( 1 );
        tbvCrud->resizeColumnToContents( 2 );
        tbvCrud->resizeColumnToContents( 3 );
        tbvCrud->resizeColumnToContents( 4 );
        tbvCrud->resizeColumnToContents( 5 );

        tbvCrud->sortByColumn( 2, Qt::AscendingOrder );
    }
    else
    {
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "Name" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Time length" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Price" ) );

        tbvCrud->resizeColumnToContents( 1 );
        tbvCrud->resizeColumnToContents( 2 );
        tbvCrud->resizeColumnToContents( 3 );

        tbvCrud->sortByColumn( 2, Qt::AscendingOrder );
    }
}

void cDlgPanelSettings::refreshTable()
{
    cTracer obTracer( "cDlgPanelSettings::refreshTable" );

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_qsQuery = QString("SELECT panelUseId, licenceId, name, useTime, (usePrice/100) as usePrice, archive FROM panelUses WHERE panelId=%1 AND active=1").arg( m_uiPanelId );
    }
    else
    {
        m_qsQuery = QString("SELECT panelUseId AS id, name, useTime, (usePrice/100) as usePrice FROM panelUses WHERE panelId=%1 AND active=1").arg( m_uiPanelId );
    }

    cDlgCrud::refreshTable();
}

void cDlgPanelSettings::enableButtons()
{
    cTracer obTracer( "cDlgPanelSettings::enableButtons" );

    m_poBtnNew->setEnabled( g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    m_poBtnEdit->setEnabled( m_uiSelectedId > 0 && g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    m_poBtnDelete->setEnabled( m_uiSelectedId > 0 && g_obUser.isInGroup( cAccessGroup::ADMIN ) );
}

void cDlgPanelSettings::newClicked( bool )
{
    cDBPanelUses *poPanelUse = new cDBPanelUses;
    poPanelUse->createNew();

    cDlgPanelUseEdit  obDlgEdit( this, poPanelUse );
    obDlgEdit.setWindowTitle( tr( "New paneluse" ) );
    if( obDlgEdit.exec() == QDialog::Accepted )
    {
        m_uiSelectedId = poPanelUse->id();
        refreshTable();
    }

    delete poPanelUse;
}

void cDlgPanelSettings::editClicked( bool )
{
    cDBPanelUses  *poPanelUse = NULL;

    try
    {
        poPanelUse = new cDBPanelUses;
        poPanelUse->load( m_uiSelectedId );

        cDlgPanelUseEdit  obDlgEdit( this, poPanelUse );
        obDlgEdit.setWindowTitle( tr("Edit paneluse") );
        if( obDlgEdit.exec() == QDialog::Accepted )
        {
            refreshTable();
        }

        if( poPanelUse ) delete poPanelUse;
    }
    catch( cSevException &e )
    {
        if( poPanelUse ) delete poPanelUse;
        g_obLogger(e.severity()) << e.what() << EOM;
        g_obGen.showTrayError( e.what() );
    }
}

void cDlgPanelSettings::deleteClicked( bool )
{
    cDBPanelUses  *poPanelUse = NULL;

    if( QMessageBox::question( this, tr( "Question" ),
                               tr( "Are you sure you want to delete this paneluse?" ),
                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
    {
        try
        {
            poPanelUse = new cDBPanelUses;
            poPanelUse->load( m_uiSelectedId );
            poPanelUse->remove();
            m_uiSelectedId = 0;
            refreshTable();
            if( poPanelUse ) delete poPanelUse;
        }
        catch( cSevException &e )
        {
            if( poPanelUse ) delete poPanelUse;
            g_obLogger(e.severity()) << e.what() << EOM;
            g_obGen.showTrayError( e.what() );
        }
    }
}

void cDlgPanelSettings::saveClicked( bool )
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

void cDlgPanelSettings::on_pbChangeDir_clicked(bool)
{
    QString qsFilename = QFileDialog::getOpenFileName( this, tr("Select image file"), ledImagePath->text() );

    qsFilename.replace( '\\', '/' );

    if( qsFilename.length() > 0 )
    {
        ledImagePath->setText( qsFilename );
    }
}

void cDlgPanelSettings::on_pbEmptyImage_clicked(bool)
{
    ledImagePath->setText( "" );
}

void cDlgPanelSettings::on_pbWTReset_clicked( bool )
{
    ledWorkTimeHour->setText( "0" );
    ledWorkTimeMin->setText( "0" );
    ledWorkTimeSec->setText( "0" );
    m_bIsSettingChanged = true;
}

void cDlgPanelSettings::on_pbCTReset_clicked( bool )
{
    ledCleanTimeHour->setText( "0" );
    ledCleanTimeMin->setText( "0" );
    ledCleanTimeSec->setText( "0" );
    m_bIsSettingChanged = true;
}

void cDlgPanelSettings::on_pbCopyToAll_clicked( bool )
{
    cDlgPanelTypeCopy   obDlgPanelTypeCopy( this, m_uiPanelId );

    obDlgPanelTypeCopy.exec();
}

void cDlgPanelSettings::on_pbEnableSystemAdmin_clicked()
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
        pbCopyToAll->setEnabled( true );
    }
}

void cDlgPanelSettings::on_pbEnableDeviceAdmin_clicked()
{
    if( g_obGen.isExtendedAdmin() )
    {
        pbWTReset->setEnabled( true );
        pbCTReset->setEnabled( true );
    }
}
