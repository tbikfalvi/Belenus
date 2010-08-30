#ifndef WNDMAIN_H
#define WNDMAIN_H

#include "ui_wndmain.h"

class cWndMain : public QMainWindow, private Ui::wndMain
{
    Q_OBJECT

public:
    cWndMain( QWidget *parent = 0 );
    ~cWndMain();

private slots:
    void on_action_Demo_triggered();
};

#endif // WNDMAIN_H
