#ifndef WNDMAIN_H
#define WNDMAIN_H

#include <QString>
#include <QResizeEvent>

#include "ui_wndmain.h"
#include "frmpanel.h"

using namespace std;

class cWndMain : public QMainWindow, private Ui::wndMain
{
    Q_OBJECT

public:
    cWndMain( QWidget *parent = 0 );
    ~cWndMain();

    bool showLogIn();
    void initPanels();

private:
    vector<cFrmPanel*>  m_obPanels;

    void updateTitle();
    void placeSubWindows();

private slots:
    void on_action_Preferences_triggered();
    void on_action_Users_triggered();
    void on_action_Usercardtypes_triggered();
    void on_action_Usercards_triggered();
    void on_action_Logs_triggered();
    void on_actionHardware_test_triggered();
    void on_actionLog_Out_triggered();
    void on_action_Producttypes_triggered();
    void on_action_Products_triggered();
    void on_actionP_anel_types_triggered();

protected:
    void resizeEvent ( QResizeEvent *p_poEvent );

};

#endif
