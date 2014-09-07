#ifndef DLGADVERTISEMENTWINDOW_H
#define DLGADVERTISEMENTWINDOW_H

#include <QDialog>
#include <QMouseEvent>

#include "../client/belenus.h"
#include "ui_dlgAdvertisement.h"
#include "dbadvertisements.h"

class cDlgAdvertisementWindow : public QDialog, protected Ui::dlgAdvertisement
{
    Q_OBJECT

public:
    explicit cDlgAdvertisementWindow(QWidget *parent = 0, unsigned int id = 0);
    ~cDlgAdvertisementWindow();

    void refreshBackground();

protected:
    void keyPressEvent ( QKeyEvent *p_poEvent );
    void timerEvent( QTimerEvent *p_poEvent );
    void mousePressEvent ( QMouseEvent *p_poEvent );

private slots:
    void on_pbStart_clicked();
    void on_pbStop_clicked();
    void on_pbRefresh_clicked();
    void on_pbSettings_clicked();

private:

    cDBAdvertisements   m_obAdvertisement;
    int                 m_nTimer;
    QStringList         m_qslImages;
    int                 m_nImageCounter;
    bool                m_bPanelVisible;

    void                _loadImage();
    void                _showButtonPanel();
};

#endif // DLGADVERTISEMENTWINDOW_H
