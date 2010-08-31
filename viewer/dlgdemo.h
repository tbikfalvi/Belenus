#ifndef DLGDEMO_H
#define DLGDEMO_H

#include <QDialog>

#include "ui_dlgdemo.h"

class cDlgDemo : public QDialog, private Ui::dlgDemo
{
    Q_OBJECT

public:
    cDlgDemo( QWidget *parent = 0 );
    ~cDlgDemo();

protected slots:
    virtual void showReport();
};

#endif // DLGDEMO_H
