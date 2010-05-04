#ifndef WNDMAIN_H
#define WNDMAIN_H

#include <QString>

#include "ui_wndmain.h"
#include "mdipanels.h"

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
    cMdiPanels         *mdiPanels;

    void updateTitle();

private slots:
    void on_action_Preferences_triggered();
    void on_action_Users_triggered();
    void on_action_Patientcardtypes_triggered();
    void on_action_Patientcards_triggered();
    void on_action_Logs_triggered();
    void on_actionHardware_test_triggered();
    void on_actionLog_Out_triggered();
    void on_action_Producttypes_triggered();
    void on_action_Products_triggered();
    void on_actionP_anel_types_triggered();
};

#endif
