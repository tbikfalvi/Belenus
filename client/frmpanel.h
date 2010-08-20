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
    void            start();
    void            reset();
    void            clear();
    void            next();
    void            inactivate();
    void            activate();
    void            reload();

    bool            isMainProcess();
    int             mainProcessTime();
    void            setMainProcessTime( const int p_inLength );
    void            setMainProcessTime( const int p_inLength, const int p_inPrice );
    void            setMainProcessTime( const unsigned int p_uiPatientCardId, const int p_inCountUnits, const int p_inLength );
    bool            isTimeIntervallValid( const int p_inLength, int *p_inPrice, int *p_inCount );
    void            cashPayed();
    void            getPanelCashData( unsigned int *p_uiPatientId, int *p_inPrice );
    bool            isHasToPay();
    QString         getPanelName();
    bool            isCanBeStartedByTime();
    bool            isCanBeStartedByCard();

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
    bool                         m_bHasToPay;
    unsigned int                 m_uiPatientToPay;

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

    vector<cDBPanelStatuses*>    m_obStatuses;

    void load( const unsigned int p_uiPanelId );
    void displayStatus();
    void activateNextStatus();
    void closeAttendance();

    QString convertCurrency( int p_nCurrencyValue, QString p_qsCurrency );
};

#endif // FRMPANEL_H
