//====================================================================================
//
// Belenus Kliens alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : frmpanel.h
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Ballok Peter, Bikfalvi Tamas
//
//====================================================================================
// Panelok kezeleset vegzo osztaly
//====================================================================================

#ifndef FRMPANEL_H
#define FRMPANEL_H

//====================================================================================

#include <QFrame>
#include <QVBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QPushButton>

#include <vector>

//====================================================================================

#include "db/dbpanelstatuses.h"
#include "db/dbledgerdevice.h"

//====================================================================================

typedef struct _used_patientcard
{
    unsigned int    uiPatientCardId;
    int             inCountUnits;
    int             inUnitTime;
} stUsedPatientCard;

//====================================================================================

class cFrmPanel : public QFrame
{
    Q_OBJECT

public:
    cFrmPanel( const unsigned int p_uiPanelId );
    ~cFrmPanel();

    bool            isWorking() const;
    bool            isStatusCanBeSkipped();
    bool            isStatusCanBeReseted();
    void            start();
    void            reset();
    void            clear();
    void            next();
    void            inactivate();
    void            activate();
    void            reload();

    unsigned int    panelId();
    bool            isMainProcess();
    int             mainProcessTime();
    void            setMainProcessTime( const int p_inLength );
    void            setMainProcessTime( const int p_inLength, const int p_inPrice );
    void            setMainProcessTime( const unsigned int p_uiPatientCardId, const int p_inCountUnits, const int p_inLength );
    bool            isTimeIntervallValid( const int p_inLength, int *p_inPrice, int *p_inCount );
    void            cashPayed( const unsigned int p_uiLedgerId );
    void            getPanelCashData( unsigned int *p_uiPatientId, int *p_inPrice, int *p_inDiscount );
    bool            isHasToPay();
    QString         getPanelName();
    bool            isCanBeStartedByTime();
    bool            isCanBeStartedByCard();
    void            setPaymentMethod( const unsigned int p_uiPaymentMethodId );
    bool            isItemInShoppingCart();
    void            itemAddedToShoppingCart();
    void            itemRemovedFromShoppingCart();
    void            patientAddedToWaitingQueue();
    void            patientWaitingQueueEmpty();

signals:
    void panelClicked( unsigned int p_uiPanelId ) const;

protected:
    void mousePressEvent ( QMouseEvent *p_poEvent );
    void timerEvent ( QTimerEvent *p_poEvent );

private:
    unsigned int                 m_uiId;
    unsigned int                 m_uiType;
    unsigned int                 m_uiStatus;
    unsigned int                 m_uiCounter;
    int                          m_inTimerId;
    unsigned int                 m_uiPanelOrderCount;

    cDBLedgerDevice             *m_pDBLedgerDevice;
    int                          m_inMainProcessLength;
    int                          m_inCashLength;
    int                          m_inCashTimeRemains;
    stUsedPatientCard            m_vrPatientCard;
    int                          m_inCardTimeRemains;
    int                          m_inCashToPay;
    int                          m_inCashNetToPay;
    int                          m_inCashDiscountToPay;
    unsigned int                 m_uiPatientToPay;
//    unsigned int                 m_uiAttendanceId;
    unsigned int                 m_uiLedgerId;
    unsigned int                 m_uiPaymentMethodId;
    bool                         m_bIsItemInShoppingCart;
    bool                         m_bIsPatientWaiting;
    unsigned int                 m_uiProcessWaitTime;

    QVBoxLayout                 *verticalLayout;
    QLabel                      *lblTitle;
    QLabel                      *lblCurrStatus;
    QLabel                      *lblCurrTimer;
    QLabel                      *lblNextStatusLen;
    QLabel                      *lblInfo;
    QSpacerItem                 *spacer1;
    QSpacerItem                 *spacer2;
    QSpacerItem                 *spacer3;
    QSpacerItem                 *spacer4;
    QHBoxLayout                 *layoutIcons;
    QPushButton                 *icoShoppingCart;
    QSpacerItem                 *spacerIcons;
    QPushButton                 *icoPanelStart;
    QPushButton                 *icoPanelNext;
    QPushButton                 *icoPanelStop;
    QPushButton                 *icoPanelCassa;
    QPushButton                 *icoScheduledGuest;

    QString                      m_qsStatus;
    QString                      m_qsTimer;
    QString                      m_qsTimerNextStatus;
    QString                      m_qsInfo;

    vector<cDBPanelStatuses*>    m_obStatuses;

    void            load( const unsigned int p_uiPanelId );
    void            displayStatus();
    void            formatStatusString( QString p_qsStatusText );
    void            formatTimerString( QString p_qsTimerText );
    void            formatNextLengthString( QString p_qsNextLengthText );
    void            formatInfoString( QString p_qsInfoText );
    void            activateNextStatus();
    void            closeAttendance();

    QString         convertCurrency( int p_nCurrencyValue, QString p_qsCurrency );
    unsigned int    _calculateWaitTime();

signals:
    void            signalPaymentActivated( unsigned int p_uiPanelId );
    void            signalOpenShoppingCart( unsigned int p_uiPanelId );
    void            signalOpenScheduleTable( unsigned int p_uiPanelId );
    void            signalStatusChanged( unsigned int p_uiPanelId, const unsigned int p_uiPanelStatusId, const QString p_qsStatus );
    void            signalSetCounterText( unsigned int p_uiPanelId, const QString &p_qsCounter );
    void            signalSetWaitTime( unsigned int p_uiPanelId, const unsigned int p_uiWaitTime );
    void            signalSetInfoText( unsigned int p_uiPanelId, const QString &p_qsInfo );

private slots:
    void            slotPanelStartClicked();
    void            slotPanelNextClicked();
    void            slotPanelStopClicked();
    void            slotPanelCassaClicked();
    void            slotShoppingCartClicked();
    void            slotScheduledGuestClicked();
};

#endif // FRMPANEL_H
