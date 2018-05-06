#ifndef DLGPROGRESS_H
#define DLGPROGRESS_H

#include <QDialog>

namespace Ui {
    class dlgProgress;
}

class dlgProgress : public QDialog
{
    Q_OBJECT

public:
    explicit dlgProgress(QWidget *parent = 0);
    ~dlgProgress();
    void        progressShow();
    void        progressHide();
    void        progressInit( QString p_qsText, int p_nMin, int p_nMax );
    void        setProgressText( QString p_qsText );
    void        setProgressMin( int p_nMin );
    void        setProgressMax( int p_nMax );
    void        setProgressValue( int p_nValue );
    void        increaseProgressValue();
    void        increaseProgressMax( int p_nMax );

private:
    Ui::dlgProgress *ui;
};

#endif // DLGPROGRESS_H
