#ifndef DLG_PANELAPPEREANCE_H
#define DLG_PANELAPPEREANCE_H

#include "../belenus.h"
#include "ui_dlgpanelappereance.h"
#include "../db/dbpanelstatussettings.h"

class cDlgPanelAppereance : public QDialog, protected Ui::dlgPanelAppereance
{
    Q_OBJECT

public:
    cDlgPanelAppereance( QWidget *p_poParent = 0 );
    virtual ~cDlgPanelAppereance();

private:

    vector<cDBPanelStatusSettings*>    m_obStatusSettings;

    bool    m_bInit;

    void updatePanelSettings();

public slots:
    virtual void accept();

private slots:
    void on_spinFontSize_valueChanged(int );
    void on_cmbFontNames_currentIndexChanged(int);
    void on_pbTextColor_clicked();
    void on_cmbPanelText_currentIndexChanged(int index);
    void on_cmbPanelStatus_currentIndexChanged(int index);
    void on_pbFont_clicked();
    void on_pbBackgroundColor_clicked();
    void on_rbActive_clicked();
    void on_pbCaptionBackgroundActive_clicked();
    void on_pbTextColorActive_clicked();
    void on_rbInactive_clicked();
    void on_pbCaptionBackgroundInactive_clicked();
    void on_pbTextColorInactive_clicked();
};

#endif // DLG_PANELAPPEREANCE_H
