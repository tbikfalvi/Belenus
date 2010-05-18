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

    void initPanels();
    void placeSubWindows();
    void start();
    void reset();

signals:
    void activePanelChanged( bool p_boActiveWorking ) const;

private:
    vector<cFrmPanel*>  m_obPanels;
    unsigned int        m_uiActivePanel;

private slots:
    void activatePanel( unsigned int p_uiPanel );

protected:
    void resizeEvent ( QResizeEvent *p_poEvent );
    void keyPressEvent ( QKeyEvent *p_poEvent );
};

#endif
