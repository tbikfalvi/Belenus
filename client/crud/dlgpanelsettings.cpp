#include <QMessageBox>

#include "belenus.h"
#include "dlgpanelsettings.h"
#include "../edit/dlgpaneluseedit.h"

cDlgPanelSettings::cDlgPanelSettings( QWidget *p_poParent, unsigned int p_uiPanelId )
    : cDlgCrud( p_poParent )
{
    setWindowTitle( tr( "Panel settings" ) );
    setWindowIcon( QIcon("./resources/40x40_device.gif") );

    m_poBtnClose->setIcon( QIcon("./resources/40x40_cancel.gif") );

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

//    horizontalSpacer1 = new QSpacerItem( 5, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
//    horizontalLayout1->addItem( horizontalSpacer1 );

    horizontalLayout2 = new QHBoxLayout();
    horizontalLayout2->setObjectName( QString::fromUtf8( "horizontalLayout2" ) );

    lblWorkTime = new QLabel( this );
    lblWorkTime->setObjectName( QString::fromUtf8( "lblWorkTime" ) );
    lblWorkTime->setText( tr("Work time: ") );
    horizontalLayout2->addWidget( lblWorkTime );

    ledWorkTime = new QLineEdit( this );
    ledWorkTime->setObjectName( QString::fromUtf8( "ledWorkTime" ) );
    horizontalLayout2->addWidget( ledWorkTime );

    horizontalSpacer2 = new QSpacerItem( 400, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    horizontalLayout2->addItem( horizontalSpacer2 );

    verticalLayout->insertLayout( 0, horizontalLayout1 );
    verticalLayout->insertLayout( 1, horizontalLayout2 );

    cmbPanelType->setEnabled( false );
    ledWorkTime->setEnabled( false );

    m_poBtnSave->setEnabled( true );
    m_poBtnSave->setVisible( true );
    m_poBtnClose->setText( tr("&Cancel") );

    connect( m_poBtnSave, SIGNAL( clicked( bool ) ), this, SLOT( saveClicked( bool ) ) );

    if( p_uiPanelId > 0 )
    {
        QSqlQuery *poQuery;

        poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM panels WHERE panelId=%1" ).arg(p_uiPanelId) );
        if( poQuery->first() )
        {
            ledTitle->setText( poQuery->value( 3 ).toString() );
            ledWorkTime->setText( poQuery->value( 4 ).toString() );

            QSqlQuery *poQueryType;
            poQueryType = g_poDB->executeQTQuery( QString( "SELECT panelTypeId, name FROM panelTypes WHERE active=1" ) );
            while( poQueryType->next() )
            {
                cmbPanelType->addItem( poQueryType->value( 1 ).toString(), poQueryType->value( 0 ) );
                if( poQueryType->value( 0 ).toUInt() == poQuery->value( 2 ).toUInt() )
                    cmbPanelType->setCurrentIndex( cmbPanelType->count()-1 );
            }
            if( poQueryType ) delete poQueryType;
        }
        if( poQuery ) delete poQuery;
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
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Time length" ) );
        m_poModel->setHeaderData( 3, Qt::Horizontal, tr( "Price" ) );
        m_poModel->setHeaderData( 4, Qt::Horizontal, tr( "Archive" ) );
    }
    else
    {
        m_poModel->setHeaderData( 1, Qt::Horizontal, tr( "Time length" ) );
        m_poModel->setHeaderData( 2, Qt::Horizontal, tr( "Price" ) );
    }
}

void cDlgPanelSettings::refreshTable()
{
    cTracer obTracer( "cDlgPanelSettings::refreshTable" );

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_qsQuery = QString("SELECT panelUseId, licenceId, useTime, usePrice, archive FROM panelUses WHERE panelId=%1").arg( m_uiPanelId );
    }
    else
    {
        m_qsQuery = QString("SELECT panelUseId AS id, useTime, usePrice FROM panelUses WHERE panelId=%1").arg( m_uiPanelId );
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

        g_obLogger << e.severity();
        g_obLogger << e.what() << cQTLogger::EOM;
    }
}

void cDlgPanelSettings::deleteClicked( bool )
{
    cDBPanelUses  *poPanelUse = NULL;

    if( QMessageBox::question( this, tr( "Confirmation" ),
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

            g_obLogger << e.severity();
            g_obLogger << e.what() << cQTLogger::EOM;
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

    if( boCanBeSaved )
    {
        QString  qsQuery;

        qsQuery = "UPDATE panels SET ";

        qsQuery += QString( "title = \"%1\", " ).arg( ledTitle->text() );
        qsQuery += QString( "archive = \"%1\" " ).arg( "MOD" );
        qsQuery += QString( " WHERE panelId = %1" ).arg( m_uiPanelId );

        QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );

        if( poQuery ) delete poQuery;

        QDialog::accept();
    }
}
