#ifndef DLGADVERTISEMENTWINDOW_H
#define DLGADVERTISEMENTWINDOW_H

#include <QDialog>
#include <QMouseEvent>

#include "belenus.h"
#include "ui_dlgAdvertisement.h"
#include "db/dbadvertisements.h"

class cDlgAdvertisementWindow : public QDialog, protected Ui::dlgAdvertisement
{
    Q_OBJECT

public:
    explicit cDlgAdvertisementWindow(QWidget *parent = 0, unsigned int id = 0);
    ~cDlgAdvertisementWindow();

    void refreshBackground();

protected:
    void keyPressEvent ( QKeyEvent *p_poEvent );
    void keyReleaseEvent( QKeyEvent *p_poEvent );
    void timerEvent( QTimerEvent *p_poEvent );
    void mousePressEvent ( QMouseEvent *p_poEvent );
    void mouseReleaseEvent ( QMouseEvent *p_poEvent );
    void mouseMoveEvent ( QMouseEvent *p_poEvent );
    void mouseDoubleClickEvent ( QMouseEvent *p_poEvent );

private slots:
    void on_pbStart_clicked();
    void on_pbStop_clicked();
    void on_pbRefresh_clicked();
    void on_pbSettings_clicked();
    void on_pbSave_clicked();

private:

    cDBAdvertisements   m_obAdvertisement;
    int                 m_nTimer;
    QStringList         m_qslImages;
    int                 m_nImageCounter;
    bool                m_bPanelVisible;
    Qt::WindowFlags     m_wndFlags;
    bool                m_bWindowCustomizeable;
    bool                m_bShiftPressed;
    bool                m_bCtrlPressed;
    bool                m_bMousePressed;
    int                 m_nMouseX;
    int                 m_nMouseY;

    void                _loadImage();
    void                _showButtonPanel();
};

#endif // DLGADVERTISEMENTWINDOW_H
