#ifndef FRMPANEL_H
#define FRMPANEL_H

#include <QFrame>
#include <QVBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <vector>

#include "db/dbpanelstatuses.h"

typedef struct _used_patientcard
{
    unsigned int    uiPatientCardId;
    int             inCountUnits;
    int             inUnitTime;
} stUsedPatientCard;

class cFrmPanel : public QFrame
{
    Q_OBJECT

public:
    cFrmPanel( const unsigned int p_uiPanelId );
    ~cFrmPanel();

    bool            isWorking() const;
    void            start();
    void            reset();
    void            next();
    void            inactivate();
    void            activate();
    void            reload();

    int             mainProcessTime();
    void            setMainProcessTime( const int p_inLength );
    void            setMainProcessTime( const unsigned int p_uiPatientCardId, const int p_inCountUnits, const int p_inLength );

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

    int                          m_inMainProcessLength;
    vector<stUsedPatientCard>    m_vrPatientCard;

    QVBoxLayout                 *verticalLayout;
    QLabel                      *lblTitle;
    QLabel                      *lblCurrStatus;
    QLabel                      *lblCurrTimer;
    QLabel                      *lblNextStatusLen;
    QLabel                      *lblInfo;
    QSpacerItem                 *spacer1;

    vector<cDBPanelStatuses*>    m_obStatuses;

    void load( const unsigned int p_uiPanelId );
    void displayStatus();
    void activateNextStatus();
};

#endif // FRMPANEL_H
