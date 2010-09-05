#ifndef DLGDEMO_H
#define DLGDEMO_H

#include <QDialog>

#include "dlgpreview.h"

class cDlgDemo : public cDlgPreview
{
    Q_OBJECT

public:
    cDlgDemo( QWidget *parent = 0 );
    ~cDlgDemo();

protected slots:
    virtual void refreshReport();
};

#endif // DLGDEMO_H
