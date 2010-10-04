//====================================================================================
//
// Belenus Kliens alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dlgpaneluseselect.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#include "belenus.h"
#include "dlgpaneluseselect.h"
#include "../db/dbpaneluses.h"

cDlgPanelUseSelect::cDlgPanelUseSelect( QWidget *p_poParent, unsigned int p_uiPanelId, int p_inPanelUseTime )
    : cDlgPanelSettings( p_poParent, p_uiPanelId )
{
    setWindowTitle( tr( "Select paneluse" ) );
    setWindowIcon( QIcon("./resources/40x40_device_withtime.png") );

    m_inPanelUseTime = p_inPanelUseTime;

    lblTitle->setVisible(false);
    ledTitle->setEnabled( false );
    ledTitle->setVisible(false);
    lblType->setVisible(false);
    cmbPanelType->setEnabled( false );
    cmbPanelType->setVisible(false);
    lblWorkTime->setVisible(false);
    ledWorkTime->setEnabled( false );
    ledWorkTime->setVisible(false);
    lblMaxWorkTime->setEnabled(false);
    lblMaxWorkTime->setVisible(false);
    ledMaxWorkTime->setEnabled(false);
    ledMaxWorkTime->setVisible(false);
    pbCopyToAll->setEnabled(false);
    pbCopyToAll->setVisible(false);

    pbWTReset->setEnabled(false);
    m_poBtnClose->setEnabled(false);
    m_poBtnDelete->setEnabled(false);
    m_poBtnEdit->setEnabled(false);
    m_poBtnNew->setEnabled(false);
    m_poBtnSave->setEnabled( false );

    pbWTReset->setVisible(false);
    m_poBtnClose->setVisible(false);
    m_poBtnDelete->setVisible(false);
    m_poBtnEdit->setVisible(false);
    m_poBtnNew->setVisible(false);
    m_poBtnSave->setVisible( false );

    pbSelect = new QPushButton( tr( "Select" ), this );
    pbSelect->setObjectName( QString::fromUtf8( "pbSelect" ) );
    pbSelect->setIconSize( QSize(20, 20) );
    pbSelect->setIcon( QIcon("./resources/40x40_ok.png") );
    btbButtonsSide->addButton( pbSelect, QDialogButtonBox::ActionRole );
    pbCancel = new QPushButton( tr( "Cancel" ), this );
    pbCancel->setObjectName( QString::fromUtf8( "pbCancel" ) );
    pbCancel->setIconSize( QSize(20, 20) );
    pbCancel->setIcon( QIcon("./resources/40x40_cancel.png") );
    btbButtonsSide->addButton( pbCancel, QDialogButtonBox::RejectRole );

    setupTableView();

    connect( pbSelect, SIGNAL(clicked(bool)), this, SLOT(on_pbSelect_clicked()) );
}

cDlgPanelUseSelect::~cDlgPanelUseSelect()
{

}

int cDlgPanelUseSelect::getPanelUsePrice()
{
    return m_inPanelUsePrice;
}

void cDlgPanelUseSelect::refreshTable()
{
    cTracer obTracer( "cDlgPanelUseSelect::refreshTable" );

    if( g_obUser.isInGroup( cAccessGroup::ROOT ) )
    {
        m_qsQuery = QString("SELECT panelUseId, licenceId, name, useTime, usePrice, archive FROM panelUses WHERE panelId=%1").arg( m_uiPanelId );
    }
    else
    {
        m_qsQuery = QString("SELECT panelUseId AS id, name, useTime, usePrice FROM panelUses WHERE panelId=%1").arg( m_uiPanelId );
    }

    if( m_inPanelUseTime > 0 )
    {
        m_qsQuery += " AND ";
        m_qsQuery += QString( "useTime=%1" ).arg( m_inPanelUseTime );
    }

    cDlgCrud::refreshTable();
}

void cDlgPanelUseSelect::enableButtons()
{
    cTracer obTracer( "cDlgPanelUseSelect::enableButtons" );

    if( m_uiSelectedId )
    {
        pbSelect->setEnabled( true );
    }
    else
    {
        pbSelect->setEnabled( false );
    }
}

void cDlgPanelUseSelect::on_pbSelect_clicked()
{
    cDBPanelUses    obDBPanelUses;

    obDBPanelUses.load( m_uiSelectedId );
    m_inPanelUsePrice = obDBPanelUses.usePrice();

    QDialog::accept();
}
