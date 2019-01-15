#ifndef DLGINFORMATION_H
#define DLGINFORMATION_H

#include "belenus.h"
#include "ui_dlgInformation.h"

class cDlgInformation : public QDialog, protected Ui::dlgInformation
{
    Q_OBJECT

public:
    cDlgInformation( QWidget *p_poParent = 0 );
    virtual ~cDlgInformation();

    void    setInformationTitle( QString p_qsTitle );
    void    setInformationText( QString p_qsInformation );
    void    setTimer( int p_nSeconds );

protected:
    void    timerEvent( QTimerEvent *p_poEvent );

private:
    QWidget *m_poParent;
    int      m_nTimer;

private slots:
    void on_pbOk_clicked();
};

#endif
