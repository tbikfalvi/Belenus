#ifndef DLGPANEL_H
#define DLGPANEL_H

#include <QTimer>

#include "ui_dlgpanel.h"

class cDlgPanel : public QDialog, private Ui::dlgPanel
{
    Q_OBJECT

public:
    cDlgPanel( QWidget *p_poParent = 0 );
    ~cDlgPanel();

private:
    unsigned int m_uiTimeLeft;
    QTimer       m_obTimer;

private slots:
    void timeout();
};

#endif // DLGPANEL_H
