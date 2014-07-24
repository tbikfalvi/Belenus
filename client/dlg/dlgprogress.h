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
    void    showError( QString p_qsMessage );
    void    showWarning( QString p_qsMessage );
    void    showInformation( QString p_qsMessage );
    void    showProgressBar( int p_nMax );
    void    stepProgressBar();
    void    setValue( int p_nValue );
    void    setMax( int p_nMax );
    void    hideProgressBar();
    void    hideProgress();

protected:

private:
    QWidget *m_poParent;

    void    _centerWindow();

private slots:

};

#endif
