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
    void            next();
    int             activePanel();
    bool            isPanelWorking();
    bool            isPanelWorking( const unsigned int p_uiPanel );
    void            reload();

    int             mainProcessTime();
    void            setMainProcessTime( const int p_inLength );
    void            setMainProcessTime( const unsigned int p_uiPatientCardId, const int p_inCountUnits, const int p_inLength );

signals:
    void activePanelChanged( bool p_boActiveWorking ) const;

private:
    vector<cFrmPanel*>  m_obPanels;
    unsigned int        m_uiActivePanel;
    bool                m_bHWTimerStarted;
    int                 m_nTimer;

private slots:
    void activatePanel( unsigned int p_uiPanel );

protected:
    void resizeEvent ( QResizeEvent *p_poEvent );
    void keyPressEvent ( QKeyEvent *p_poEvent );
    void timerEvent(QTimerEvent *event);
};

#endif
