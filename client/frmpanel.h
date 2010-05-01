#ifndef FRMPANEL_H
#define FRMPANEL_H

#include <QFrame>

class cFrmPanel : public QFrame
{
    Q_OBJECT

public:
    cFrmPanel( QWidget *p_poParent = 0, Qt::WindowFlags p_inFlags = 0 );
    ~cFrmPanel();
};

#endif // FRMPANEL_H
