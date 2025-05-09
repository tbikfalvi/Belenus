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
    void    setMaxValue( int p_nMaxValue );
    void    stepValue();
    void    showStepProgress();
    void    hideStepProgress();
    void    setStepsMaxValue( int p_nMaxValue );
    void    stepStepsValue();
    void    resetStepsValue();

protected:

private:
    QWidget *m_poParent;

private slots:

};

#endif
