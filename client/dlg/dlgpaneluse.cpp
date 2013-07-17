
#include <QMessageBox>

#include "dlgpaneluse.h"
#include "db/dbpatientcard.h"

//====================================================================================
cPanelPCUnitUse::cPanelPCUnitUse(QWidget *p_poParent, QStringList *p_qslParameters)
{
    horizontalLayout = new QHBoxLayout( this );
    horizontalLayout->setObjectName( QString::fromUtf8( "horizontalLayout" ) );

    pbUseUnitType = new QPushButton( this );
    pbUseUnitType->setObjectName( QString::fromUtf8( "pbUseUnitType" ) );
    pbUseUnitType->setMinimumWidth( 200 );
    pbUseUnitType->setMinimumHeight( 30 );
    pbUseUnitType->setMaximumHeight( 30 );
    pbUseUnitType->setText( tr("%1 minute(s) unit").arg( p_qslParameters->at(1) ) );
    pbUseUnitType->setToolTip( tr("Using this patient card unit type gives %1 minute(s) device usage.") );
    pbUseUnitType->setIconSize( QSize(20,20) );
    pbUseUnitType->setIcon( QIcon("./resources/40x40_device_withcard.png") );
    pbUseUnitType->setCheckable( true );
    horizontalLayout->addWidget( pbUseUnitType );

    horizontalSpacer1 = new QSpacerItem( 20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum );
    horizontalLayout->addItem( horizontalSpacer1 );

    cmbUseUnitCount = new QComboBox( this );
    cmbUseUnitCount->setObjectName( QString::fromUtf8( "cmbUseUnitCount" ) );
    horizontalLayout->addWidget( cmbUseUnitCount );

    horizontalSpacer2 = new QSpacerItem( 500, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    horizontalLayout->addItem( horizontalSpacer2 );
}

//====================================================================================
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

        QString qsQuery = QString( "SELECT patientCardUnitId, unitTime, validDateFrom, validDateTo, COUNT(unitTime) "
                                   "FROM patientcardunits "
                                   "WHERE patientCardId=%1 "
                                   "AND validDateFrom<CURDATE() AND validDateTo>=CURDATE() "
                                   "AND active=1 "
                                   "GROUP BY unitTime, validDateTo ORDER BY validDateTo" ).arg( m_uiPanelUsePatientCardId );

        QStringList      qslUnits;

        qslUnits << "4124" << "1";

        cPanelPCUnitUse *obUnitPanel = new cPanelPCUnitUse( this, &qslUnits );

        vlUnits->insertWidget( 0, obUnitPanel );
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

void cDlgPanelUse::_fillPatientCardUnits()
{

}

