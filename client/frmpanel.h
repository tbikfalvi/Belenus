#ifndef FRMPANEL_H
#define FRMPANEL_H

#include <QFrame>
#include <QVBoxLayout>
#include <QLabel>
#include <QMouseEvent>

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
    void mousePressEvent ( QMouseEvent * p_poEvent );

private:
    unsigned int  m_uiId;
    unsigned int  m_uiStatus;

    QVBoxLayout  *poVerticalLayout;
    QLabel       *poTitle;
    QSpacerItem  *poSpacer1;

    void load( const unsigned int p_uiPanelId );
    void displayStatus();
};

#endif // FRMPANEL_H
