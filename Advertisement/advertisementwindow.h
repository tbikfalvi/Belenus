#ifndef ADVERTISEMENTWINDOW_H
#define ADVERTISEMENTWINDOW_H

#include <QDialog>
#include <QMouseEvent>
#include <QtSql>
#include <vector>

using namespace std;

namespace Ui { class advertisementwindow; }

class advertisementwindow : public QDialog
{
    Q_OBJECT

public:
    explicit advertisementwindow(QWidget *parent = 0, unsigned int id = 0);
    ~advertisementwindow();

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
    void on_pbSave_clicked();

private:
    Ui::advertisementwindow *ui;

    vector<QPixmap *>   m_obAdImages;

    QSqlDatabase       *m_poDB;
    int                 m_nTimer;
    unsigned int        m_uiDBId;
    QString             m_qsDBCaption;
    QString             m_qsDBBackground;
    QString             m_qsDBPath;
    int                 m_nDBTimerLength;
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
    int                 m_nTimerCounter;
    bool                m_bImageChangeEnabled;
    QString             m_qsCMDAction;
    QStringList         m_qslCommands;
    QString             m_qsCMDStatus;

    void                _refreshSettings();
    void                _loadImage();
    void                _showButtonPanel();
    void                _loadPosition();
    void                _savePosition();
    int                 _loadSetting( QString p_Identifier, int p_Default );
    void                _saveSetting( QString p_Identifier, int p_Value );
    void                _loadImages();
    void                _updateGui();
    void                _readCommandFile();
    void                _updateCommand();
    void                _updateStatus();
};

#endif // ADVERTISEMENTWINDOW_H
