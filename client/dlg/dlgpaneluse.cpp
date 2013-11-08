
#include <QMessageBox>

#include "dlgpaneluse.h"
#include "db/dbpatientcard.h"
#include "db/dbpatientcardunits.h"
#include "edit/dlgpatientcardsell.h"

//==============================================================================================
//
//
//
//==============================================================================================
cPanelPCUnitUse::cPanelPCUnitUse(QWidget *p_poParent, QStringList *p_qslParameters)
{
    m_uiOrderNum    = 0;

    horizontalLayout = new QHBoxLayout( this );
    horizontalLayout->setObjectName( QString::fromUtf8( "horizontalLayout" ) );
    horizontalLayout->setSpacing( 0 );
    horizontalLayout->setMargin( 1 );
    setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );

    pbUseUnitType = new QPushButton( this );
    pbUseUnitType->setObjectName( QString::fromUtf8( "pbUseUnitType" ) );
    pbUseUnitType->setMinimumWidth( 200 );
    pbUseUnitType->setMinimumHeight( 30 );
    pbUseUnitType->setMaximumHeight( 30 );
    pbUseUnitType->setText( tr("%1 minute(s) unit").arg( p_qslParameters->at(1) ) );
    pbUseUnitType->setToolTip( tr("Using this patient card unit type gives %1 minute(s) device usage.").arg( p_qslParameters->at(1) ) );
    pbUseUnitType->setIconSize( QSize(20,20) );
    pbUseUnitType->setIcon( QIcon("./resources/40x40_device_withcard.png") );
    pbUseUnitType->setCheckable( true );
    pbUseUnitType->setAutoDefault( false );
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
    lblValidData->setText( tr("%1 day(s)").arg( QDate::currentDate().daysTo( QDate::fromString(p_qslParameters->at( 3 ), "yyyy-MM-dd") ) ) );
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
    cmbUseUnitCount->setFocus();
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
//----------------------------------------------------------------------------------------------
void cPanelPCUnitUse::setFocus()
{
    pbUseUnitType->setFocus();
}
//----------------------------------------------------------------------------------------------
void cPanelPCUnitUse::setAutoSelected()
{
    pbUseUnitType->setChecked( true );
    pbUseUnitType->setEnabled( false );
    cmbUseUnitCount->setCurrentIndex(0);
    slotButtonClicked();
}
//----------------------------------------------------------------------------------------------
void cPanelPCUnitUse::setOrderNum(unsigned int p_uiOrderNum)
{
    m_uiOrderNum = p_uiOrderNum;
    pbUseUnitType->setText( QString("&%1. %2").arg(p_uiOrderNum).arg(pbUseUnitType->text()) );
}
//==============================================================================================
//
//
//
//==============================================================================================
cDlgPanelUse::cDlgPanelUse( QWidget *p_poParent, unsigned int p_uiPanelId ) : QDialog( p_poParent )
{
    setupUi( this );

    setWindowIcon( QIcon("./resources/40x40_device.png") );

    pbOk->setIcon( QIcon("./resources/40x40_ok.png") );
    pbCancel->setIcon( QIcon("./resources/40x40_cancel.png") );

    m_uiPanelId                 = p_uiPanelId;
    m_uiPanelUsePatientCardId   = 0;
    m_uiPanelUseTimeCard        = 0;
    m_uiPanelUseTimeCash        = 0;
    m_uiPanelUsePrice           = 0;
    m_bIsEnterAccepted          = true;

    m_obDBPatientCard.createNew();

    QString qsQuery = "";
    QSqlQuery *poQuery = NULL;

    qsQuery = QString( "SELECT title FROM panels WHERE panelId=%1 " ).arg( m_uiPanelId );
    poQuery = g_poDB->executeQTQuery( qsQuery );
    poQuery->first();

    lblPanelTitle->setText( poQuery->value(0).toString() );

    qsQuery = QString( "SELECT * FROM paneluses WHERE panelId=%1 ORDER BY useTime " ).arg( m_uiPanelId );
    poQuery = g_poDB->executeQTQuery( qsQuery );

    cmbTimeIntervall->addItem( tr("<No time intervall selected>"), 0 );
    while( poQuery->next() )
    {
        cCurrency   cPrice( poQuery->value(5).toInt() );

        cmbTimeIntervall->addItem( QString( "%1 - %2 (%3) " ).arg( poQuery->value(4).toInt() ).arg( poQuery->value(3).toString() ).arg( cPrice.currencyFullStringShort() ), poQuery->value(0).toUInt() );
    }

    setPanelUsePrice();
}
//----------------------------------------------------------------------------------------------
cDlgPanelUse::~cDlgPanelUse()
{
}
//----------------------------------------------------------------------------------------------
void cDlgPanelUse::enableCardUsage(bool p_bEnabled)
{
    m_bIsCardCanBeUsed = p_bEnabled;

    _enablePanelUseTypes();
}
//----------------------------------------------------------------------------------------------
void cDlgPanelUse::enableCashUsage(bool p_bEnabled)
{
    m_bIsCashCanBeUsed = p_bEnabled;

    _enablePanelUseTypes();
}
//----------------------------------------------------------------------------------------------
void cDlgPanelUse::setPanelUsePatientCard(unsigned int p_uiPatientCardId)
{
    try
    {
        if( m_uiPanelUsePatientCardId != p_uiPatientCardId && p_uiPatientCardId > 0)
        {
            m_uiPanelUsePatientCardId = p_uiPatientCardId;
            m_obDBPatientCard.load( m_uiPanelUsePatientCardId );
            ledPatientCardBarcode->setText( m_obDBPatientCard.barcode() );
        }

        for( int i=0;i<qvPanelUseUnits.count(); i++ )
        {
            vlUnits->removeWidget( qvPanelUseUnits.at(i) );
            delete qvPanelUseUnits.at(i);
        }
        qvPanelUseUnits.clear();

        if( m_uiPanelUsePatientCardId > 0 )
        {
            if( m_obDBPatientCard.parentId() > 1 )
            {
                m_obDBPatientCard.load( m_obDBPatientCard.parentId() );
            }

            QString qsQuery = QString( "SELECT patientCardUnitId, unitTime, validDateFrom, validDateTo, COUNT(unitTime) "
                                       "FROM patientcardunits "
                                       "WHERE patientCardId=%1 "
                                       "AND validDateFrom<=CURDATE() AND validDateTo>=CURDATE() "
                                       "AND active=1 "
                                       "GROUP BY unitTime, validDateTo ORDER BY validDateTo, patientCardUnitId" ).arg( m_obDBPatientCard.id() );
            QSqlQuery *poQuery = g_poDB->executeQTQuery( qsQuery );

            while( poQuery->next() )
            {
                QStringList     qslUnits;
                qslUnits << poQuery->value( 0 ).toString() << poQuery->value( 1 ).toString() << poQuery->value( 2 ).toString() << poQuery->value( 3 ).toString() << poQuery->value( 4 ).toString();
                cPanelPCUnitUse *pPanelUseFrame = new cPanelPCUnitUse( this, &qslUnits );
                vlUnits->insertWidget( qvPanelUseUnits.count(), pPanelUseFrame );
                connect( pPanelUseFrame, SIGNAL(signalButtonClicked()), this, SLOT(slotPatientCardUseUpdated()) );
                connect( pPanelUseFrame, SIGNAL(signalComboIndexChanged()), this, SLOT(slotPatientCardUseUpdated()) );
                qvPanelUseUnits.append( pPanelUseFrame );
                pPanelUseFrame->setOrderNum( qvPanelUseUnits.count() );
            }
            if( qvPanelUseUnits.count() > 0 )
                qvPanelUseUnits.at(0)->setFocus();
        }
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
    }

    if( qvPanelUseUnits.count() == 1 )
    {
        qvPanelUseUnits.at(0)->setAutoSelected();
        slotPatientCardUseUpdated();
    }

    int nUnitHeight = (qvPanelUseUnits.count()-1)*40;

    if( nUnitHeight < 0 ) nUnitHeight = 0;

    setMinimumHeight( 310 + nUnitHeight );
    setMaximumHeight( 310 + nUnitHeight );
}
//----------------------------------------------------------------------------------------------
void cDlgPanelUse::setPanelUseTimeCard(unsigned int p_uiSeconds)
{
    m_uiPanelBaseTimeCard = p_uiSeconds;

    setPanelUseTime();
}
//----------------------------------------------------------------------------------------------
void cDlgPanelUse::setPanelUseTimeCash(unsigned int p_uiSeconds)
{
    m_uiPanelBaseTimeCash = p_uiSeconds;

    setPanelUseTime();
}
//----------------------------------------------------------------------------------------------
void cDlgPanelUse::setPanelUseTime()
{
    QTime   qtPanelUseTime( (m_uiPanelBaseTimeCard+m_uiPanelBaseTimeCash+m_uiPanelUseTimeCard+m_uiPanelUseTimeCash)/3600,
                            ((m_uiPanelBaseTimeCard+m_uiPanelBaseTimeCash+m_uiPanelUseTimeCard+m_uiPanelUseTimeCash)%3600)/60,
                            ((m_uiPanelBaseTimeCard+m_uiPanelBaseTimeCash+m_uiPanelUseTimeCard+m_uiPanelUseTimeCash)%3600)%60 );

    lblTotalTimeValue->setText( qtPanelUseTime.toString( "hh:mm:ss" ) );

    if( (m_uiPanelUseTimeCard+m_uiPanelUseTimeCash) > 0 )
    {
        pbOk->setEnabled( true );
    }
    else
    {
        pbOk->setEnabled( false );
    }
}
//----------------------------------------------------------------------------------------------
void cDlgPanelUse::setPanelUsePrice()
{
    cCurrency   cPrice( m_uiPanelUsePrice );

    lblTotalPriceValue->setText( cPrice.currencyFullStringShort() );
}
//----------------------------------------------------------------------------------------------
unsigned int cDlgPanelUse::panelUseSecondsCard()
{
    return m_uiPanelUseTimeCard;
}
//----------------------------------------------------------------------------------------------
unsigned int cDlgPanelUse::panelUseSecondsCash()
{
    return m_uiPanelUseTimeCash;
}
//----------------------------------------------------------------------------------------------
unsigned int cDlgPanelUse::panelUsePrice()
{
    return m_uiPanelUsePrice;
}
//----------------------------------------------------------------------------------------------
unsigned int cDlgPanelUse::panelUsePatientCardId()
{
    return m_obDBPatientCard.id();
}
//----------------------------------------------------------------------------------------------
int cDlgPanelUse::countPatientCardUnitsLeft()
{
    return ( m_obDBPatientCard.units() - m_qslUnitIds.count() );
}
//----------------------------------------------------------------------------------------------
QStringList cDlgPanelUse::panelUnitIds()
{
    return m_qslUnitIds;
}
//----------------------------------------------------------------------------------------------
void cDlgPanelUse::slotPatientCardUseUpdated()
{
    m_uiPanelUseTimeCard = 0;
    m_qslUnitIds.clear();

    for( int i=0; i<qvPanelUseUnits.count(); i++ )
    {
        m_uiPanelUseTimeCard += qvPanelUseUnits.at(i)->lengthSeconds();
        m_qslUnitIds << qvPanelUseUnits.at(i)->usedUnitIds();
    }
    setPanelUseTime();
}
//----------------------------------------------------------------------------------------------
void cDlgPanelUse::on_pbOk_clicked()
{
    if( !m_bIsEnterAccepted )
    {
        m_bIsEnterAccepted = true;
        return;
    }

    QDialog::accept();
}
//----------------------------------------------------------------------------------------------
void cDlgPanelUse::on_pbCancel_clicked()
{
    QDialog::reject();
}
//----------------------------------------------------------------------------------------------
void cDlgPanelUse::on_cmbTimeIntervall_currentIndexChanged(int index)
{
    QString qsQuery = QString( "SELECT * FROM paneluses WHERE panelUseId=%1" ).arg( cmbTimeIntervall->itemData( index ).toUInt() );
    QSqlQuery *poQuery = g_poDB->executeQTQuery( qsQuery );
    poQuery->first();

    m_uiPanelUseTimeCash = poQuery->value(4).toInt()*60;
    m_uiPanelUsePrice    = poQuery->value(5).toInt();

    setPanelUseTime();
    setPanelUsePrice();
}
//----------------------------------------------------------------------------------------------
void cDlgPanelUse::on_ledPatientCardBarcode_returnPressed()
{
    m_bIsEnterAccepted = false;
    pbOk->setEnabled( false );
    on_pbReloadPC_clicked();
}
//----------------------------------------------------------------------------------------------
void cDlgPanelUse::on_pbReloadPC_clicked()
{
    if( ledPatientCardBarcode->text().length() != g_poPrefs->getBarcodeLength() )
    {
        QMessageBox::warning( this, tr("Warning"),
                              tr( "Invalid barcode length.\n"
                                  "The length of the barcode should be %1." ).arg(g_poPrefs->getBarcodeLength()) );
        return;
    }

    try
    {
        m_obDBPatientCard.load( ledPatientCardBarcode->text() );

        if( m_obDBPatientCard.patientCardTypeId() == 1 && !g_obUser.isInGroup(cAccessGroup::SYSTEM) )
        {
            QMessageBox::warning( this, tr("Attention"),
                                  tr("You are not allowed to use system administrator card.\nPlease log in as a system administrator if you want to use this card.") );
            return;
        }

        if( m_obDBPatientCard.pincode().compare("LOST") == 0 )
        {
            QMessageBox::warning( this, tr("Attention"),
                                  tr("This patientcard has been lost and replaced\nand can not be used or sold again.") );
            return;
        }

    }
    catch( cSevException &e )
    {
        if( QString(e.what()).compare("Patientcard barcode not found") != 0 )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
        }
        else
        {
            if( QMessageBox::question( this, tr("Question"),
                                       tr("This barcode has not found in the database.\n"
                                          "Do you want to save it and sell it now?"),
                                       QMessageBox::Yes,QMessageBox::No ) == QMessageBox::Yes )
            {
                m_obDBPatientCard.createNew();
                m_obDBPatientCard.setLicenceId( g_poPrefs->getLicenceId() );
                m_obDBPatientCard.setBarcode( ledPatientCardBarcode->text() );
                m_obDBPatientCard.save();

                cDlgPatientCardSell obDlgPatientCardSell( this, &m_obDBPatientCard );
                obDlgPatientCardSell.setPatientCardOwner( g_obGuest.id() );
                obDlgPatientCardSell.exec();
            }
        }
    }

    setPanelUsePatientCard( m_obDBPatientCard.id() );
    slotPatientCardUseUpdated();
}
//----------------------------------------------------------------------------------------------
void cDlgPanelUse::_enablePanelUseTypes()
{
    gbTime->setEnabled( m_bIsCashCanBeUsed );
}
//----------------------------------------------------------------------------------------------
