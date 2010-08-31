#ifndef DLGPREVIEW_H
#define DLGPREVIEW_H

#include <QDialog>

#include "ui_dlgpreview.h"

class cDlgPreview : public QDialog, private Ui::dlgPreview
{
    Q_OBJECT

public:
    cDlgPreview( QWidget *parent = 0 );
    ~cDlgPreview();

protected slots:
    virtual void printReport();
};

#endif // DLGPREVIEW_H
