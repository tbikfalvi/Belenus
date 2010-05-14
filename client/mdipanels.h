#ifndef MDIPANELS_H
#define MDIPANELS_H

#include <QMdiArea>
#include <QResizeEvent>
#include <vector>

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
    int                 m_inActivePanel;

    void activatePanel( const int p_inPanel );

protected:
    void resizeEvent ( QResizeEvent *p_poEvent );
    void keyPressEvent ( QKeyEvent *p_poEvent );
};

#endif
