#ifndef FRMPANEL_H
#define FRMPANEL_H

#include <QFrame>
#include <QVBoxLayout>
#include <QLabel>

class cFrmPanel : public QFrame
{
    Q_OBJECT

public:
    cFrmPanel( const unsigned int p_uiPanelId );
    ~cFrmPanel();

    bool isWorking() const;
    void start();
    void pause();
    void reset();

private:
    unsigned int  m_uiId;
    bool          m_poWorking;

    QVBoxLayout  *poVerticalLayout;
    QLabel       *poTitle;
    QSpacerItem  *poSpacer1;

    void load( const unsigned int p_uiPanelId );
};

#endif // FRMPANEL_H
