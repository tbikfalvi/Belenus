
#include <QMessageBox>

#include "dlgpaneluse.h"
#include "db/dbpatientcard.h"
#include "db/dbpatientcardunits.h"
#include "edit/dlgpatientcardsell.h"
#include "edit/dlgpatientcardrefill.h"
#include "db/dbdiscount.h"
#include "communication_rfid.h"

extern cCommRFID       *g_poCommRFID;

//==============================================================================================
//
//
//
//==============================================================================================
cPanelPCUnitUse::cPanelPCUnitUse(QWidget *p_poParent, QStringList *p_qslParameters)
{
    setParent( p_poParent );

    m_uiOrderNum    = 0;

    horizontalLayout = new QHBoxLayout( this );
    horizontalLayout->setObjectName( QString::fromUtf8( "horizontalLayout" ) );
    horizontalLayout->setSpacing( 0 );
    horizontalLayout->setMargin( 1 );
    setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );

    QString qsToolTip = tr("<b>Number of units:</b> %1<br>"
                           "<b>Card type:</b> %2<br>"
                           "<b>Unit time:</b> %3 minute(s)<br>"
                           "<b>Valid:</b> %4").arg( p_qslParameters->at(5) )
                                              .arg( p_qslParameters->at(6) )
                                              .arg( p_qslParameters->at(2) )
                                              .arg( p_qslParameters->at(7) );

    pbUseUnitType = new QPushButton( this );
    pbUseUnitType->setObjectName( QString::fromUtf8( "pbUseUnitType" ) );
    pbUseUnitType->setMinimumWidth( 200 );
    pbUseUnitType->setMinimumHeight( 30 );
    pbUseUnitType->setMaximumHeight( 30 );
    pbUseUnitType->setText( tr("%1 min. (%2)").arg( p_qslParameters->at(2) )
                                              .arg( p_qslParameters->at(6) ) );
    pbUseUnitType->setToolTip( qsToolTip );
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
    int nCountUnits = p_qslParameters->at( 5 ).toInt();
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
    lblValidData->setText( tr("%1 day(s)").arg( QDate::currentDate().daysTo( QDate::fromString(p_qslParameters->at( 4 ), "yyyy-MM-dd") ) ) );
    lblValidData->setStyleSheet( "font: 75 12pt;\ncolor: rgb(0, 125, 0);" );
    horizontalLayout->addWidget( lblValidData );

    horizontalSpacer3 = new QSpacerItem( 500, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    horizontalLayout->addItem( horizontalSpacer3 );

    m_nUnitTime = p_qslParameters->at(2).toInt();

    cDBPatientcardUnit  obDBPatientcardUnit;

    try
    {
        obDBPatientcardUnit.load( p_qslParameters->at(0).toInt() );
        QString qsQuery = QString( "SELECT * FROM patientcardunits WHERE "
                                   "patientCardId=%1 AND "
                                   "patientCardTypeId=%2 AND "
                                   "unitTime=%3 AND "
                                   "validDateTo=\"%4\" AND "
                                   "prepared=0 AND "
                                   "active=1" ).arg( obDBPatientcardUnit.patientCardId() )
                                               .arg( p_qslParameters->at(1).toInt() )
                                               .arg( obDBPatientcardUnit.unitTime() )
                                               .arg( obDBPatientcardUnit.validDateTo() );
        QSqlQuery      *poQuery = g_poDB->executeQTQuery( qsQuery );

        while( poQuery->next() )
        {
            m_qslUnitIds << poQuery->value( 0 ).toString();
        }
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
        g_obGen.showTrayError( e.what() );
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

    g_obLogger(cSeverity::DEBUG) << "qslUnitIds ["
                                 << qslUnitIds.join("|")
                                 << "]"
                                 << EOM;
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
//=====================================================================================================================
//=====================================================================================================================
//
//
//
//=====================================================================================================================
//=====================================================================================================================
cDlgPanelUse::cDlgPanelUse( QWidget *p_poParent, unsigned int p_uiPanelId ) : QDialog( p_poParent )
{
    _initUiItems();

    m_dlgProgress               = new cDlgProgress( this );
    m_poParent                  = p_poParent;
    m_poMsg                     = p_poParent;
    m_uiPanelId                 = p_uiPanelId;
    m_uiPanelUsePatientCardId   = 0;
    m_uiPanelBaseTimeCard       = 0;
    m_uiPanelBaseTimeCash       = 0;
    m_uiPanelUseTimeCard        = 0;
    m_uiPanelUseTimeCash        = 0;
    m_uiPanelUsePrice           = 0;
    m_bIsEnterAccepted          = true;
    m_qslPanelUseTimes          = QStringList();
    m_uiIndexOfTime             = 0;
    m_uiPanelTypeId             = 0;
    m_bIsCardReadByRFIDReader   = false;
    m_bParentLoaded             = false;

    m_obDBPatientCard.createNew();

    _fillUiItems();

    m_bInit = false;

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "PanelUse", QPoint(500,360) );
    resize( qpDlgSize.x(), qpDlgSize.y() );
}
//----------------------------------------------------------------------------------------------
cDlgPanelUse::~cDlgPanelUse()
//----------------------------------------------------------------------------------------------
{
    delete m_dlgProgress;

    g_poPrefs->setDialogSize( "PanelUse", QPoint( width(), height() ) );
}
//----------------------------------------------------------------------------------------------
void cDlgPanelUse::_initUiItems()
//----------------------------------------------------------------------------------------------
{
    m_bInit = true;

    setupUi( this );

    setWindowIcon( QIcon("./resources/40x40_device.png") );

    pbReloadPC->setIcon( QIcon("./resources/40x40_refresh.png") );
    pbReadRFID->setIcon( QIcon("./resources/40x40_rfid.png") );
    pbOk->setIcon( QIcon("./resources/40x40_ok.png") );
    pbCancel->setIcon( QIcon("./resources/40x40_cancel.png") );
    pbInformation->setIcon( QIcon("resources/40x40_information.png") );
    pbOwnerLastVisitInformation->setIcon( QIcon("resources/40x40_information.png") );

    pbOk->setText( tr("Start") );
    pbInformation->setEnabled( false );
    pbOwnerLastVisitInformation->setEnabled( false );

    lblCardOwner->setText( tr("Owner : ") );
    lblComment->setText( tr("Comment :\n") );

    ledPatientCardRFID->setVisible( false );

    if( g_poPrefs->isBarcodeHidden() && !g_obUser.isInGroup( cAccessGroup::ADMIN ) )
    {
        ledPatientCardBarcode->setEchoMode( QLineEdit::Password );
    }
    else
    {
        ledPatientCardBarcode->setEchoMode( QLineEdit::Normal );
    }

}
//----------------------------------------------------------------------------------------------
void cDlgPanelUse::_fillUiItems()
//----------------------------------------------------------------------------------------------
{
    QString qsQuery = "";
    QSqlQuery *poQuery = NULL;

    qsQuery = QString( "SELECT title, panelTypeId FROM panels WHERE panelId=%1 " ).arg( m_uiPanelId );
    poQuery = g_poDB->executeQTQuery( qsQuery );
    poQuery->first();

    lblPanelTitle->setText( poQuery->value(0).toString() );
    m_uiPanelTypeId = poQuery->value(1).toUInt();

    cmbTimeIntervall->addItem( tr("<No time intervall selected>"), 0 );
    m_qslPanelUseTimes.append( QString("0|0") );

    qsQuery = QString( "SELECT * FROM paneluses WHERE panelId=%1 ORDER BY useTime " ).arg( m_uiPanelId );
    poQuery = g_poDB->executeQTQuery( qsQuery );
    while( poQuery->next() )
    {
        cCurrency   cPrice( poQuery->value(5).toInt() );

        cmbTimeIntervall->addItem( QString( "%1 (%2 %3) " ).arg( poQuery->value(4).toInt() ).arg( poQuery->value(3).toString() ).arg( cPrice.currencyFullStringShort() ), poQuery->value(0).toUInt() );
        m_qslPanelUseTimes.append( QString("%1|%2").arg( poQuery->value(4).toInt()*60 ).arg( poQuery->value(5).toInt() ) );
    }

    calculateTotalTimeValue();
    calculateTotalPriceValue();
}
//----------------------------------------------------------------------------------------------
void cDlgPanelUse::timerEvent(QTimerEvent *)
//----------------------------------------------------------------------------------------------
{
    m_nTimerCounter++;

    if( m_nTimerCounter > 40 )
    {
        killTimer( m_nTimer );
        m_dlgProgress->hideProgress();
    }

    if( g_poCommRFID != NULL && g_poCommRFID->isRFIDConnected() )
    {
        QString qsRFID = g_poCommRFID->readRFID();

        if( qsRFID.length() > 0 )
        {
            try
            {
                cDBPatientCard  obDBPatientCard;

                // remove \n\r from the end
                qsRFID = qsRFID.left( qsRFID.length()-2 );

                obDBPatientCard.loadRFID( qsRFID );
                ledPatientCardBarcode->setText( obDBPatientCard.barcode() );
                ledPatientCardRFID->setText( qsRFID );
                killTimer( m_nTimer );
                m_dlgProgress->hideProgress();
                m_bIsCardReadByRFIDReader = true;
                _newCardReading();
                on_pbReloadPC_clicked();
            }
            catch( cSevException &e )
            {
                g_obLogger(cSeverity::INFO) << "RFID [" << qsRFID << "] not found in database" << EOM;
                g_obGen.showTrayWarning( tr( "Reading card data failed or this card is not registered in database." ) );
            }

            g_obLogger(cSeverity::INFO) << "RFID read [" << qsRFID << "] " << EOM;
        }
    }
}
//----------------------------------------------------------------------------------------------
void cDlgPanelUse::enableCardUsage(bool p_bEnabled)
//----------------------------------------------------------------------------------------------
{
    m_bIsCardCanBeUsed = p_bEnabled;

    _enablePanelUseTypes();
}
//----------------------------------------------------------------------------------------------
void cDlgPanelUse::enableCashUsage(bool p_bEnabled)
//----------------------------------------------------------------------------------------------
{
    m_bIsCashCanBeUsed = p_bEnabled;

    _enablePanelUseTypes();
}
//----------------------------------------------------------------------------------------------
void cDlgPanelUse::_enablePanelUseTypes()
//----------------------------------------------------------------------------------------------
{
    gbPatientCard->setEnabled( m_bIsCardCanBeUsed );
    gbTime->setEnabled( m_bIsCashCanBeUsed );
}
//----------------------------------------------------------------------------------------------
void cDlgPanelUse::initPanelUseWithPatientCard(QString p_qsPatientCardBarcode, QString p_qsPatientCardRFID)
{
    m_bIsCardReadByRFIDReader = false;
    ledPatientCardBarcode->setText( p_qsPatientCardBarcode );
    ledPatientCardRFID->setText( p_qsPatientCardRFID );
    if( p_qsPatientCardRFID.length() > 0 )
    {
        m_bIsCardReadByRFIDReader = true;
    }
    m_bIsEnterAccepted = false;
    pbOk->setEnabled( false );
    m_poMsg = m_poParent;
    on_pbReloadPC_clicked();
    m_poMsg = this;
}
//----------------------------------------------------------------------------------------------
void cDlgPanelUse::setPanelUsePatientCard(/*unsigned int p_uiPatientCardId*/)
{
    QString         qsValidPeriods  = "";
    unsigned int    uiPatientId   = 0;

    pbOwnerLastVisitInformation->setEnabled( false );

    try
    {
        _removeUnitsFromDialog();

        if( m_obDBPatientCard.id() > 0 )
        {
            QSqlQuery *poQuery = g_poDB->executeQTQuery( QString("SELECT patients.name AS owner, patientcards.comment, patients.patientId "
                                                                 "FROM patientcards, patients "
                                                                 "WHERE patientcards.patientId=patients.patientId "
                                                                 "AND patientcards.patientCardId=%1").arg(m_obDBPatientCard.id()) );
            poQuery->first();

            pbInformation->setEnabled( true );
            lblCardOwner->setText( tr("Owner : %1").arg( poQuery->value(0).toString() ) );
            lblComment->setText( tr("Comment :\n%1").arg( poQuery->value(1).toString() ) );
            uiPatientId = poQuery->value(2).toUInt();

            m_bParentLoaded = false;
            if( m_obDBPatientCard.parentId() > 1 )
            {
                m_obDBPatientCard.load( m_obDBPatientCard.parentId() );
                m_bParentLoaded = true;
                //ledPatientCardBarcode->setText( m_obDBPatientCard.barcode() );
            }

            QString qsQuery = QString( "SELECT "
                                            "patientcardunits.patientCardUnitId, "
                                            "patientcardunits.patientCardTypeId, "
                                            "patientcardunits.unitTime, "
                                            "patientcardunits.validDateFrom, "
                                            "patientcardunits.validDateTo, "
                                            "COUNT(patientcardunits.unitTime), "
                                            "name "
                                       "FROM patientcardunits, patientcardtypes "
                                       "WHERE "
                                            "patientCardId=%1 "
                                            "AND patientcardunits.validDateFrom<=CURDATE() "
                                            "AND patientcardunits.validDateTo>=CURDATE() "
                                            "AND prepared=0 "
                                            "AND patientcardunits.active=1 "
                                            "AND patientcardunits.patientCardTypeId=patientcardtypes.patientCardTypeId "
                                       "GROUP BY "
                                            "patientcardunits.unitTime, "
                                            "patientcardunits.validDateTo, "
                                            "patientcardunits.patientCardTypeId "
                                       "ORDER BY "
                                            "patientcardunits.validDateTo, "
                                            "patientcardunits.patientCardUnitId" ).arg( m_obDBPatientCard.id() );
            poQuery = g_poDB->executeQTQuery( qsQuery );

            while( poQuery->next() )
            {
                QString qsValid;
                unsigned int uiPCTId = poQuery->value( 1 ).toUInt();

                if( uiPCTId > 0 )
                {
                    bool    isValid = m_obDBPatientCard.isPatientCardCanBeUsed( uiPCTId, &qsValid );
                    qsValidPeriods.append( tr("\n<b>%1 units (%2 minutes) valid on</b>%3")
                                           .arg( poQuery->value( 5 ).toString() )
                                           .arg( poQuery->value( 2 ).toString() )
                                           .arg( qsValid ) );
                    if( !isValid )
                    {
                        continue;
                    }
                }
                QStringList     qslUnits;
                qslUnits << poQuery->value( 0 ).toString()
                         << poQuery->value( 1 ).toString()
                         << poQuery->value( 2 ).toString()
                         << poQuery->value( 3 ).toString()
                         << poQuery->value( 4 ).toString()
                         << poQuery->value( 5 ).toString()
                         << poQuery->value( 6 ).toString()
                         << qsValid;
                cPanelPCUnitUse *pPanelUseFrame = new cPanelPCUnitUse( this, &qslUnits );
                vlUnits->insertWidget( qvPanelUseUnits.count(), pPanelUseFrame );
                connect( pPanelUseFrame, SIGNAL(signalButtonClicked()), this, SLOT(slotPatientCardUseUpdated()) );
                connect( pPanelUseFrame, SIGNAL(signalComboIndexChanged()), this, SLOT(slotPatientCardUseUpdated()) );
                qvPanelUseUnits.append( pPanelUseFrame );
                pPanelUseFrame->setOrderNum( qvPanelUseUnits.count() );
            }
            if( qvPanelUseUnits.count() > 0 )
            {
                qvPanelUseUnits.at(0)->setFocus();
            }
            qsValidPeriods.replace("\n","<br>");
        }
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
        g_obGen.showTrayError( e.what() );
    }

    if( qvPanelUseUnits.count() == 1 )
    {
        qvPanelUseUnits.at(0)->setAutoSelected();
        slotPatientCardUseUpdated();
    }

    _resizeDialogOnUnits();

    if( uiPatientId > 0 )
    {
        pbOwnerLastVisitInformation->setEnabled( true );
    }

    if( g_poPrefs->isShowPatientInfoOnStart() && uiPatientId > 0 )
    {
        on_pbOwnerLastVisitInformation_clicked();
    }
}
//----------------------------------------------------------------------------------------------
void cDlgPanelUse::setPanelUseTimeCash(unsigned int p_uiSeconds)
{
    m_uiPanelUseTimeCash = p_uiSeconds;
    m_poMsg = m_poParent;
    calculateTotalTimeValue();
    m_poMsg = this;
}
//----------------------------------------------------------------------------------------------
void cDlgPanelUse::setPanelUseTime(unsigned int p_uiSeconds)
{
    if( !m_bIsCashCanBeUsed )  m_uiPanelBaseTimeCash = p_uiSeconds;
    else                       m_uiPanelBaseTimeCard = p_uiSeconds;

    calculateTotalTimeValue();
}
//----------------------------------------------------------------------------------------------
void cDlgPanelUse::calculateTotalTimeValue()
{
    if( m_uiPanelUseTimeCash > 0 )
    {
        if( m_uiIndexOfTime == 0 )
        {
            for( int i=0; i<m_qslPanelUseTimes.count(); i++ )
            {
                QStringList qslTemp = m_qslPanelUseTimes.at(i).split('|');
                if( qslTemp.at(0).compare( QString::number(m_uiPanelUseTimeCash) ) == 0 )
                {
                    cmbTimeIntervall->setCurrentIndex( i );
                    break;
                }
            }
        }
        if( cmbTimeIntervall->currentIndex() == 0 )
        {
            QMessageBox::warning( m_poMsg, tr("Warning"),
                                  tr("This time period did not saved in the database\n"
                                     "for the actually selected device.\n"
                                     "Please select valid value from the list.") );
            m_uiPanelUseTimeCash = 0;
        }
        cmbTimeIntervall->setFocus();
    }

    QTime   qtPanelUseTime = QTime( (m_uiPanelBaseTimeCard+m_uiPanelBaseTimeCash+m_uiPanelUseTimeCard+m_uiPanelUseTimeCash)/3600,
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
void cDlgPanelUse::calculateTotalPriceValue()
{
    int nTimezoneDiscount   = 0;

    try
    {
        cDBDiscount obDiscount;

        obDiscount.loadTimeZone();
        nTimezoneDiscount = obDiscount.discount( m_uiPanelUsePrice );
    }
    catch( cSevException &e )
    {
//        g_obLogger(e.severity()) << e.what() << EOM;
//        g_obGen.showTrayError( e.what() );
    }

    cCurrency   cPrice( m_uiPanelUsePrice-nTimezoneDiscount );

    lblTotalPriceValue->setText( cPrice.currencyFullStringShort() );

    lblDiscountCaption->setVisible( false );
    lblDiscountValue->setVisible( false );

    if( nTimezoneDiscount > 0 )
    {
        lblDiscountCaption->setVisible( true );
        lblDiscountValue->setVisible( true );

        cCurrency   cDisc( nTimezoneDiscount );

        lblDiscountValue->setText( QString("  ") + cDisc.currencyFullStringShort() );
    }
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
QString cDlgPanelUse::panelUsePatientCardBarcode()
{
    return m_obDBPatientCard.barcode();
}
unsigned int cDlgPanelUse::panelTypeId()
{
    return m_uiPanelTypeId;
}
//----------------------------------------------------------------------------------------------
int cDlgPanelUse::countPatientCardUnitsLeft()
{
    int nRet = 0;

    QString qsQuery = QString( "SELECT COUNT(unitTime) "
                               "FROM patientcardunits "
                               "WHERE patientCardId=%1 AND "
                               "validDateFrom<=CURDATE() AND "
                               "validDateTo>=CURDATE() AND prepared=0 AND active=1" )
                                .arg( m_obDBPatientCard.id() );
    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
    if( poQuery->first() )
    {
        nRet = poQuery->value( 0 ).toInt();
    }
    return nRet;
//    return ( m_obDBPatientCard.units() - m_qslUnitIds.count() );
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
    calculateTotalTimeValue();

    g_obLogger(cSeverity::DEBUG) << "Units to use: " << m_qslUnitIds.join("|") << EOM;
}
//----------------------------------------------------------------------------------------------
void cDlgPanelUse::on_pbOk_clicked()
{
    if( !m_bIsEnterAccepted )
    {
        pbOk->setText( tr("Ok") );
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
    if( m_bInit ) return;

    m_uiIndexOfTime = index;

    QStringList qslTimePrice = m_qslPanelUseTimes.at( index ).split('|');

    m_uiPanelUseTimeCash = qslTimePrice.at(0).toInt();
    m_uiPanelUsePrice    = qslTimePrice.at(1).toInt();

    calculateTotalTimeValue();
    calculateTotalPriceValue();
}
//----------------------------------------------------------------------------------------------
void cDlgPanelUse::on_ledPatientCardBarcode_returnPressed()
{
    cTracer obTracer( "cDlgPanelUse::on_ledPatientCardBarcode_returnPressed" );
    pbOk->setEnabled( false );
    on_pbReloadPC_clicked();
    m_bIsEnterAccepted = false;
}
//----------------------------------------------------------------------------------------------
void cDlgPanelUse::on_pbReloadPC_clicked()
{
    cTracer obTracer( "cDlgPanelUse::on_pbReloadPC_clicked" );

    if( ledPatientCardBarcode->text().length() == 0 )
        return;

    if( ledPatientCardBarcode->text().compare( "000000" ) &&
        ledPatientCardBarcode->text().length() != g_poPrefs->getBarcodeLength() )
    {
        QMessageBox::warning( m_poMsg, tr("Warning"),
                              tr( "Invalid barcode length.\n"
                                  "The length of the barcode should be %1." ).arg(g_poPrefs->getBarcodeLength()) );
        return;
    }

    try
    {
        m_obDBPatientCard.load( ledPatientCardBarcode->text() );

        if( m_obDBPatientCard.isServiceCard() && !g_obUser.isInGroup(cAccessGroup::SYSTEM) )
        {
            if( !g_obGen.isSystemAdmin() )
            {
                QMessageBox::warning( m_poMsg, tr("Attention"),
                                      tr("You are not allowed to use system administrator card.\nPlease log in as a system administrator if you want to use this card.") );
                return;
            }
        }

        if( m_obDBPatientCard.RFID().length() > 0 && !m_bIsCardReadByRFIDReader && !(g_obUser.isInGroup( cAccessGroup::SYSTEM ) && m_obDBPatientCard.isServiceCard()) )
        {
            QMessageBox::warning( m_poMsg, tr("Attention"),
                                  tr("You are not allowed to use RFID card with barcode.\nPlease use the RFID reader if you want to use this card.") );
            return;
        }

        if( m_obDBPatientCard.pincode().compare("LOST") == 0 )
        {
            QMessageBox::warning( m_poMsg, tr("Attention"),
                                  tr("This patientcard has been lost and replaced\nand can not be used or sold again.") );
            return;
        }

        if( m_obDBPatientCard.active() )
        {
            m_obDBPatientCard.synchronizeUnits();
            m_obDBPatientCard.synchronizeTime();
            m_obDBPatientCard.save();
            // resolved conflict
            if( m_obDBPatientCard.units() < 1 || m_obDBPatientCard.timeLeft() < 1 )
            {
                QString     qsTemp = "";

                if( m_obDBPatientCard.timeLeft() < 1 )
                {
                    qsTemp = tr("\n\nDue to there is no time left, the patientcard will be reseted and deactivated.");
                }
                if( QMessageBox::question( m_poMsg, tr("Question"),
                                           tr("This patientcard can not be used with these settings:\n\n"
                                              "Available units: %1\n"
                                              "Available time: %2 (hh:mm:ss)\n\n"
                                              "Do you want to refill the patientcard now?%3").arg(m_obDBPatientCard.units()).arg(m_obDBPatientCard.timeLeftStr()).arg(qsTemp),
                                           QMessageBox::Yes,QMessageBox::No ) == QMessageBox::No )
                {
                    return;
                }
                else
                {
                    m_obDBPatientCard.setParentId( 0 );
                    m_obDBPatientCard.setActive( false );

                    cDlgPatientCardRefill obDlgPatientCardRefill( this, &m_obDBPatientCard );

                    if( obDlgPatientCardRefill.exec() != QDialog::Accepted )
                    {
                        return;
                    }
                }
            }

/*            QString qsValid;
            if( !m_obDBPatientCard.isPatientCardCanBeUsed( &qsValid ) )
            {
                QMessageBox::warning( m_poMsg, tr("Warning"),
                                      tr("This patientcard currently can not be used.\n"
                                         "Please check it's validity time period.\n\n%1").arg(qsValid) );
                return;
            }*/
        }
        else
        {
            int nRet = QMessageBox::question( m_poMsg, tr("Question"),
                                              tr("This barcode has not been activated yet.\n"
                                                 "Do you want to activate and sell it now?"),
                                              QMessageBox::Yes,QMessageBox::No );
            if( nRet == QMessageBox::Yes )
            {
                cDlgPatientCardSell obDlgPatientCardSell( this, &m_obDBPatientCard );
                obDlgPatientCardSell.setPatientCardOwner( g_obGuest.id() );
                obDlgPatientCardSell.exec();
            }
            else
            {
                return;
            }
        }
    }
    catch( cSevException &e )
    {
        if( QString(e.what()).compare("Patientcard barcode not found") != 0 )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
            g_obGen.showTrayError( e.what() );
        }
        else
        {
            if( QMessageBox::question( m_poMsg, tr("Question"),
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

    setPanelUsePatientCard( /*m_obDBPatientCard.id()*/ );
    slotPatientCardUseUpdated();
}
//----------------------------------------------------------------------------------------------
void cDlgPanelUse::on_ledPatientCardBarcode_textEdited(const QString &/*arg1*/)
//----------------------------------------------------------------------------------------------
{
    if( m_obDBPatientCard.id() > 0 )
    {
        g_obGen.showTrayInfo( "on_ledPatientCardBarcode_textEdited" );
        m_bIsCardReadByRFIDReader = false;
        m_obDBPatientCard.createNew();
        setPanelUsePatientCard( /*m_obDBPatientCard.id()*/ );
        _newCardReading();
    }
}
//----------------------------------------------------------------------------------------------
void cDlgPanelUse::on_pbInformation_clicked()
{
    g_obGen.showPatientCardInformation( ledPatientCardBarcode->text() );
}
//----------------------------------------------------------------------------------------------
void cDlgPanelUse::on_pbOwnerLastVisitInformation_clicked()
{
    g_obGen.showPatientLastVisitInformation( ledPatientCardBarcode->text(), g_poPrefs->getCloseInfoWindowAfterSecs() );
}
//----------------------------------------------------------------------------------------------
void cDlgPanelUse::on_pbReadRFID_clicked()
{
    m_nTimerCounter = 0;
    m_nTimer = startTimer( 500 );
    m_dlgProgress->showInformation( tr( "Please read your RFID card!" ) );
}
//----------------------------------------------------------------------------------------------
void cDlgPanelUse::_removeUnitsFromDialog()
//----------------------------------------------------------------------------------------------
{
    for( int i=0;i<qvPanelUseUnits.count(); i++ )
    {
        vlUnits->removeWidget( qvPanelUseUnits.at(i) );
        delete qvPanelUseUnits.at(i);
    }
    qvPanelUseUnits.clear();

}
//----------------------------------------------------------------------------------------------
void cDlgPanelUse::_resizeDialogOnUnits()
//----------------------------------------------------------------------------------------------
{
    resize( width(), 340 );

    int nUnitHeight = (qvPanelUseUnits.count()-1)*50;

    if( nUnitHeight < 0 ) nUnitHeight = 0;

    if( height() < 340 + nUnitHeight )
        resize( width(), 340 + nUnitHeight );
}
//----------------------------------------------------------------------------------------------
void cDlgPanelUse::_newCardReading()
//----------------------------------------------------------------------------------------------
{
    slotPatientCardUseUpdated();
    pbInformation->setEnabled( false );
    lblCardOwner->setText( "" );
    lblComment->setText( tr("Comment :\n") );
    _removeUnitsFromDialog();
    _resizeDialogOnUnits();
}
//----------------------------------------------------------------------------------------------
