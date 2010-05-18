#ifndef FRMPANEL_H
#define FRMPANEL_H

#include <QFrame>
#include <QVBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <vector>

#include "db/dbpanelstatuses.h"

class cFrmPanel : public QFrame
{
    Q_OBJECT

public:
    cFrmPanel( const unsigned int p_uiPanelId );
    ~cFrmPanel();

    bool isWorking() const;
    void start();
    void reset();
    void inactivate();
    void activate();

signals:
    void panelClicked( unsigned int p_uiPanelId ) const;

protected:
    void mousePressEvent ( QMouseEvent *p_poEvent );
    void timerEvent ( QTimerEvent *p_poEvent );

private:
    unsigned int  m_uiId;
    unsigned int  m_uiType;
    unsigned int  m_uiStatus;
    unsigned int  m_uiCounter;
    int           m_inTimerId;

    QVBoxLayout  *verticalLayout;
    QLabel       *lblTitle;
    QLabel       *lblCurrStatus;
    QLabel       *lblCurrTimer;
    QLabel       *lblNextStatusLen;
    QLabel       *lblInfo;
    QSpacerItem  *spacer1;

    vector<cDBPanelStatuses*>  m_obStatuses;

    void load( const unsigned int p_uiPanelId );
    void displayStatus();
    void activateNextStatus();
};

#endif // FRMPANEL_H
