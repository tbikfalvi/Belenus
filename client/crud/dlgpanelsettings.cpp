#include <QMessageBox>

#include "belenus.h"
#include "dlgpanelsettings.h"
#include "../edit/dlgpaneluseedit.h"
#include "../db/dbpanels.h"

cDlgPanelSettings::cDlgPanelSettings( QWidget *p_poParent, unsigned int p_uiPanelId )
    : cDlgCrud( p_poParent )
{
    setWindowTitle( tr( "Panel settings" ) );
    setWindowIcon( QIcon("./resources/40x40_device.png") );

    m_poBtnClose->setIcon( QIcon("./resources/40x40_exit.png") );

    m_uiPanelId = p_uiPanelId;

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
    horizontalLayout1->addWidget( cmbPanelType );

    horizontalLayout2 = new QHBoxLayout();
    horizontalLayout2->setObjectName( QString::fromUtf8( "horizontalLayout2" ) );

    lblWorkTime = new QLabel( this );
    lblWorkTime->setObjectName( QString::fromUtf8( "lblWorkTime" ) );
    lblWorkTime->setText( tr("Work time (hh:mm:ss): ") );
    horizontalLayout2->addWidget( lblWorkTime );

    ledWorkTime = new QLineEdit( this );
    ledWorkTime->setObjectName( QString::fromUtf8( "ledWorkTime" ) );
    ledWorkTime->setMinimumWidth( 70 );
    ledWorkTime->setMaximumWidth( 70 );
    horizontalLayout2->addWidget( ledWorkTime );

    pbWTReset = new QPushButton( this );
    pbWTReset->setObjectName( QString::fromUtf8( "pbWTReset" ) );
    pbWTReset->setMinimumHeight( 30 );
    pbWTReset->setMaximumHeight( 30 );
    pbWTReset->setText( tr("Reset") );
    pbWTReset->setToolTip( tr("Reset the worktime of the device.") );
    pbWTReset->setIconSize( QSize(20,20) );
    pbWTReset->setIcon( QIcon("./resources/40x40_hourglass.png") );
    pbWTReset->setEnabled( g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    horizontalLayout2->addWidget( pbWTReset );

    lblMaxWorkTime = new QLabel( this );
    lblMaxWorkTime->setObjectName( QString::fromUtf8( "lblMaxWorkTime" ) );
    lblMaxWorkTime->setText( tr("Maximum work time (hour): ") );
    horizontalLayout2->addWidget( lblMaxWorkTime );

    ledMaxWorkTime = new QLineEdit( this );
    ledMaxWorkTime->setObjectName( QString::fromUtf8( "ledMaxWorkTime" ) );
    ledMaxWorkTime->setMinimumWidth( 50 );
    ledMaxWorkTime->setMaximumWidth( 50 );
    ledMaxWorkTime->setInputMask( "0000" );
    horizontalLayout2->addWidget( ledMaxWorkTime );

    horizontalSpacer2 = new QSpacerItem( 400, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    horizontalLayout2->addItem( horizontalSpacer2 );

    horizontalLayout3 = new QHBoxLayout();
    horizontalLayout3->setObjectName( QString::fromUtf8( "horizontalLayout3" ) );

    horizontalSpacer3 = new QSpacerItem( 400, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    horizontalLayout3->addItem( horizontalSpacer3 );

    pbCopyToAll = new QPushButton( this );
    pbCopyToAll->setObjectName( QString::fromUtf8( "pbCopyToAll" ) );
    pbCopyToAll->setMinimumHeight( 30 );
    pbCopyToAll->setMaximumHeight( 30 );
    pbCopyToAll->setText( tr("Copy to all") );
    pbCopyToAll->setToolTip( tr("Copy and assign all device usages to all other devices.") );
    pbCopyToAll->setIconSize( QSize(20,20) );
    pbCopyToAll->setIcon( QIcon("./resources/40x40_save.png") );
    pbCopyToAll->setEnabled( g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    horizontalLayout3->addWidget( pbCopyToAll );

    verticalLayout->insertLayout( 0, horizontalLayout1 );
    verticalLayout->insertLayout( 1, horizontalLayout2 );
    verticalLayout->insertLayout( 3, horizontalLayout3 );

    cmbPanelType->setEnabled( false );
    ledWorkTime->setEnabled( false );

    m_poBtnSave->setEnabled( true );
    m_poBtnSave->setVisible( true );

    connect( m_poBtnSave, SIGNAL( clicked( bool ) ), this, SLOT( saveClicked( bool ) ) );
    connect( pbWTReset, SIGNAL( clicked( bool ) ), this, SLOT( on_pbWTReset_clicked( bool ) ) );
    connect( pbCopyToAll, SIGNAL( clicked( bool ) ), this, SLOT( on_pbCopyToAll_clicked( bool ) ) );

    if( p_uiPanelId > 0 )
    {
        cDBPanel    obDBPanel;

        obDBPanel.load( m_uiPanelId );

        unsigned int hour       = obDBPanel.workTime()/3600;
        unsigned int minute     = (obDBPanel.workTime()-(hour*3600))/60;
        unsigned int second     = (obDBPanel.workTime()-(hour*3600))%60;

        QTime   workTime = QTime( hour, minute, second, 0 );

        ledTitle->setText( obDBPanel.title() );
        ledWorkTime->setText( workTime.toString("hh:mm:ss") );
        ledMaxWorkTime->setText( QString::number(obDBPanel.maxWorkTime()) );

        QSqlQuery *poQueryType;
        poQueryType = g_poDB->executeQTQuery( QString( "SELECT panelTypeId, name FROM panelTypes WHERE active=1" ) );
        while( poQueryType->next() )
        {
            cmbPanelType->addItem( poQueryType->value( 1 ).toString(), poQueryType->value( 0 ) );
            if( poQueryType->value( 0 ).toUInt() == obDBPanel.panelTypeId() )
                cmbPanelType->setCurrentIndex( cmbPanelType->count()-1 );
        }
        if( poQueryType ) delete poQueryType;
    }

    setupTableView();

}

cDlgPanelSettings::~cDlgPanelSettings()
{

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
        m_qsQuery = QString("SELECT panelUseId, licenceId, name, useTime, usePrice, archive FROM panelUses WHERE panelId=%1").arg( m_uiPanelId );
    }
    else
    {
        m_qsQuery = QString("SELECT panelUseId AS id, name, useTime, usePrice FROM panelUses WHERE panelId=%1").arg( m_uiPanelId );
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

    cDlgPanelUseEdit  obDlgEdit( this, poPanelUse, m_uiPanelId );
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

        cDlgPanelUseEdit  obDlgEdit( this, poPanelUse, m_uiPanelId );
        obDlgEdit.setWindowTitle( "Edit paneluse" );
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

    if( boCanBeSaved )
    {
        QTime   workTime = QTime::fromString(ledWorkTime->text(),"hh:mm:ss");

        cDBPanel    obDBPanel;

        obDBPanel.load( m_uiPanelId );
        obDBPanel.setTitle( ledTitle->text() );
        obDBPanel.setWorkTime( workTime.hour()*3600 + workTime.minute()*60 + workTime.second() );
        obDBPanel.setMaxWorkTime( ledMaxWorkTime->text().toUInt() );
        obDBPanel.save();

        QDialog::accept();
    }
}

void cDlgPanelSettings::on_pbWTReset_clicked( bool )
{
    ledWorkTime->setText( "00:00:00" );
}

void cDlgPanelSettings::on_pbCopyToAll_clicked( bool )
{
    for( unsigned int i=1; i<g_poPrefs->getPanelCount()+1; i++ )
    {
        if( i != m_uiPanelId )
        {
            QString qsQuery = QString( "INSERT INTO panelUses (licenceId, panelId, name, useTime, usePrice, active, archive) "
                                       "SELECT panelUses.licenceId, %1, panelUses.name, panelUses.useTime, panelUses.usePrice, 1, \"NEW\" "
                                       "FROM panelUses, panels WHERE panelUses.panelId=panels.panelId AND panels.panelId=%2" ).arg(i).arg(m_uiPanelId);

            QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
            if( poQuery ) delete poQuery;
        }
    }
    QMessageBox::information( this, tr("Information"),
                              tr("Device usage copy process finished.") );
}
