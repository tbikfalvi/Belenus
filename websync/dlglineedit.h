#ifndef DLGLINEEDIT_H
#define DLGLINEEDIT_H

#include <QDialog>

namespace Ui {
class dlgLineEdit;
}

class dlgLineEdit : public QDialog
{
    Q_OBJECT

public:
    explicit dlgLineEdit(QWidget *parent = 0);
    ~dlgLineEdit();

    void        setTitle( QString p_qsTitle = "" );
    QString     value();

private:
    Ui::dlgLineEdit *ui;
};

#endif // DLGLINEEDIT_H
