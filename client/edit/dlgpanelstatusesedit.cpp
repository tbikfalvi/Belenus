#include <QPushButton>
#include <QMessageBox>

#include "dlgpanelstatusesedit.h"

cDlgPanelStatusesEdit::cDlgPanelStatusesEdit( QWidget *p_poParent, cDBPanelStatuses *p_poPanelStatuses )
    : QDialog( p_poParent )
{
    setupUi( this );

    setWindowTitle( tr( "Panel status" ) );
    setWindowIcon( QIcon("./resources/40x40_device_settings.png") );

    pbOk->setIcon(     QIcon("./resources/40x40_ok.png") );
    pbCancel->setIcon( QIcon("./resources/40x40_cancel.png") );

    m_poPanelStatuses = p_poPanelStatuses;
    if( m_poPanelStatuses )
    {
        QSqlQuery *poQuery;

        ledName->setText( m_poPanelStatuses->name() );
        poQuery = g_poDB->executeQTQuery( QString( "SELECT panelTypeId, name FROM panelTypes WHERE archive<>\"DEL\" ORDER BY name" ) );
        while( poQuery->next() )
        {
            cmbPanelType->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
            if( m_poPanelStatuses->panelTypeId() == poQuery->value( 0 ) )
                cmbPanelType->setCurrentIndex( cmbPanelType->count()-1 );
        }
        sbSeqNumber->setValue( m_poPanelStatuses->sequenceNumber() );
        sbLength->setValue( m_poPanelStatuses->length() );
        poQuery = g_poDB->executeQTQuery( QString( "SELECT activateCommandId, name FROM activateCommand ORDER BY activateCommandId" ) );
        while( poQuery->next() )
        {
            cmbActivateCmd->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
            if( m_poPanelStatuses->activateCommand() == poQuery->value( 0 ) )
                cmbActivateCmd->setCurrentIndex( cmbActivateCmd->count()-1 );
        }

        chkAllowToSkip->setChecked( m_poPanelStatuses->allowedToSkip() );
        chkAllowToStop->setChecked( m_poPanelStatuses->allowedToStop() );

        int i;

        for( i = cAccessGroup::MIN + 1; i < cAccessGroup::MAX; i++ )
            if( g_obUser.isInGroup( (cAccessGroup::teAccessGroup)i ) ) cmbSkipLevel->addItem( cAccessGroup::toStr( (cAccessGroup::teAccessGroup)i ) );
        cmbSkipLevel->setCurrentIndex( m_poPanelStatuses->skipLevel() - 1 );
        cmbSkipLevel->setEnabled( m_poPanelStatuses->allowedToSkip() );

        for( i = cAccessGroup::MIN + 1; i < cAccessGroup::MAX; i++ )
            if( g_obUser.isInGroup( (cAccessGroup::teAccessGroup)i ) ) cmbStopLevel->addItem( cAccessGroup::toStr( (cAccessGroup::teAccessGroup)i ) );
        cmbStopLevel->setCurrentIndex( m_poPanelStatuses->stopLevel() - 1 );
        cmbStopLevel->setEnabled( m_poPanelStatuses->allowedToStop() );
    }
    ledName->setEnabled( g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    cmbPanelType->setEnabled( g_obUser.isInGroup( cAccessGroup::SYSTEM ) );
    sbSeqNumber->setEnabled( g_obUser.isInGroup( cAccessGroup::SYSTEM ) );
    sbLength->setEnabled( g_obUser.isInGroup( cAccessGroup::ADMIN ) );
    cmbActivateCmd->setEnabled( g_obUser.isInGroup( cAccessGroup::SYSTEM ) );
}

cDlgPanelStatusesEdit::~cDlgPanelStatusesEdit()
{
}

void cDlgPanelStatusesEdit::on_pbOk_clicked()
{
    bool  boCanBeSaved = true;

    if( (ledName->text() == "") )
    {
        boCanBeSaved = false;
        QMessageBox::critical( this, tr( "Error" ), tr( "Panelstatus name cannot be empty." ) );
    }

    if( sbLength->value() == 0 && cmbActivateCmd->currentIndex() > 0 && cmbActivateCmd->currentIndex() != 3 )
    {
        boCanBeSaved = false;
        QMessageBox::critical( this, tr( "Error" ), tr( "Length of status process time cannot be zero." ) );
    }

    if( cmbActivateCmd->currentIndex() == 0 )
    {
        chkAllowToSkip->setChecked( false );
        cmbSkipLevel->setCurrentIndex( -1 );
        chkAllowToStop->setChecked( false );
        cmbStopLevel->setCurrentIndex( -1 );
    }

    if( !chkAllowToSkip->isChecked() )
    {
        cmbSkipLevel->setCurrentIndex( -1 );
    }
    else if( cmbSkipLevel->currentIndex() == -1 )
    {
        boCanBeSaved = false;
        QMessageBox::warning( this, tr( "Warning" ), tr( "Please select user level from the dropdown list!" ) );
    }

    if( !chkAllowToStop->isChecked() )
    {
        cmbStopLevel->setCurrentIndex( -1 );
    }
    else if( cmbStopLevel->currentIndex() == -1 )
    {
        boCanBeSaved = false;
        QMessageBox::warning( this, tr( "Warning" ), tr( "Please select user level from the dropdown list!" ) );
    }

    QSqlQuery *poQuery = NULL;
    try
    {
        poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM panelStatuses WHERE archive<>\"DEL\" AND panelStatusId<>%1 AND seqNumber=%2 AND panelTypeId=%3" ).arg(m_poPanelStatuses->id()).arg(sbSeqNumber->value()).arg(cmbPanelType->itemData(cmbPanelType->currentIndex()).toInt()) );
        if( poQuery->size() > 0 )
        {
            boCanBeSaved = false;
            QMessageBox::critical( this, tr( "Error" ), tr( "Another panelstatus has the same value for sequence order.\nPlease define a different one." ) );
        }

        if( boCanBeSaved )
        {
            m_poPanelStatuses->setLicenceId( g_poPrefs->getLicenceId() );
            m_poPanelStatuses->setName( ledName->text() );
            m_poPanelStatuses->setPanelTypeId( cmbPanelType->itemData( cmbPanelType->currentIndex() ).toInt() );
            m_poPanelStatuses->setSequenceNumber( sbSeqNumber->value() );
            m_poPanelStatuses->setLength( sbLength->value() );
            m_poPanelStatuses->setActivateCommand( cmbActivateCmd->itemData( cmbActivateCmd->currentIndex() ).toInt() );
            m_poPanelStatuses->setAllowedToSkip( chkAllowToSkip->isChecked() );
            m_poPanelStatuses->setSkipLevel( (cmbSkipLevel->currentIndex() + 1) );
            m_poPanelStatuses->setAllowedToStop( chkAllowToStop->isChecked() );
            m_poPanelStatuses->setStopLevel( (cmbStopLevel->currentIndex() + 1) );
            m_poPanelStatuses->save();

            QDialog::accept();
        }
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
    }

    if( poQuery ) delete poQuery;
}

void cDlgPanelStatusesEdit::on_pbCancel_clicked()
{
    QDialog::reject();
}

void cDlgPanelStatusesEdit::on_chkAllowToSkip_clicked(bool checked)
{
    cmbSkipLevel->setEnabled( checked );
}

void cDlgPanelStatusesEdit::on_chkAllowToStop_clicked(bool checked)
{
    cmbStopLevel->setEnabled( checked );
}
