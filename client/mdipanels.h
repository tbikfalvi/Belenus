#ifndef MDIPANELS_H
#define MDIPANELS_H

#include <QMdiArea>
#include <QResizeEvent>
#include <vector>
#include <QResizeEvent>
#include <QKeyEvent>

#include "frmpanel.h"

using namespace std;

class cMdiPanels : public QMdiArea
{
    Q_OBJECT

public:
    cMdiPanels( QWidget *p_poParent = 0 );
    ~cMdiPanels();

    void            initPanels();
    void            placeSubWindows();

    void            start();
    void            reset();
    void            clear();
    void            next();
    int             activePanel();
    unsigned int    activePanelId();
    bool            isPanelWorking();
    bool            isPanelWorking( const unsigned int p_uiPanel );
    bool            isStatusCanBeSkipped( const unsigned int p_uiPanel );
    bool            isStatusCanBeReseted();
    void            reload();

    bool            isMainProcess();
    int             mainProcessTime();
    void            setMainProcessTime( const int p_inLength );
    void            setMainProcessTime( const int p_inLength, const int p_inPrice );
    void            setMainProcessTime( const unsigned int p_uiPatientCardId, const int p_inCountUnits, const int p_inLength );
    bool            isTimeIntervallValid( const int p_inLength, int *p_inPrice, int *p_inCount );
    void            cashPayed( const unsigned int p_uiLedgerId );
    void            cashPayed( const unsigned int p_uiPanelId, const unsigned int p_uiLedgerId );
    void            getPanelCashData( unsigned int *p_uiPatientId, int *p_inPrice, int *p_inDiscount );
    bool            isHasToPay();
    QString         getActivePanelCaption();
    QString         getPanelCaption( const unsigned int p_uiPanelId );
    bool            isCanBeStartedByTime();
    bool            isCanBeStartedByCard();
    void            setPaymentMethod( const unsigned int p_uiPaymentMethodId );
    void            setPaymentMethod( const unsigned int p_uiPanelId, const unsigned int p_uiPaymentMethodId );
    bool            isItemInShoppingCart();
    void            itemAddedToShoppingCart();
    void            itemRemovedFromShoppingCart();
    void            itemRemovedFromShoppingCart( const unsigned int p_uiPanelId );
    void            patientAddedToWaitingQueue();
    void            patientWaitingQueueEmpty();

signals:
//    void activePanelChanged( bool p_boActiveWorking ) const;
    void            activePanelChanged() const;
    void            signalOpenShoppingCart( unsigned int p_uiPanelId );
    void            signalPaymentActivated();
    void            signalOpenScheduleTable( unsigned int p_uiPanelId );

private:
    vector<cFrmPanel*>  m_obPanels;
    unsigned int        m_uiActivePanel;
    bool                m_bHWTimerStarted;
    int                 m_nTimer;

private slots:
    void activatePanel( unsigned int p_uiPanel );
    void slotPaymentActivated( unsigned int p_uiPanelId );
    void openShoppingCart( unsigned int p_uiPanelId );
    void slotOpenScheduleTable( unsigned int p_uiPanelId );

protected:
    void resizeEvent ( QResizeEvent *p_poEvent );
    void keyPressEvent ( QKeyEvent *p_poEvent );
    void timerEvent(QTimerEvent *event);
};

#endif
