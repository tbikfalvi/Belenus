#ifndef CDLGTEST_H
#define CDLGTEST_H

#include <QDialog>

namespace Ui {
class cDlgTest;
}

class cDlgTest : public QDialog
{
    Q_OBJECT
    
public:
    explicit cDlgTest(QWidget *parent = 0);
    ~cDlgTest();
    
private:
    Ui::cDlgTest *ui;
};

#endif // CDLGTEST_H
