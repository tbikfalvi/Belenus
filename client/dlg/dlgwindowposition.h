#ifndef DLGWINDOWPOSITION_H
#define DLGWINDOWPOSITION_H

#include "belenus.h"
#include "ui_dlgWindowPosition.h"

class cDlgWindowPosition : public QDialog, private Ui::dlgWindowPosition
{
    Q_OBJECT

public:
    cDlgWindowPosition(QWidget *p_poParent = 0);
    virtual ~cDlgWindowPosition();

public slots:

private slots:
    void on_pbClose_clicked();
    void on_pbMove_clicked();
    void on_pbResize_clicked();
    void on_cmbWindows_currentIndexChanged(const QString &itemText);

private:
    QString m_qsAdId;
    QString m_qsLeft;
    QString m_qsTop;
    QString m_qsWidth;
    QString m_qsHeight;
};

#endif // DLGWINDOWPOSITION_H
