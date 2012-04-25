#ifndef DLGSECONDARYWINDOW_H
#define DLGSECONDARYWINDOW_H

#include <QDialog>
#include <QGridLayout>
#include <QStringList>

#include "belenus.h"
#include "ui_dlgsecondarywindow.h"
#include "../dsppanel.h"

class cDlgSecondaryWindow : public QDialog, protected Ui::dlgSecondaryWindow
{
    Q_OBJECT

public:
    explicit cDlgSecondaryWindow(QWidget *parent = 0);
    ~cDlgSecondaryWindow();

    void                         initPanels();
    void                         refreshTitle( unsigned int p_uiPanelId );
    void                         setPanelStatus( unsigned int p_uiPanelId, const unsigned int p_uiPanelStatusId );
    void                         setPanelStatusText( unsigned int p_uiPanelId, const QString &p_qsStatus );
    void                         setPanelInfoText( unsigned int p_uiPanelId, const QString &p_qsInfo );
    void                         setPanelCounter( unsigned int p_uiPanelId, const unsigned int p_uiCounter );
    void                         setPanelWaitTime( unsigned int p_uiPanelId, const unsigned int p_uiWaitTime );

private:
    vector<cDspPanel*>           m_obPanels;
    QGridLayout                 *layoutMain;

};

#endif // DLGSECONDARYWINDOW_H
