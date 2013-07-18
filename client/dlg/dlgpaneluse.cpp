
#include <QMessageBox>

#include "dlgpaneluse.h"
#include "db/dbpatientcard.h"
#include "db/dbpatientcardunits.h"

//==============================================================================================
//
//
//
//==============================================================================================
cPanelPCUnitUse::cPanelPCUnitUse(QWidget *p_poParent, QStringList *p_qslParameters)
{
    horizontalLayout = new QHBoxLayout( this );
    horizontalLayout->setObjectName( QString::fromUtf8( "horizontalLayout" ) );
    horizontalLayout->setSpacing( 0 );
    horizontalLayout->setMargin( 1 );

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
    connect( pbUseUnitType, SIGNAL(clicked()), this, SLOT(slotButtonClicked()) );

    horizontalSpacer1 = new QSpacerItem( 20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum );
    horizontalLayout->addItem( horizontalSpacer1 );

    cmbUseUnitCount = new QComboBox( this );
    cmbUseUnitCount->setObjectName( QString::fromUtf8( "cmbUseUnitCount" ) );
    cmbUseUnitCount->setEnabled( false );
    horizontalLayout->addWidget( cmbUseUnitCount );
    connect( cmbUseUnitCount, SIGNAL(currentIndexChanged(int)), this, SLOT(slotComboUnitUpdated()) );
    int nCountUnits = p_qslParameters->at( 4 ).toInt();
    for( int i=0; i<nCountUnits; i++ )
    {
        cmbUseUnitCount->addItem( QString::number(i+1) );
    }

    horizontalSpacer2 = new QSpacerItem( 20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum );
    horizontalLayout->addItem( horizontalSpacer2 );

    lblValid = new QLabel( this );
    lblValid->setObjectName( QString::fromUtf8( "lblValid" ) );
    lblValid->setText( tr("Valid for ") );
    horizontalLayout->addWidget( lblValid );

    lblValidData = new QLabel( this );
    lblValidData->setObjectName( QString::fromUtf8( "lblValidData" ) );
    lblValidData->setText( QString("%1 day(s)").arg( QDate::currentDate().daysTo( QDate::fromString(p_qslParameters->at( 3 ), "yyyy-MM-dd") ) ) );
    lblValidData->setStyleSheet( "font: 75 12pt;\ncolor: rgb(0, 125, 0);" );
    horizontalLayout->addWidget( lblValidData );

    horizontalSpacer3 = new QSpacerItem( 500, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    horizontalLayout->addItem( horizontalSpacer3 );

    m_nUnitTime = p_qslParameters->at(1).toInt();

    cDBPatientcardUnit  obDBPatientcardUnit;

    try
    {
        obDBPatientcardUnit.load( p_qslParameters->at(0).toInt() );
        QString qsQuery = QString( "SELECT * FROM patientcardunits WHERE "
                                   "patientCardId=%1 AND "
                                   "unitTime=%2 AND "
                                   "validDateTo=\"%3\" AND "
                                   "active=1" ).arg( obDBPatientcardUnit.patientCardId() ).arg( obDBPatientcardUnit.unitTime() ).arg( obDBPatientcardUnit.validDateTo() );
        QSqlQuery      *poQuery = g_poDB->executeQTQuery( qsQuery );

        while( poQuery->next() )
        {
            m_qslUnitIds << poQuery->value( 0 ).toString();
        }
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
    }
}
//----------------------------------------------------------------------------------------------
void cPanelPCUnitUse::slotButtonClicked()
{
    cmbUseUnitCount->setEnabled( pbUseUnitType->isChecked() );
    emit signalButtonClicked();
}
//----------------------------------------------------------------------------------------------
void cPanelPCUnitUse::slotComboUnitUpdated()
{
    emit signalComboIndexChanged();
}
//----------------------------------------------------------------------------------------------
int cPanelPCUnitUse::lengthSeconds()
{
    int nRet = 0;

    if( pbUseUnitType->isChecked() )
    {
        nRet = m_nUnitTime * 60 * ( cmbUseUnitCount->currentIndex() + 1 );
    }
    return nRet;
}
//----------------------------------------------------------------------------------------------
QStringList cPanelPCUnitUse::usedUnitIds()
{
    QStringList qslUnitIds;

    if( pbUseUnitType->isChecked() && cmbUseUnitCount->count() == m_qslUnitIds.count() )
    {
        for( int i=0; i<cmbUseUnitCount->currentIndex()+1; i++ )
        {
            qslUnitIds << m_qslUnitIds.at( i );
        }
    }

    return qslUnitIds;
}
//==============================================================================================
//
//
//
//==============================================================================================
cDlgPanelUse::cDlgPanelUse( QWidget *p_poParent, QString p_qsPanelTitle ) : QDialog( p_poParent )
{
    setupUi( this );

    setWindowIcon( QIcon("./resources/40x40_device.png") );

    lblPanelTitle->setText( p_qsPanelTitle );

    pbOk->setIcon( QIcon("./resources/40x40_ok.png") );
    pbCancel->setIcon( QIcon("./resources/40x40_cancel.png") );

    m_uiPanelUsePatientCardId   = 0;
    m_uiPanelUseTime            = 0;
    m_uiPanelUsePrice           = 0;

    setPanelUsePrice();
}
//----------------------------------------------------------------------------------------------
cDlgPanelUse::~cDlgPanelUse()
{
}
//----------------------------------------------------------------------------------------------
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
        QSqlQuery      *poQuery = g_poDB->executeQTQuery( qsQuery );

        while( poQuery->next() )
        {
            QStringList     qslUnits;
            qslUnits << poQuery->value( 0 ).toString() << poQuery->value( 1 ).toString() << poQuery->value( 2 ).toString() << poQuery->value( 3 ).toString() << poQuery->value( 4 ).toString();
            cPanelPCUnitUse *pPanelUseFrame = new cPanelPCUnitUse( this, &qslUnits );
            vlUnits->insertWidget( qvPanelUseUnits.count(), pPanelUseFrame );
            connect( pPanelUseFrame, SIGNAL(signalButtonClicked()), this, SLOT(slotPatientCardUseUpdated()) );
            connect( pPanelUseFrame, SIGNAL(signalComboIndexChanged()), this, SLOT(slotPatientCardUseUpdated()) );
            qvPanelUseUnits.append( pPanelUseFrame );
        }
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
    }
}
//----------------------------------------------------------------------------------------------
void cDlgPanelUse::setPanelUseTime(unsigned int p_uiSeconds)
{
    m_uiPanelUseTime = p_uiSeconds;

    setPanelUseTime();
}
//----------------------------------------------------------------------------------------------
void cDlgPanelUse::setPanelUseTime()
{
    QTime   qtPanelUseTime( m_uiPanelUseTime/3600, (m_uiPanelUseTime%3600)/60, (m_uiPanelUseTime%3600)%60 );

    lblTotalTimeValue->setText( qtPanelUseTime.toString( "hh:mm:ss" ) );
}
//----------------------------------------------------------------------------------------------
void cDlgPanelUse::setPanelUsePrice()
{
    cCurrency   cPrice( m_uiPanelUsePrice );

    lblTotalPriceValue->setText( cPrice.currencyFullStringShort() );
}
//----------------------------------------------------------------------------------------------
unsigned int cDlgPanelUse::panelUseSeconds()
{
    return m_uiPanelUseTime;
}
//----------------------------------------------------------------------------------------------
unsigned int cDlgPanelUse::panelUsePrice()
{
    return m_uiPanelUsePrice;
}
//----------------------------------------------------------------------------------------------
QStringList cDlgPanelUse::panelUnitIds()
{
    return m_qslUnitIds;
}
//----------------------------------------------------------------------------------------------
void cDlgPanelUse::slotPatientCardUseUpdated()
{
    m_uiPanelUseTime = 0;
    m_qslUnitIds.clear();

    for( int i=0; i<qvPanelUseUnits.count(); i++ )
    {
        m_uiPanelUseTime += qvPanelUseUnits.at(i)->lengthSeconds();
        m_qslUnitIds << qvPanelUseUnits.at(i)->usedUnitIds();
    }
    setPanelUseTime();
}
//----------------------------------------------------------------------------------------------
void cDlgPanelUse::on_pbOk_clicked()
{
    QDialog::accept();
}
//----------------------------------------------------------------------------------------------
void cDlgPanelUse::on_pbCancel_clicked()
{
    QDialog::reject();
}
//----------------------------------------------------------------------------------------------
