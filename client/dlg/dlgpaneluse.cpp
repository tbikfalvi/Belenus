
#include <QMessageBox>

#include "dlgpaneluse.h"
#include "db/dbpatientcard.h"

cDlgPanelUse::cDlgPanelUse( QWidget *p_poParent, QString p_qsPanelTitle ) : QDialog( p_poParent )
{
    setupUi( this );

    setWindowIcon( QIcon("./resources/40x40_device.png") );

    lblPanelTitle->setText( p_qsPanelTitle );

    pbOk->setIcon( QIcon("./resources/40x40_ok.png") );
    pbCancel->setIcon( QIcon("./resources/40x40_cancel.png") );

    m_uiPanelUsePatientCardId   = 0;
    m_uiPanelUseTime            = 0;
}

cDlgPanelUse::~cDlgPanelUse()
{
}

void cDlgPanelUse::setPanelUsePatientCard(unsigned int p_uiPatientCardId)
{
    m_uiPanelUsePatientCardId = p_uiPatientCardId;

    cDBPatientCard  obDBPatientCard;

    try
    {
        obDBPatientCard.load( m_uiPanelUsePatientCardId );

        ledPatientCardBarcode->setText( obDBPatientCard.barcode() );
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
    }
}

void cDlgPanelUse::setPanelUseTime(unsigned int p_uiSeconds)
{
    m_uiPanelUseTime = p_uiSeconds;

    QTime   qtPanelUseTime( m_uiPanelUseTime/3600, (m_uiPanelUseTime%3600)/60, (m_uiPanelUseTime%3600)%60 );

    lblTotalTimeValue->setText( qtPanelUseTime.toString( "hh:mm:ss" ) );
}
