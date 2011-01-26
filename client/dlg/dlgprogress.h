#ifndef DLGPROGRESS_H
#define DLGPROGRESS_H

#include "ui_dlgprogress.h"

class cDlgProgress : public QDialog, protected Ui::dlgProgress
{
    Q_OBJECT

public:
    cDlgProgress( QWidget *p_poParent = 0 );
    virtual ~cDlgProgress();

    void    showProgress();
    void    hideProgress();

protected:

private:
    QWidget *m_poParent;

private slots:

};

#endif
