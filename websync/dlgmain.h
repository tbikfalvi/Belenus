#ifndef DLGMAIN_H
#define DLGMAIN_H

#include <QSystemTrayIcon>
#include <QDialog>
#include <QMenu>
#include <QAction>
#include <QTranslator>
#include <QFrame>
#include <QLabel>
#include <QSpacerItem>
#include <QHBoxLayout>
#include <QPushButton>

extern QTranslator     *poTransApp;
extern QTranslator     *poTransQT;
extern QApplication    *apMainApp;

namespace Ui { class dlgMain; }

class dlgMain : public QDialog
{
    Q_OBJECT

public:
    explicit dlgMain(QWidget *parent = 0);
    ~dlgMain();
    bool checkTimeValues();

protected:
    void timerEvent( QTimerEvent *p_poEvent );
    void mousePressEvent ( QMouseEvent *p_poEvent );
    void mouseReleaseEvent ( QMouseEvent *p_poEvent );
    void mouseMoveEvent ( QMouseEvent *p_poEvent );

public slots:
    void slotSettings();

signals:
    void signalStartCounter(int p_nTimerLength, int p_nExtendedLength);
    void signalContinueCounter();
    void signalStopCounter();
    void signalResetCounter(int p_nTimerLength, int p_nExtendedLength);
    void signalSetTimer(int p_nTimerLength);
    void signalSetExtTimer(int p_nExtendedLength);

private slots:
    void on_pbHide_clicked();
    void on_pbExit_clicked();
    void on_cmbLang_currentIndexChanged(const QString &arg1);
    void on_pbRetranslate_clicked();

private:
    Ui::dlgMain         *ui;
    QSystemTrayIcon     *trayIcon;
    QMenu               *trayIconMenu;
    QAction             *actionSettings;
    QAction             *actionShow;
    QAction             *actionHide;
    QAction             *actionStart;
    QAction             *actionStop;
    QAction             *actionReset;
    QAction             *actionLogoff;
    QAction             *actionReboot;
    QAction             *actionShutdown;
    QAction             *actionExit;
    int                  m_nTimer;
    bool                 m_bMousePressed;
    int                  m_nMouseX;
    int                  m_nMouseY;
    QString              m_qsLang;
    bool                 m_bShowMainWindowOnStart;
    bool                 m_bShowTimerPanelOnStart;
    bool                 m_bStartTimerOnStart;
    bool                 m_bSaveTimerSettingsOnExit;
    bool                 m_bContinueExtended;
    int                  m_nActionOnFinish;
    QString              m_qsPopupMessage;
    bool                 m_bUseSavedTimers;
    bool                 m_bReloadLanguage;

    void                _setActions();
    void                _setMenu();
};

#endif // DLGMAIN_H
