#ifndef DLGCOMMENT_H
#define DLGCOMMENT_H

#include "../belenus.h"
#include "ui_dlgcomment.h"

class dlgComment : public QDialog, protected Ui::dlgComment
{
    Q_OBJECT

public:
    dlgComment( QWidget *p_poParent = 0, QString p_qsTitle = "" );
    virtual ~dlgComment();

    QString     resultComment();

private slots:
    void on_pbOk_clicked();
    void on_pbCancel_clicked();
};

#endif
