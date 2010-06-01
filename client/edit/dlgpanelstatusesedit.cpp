#include <QPushButton>
#include <QMessageBox>

#include "dlgpanelstatusesedit.h"

cDlgPanelStatusesEdit::cDlgPanelStatusesEdit( QWidget *p_poParent, cDBPanelStatuses *p_poPanelStatuses )
    : QDialog( p_poParent )
{
    setupUi( this );

    pbOk->setIcon(     QIcon("./resources/40x40_ok.gif") );
    pbCancel->setIcon( QIcon("./resources/40x40_cancel.gif") );
    checkIndependent->setEnabled( false );
    checkIndependent->setVisible( false );

    m_poPanelStatuses = p_poPanelStatuses;
    if( m_poPanelStatuses )
    {
        QSqlQuery *poQuery;

        ledName->setText( QString::fromStdString( m_poPanelStatuses->name() ) );
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

        /*if( m_poPanelStatuses->licenceId() == 0 && m_poPanelStatuses->id() > 0 )
            checkIndependent->setChecked( true );

        if( !g_obUser.isInGroup( "root" ) && !g_obUser.isInGroup( "system" ) )
        {
            checkIndependent->setEnabled( false );
            if( m_poPanelStatuses->licenceId() == 0 && m_poPanelStatuses->id() > 0 )
            {
                gbIdentification->setEnabled( false );
                gbProcess->setEnabled( false );
                pbOk->setEnabled( false );
            }
        }*/
    }
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

    if( sbLength->value() == 0 && cmbActivateCmd->currentIndex() > 0 )
    {
        boCanBeSaved = false;
        QMessageBox::critical( this, tr( "Error" ), tr( "Length of status process time cannot be zero." ) );
    }

    QSqlQuery *poQuery = NULL;
    try
    {
        poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM panelStatuses WHERE archive<>\"DEL\" AND panelStatusId<>%1 AND seqNumber=%2" ).arg(m_poPanelStatuses->id()).arg(sbSeqNumber->value()) );
        if( poQuery->size() > 0 )
        {
            boCanBeSaved = false;
            QMessageBox::critical( this, tr( "Error" ), tr( "Another panelstatus has the same value for sequence order.\nPlease define a different one." ) );
        }

        if( boCanBeSaved )
        {
            m_poPanelStatuses->setLicenceId( g_poPrefs->getLicenceId() );
            m_poPanelStatuses->setName( ledName->text().toStdString() );
            m_poPanelStatuses->setPanelTypeId( cmbPanelType->itemData( cmbPanelType->currentIndex() ).toInt() );
            m_poPanelStatuses->setSequenceNumber( sbSeqNumber->value() );
            m_poPanelStatuses->setLength( sbLength->value() );
            m_poPanelStatuses->setActivateCommand( cmbActivateCmd->itemData( cmbActivateCmd->currentIndex() ).toInt() );

            /*if( checkIndependent->isChecked() )
            {
                m_poPanelStatuses->setLicenceId( 0 );
            }
            else
            {
                m_poPanelStatuses->setLicenceId( g_poPrefs->getLicenceId() );
            }*/
            m_poPanelStatuses->save();

            QDialog::accept();
        }
    }
    catch( cSevException &e )
    {
        g_obLogger << e.severity() << e.what() << cQTLogger::EOM;
    }

    if( poQuery ) delete poQuery;
}

void cDlgPanelStatusesEdit::on_pbCancel_clicked()
{
    QDialog::reject();
}
