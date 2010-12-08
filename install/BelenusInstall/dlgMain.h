#ifndef DLGMAIN_H
#define DLGMAIN_H

#include <QDialog>

namespace Ui {
    class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private:
    Ui::Dialog *ui;
};

#endif // DLGMAIN_H
