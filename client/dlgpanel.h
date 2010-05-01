#ifndef DLGPANEL_H
#define DLGPANEL_H

#include <QDialog>
#include <QTimer>

class cDlgPanel : public QDialog
{
    Q_OBJECT

public:
    cDlgPanel( QWidget *p_poParent = 0, Qt::WindowFlags p_inFlags = 0 );
    ~cDlgPanel();
};

#endif // DLGPANEL_H
