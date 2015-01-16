
#include <QMessageBox>

#include "dlgpaneluse.h"
#include "db/dbpatientcard.h"
#include "db/dbpatientcardunits.h"
#include "edit/dlgpatientcardsell.h"
#include "edit/dlgpatientcardrefill.h"

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
                           "<b>Unit time:</b> %2 minute(s)<br>"
                           "<b>Valid:</b> %3").arg( p_qslParameters->at(5) )
                                       .arg( p_qslParameters->at(2) )
                                       .arg( p_qslParameters->at(6) );

    pbUseUnitType = new QPushButton( this );
    pbUseUnitType->setObjectName( QString::fromUtf8( "pbUseUnitType" ) );
    pbUseUnitType->setMinimumWidth( 200 );
    pbUseUnitType->setMinimumHeight( 30 );
    pbUseUnitType->setMaximumHeight( 30 );
    pbUseUnitType->setText( tr("%1 minute(s) unit").arg( p_qslParameters->at(2) ) );
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
//==============================================================================================
//
//
//
//==============================================================================================
cDlgPanelUse::cDlgPanelUse( QWidget *p_poParent, unsigned int p_uiPanelId ) : QDialog( p_poParent )
{
    m_bInit = true;

    setupUi( this );

    setWindowIcon( QIcon("./resources/40x40_device.png") );

    pbReloadPC->setIcon( QIcon("./resources/40x40_refresh.png") );
    pbOk->setIcon( QIcon("./resources/40x40_ok.png") );
    pbCancel->setIcon( QIcon("./resources/40x40_cancel.png") );

    pbOk->setText( tr("Start") );
    pbInformation->setIcon( QIcon("resources/40x40_information.png") );
    pbInformation->setEnabled( false );

    lblCardType->setText( tr("Card type : ") );
    lblCardOwner->setText( tr("Owner : ") );

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

    m_obDBPatientCard.createNew();

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

    m_bInit = false;

    setPanelUseTime();
    setPanelUsePrice();

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "PanelUse", QPoint(500,360) );
    resize( qpDlgSize.x(), qpDlgSize.y() );
}
//----------------------------------------------------------------------------------------------
cDlgPanelUse::~cDlgPanelUse()
{
    g_poPrefs->setDialogSize( "PanelUse", QPoint( width(), height() ) );
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
void cDlgPanelUse::setPanelUsePatientCard(QString p_qsPatientCardBarcode)
{
    ledPatientCardBarcode->setText( p_qsPatientCardBarcode );
    m_bIsEnterAccepted = false;
    pbOk->setEnabled( false );
    m_poMsg = m_poParent;
    on_pbReloadPC_clicked();
    m_poMsg = this;
}
//----------------------------------------------------------------------------------------------
void cDlgPanelUse::setPanelUsePatientCard(unsigned int p_uiPatientCardId)
{
    QString     qsValidPeriods = "";

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

            QString qsQuery = QString( "SELECT patientCardUnitId, patientCardTypeId, unitTime, validDateFrom, validDateTo, COUNT(unitTime) "
                                       "FROM patientcardunits "
                                       "WHERE patientCardId=%1 "
                                       "AND validDateFrom<=CURDATE() AND validDateTo>=CURDATE() "
                                       "AND prepared=0 "
                                       "AND active=1 "
                                       "GROUP BY unitTime, validDateTo, patientCardTypeId ORDER BY validDateTo, patientCardUnitId" ).arg( m_obDBPatientCard.id() );
            QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );

            while( poQuery->next() )
            {
                QString qsValid;
                unsigned int uiPCTId = poQuery->value( 1 ).toUInt();

                if( uiPCTId == 0 )
                {
                    uiPCTId = m_obDBPatientCard.patientCardTypeId();
                }
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
/*            else
            {
                QMessageBox::warning( m_poMsg, tr("Warning"),
                                      tr("This patientcard currently can not be used.\n"
                                         "Please check it's validity time period.\n%1").arg(qsValidPeriods) );
            }*/
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

    if( m_obDBPatientCard.id() > 0 )
    {
        QSqlQuery *poQuery = g_poDB->executeQTQuery( QString("SELECT patientcardtypes.name AS type, patients.name AS owner "
                                                             "FROM patientcards JOIN patientcardtypes ON "
                                                             "patientcards.patientCardTypeId=patientcardtypes.patientCardTypeId "
                                                             "JOIN patients ON "
                                                             "patientcards.patientId=patients.patientId "
                                                             "WHERE patientcards.patientCardId=%1").arg(m_obDBPatientCard.id()) );
        poQuery->first();

        qsValidPeriods.replace("\n","<br>");
        pbInformation->setEnabled( true );
        lblCardType->setText( tr("Type : %1").arg( poQuery->value(0).toString() ) );
        lblCardOwner->setText( tr("Owner : %1").arg( poQuery->value(1).toString() ) );
    }

    int nUnitHeight = (qvPanelUseUnits.count()-1)*40;

    if( nUnitHeight < 0 ) nUnitHeight = 0;

    setMinimumHeight( 340 + nUnitHeight );
//    setMaximumHeight( 340 + nUnitHeight );
}
//----------------------------------------------------------------------------------------------
void cDlgPanelUse::setPanelUseTimeCash(unsigned int p_uiSeconds)
{
    m_uiPanelUseTimeCash = p_uiSeconds;
    m_poMsg = m_poParent;
    setPanelUseTime();
    m_poMsg = this;
}
//----------------------------------------------------------------------------------------------
void cDlgPanelUse::setPanelUseTime(unsigned int p_uiSeconds)
{
    if( !m_bIsCardCanBeUsed )       m_uiPanelBaseTimeCard = p_uiSeconds;
    else if( !m_bIsCashCanBeUsed )  m_uiPanelBaseTimeCash = p_uiSeconds;

    setPanelUseTime();
}
//----------------------------------------------------------------------------------------------
void cDlgPanelUse::setPanelUseTime()
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

    setPanelUseTime();
    setPanelUsePrice();
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

        if( m_obDBPatientCard.patientCardTypeId() == 1 && !g_obUser.isInGroup(cAccessGroup::SYSTEM) )
        {
            if( !g_obGen.isSystemAdmin() )
            {
                QMessageBox::warning( m_poMsg, tr("Attention"),
                                      tr("You are not allowed to use system administrator card.\nPlease log in as a system administrator if you want to use this card.") );
                return;
            }
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
            if( QMessageBox::question( m_poMsg, tr("Question"),
                                       tr("This barcode has not been activated yet.\n"
                                          "Do you want to activate and sell it now?"),
                                       QMessageBox::Yes,QMessageBox::No ) == QMessageBox::Yes )
            {
                cDlgPatientCardSell obDlgPatientCardSell( this, &m_obDBPatientCard );
                obDlgPatientCardSell.setPatientCardOwner( g_obGuest.id() );
                obDlgPatientCardSell.exec();
            }
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

    setPanelUsePatientCard( m_obDBPatientCard.id() );
    slotPatientCardUseUpdated();
}
//----------------------------------------------------------------------------------------------
void cDlgPanelUse::_enablePanelUseTypes()
{
    gbPatientCard->setEnabled( m_bIsCardCanBeUsed );

    gbTime->setEnabled( m_bIsCashCanBeUsed );
}
//----------------------------------------------------------------------------------------------
void cDlgPanelUse::on_ledPatientCardBarcode_textEdited(const QString &/*arg1*/)
{
    if( m_obDBPatientCard.id() > 0 )
    {
        m_obDBPatientCard.createNew();
        setPanelUsePatientCard( m_obDBPatientCard.id() );
        slotPatientCardUseUpdated();
        pbInformation->setEnabled( false );
        lblCardType->setText( "" );
        lblCardOwner->setText( "" );
    }
}
//----------------------------------------------------------------------------------------------
void cDlgPanelUse::on_pbInformation_clicked()
{
    g_obGen.showPatientCardInformation( ledPatientCardBarcode->text() );
}
