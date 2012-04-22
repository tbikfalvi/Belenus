#ifndef DLGSECONDARYWINDOW_H
#define DLGSECONDARYWINDOW_H

#include <QDialog>
#include <QGridLayout>

#include "belenus.h"
#include "ui_dlgsecondarywindow.h"
#include "../dsppanel.h"

class cDlgSecondaryWindow : public QDialog, protected Ui::dlgSecondaryWindow
{
    Q_OBJECT

public:
    explicit cDlgSecondaryWindow(QWidget *parent = 0);
    ~cDlgSecondaryWindow();

    void                initPanels();

private:
    vector<cDspPanel*>  m_obPanels;

    QGridLayout        *layoutMain;

    void                placePanels();

};

#endif // DLGSECONDARYWINDOW_H
